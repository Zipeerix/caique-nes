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

#ifndef CAIQUE_NES_APU_HPP
#define CAIQUE_NES_APU_HPP

#include "Core/MMU.hpp"
#include "Utils/Types.hpp"

namespace Nes {
    class APU : Module {
    public:
        explicit APU(MMU& mmu);

        void handleAPURegisterWrite(Addr addr, Byte value);
        Byte handleAPURegisterRead(Addr addr);

    private:
        MMU& m_mmu;

        std::vector<Byte> m_registers{};
    };
}

#endif //CAIQUE_NES_APU_HPP
