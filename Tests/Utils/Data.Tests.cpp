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
#include "Utils/Data.hpp"

TEST(Utils_Tests, isBitSet) {
    const int value = 0b01;

    ASSERT_TRUE(Utils::isBitSet(value, 0));
    ASSERT_FALSE(Utils::isBitSet(value, 1));
    ASSERT_FALSE(Utils::isBitSet(value, sizeof(int) + 1));
}

TEST(Utils_Tests, setBit) {
    const int value = 0b01;

    ASSERT_EQ(Utils::setBit(value, 1), 0b11);
    ASSERT_EQ(Utils::setBit(value, 0), 0b01);
}

TEST(Utils_Tests, clearBit) {
    const int value = 0b01;

    ASSERT_EQ(Utils::clearBit(value, 0), 0b00);
    ASSERT_EQ(Utils::clearBit(value, 1), 0b01);
}

TEST(Utils_Tests, combineBits) {
    ASSERT_EQ(Utils::combineBits(0, 0), 0b00);
    ASSERT_EQ(Utils::combineBits(1, 0), 0b10);
    ASSERT_EQ(Utils::combineBits(0, 1), 0b01);
    ASSERT_EQ(Utils::combineBits(1, 1), 0b11);
}

TEST(Utils_Tests, getUpperByte) {
    ASSERT_EQ(Utils::getUpperByte(0xABCD), 0xAB);
    ASSERT_EQ(Utils::getUpperByte(0x00CD), 0x00);
}

TEST(Utils_Tests, getLowerByte) {
    ASSERT_EQ(Utils::getLowerByte(0xABCD), 0xCD);
    ASSERT_EQ(Utils::getLowerByte(0xAB00), 0x00);
}

TEST(Utils_Tests, combineBytes) {
    ASSERT_EQ(Utils::combineBytes(0xAB, 0xCD), 0xABCD);
    ASSERT_EQ(Utils::combineBytes(0x00, 0xCD), 0x00CD);
    ASSERT_EQ(Utils::combineBytes(0xAB, 0x00), 0xAB00);
}

TEST(Utils_Tests, isValueNegative) {
    const std::uint8_t positiveByte = 0b00011111;
    const std::uint8_t negativeByte = 0b10001111;
    const std::int8_t signedPositiveByte = 124;
    const std::int8_t signedNegativeByte = - 124;

    const std::uint16_t positiveShort = 0b0000111100001111;
    const std::uint16_t negativeShort = 0b1000111100001111;
    const std::int16_t signedPositiveShort = 5121;
    const std::int16_t signedNegativeShort = -5121;

    ASSERT_FALSE(Utils::isValueNegative(positiveByte));
    ASSERT_FALSE(Utils::isValueNegative(signedPositiveByte));
    ASSERT_FALSE(Utils::isValueNegative(positiveShort));
    ASSERT_FALSE(Utils::isValueNegative(signedPositiveShort));

    ASSERT_TRUE(Utils::isValueNegative(negativeByte));
    ASSERT_TRUE(Utils::isValueNegative(signedNegativeByte));
    ASSERT_TRUE(Utils::isValueNegative(negativeShort));
    ASSERT_TRUE(Utils::isValueNegative(signedNegativeShort));
}
