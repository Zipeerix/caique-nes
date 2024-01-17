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

#ifndef CAIQUE_NES_PPU_HPP
#define CAIQUE_NES_PPU_HPP

#include "Core/OAMEntry.hpp"
#include "Core/Tile.hpp"
#include "Core/MMU.hpp"
#include "Graphics/FrameBuffer.hpp"
#include "Utils/BitIndexedValue.hpp"
#include "Utils/Types.hpp"

#ifdef TESTING_ENVIRONMENT_NESTEST
#include "../../Tests/External/NesTest/NesTest.TestUtils.hpp"
#endif

namespace Nes {
    using CycleCount = int;

    namespace Const {
        constexpr CycleCount ppuCycleThreshold = 341;
        constexpr int cpuToPpuCycleMultiplier  = 3;

        constexpr int frameRate    = 60;
        constexpr int screenWidth  = 256;
        constexpr int screenHeight = 240;

#ifdef TESTING_ENVIRONMENT_NESTEST
        constexpr CycleCount ppuInitialCycles = 21;
#else
        constexpr CycleCount ppuInitialCycles = 0;
#endif

        constexpr int baseOamCycles = 513;

        constexpr std::array<Addr, 4> paletteMirrors = {0x3F10, 0x3F14, 0x3F18, 0x3F1C};
        constexpr int paletteMirrorsOffset = 0x10;

        constexpr int largerVRamIncrement  = 32;
        constexpr int smallerVRamIncrement = 1;

        constexpr int nametableSize      = 1024;
        constexpr int attributeTableSize = 64;

        constexpr Addr backgroundBankWhenSet = 0x1000;

        constexpr Addr spritePaletteOffset = 0x11;

        namespace Scanline {
            constexpr int vBlank = 241;
            constexpr int final  = 262;
        }

        namespace PPUAddrRange {
            constexpr Utils::Range<Addr> chr        = {0x0000, 0x1FFF};
            constexpr Utils::Range<Addr> nametables = {0x2000, 0x2FFF};
            constexpr Utils::Range<Addr> palette    = {0x3F00, 0x3FFF};

            // Note: when implementing other nametables createBackgroundPalette also need to be changed
            constexpr Utils::Range<Addr> nametable0 = {0x0000, nametableSize};
        }
        namespace MemorySize {
            constexpr int palettes = 32;
            constexpr int oam      = 256;
            constexpr int vram     = 2048;
        }

        namespace DefaultValue {
            constexpr bool scrollLatch = false;
            constexpr bool addrLatch   = true;
        }

        namespace RegisterAddress {
            constexpr Addr control = 0x2000;
            constexpr Addr mask    = 0x2001;
            constexpr Addr status  = 0x2002;
            constexpr Addr oamAddr = 0x2003;
            constexpr Addr oamData = 0x2004;
            constexpr Addr scroll  = 0x2005;
            constexpr Addr addr    = 0x2006;
            constexpr Addr data    = 0x2007;
        }
    }

    using FrameBuffer = Graphics::FrameBuffer<Const::screenWidth, Const::screenHeight>;
    using DrawFunction = std::function<void(const FrameBuffer&)>;

    enum class StatusRegisterFlag {
        SpriteOverflow = 5,
        SpriteHitZero,
        VBlank
    };

    enum class ControlRegisterFlag {
        BaseNametable,
        LargerVRamIncrement = 2,
        SpritePatternTableAddr,
        BackgroundPatternTableAddr,
        SpriteSize,
        MasterSlaveSelector,
        ShouldGenerateVBlankNMI,
    };

    enum class MaskRegisterFlag {
        Greyscale,
        ShowBackgroundLeftmost,
        ShowSpritesLeftmost,
        ShowBackground,
        ShowSprites,
        EmphasizeRed,
        EmphasizeGreen,
        EmphasizeBlue
    };

    template <typename FlagEnum>
    using BitIndexedRegister = Utils::BitIndexedValue<Byte, FlagEnum>;

    class PPU : Module {
    public:
        PPU(MMU& mmu, DrawFunction drawCallback);

        bool nmiStatus();

        void tick(CycleCount cpuCycleCount);

        void handleOamDmaRequest(Byte addressUpperByte);
        void handlePPURegisterWrite(Addr addr, Byte value);
        Byte handlePPURegisterRead(Addr addr);

    private:
        MMU& m_mmu;

        DrawFunction m_drawCallback;
        FrameBuffer  m_frameBuffer{};

        CycleCount m_cycles = Const::ppuInitialCycles;
        int m_scanline      = 0;
        bool m_nmiStatus    = false;

        Byte m_lastReadBuffer = 0;

        /* Registers */
        BitIndexedRegister<ControlRegisterFlag> m_control{0};
        BitIndexedRegister<StatusRegisterFlag>  m_status {0};
        BitIndexedRegister<MaskRegisterFlag>    m_mask   {0};
        Byte m_oamAddr = 0;
        Word m_addr    = 0;
        Byte m_scrollX = 0;
        Byte m_scrollY = 0;

        /* Register Helpers */
        bool m_scrollLatch = Const::DefaultValue::scrollLatch;
        bool m_addrLatch   = Const::DefaultValue::addrLatch;

        /* PPU Memory Space */
        std::array<Byte, Const::MemorySize::palettes> m_palettes{0};
        std::array<Byte, Const::MemorySize::oam>      m_oam{0};
        std::array<Byte, Const::MemorySize::vram>     m_nametables{0};

        void incrementAddrRegisterBasedOnControlRegister();

        /* Memory Helpers */
        Byte returnAndSwapBuffer(Byte value);
        Addr normaliseAddrRegister() const;
        Addr normalizePaletteAddr()  const;
        Addr normalizeNametableAddr() const;
        void writeToOam(Byte value);
        void write(Byte value);
        Byte read();

        /* Scanline Handlers */
        void handleVBlankScanline();
        void handleFinalScanline();

        /* Graphics Helpers */
        Palette createBackgroundPalette(Word column, Word row) const;
        Palette createSpritePalette(Word paletteId) const;
        Tile createTile(ControlRegisterFlag addrControlBit, Addr tileNumber, const Palette& palette) const;
        OAMEntry createOAMEntry(Addr startingAddr) const;

        /* Drawing Helpers */
        void drawFrame();
        void drawBackground();
        void drawSprites();

#ifdef TESTING_ENVIRONMENT_NESTEST
        public:
            std::pair<bool, std::string> matchesEntry(const NesTestUtils::Entry& state) const {

            if (state.ppuCycles != m_cycles) {
               return {false, "(PPU Cycles) Expected: " + std::to_string(state.ppuCycles) +
                              " Actual: " + std::to_string(m_cycles)};
            }

            if (state.scanline != m_scanline) {
               return {false, "(Scanline) Expected: " + std::to_string(state.scanline) +
                              " Actual: " + std::to_string(m_scanline)};
            }

            return {true, ""};
        }
#endif
#ifdef TESTING_ENVIRONMENT_PPU
    public:
        std::array<Byte, Const::MemorySize::vram>& accessNametables() {
            return m_nametables;
        };

        std::array<Byte, Const::MemorySize::oam>& accessOam() {
            return m_oam;
        };

        std::array<Byte, Const::MemorySize::palettes>& accessPalettes() {
            return m_palettes;
        };

        BitIndexedRegister<StatusRegisterFlag>& accesssStatus() {
            return m_status;
        }

        Word getAddrRegister() {
            return m_addr;
        }
#endif
    };
}

#endif //CAIQUE_NES_PPU_HPP
