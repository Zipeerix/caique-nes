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
#include "Utils/BitIndexedValue.hpp"

enum class BitIndexedValueTestFlag {
    BitIndex0,
    BitIndex1,
    BitIndex2,
    BitIndex3,
};

TEST(Utils_BitIndexedValue, GetAndSet) {
    Utils::BitIndexedValue<int, BitIndexedValueTestFlag> biv{55};
    ASSERT_EQ(biv.getCombinedValue(), 55);

    biv.setCombinedValue(0xAB);
    ASSERT_EQ(biv.getCombinedValue(), 0xAB);
}

TEST(Utils_BitIndexedValue, isBitSet) {
    Utils::BitIndexedValue<int, BitIndexedValueTestFlag> biv{0b0101};

    ASSERT_TRUE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex0));
    ASSERT_TRUE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex2));

    ASSERT_FALSE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex1));
    ASSERT_FALSE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex3));
}

TEST(Utils_BitIndexedValue, setBit) {
    Utils::BitIndexedValue<int, BitIndexedValueTestFlag> biv{0};

    biv.setBit(BitIndexedValueTestFlag::BitIndex1);
    biv.setBit(BitIndexedValueTestFlag::BitIndex3);
    ASSERT_TRUE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex1));
    ASSERT_TRUE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex3));
}

TEST(Utils_BitIndexedValue, clearBit) {
    Utils::BitIndexedValue<int, BitIndexedValueTestFlag> biv{INT_MAX};

    biv.clearBit(BitIndexedValueTestFlag::BitIndex0);
    biv.clearBit(BitIndexedValueTestFlag::BitIndex2);

    ASSERT_FALSE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex0));
    ASSERT_FALSE(biv.isBitSet(BitIndexedValueTestFlag::BitIndex2));
}
