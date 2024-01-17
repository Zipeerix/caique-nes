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

#include <algorithm>
#include "Core/MMU.hpp"
#include "Utils/Log.hpp"
#include "NROM.hpp"

Nes::NROM::NROM(Cartridge& cartridge, int sizeOfPRG, int sizeOfCHR) :
        BaseMapper(cartridge, sizeOfPRG, sizeOfCHR, 0, &NROM::validate)
{
    m_dualBankMode = sizeOfPRG == Const::dualBankPRGSize;
}

Nes::Byte Nes::NROM::readPRG(Addr addr) const {
    if (!m_dualBankMode && addr >= Const::bankSeparationAddr) {
        addr -= Const::bankSeparationAddr;
    }

    return m_cartridge.directReadPRG(addr);
}

void Nes::NROM::writePRG(Addr addr, Byte value) {
    Utils::log("Attempted to write to NROM mapped cartridge PRG");
}

Nes::Byte Nes::NROM::readCHR(Addr addr) const {
    return m_cartridge.directReadCHR(addr);
}

void Nes::NROM::writeCHR(Addr addr, Byte value) {
    Utils::log("Attempted to write to NROM mapped cartridge CHR");
}

bool Nes::NROM::validate(int sizeOfPRG, int sizeOfCHR) {
    bool validPRGSize = std::any_of(Const::Validation::PRGSizes.cbegin(), Const::Validation::PRGSizes.cend(), [&](int validSize) {
        return sizeOfPRG == validSize;
    });
    bool validCHRSize = sizeOfCHR == Const::Validation::CHRSize;

    return validPRGSize && validCHRSize;
}
