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

#ifndef CAIQUE_NES_VIRTUALMACHINE_HPP
#define CAIQUE_NES_VIRTUALMACHINE_HPP

#include <expected>
#include <string>
#include "Core/Cartridge.hpp"
#include "Core/Joypad.hpp"
#include "Core/MMU.hpp"
#include "Core/CPU.hpp"
#include "Core/PPU.hpp"
#include "Core/APU.hpp"

#ifdef TESTING_ENVIRONMENT_NESTEST
#include "../../Tests/External/NesTest/NesTest.TestUtils.hpp"
#endif

namespace Nes {
    namespace Const {
        constexpr CycleCount cyclesPerFrame = 29780;
    }

    class VirtualMachine {
    public:
        explicit VirtualMachine(DrawFunction drawCallback);

        std::expected<void, Utils::ErrorString> loadRom(const std::string& path);

        void tick();

        void handleKeyPress(JoypadButton button);
        void handleKeyRelease(JoypadButton button);

    private:
        Cartridge m_cartridge;
        MMU m_mmu;
        Joypad m_firstJoypad;
        Joypad m_secondJoypad;
        APU m_apu;
        PPU m_ppu;
        CPU m_cpu;

#if defined(TESTING_ENVIRONMENT_NESTEST) | defined(TESTING_ENVIRONMENT_PACMAN)
    public:
        PPU* accessPPU() {
            return &m_ppu;
        }

        CPU* accessCPU() {
            return &m_cpu;
        }
#endif
    };
}

#endif //CAIQUE_NES_VIRTUALMACHINE_HPP
