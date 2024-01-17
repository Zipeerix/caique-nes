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

#ifndef CAIQUE_NES_CARTRIDGE_HPP
#define CAIQUE_NES_CARTRIDGE_HPP

#include <expected>
#include <vector>
#include <string>
#include "Core/BaseMapper.hpp"
#include "Utils/Types.hpp"

namespace Nes {
    namespace Const {
        constexpr const char* headerConstant = "NES";
        constexpr int headerSize             = 16;
        constexpr int prgSizeMultiplier      = 16;
        constexpr int chrSizeMultiplier      = 8;
        constexpr int ines2Value             = 0x2;

        namespace AddressOf {
            constexpr Addr headerConstant = 0x0;
            constexpr Addr sizeOfPRG      = 0x4;
            constexpr Addr sizeOfCHR      = 0x5;

            constexpr Addr firstFlag = 0x6;
            constexpr Addr lastFlag  = 0xA;
        }

        namespace BitIndex {
            namespace Flag6 {
                constexpr int mirroring  = 0;
                constexpr int hasTrainer = 2;
                constexpr int fourScreen = 3;
            }

            namespace Flag7 {
                constexpr int ines2FirstBit  = 2;
                constexpr int ines2SecondBit = 3;
            }

            namespace Flag9 {
                constexpr int tvSystem = 0;
            }

            namespace Flag10 {
                constexpr int PRGRamPresent = 4;
            }
        }
    }

    enum class Mirroring {
        Horizontal,
        Vertical,
        FourScreen
    };

    enum class TVSystem {
        NTSC,
        PAL
    };

    class Cartridge : Module {
    public:
        std::expected<void, Utils::ErrorString> loadRawBytes(const std::vector<Byte>& bytes);
        std::expected<void, Utils::ErrorString> loadFromFilesystem(const std::string& path);

        Mirroring mirroring() const;

        Byte directReadPRG(Addr addr) const;
        Byte mappedReadPRG(Addr addr) const;
        Byte directReadCHR(Addr addr) const;
        Byte mappedReadCHR(Addr addr) const;

        void directWritePRG(Addr addr, Byte value);
        void mappedWritePRG(Addr addr, Byte value);
        void directWriteCHR(Addr addr, Byte value);
        void mappedWriteCHR(Addr addr, Byte value);

    private:
        std::unique_ptr<BaseMapper> m_mapper;

        std::vector<Byte> m_prg;
        std::vector<Byte> m_chr;

        Mirroring m_mirroring;
        TVSystem  m_tvSystem;

        bool m_hasTrainer;
        bool m_isNes2;
        bool m_isPRGRamPresent;

        std::expected<void, Utils::ErrorString> parseBytes(const std::vector<Byte>& rawRomBytes);
        void parseFlagByte(Byte flagByte, int flagIndex, Byte& mapperLowerNibble, Byte& mapperUpperNibble);

#ifdef TESTING_ENVIRONMENT_PPU
    public:
        void loadEmptyTestCartridge() {
            std::vector<Byte> empty{};
            empty.resize(Utils::kilobytesToBytes(24) + 16, 0x00);
            empty[0] = 'N';
            empty[1] = 'E';
            empty[2] = 'S';

            empty[Const::AddressOf::sizeOfPRG] = 1;
            empty[Const::AddressOf::sizeOfCHR] = 1;

            const auto result = loadRawBytes(empty);
            if (!result.has_value()) {
                throw std::logic_error("Unable to load empty cartridge: " + result.error());
            }
        };

        void forceMirroring(Mirroring mirroring) {
            m_mirroring = mirroring;
        };
#endif
    };
}

#endif //CAIQUE_NES_CARTRIDGE_HPP
