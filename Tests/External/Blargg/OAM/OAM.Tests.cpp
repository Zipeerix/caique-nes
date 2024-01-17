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
#include "../Blargg.TestUtils.hpp"

TEST(Blargg_Oam, Read) {
    const auto result = BlarggUtils::runTest("OAM", "oam_read");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Oam, Stress) {
    const auto result = BlarggUtils::runTest("OAM   ", "oam_stress");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}
