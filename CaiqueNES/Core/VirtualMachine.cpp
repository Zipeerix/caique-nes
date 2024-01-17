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

#include "Core/VirtualMachine.hpp"

Nes::VirtualMachine::VirtualMachine(DrawFunction drawFunction) :
    m_mmu(m_cartridge),
    m_firstJoypad(m_mmu, false),
    m_secondJoypad(m_mmu, true),
    m_apu(m_mmu),
    m_ppu(m_mmu, std::move(drawFunction)),
    m_cpu(m_mmu, m_ppu)
{
}

std::expected<void, Utils::ErrorString> Nes::VirtualMachine::loadRom(const std::string& path) {
    const auto loadResult = m_cartridge.loadFromFilesystem(path);
    if (!loadResult.has_value()) {
        return std::unexpected(loadResult.error());
    }

    m_cpu.loadProgramCounter();

    return {};
}

void Nes::VirtualMachine::tick() {
    CycleCount thisTickCycles = 0;
    while (thisTickCycles < Const::cyclesPerFrame) {
        thisTickCycles += m_cpu.tick();
    }
}

void Nes::VirtualMachine::handleKeyPress(JoypadButton button) {
    m_firstJoypad.press(button);
}

void Nes::VirtualMachine::handleKeyRelease(JoypadButton button) {
    m_firstJoypad.release(button);
}
