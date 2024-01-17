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
#include "Utils/Range.hpp"

TEST(Utils_Range_Tests, IsValueWithin) {
    const Utils::Range<int> range = {15, 50};

    ASSERT_TRUE(range.isValueWithin(15));
    ASSERT_TRUE(range.isValueWithin(50));
    ASSERT_TRUE(range.isValueWithin(35));

    ASSERT_FALSE(range.isValueWithin(-1));
    ASSERT_FALSE(range.isValueWithin(51));
}

TEST(Utils_Range_Tests, Extent) {
    const Utils::Range<int> range = {0, 50};

    ASSERT_EQ(range.extent(), 51);
}

TEST(Utils_Range_tests, Extent_MaximumRangeOverflowHandling) {
    const Utils::Range<int> range = {0, INT_MAX};

    ASSERT_EQ(range.extent(), static_cast<uint64_t>(INT_MAX) + 1);
}
