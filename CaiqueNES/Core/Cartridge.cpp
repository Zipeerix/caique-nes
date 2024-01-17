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

#include <stdexcept>
#include <cstring>
#include "Core/Cartridge.hpp"
#include "Utils/String.hpp"
#include "Utils/Host.hpp"

std::expected<void, Utils::ErrorString> Nes::Cartridge::loadRawBytes(const std::vector<Byte>& bytes) {
    const auto parseResult = parseBytes(bytes);
    if (!parseResult.has_value()) {
        return std::unexpected("Unable to parse ROM: " + parseResult.error());
    }

    return {};
}

std::expected<void, Utils::ErrorString> Nes::Cartridge::loadFromFilesystem(const std::string& path) {
    const auto fileLoadResult = Utils::loadExternalFileToVector(path);
    if (!fileLoadResult.has_value()) {
        return std::unexpected("Unable to create Cartridge module due to: " + fileLoadResult.error());
    }

    const auto parseResult = parseBytes(fileLoadResult.value());
    if (!parseResult.has_value()) {
        return std::unexpected("Unable to parse ROM: " + parseResult.error());
    }

    return {};
}

Nes::Mirroring Nes::Cartridge::mirroring() const {
    return m_mirroring;
}

Nes::Byte Nes::Cartridge::directReadPRG(Addr addr) const {
    return m_prg.at(addr);
}

Nes::Byte Nes::Cartridge::mappedReadPRG(Addr addr) const {
    return m_mapper->readPRG(addr);
}

Nes::Byte Nes::Cartridge::directReadCHR(Addr addr) const {
    return m_chr.at(addr);
}

Nes::Byte Nes::Cartridge::mappedReadCHR(Addr addr) const {
    return m_mapper->readCHR(addr);
}

void Nes::Cartridge::directWritePRG(Addr addr, Byte value) {
    m_prg.at(addr) = value;
}

void Nes::Cartridge::mappedWritePRG(Addr addr, Byte value) {
    m_mapper->writePRG(addr, value);
}

void Nes::Cartridge::directWriteCHR(Addr addr, Byte value) {
    m_chr.at(addr) = value;
}

void Nes::Cartridge::mappedWriteCHR(Addr addr, Byte value) {
    m_mapper->writeCHR(addr, value);
}

std::expected<void, Utils::ErrorString> Nes::Cartridge::parseBytes(const std::vector<Byte>& rawRomBytes) {
    for (std::size_t i = Const::AddressOf::headerConstant; i < std::strlen(Const::headerConstant); i++) {
        if (rawRomBytes[i] != Const::headerConstant[i]) {
            return std::unexpected("Header constant check failed");
        }
    }

    const std::size_t sizeOfPRG = rawRomBytes[Const::AddressOf::sizeOfPRG] * Const::prgSizeMultiplier;
    const std::size_t sizeOfCHR = rawRomBytes[Const::AddressOf::sizeOfCHR] * Const::chrSizeMultiplier;

    Byte mapperUpperNibble = 0;
    Byte mapperLowerNibble = 0;
    for (auto currentFlag = Const::AddressOf::firstFlag; currentFlag <= Const::AddressOf::lastFlag; currentFlag++) {
        parseFlagByte(rawRomBytes[currentFlag], currentFlag, mapperLowerNibble, mapperUpperNibble);
    }

    if (m_hasTrainer) {
        return std::unexpected("ROMs with trainers are not supported");
    }

    if (m_isNes2) {
        return std::unexpected("NES 2.0 ROMs are not supported");
    }

    try {
        const Byte mapperId = (mapperUpperNibble << 4) | mapperLowerNibble;
        m_mapper = BaseMapper::createFromID(*this, mapperId, sizeOfPRG, sizeOfCHR);
        if (m_mapper == nullptr) {
            return std::unexpected("Unsupported mapper with ID " + std::to_string(mapperId));
        }

    } catch (const std::invalid_argument& e) {
        return std::unexpected(e.what());
    }

    const std::size_t prgSizeInBytes = Utils::kilobytesToBytes(sizeOfPRG);
    const std::size_t chrSizeInBytes = Utils::kilobytesToBytes(sizeOfCHR);

    m_prg.resize(prgSizeInBytes, 0x00);
    m_chr.resize(chrSizeInBytes, 0x00);

    std::copy(rawRomBytes.begin() + Const::headerSize,
              rawRomBytes.begin() + Const::headerSize + prgSizeInBytes,
              m_prg.begin());

    std::copy(rawRomBytes.begin() + Const::headerSize + prgSizeInBytes,
              rawRomBytes.begin() + Const::headerSize + prgSizeInBytes + chrSizeInBytes,
              m_chr.begin());

    return {};
}

void Nes::Cartridge::parseFlagByte(Byte flagByte, int flagIndex, Byte& mapperLowerNibble, Byte& mapperUpperNibble) {
    switch (flagIndex) {
        case 6:
            if (Utils::isBitSet(flagByte, Const::BitIndex::Flag6::fourScreen)) {
                m_mirroring = Mirroring::FourScreen;
            } else {
                m_mirroring = static_cast<Mirroring>(Utils::isBitSet(flagByte, Const::BitIndex::Flag6::mirroring));
            }

            m_hasTrainer = Utils::isBitSet(flagByte, Const::BitIndex::Flag6::hasTrainer);

            mapperLowerNibble = flagByte >> 4;
            break;

        case 7: {
            const auto firstBit  = Utils::isBitSet(flagByte, Const::BitIndex::Flag7::ines2FirstBit);
            const auto secondBit = Utils::isBitSet(flagByte, Const::BitIndex::Flag7::ines2SecondBit);

            m_isNes2 = ((secondBit << 1) | firstBit) == Const::ines2Value;

            mapperUpperNibble = flagByte >> 4;
            break;
        }

        case 8:
            /* Ignore */
            break;

        case 9:
            m_tvSystem = static_cast<TVSystem>(Utils::isBitSet(flagByte, Const::BitIndex::Flag9::tvSystem));
            break;

        case 10:
            m_isPRGRamPresent = Utils::isBitSet(flagByte, Const::BitIndex::Flag10::PRGRamPresent);
            break;

        default:
            throw std::invalid_argument("Invalid flag index: " + Utils::convertToHexString(flagIndex, true));
    }
}
