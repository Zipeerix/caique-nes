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

#ifndef CAIQUE_NES_NROM_HPP
#define CAIQUE_NES_NROM_HPP

#include <array>
#include "Core/Cartridge.hpp"
#include "Core/BaseMapper.hpp"

namespace Nes {
    namespace Const {
        namespace Validation {
            constexpr std::array<int, 2> PRGSizes = {16, 32};
            constexpr int CHRSize = 8;
        }

        constexpr int dualBankPRGSize    = Validation::PRGSizes[1];
        constexpr int bankSeparationAddr = Utils::kilobytesToBytes(16);
    }

    class NROM : public BaseMapper {
    public:
        NROM(Cartridge& cartridge, int sizeOfPRG, int sizeOfCHR);
        ~NROM() override = default;

        Byte readPRG(Addr addr) const override;
        void writePRG(Addr addr, Byte value) override;

        Byte readCHR(Addr addr) const override;
        void writeCHR(Addr addr, Byte value) override;

    private:
        bool m_dualBankMode;

        static bool validate(int sizeOfPRG, int sizeOfCHR);
    };
}


#endif //CAIQUE_NES_NROM_HPP
