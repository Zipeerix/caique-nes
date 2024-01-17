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

#ifndef CAIQUE_NES_MAPPERS_TESTUTILS_HPP
#define CAIQUE_NES_MAPPERS_TESTUTILS_HPP

#include "Core/Cartridge.hpp"

namespace MapperTestUtils {
    std::vector<Nes::Byte> createMockROMBytes(Nes::Byte mapperId, int prgSize, int chrSize) {
        // To make sure sizes are not confused with kilobytes
        assert(prgSize < 10 && chrSize < 10);

        std::vector<Nes::Byte> bytes;

        const auto prgSizeInBytes = Utils::kilobytesToBytes(prgSize);
        const auto chrSizeInBytes = Utils::kilobytesToBytes(chrSize);

        bytes.resize(prgSizeInBytes + chrSizeInBytes + Nes::Const::headerSize, 0x00);

        if (mapperId != 0) {
            throw std::logic_error("Unimplemented test utils for other mappers");
        }

        bytes[0x0] = 'N';
        bytes[0x1] = 'E';
        bytes[0x2] = 'S';
        bytes[0x4] = prgSize;
        bytes[0x5] = chrSize;

        return bytes;
    }
}

#endif //CAIQUE_NES_MAPPERS_TESTUTILS_HPP
