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

#include "Core/Joypad.hpp"

Nes::Joypad::Joypad(MMU& mmu, bool isSecondPlayer) :
    m_mmu(mmu)
{
    const auto ioRange = isSecondPlayer ? Const::AddrRange::joypad2IO : Const::AddrRange::joypadIO;
    m_mmu.addMemoryRegion(ioRange,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte value) {
                              handleWrite(value);
                          },
                          [&](MemoryRegion*, const MMU*, Addr) {
                              return handleRead();
                          },
                          false);

    for (auto i = 0; i < Const::joypadButtonCount; i++) {
        m_buttonStatus[static_cast<JoypadButton>(i)] = false;
    }
}

void Nes::Joypad::handleWrite(Byte value) {
    m_singleButtonMode = Utils::isBitSet(value, 0);
    if (m_singleButtonMode) {
        m_singleButtonModeIndex = static_cast<Byte>(JoypadButton::A);
    }
}

Nes::Byte Nes::Joypad::handleRead() {
    if (m_singleButtonModeIndex >= Const::joypadButtonCount) {
        return Const::invalidKeyResponse;
    }

    const auto currentButton = static_cast<JoypadButton>(m_singleButtonModeIndex);
    const auto isKeyPressed  = m_buttonStatus[currentButton];
    if (!m_singleButtonMode) {
        m_singleButtonModeIndex++;
    }

    return isKeyPressed;
}

void Nes::Joypad::press(JoypadButton button) {
    m_buttonStatus[button] = true;
}

void Nes::Joypad::release(JoypadButton button) {
    m_buttonStatus[button] = false;
}
