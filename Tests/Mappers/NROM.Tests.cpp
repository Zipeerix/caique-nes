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

#include <gtest/gtest.h>
#include "Mappers/NROM.hpp"
#include "Mappers.TestUtils.hpp"

TEST(Nes_Mapper_NROM, Creation) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 2, 1));

    ASSERT_TRUE(result.has_value());
}

TEST(Nes_Mapper_NROM, Creation_BadPRGSize) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 3, 1));

    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(result.error(), "Unable to parse ROM: Sanity check failed for mapper with ID 0");
}

TEST(Nes_Mapper_NROM, Creation_BadCHRSize) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 2, 2));

    ASSERT_FALSE(result.has_value());
    ASSERT_EQ(result.error(), "Unable to parse ROM: Sanity check failed for mapper with ID 0");
}

TEST(Nes_Mapper_NROM, ReadWrite_PRG_SingleBank) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 1, 1));

    ASSERT_TRUE(result.has_value());

    cartridge.directWritePRG(0x000F, 0xAB);

    ASSERT_EQ(cartridge.mappedReadPRG(0x000F), 0xAB);
    ASSERT_EQ(cartridge.mappedReadPRG(0x400F), 0xAB);
}

TEST(Nes_Mapper_NROM, ReadWrite_PRG_Dual) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 2, 1));

    ASSERT_TRUE(result.has_value());

    cartridge.directWritePRG(0x4001, 0xAB);

    ASSERT_EQ(cartridge.mappedReadPRG(0x4001), 0xAB);
}

TEST(Nes_Mapper_NROM, ReadWrite_CHR) {
    Nes::Cartridge cartridge;
    const auto result = cartridge.loadRawBytes(MapperTestUtils::createMockROMBytes(0, 2, 1));

    ASSERT_TRUE(result.has_value());

    cartridge.directWriteCHR(0x1FFF, 0xAB);

    ASSERT_EQ(cartridge.mappedReadCHR(0x1FFF), 0xAB);
}


