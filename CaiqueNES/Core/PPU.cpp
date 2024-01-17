/***********************************************************************************************************************
*
*   Copyright 2023 CaiqueNES
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
***********************************************************************************************************************/

#include "Core/PPU.hpp"
#include "Utils/String.hpp"
#include "Utils/Log.hpp"

Nes::PPU::PPU(MMU& mmu, DrawFunction drawCallback) :
    m_mmu(mmu),
    m_drawCallback(std::move(drawCallback))
{
    m_mmu.addMemoryRegion(Const::AddrRange::ppuRegisters,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte byte) {
                              handlePPURegisterWrite(addr, byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr addr) {
                              return handlePPURegisterRead(addr);
                          },
                          false);

    m_mmu.addMemoryRegion(Const::AddrRange::ppuRegistersMirror,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte byte) {
                              handlePPURegisterWrite(addr & Const::AddrRange::ppuRegisters.to, byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr addr) {
                              return handlePPURegisterRead(addr & Const::AddrRange::ppuRegisters.to);
                          },
                          false);

    m_mmu.addMemoryRegion(Const::AddrRange::oamDmaRequest,
                          [&](MemoryRegion*, MMU*, Addr, Byte byte) {
                              handleOamDmaRequest(byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr) -> Byte {
                              Utils::log("Attempted to read OAM DMA request address");
                              return 0x00;
                          },
                          false);
}

bool Nes::PPU::nmiStatus() {
    const auto oldNmiStatus = m_nmiStatus;
    m_nmiStatus = false;

    return oldNmiStatus;
}

void Nes::PPU::handleOamDmaRequest(Byte addressUpperByte) {
    const Addr copyFrom = Utils::combineBytes(addressUpperByte, 0x00);
    const Addr copyUpTo = Utils::combineBytes(addressUpperByte, 0xFF);

    for (Addr currentAddr = copyFrom; currentAddr <= copyUpTo; currentAddr++) {
        const auto value = m_mmu.read(currentAddr);
        writeToOam(value);
    }

    const CycleCount cyclesTaken = m_cycles % 2 == 1 ? Const::baseOamCycles + 1 : Const::baseOamCycles;
    tick(cyclesTaken);
}

void Nes::PPU::tick(CycleCount cpuCycleCount) {
    m_cycles += Const::cpuToPpuCycleMultiplier * cpuCycleCount;
    if (m_cycles >= Const::ppuCycleThreshold) {
        m_cycles %= Const::ppuCycleThreshold;

        switch (++m_scanline) {
            case Const::Scanline::vBlank: handleVBlankScanline(); break;
            case Const::Scanline::final:  handleFinalScanline();  break;
            default:  /* Continue execution until threshold */    break;
        }
    }
}

void Nes::PPU::handlePPURegisterWrite(Addr addr, Byte value) {
    switch (addr) {
        case Const::RegisterAddress::oamData: writeToOam(value); break;
        case Const::RegisterAddress::data:    write(value);      break;

        case Const::RegisterAddress::scroll:
            if (m_scrollLatch) {
                m_scrollY = value;
            } else {
                m_scrollX = value;
            }

            m_scrollLatch = !m_scrollLatch;
            break;

        case Const::RegisterAddress::addr: {
            if (m_addrLatch) {
                m_addr = Utils::combineBytes(value, Utils::getLowerByte(m_addr));
            } else {
                m_addr = Utils::combineBytes(Utils::getUpperByte(m_addr), value);
            }

            m_addr = normaliseAddrRegister();

            m_addrLatch = !m_addrLatch;
            break;
        }

        case Const::RegisterAddress::control: {
            const auto oldShouldGenerateVBlank = m_control.isBitSet(ControlRegisterFlag::ShouldGenerateVBlankNMI);
            m_control.setCombinedValue(value);
            if (!oldShouldGenerateVBlank &&
                m_control.isBitSet(ControlRegisterFlag::ShouldGenerateVBlankNMI) &&
                m_status.isBitSet(StatusRegisterFlag::VBlank))
            {
                m_nmiStatus = true;
            }
        }

        case Const::RegisterAddress::mask:    m_mask.setCombinedValue(value); break;
        case Const::RegisterAddress::oamAddr: m_oamAddr = value;              break;

        case Const::RegisterAddress::status:
            Utils::log("Write to read-only PPU status register");
            break;

        default:
            throw std::logic_error("Writing to invalid PPU register address: " + Utils::convertToHexString(addr, true, 4));
    }
}

Nes::Byte Nes::PPU::handlePPURegisterRead(Addr addr) {
    switch (addr) {
        case Const::RegisterAddress::oamData: return m_oam[m_oamAddr];
        case Const::RegisterAddress::data: {
            const Byte value = read();
            incrementAddrRegisterBasedOnControlRegister();

            return value;
        }

        case Const::RegisterAddress::status: {
            const Byte value = m_status.getCombinedValue();

            m_status.clearBit(StatusRegisterFlag::VBlank);
            m_addrLatch   = Const::DefaultValue::addrLatch;
            m_scrollLatch = Const::DefaultValue::scrollLatch;

            return value;
        }

        case Const::RegisterAddress::control:
        case Const::RegisterAddress::mask:
        case Const::RegisterAddress::oamAddr:
        case Const::RegisterAddress::scroll:
        case Const::RegisterAddress::addr:
            Utils::log("Reading from write-only PPU register at: " + Utils::convertToHexString(addr, true, 4));
            return 0x00;

        default:
            throw std::logic_error("Reading from invalid PPU register address: " + Utils::convertToHexString(addr, true, 4));
    }
}

Nes::Byte Nes::PPU::returnAndSwapBuffer(Byte value) {
    const Byte lastReadValue = m_lastReadBuffer;
    m_lastReadBuffer = value;

    return lastReadValue;
}

void Nes::PPU::incrementAddrRegisterBasedOnControlRegister() {
    const auto incrementValue = m_control.isBitSet(ControlRegisterFlag::LargerVRamIncrement) ?
                                Const::largerVRamIncrement : Const::smallerVRamIncrement;

    m_addr += incrementValue;
    m_addr = normaliseAddrRegister();
}

Nes::Addr Nes::PPU::normaliseAddrRegister() const {
    Addr addr = m_addr;
    if (addr > Const::PPUAddrRange::palette.to) {
        addr &= Const::PPUAddrRange::palette.to;
    }

    return addr;
}

Nes::Addr Nes::PPU::normalizePaletteAddr() const {
    Addr requestedAddr = m_addr;
    if (std::find(Const::paletteMirrors.cbegin(), Const::paletteMirrors.cend(), m_addr) != Const::paletteMirrors.end()) {
        requestedAddr -= Const::paletteMirrorsOffset;
    }

    return requestedAddr - Const::PPUAddrRange::palette.from;
}

Nes::Addr Nes::PPU::normalizeNametableAddr() const {
    const Addr normalisedAddr = (m_addr & Const::PPUAddrRange::nametables.to) - Const::PPUAddrRange::nametables.from;
    const auto mirroring      = m_mmu.accessCartridge().mirroring();
    const Word nametableIndex = normalisedAddr / Const::nametableSize;

    if (mirroring == Mirroring::FourScreen) {
        return normalisedAddr;
    }

    switch (nametableIndex) {
        case 0x01: return mirroring == Mirroring::Horizontal ? normalisedAddr - Const::nametableSize : normalisedAddr;
        case 0x02: return mirroring == Mirroring::Horizontal ? normalisedAddr - Const::nametableSize : normalisedAddr - (2 * Const::nametableSize);
        case 0x03: return normalisedAddr - (2 * Const::nametableSize);
        default:   return normalisedAddr;
    }
}

void Nes::PPU::writeToOam(Byte value) {
    m_oam[m_oamAddr] = value;
    m_oamAddr++;
}

void Nes::PPU::write(Byte value) {
    if (Const::PPUAddrRange::nametables.isValueWithin(m_addr)) {
        const auto timetableAddr = normalizeNametableAddr();
        m_nametables[timetableAddr] = value;
    } else if (Const::PPUAddrRange::palette.isValueWithin(m_addr)) {
        m_palettes[normalizePaletteAddr()] = value;
    } else {
        throw std::logic_error("Writing to invalid address: " + Utils::convertToHexString(m_addr, true, 4));
    }

    incrementAddrRegisterBasedOnControlRegister();
}

Nes::Byte Nes::PPU::read() {
    if (Const::PPUAddrRange::chr.isValueWithin(m_addr)) {
        return returnAndSwapBuffer(m_mmu.accessCartridge().mappedReadCHR(m_addr));
    } else if (Const::PPUAddrRange::nametables.isValueWithin(m_addr)) {
        const auto timetableAddr = normalizeNametableAddr();
        return returnAndSwapBuffer(m_nametables[timetableAddr]);
    } else if (Const::PPUAddrRange::palette.isValueWithin(m_addr)) {
        return m_palettes[normalizePaletteAddr()];
    } else {
        throw std::logic_error("Reading from invalid address: " + Utils::convertToHexString(m_addr, true, 4));
    }
}

void Nes::PPU::handleVBlankScanline() {
    m_status.setBit(StatusRegisterFlag::VBlank);
    m_status.clearBit(StatusRegisterFlag::SpriteHitZero);
    if (m_control.isBitSet(ControlRegisterFlag::ShouldGenerateVBlankNMI)) {
        m_nmiStatus = true;
    }
}

void Nes::PPU::handleFinalScanline() {
    m_scanline  = 0;
    m_nmiStatus = false;

    m_status.clearBit(StatusRegisterFlag::SpriteHitZero);
    m_status.clearBit(StatusRegisterFlag::VBlank);

    drawFrame();
}

Nes::Palette Nes::PPU::createBackgroundPalette(Word column, Word row) const {
    const auto attributeTableId = row / 4 * 8 + column / 4;
    const Byte attributeByte = m_nametables[Const::PPUAddrRange::nametable0.to - Const::attributeTableSize + attributeTableId];

    const auto columnCheck = ((column % 4) / 2) == 1;
    const auto rowCheck    = ((row % 4) / 2)    == 1;

    int shiftMagnitude = 0;
    shiftMagnitude += columnCheck ? 2 : 0;
    shiftMagnitude += rowCheck ? 4 : 0;

    const Addr paletteStartingAddr = Const::colorIndexCount * ((attributeByte >> shiftMagnitude) & 0b11) + 1;

    std::array<Byte, Const::colorIndexCount> paletteBytes{0};
    paletteBytes[0] = m_palettes[0];
    for (auto i = 1; i < Const::colorIndexCount; i++) {
        paletteBytes[i] = m_palettes[paletteStartingAddr + i - 1];
    }

    return Palette(paletteBytes);
}

Nes::Palette Nes::PPU::createSpritePalette(Word paletteId) const {
    const auto paletteStartingAddr = Const::spritePaletteOffset + (paletteId * Const::colorIndexCount);

    std::array<Byte, Const::colorIndexCount> paletteBytes{0};
    for (auto i = 1; i < Const::colorIndexCount; i++) {
        paletteBytes[i] = m_palettes[paletteStartingAddr + i - 1];
    }

    return Palette(paletteBytes);
}

Nes::Tile Nes::PPU::createTile(ControlRegisterFlag addrControlBit, Addr tileNumber, const Palette& palette) const {
    const auto bank = m_control.isBitSet(addrControlBit) ? Const::backgroundBankWhenSet : 0x0;
    const auto startingAddr = (tileNumber * Const::tileSize) + bank;

    std::array<Byte, Const::tileSize> tileBytes{};
    for (auto i = startingAddr; i < (startingAddr + Const::tileSize); i++) {
        tileBytes[i - startingAddr] = m_mmu.accessCartridge().mappedReadCHR(i);
    }

    return Tile(tileBytes, palette);
}

Nes::OAMEntry Nes::PPU::createOAMEntry(Addr startingAddr) const {
    std::array<Byte, Const::oamEntrySize> bytes{0};
    for (auto i = startingAddr; i < (startingAddr + Const::oamEntrySize); i++) {
        bytes[i - startingAddr] = m_oam[i];
    }

    return OAMEntry(bytes);
}

void Nes::PPU::drawFrame() {
    drawBackground();
    drawSprites();
    m_drawCallback(m_frameBuffer);
}

void Nes::PPU::drawBackground() {
    for(auto i = 0; i < Const::PPUAddrRange::nametable0.to - Const::attributeTableSize; i++) {
        const auto index  = m_nametables[i];
        const auto column = i % 32;
        const auto row    = i / 32;

        const auto palette = createBackgroundPalette(column, row);
        const auto tile    = createTile(ControlRegisterFlag::BackgroundPatternTableAddr, index, palette);

        for (auto pixelY = 0; pixelY < Const::tileDimension; pixelY++) {
            for (auto pixelX = (Const::tileDimension - 1); pixelX >= 0; pixelX--) {
                const Graphics::PixelColor color = tile.getColor(pixelX, pixelY);
                m_frameBuffer.updatePixel(column * 8 + pixelX, row * 8 + pixelY, color);
            }
        }
    }
}

void Nes::PPU::drawSprites() {
    for (auto i = 0; i < m_oam.size(); i += 4) {
        const auto entry    = createOAMEntry(i);
        const auto palette  = createSpritePalette(entry.getPaletteId());
        const auto tile     = createTile(ControlRegisterFlag::SpritePatternTableAddr, entry.getSpriteId(), palette);

        for (auto pixelY = 0; pixelY < Const::tileDimension; pixelY++) {
            for (auto pixelX = (Const::tileDimension - 1); pixelX >= 0; pixelX--) {
                const Graphics::Position position = entry.getPosition(pixelX, pixelY);

                if (tile.skipDrawingSpritePixel(pixelX, pixelY) || !m_frameBuffer.withinBounds(position.x, position.y)) {
                    continue;
                }

                const Graphics::PixelColor color = tile.getColor(pixelX, pixelY);
                m_frameBuffer.updatePixel(position.x, position.y, color);
            }
        }
    }
}
