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

#include "Core/MMU.hpp"
#include "Utils/String.hpp"
#include "Utils/Log.hpp"

Nes::MemoryRegion::MemoryRegion(Utils::Range<Addr> addrRange, MemoryWriteFunction writeFunction,
                                MemoryReadFunction readFunction, bool needsItsOwnMemory) :
    addrRange(addrRange),
    writeFunction(std::move(writeFunction)),
    readFunction(std::move(readFunction))
{
    if (needsItsOwnMemory) {
        memory.resize(addrRange.extent(), 0);
    }
}

Nes::MemoryRegion::MemoryRegion(Utils::Range<Addr> addrRange) :
        MemoryRegion(addrRange,
                     [&](MemoryRegion* region, MMU*, Addr addr, Byte value) {
                         region->memory.at(addr - region->addrRange.from) = value;
                     },
                     [&](MemoryRegion* region, const MMU*, Addr addr) {
                         return region->memory.at(addr - region->addrRange.from);
                     },
                     true)
{
}

Nes::MMU::MMU(Cartridge& cartridge) :
    m_cartridge(cartridge)
{
    addMemoryRegion(Const::AddrRange::internalRAM);
    addMemoryRegion(Const::AddrRange::workRAM);
    addMemoryRegion(Const::AddrRange::unmapped); // <- Note: Remove when non-nrom mappers added

    addMemoryRegion(Const::AddrRange::mirror,
                    [](MemoryRegion*, MMU* mmu, Addr addr, Byte value) {
                        mmu->write(addr & Const::AddrRange::internalRAM.to, value);
                    },
                    [](MemoryRegion*, MMU* mmu, Addr addr) {
                        return mmu->read(addr & Const::AddrRange::internalRAM.to);
                    },
                    false);

    addMemoryRegion(Const::AddrRange::cartridgePRG,
                    [](MemoryRegion* region, MMU* mmu, Addr addr, Byte value) {
                        mmu->m_cartridge.mappedWritePRG(addr - region->addrRange.from, value);
                    },
                    [](MemoryRegion* region, MMU* mmu, Addr addr) -> Byte {
                        return mmu->m_cartridge.mappedReadPRG(addr - region->addrRange.from);
                    },
                    false);
}

const Nes::Cartridge& Nes::MMU::accessCartridge() {
    return m_cartridge;
}

void Nes::MMU::addMemoryRegion(Utils::Range<Addr> addrRange, const MemoryWriteFunction& writeFunction,
                                   const MemoryReadFunction& readFunction, bool needItsOwnMemory = false) {
    m_memoryRegions.emplace_back(addrRange, writeFunction, readFunction, needItsOwnMemory);
}

void Nes::MMU::addMemoryRegion(Utils::Range<Addr> addrRange) {
    m_memoryRegions.emplace_back(addrRange);
}

void Nes::MMU::clearMemoryRegions() {
    m_memoryRegions.clear();
}

void Nes::MMU::write(Addr addr, Byte value) {
    auto region = std::find_if(m_memoryRegions.begin(), m_memoryRegions.end(), [&](const MemoryRegion& memoryRegion) {
        return memoryRegion.addrRange.isValueWithin(addr);
    });

    if (region != m_memoryRegions.end()) {
        auto& regionObj = *region;
        regionObj.writeFunction(&regionObj, this, addr, value);
    } else {
        Utils::log("Writing to invalid address: " + Utils::convertToHexString(addr, true, 4));
    }
}

void Nes::MMU::writeWord(Addr addr, Word value) {
    write(addr + 1, Utils::getUpperByte(value));
    write(addr, Utils::getLowerByte(value));
}

Nes::Byte Nes::MMU::read(Addr addr) {
    const auto region = std::find_if(m_memoryRegions.begin(), m_memoryRegions.end(), [&](const MemoryRegion& memoryRegion) {
        return memoryRegion.addrRange.isValueWithin(addr);
    });

    if (region != m_memoryRegions.end()) {
        auto& regionObj = *region;
        return regionObj.readFunction(&regionObj, this, addr);
    } else {
        Utils::log("Reading from invalid address: " + Utils::convertToHexString(addr, true, 4));
        return 0x00;
    }
}

Nes::Word Nes::MMU::readWord(Addr addr) {
    const Byte upper = read(addr + 1);
    const Byte lower = read(addr);

    return Utils::combineBytes(upper, lower);
}
