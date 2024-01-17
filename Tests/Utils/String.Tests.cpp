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
#include "Utils/String.hpp"

TEST(Utils_Tests, convertToHexString) {
    ASSERT_EQ(Utils::convertToHexString(0x1B), "1B");
}

TEST(Utils_Tests, convertToHexString_Prefixed) {
    ASSERT_EQ(Utils::convertToHexString(0x1B, true), "0x1B");
}

TEST(Utils_Tests, convertToHexString_ForcedSize) {
    ASSERT_EQ(Utils::convertToHexString(0x1B, true, 4), "0x001B");
}
