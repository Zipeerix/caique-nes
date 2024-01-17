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

TEST(Blargg_Instructions, Basics) {
    const auto result = BlarggUtils::runTest("Instructions", "01-basics");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Implied) {
    const auto result = BlarggUtils::runTest("Instructions", "02-implied");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Immediate) {
    const auto result = BlarggUtils::runTest("Instructions", "03-immediate");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, ZeroPage) {
    const auto result = BlarggUtils::runTest("Instructions", "04-zero_page");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, ZpXy) {
    const auto result = BlarggUtils::runTest("Instructions", "05-zp_xy");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Absolute) {
    const auto result = BlarggUtils::runTest("Instructions", "06-absolute");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, AbsoluteXY) {
    const auto result = BlarggUtils::runTest("Instructions", "07-abs_xy");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, IndirectX) {
    const auto result = BlarggUtils::runTest("Instructions", "08-ind_x");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, IndirectY) {
    const auto result = BlarggUtils::runTest("Instructions", "09-ind_y");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Branches) {
    const auto result = BlarggUtils::runTest("Instructions", "10-branches");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Stack) {
    const auto result = BlarggUtils::runTest("Instructions", "11-stack");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Jumps) {
    const auto result = BlarggUtils::runTest("Instructions", "12-jmp_jsr");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Rts) {
    const auto result = BlarggUtils::runTest("Instructions", "13-rts");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Rti) {
    const auto result = BlarggUtils::runTest("Instructions", "14-rti");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Break) {
    const auto result = BlarggUtils::runTest("Instructions", "15-brk");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}

TEST(Blargg_Instructions, Special) {
    const auto result = BlarggUtils::runTest("Instructions", "16-special");
    ASSERT_TRUE(result.has_value()) << result.error();

    const auto testOutput = result.value();
    ASSERT_TRUE(testOutput.contains("Passed")) << "Output: " << result.value();
}
