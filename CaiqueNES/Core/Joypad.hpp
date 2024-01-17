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

#ifndef CAIQUE_NES_JOYPAD_HPP
#define CAIQUE_NES_JOYPAD_HPP

#include <map>
#include "Core/MMU.hpp"

namespace Nes {
    namespace Const {
        const Byte invalidKeyResponse = 0x1;
        const int joypadButtonCount   = 8;
    }

    enum class JoypadButton {
        A,
        B,
        Select,
        Start,
        Up,
        Down,
        Left,
        Right
    };

    class Joypad {
    public:
        Joypad(MMU& mmu, bool isSecondPlayer);

        void handleWrite(Byte value);
        Byte handleRead();

        void press(JoypadButton button);
        void release(JoypadButton button);

    private:
        MMU& m_mmu;

        std::map<JoypadButton, bool> m_buttonStatus{};

        bool m_singleButtonMode      = false;
        Byte m_singleButtonModeIndex = 0;
    };
}

#endif //CAIQUE_NES_JOYPAD_HPP
