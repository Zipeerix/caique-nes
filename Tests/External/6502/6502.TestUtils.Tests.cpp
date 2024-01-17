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
#include "6502.TestUtils.hpp"

TEST(Cpu6502TestUtils, readTest) {
    const std::string content =
            "[{\n"
            "\t\"name\": \"b1 71 8b\",\n"
            "\t\"initial\": {\n"
            "\t\t\"pc\": 9023,\n"
            "\t\t\"s\": 240,\n"
            "\t\t\"a\": 47,\n"
            "\t\t\"x\": 162,\n"
            "\t\t\"y\": 170,\n"
            "\t\t\"p\": 170,\n"
            "\t\t\"ram\": [\n"
            "\t\t\t[9023, 177],\n"
            "\t\t\t[9024, 113]\n"
            "\t\t]\n"
            "\t},\n"
            "\t\"final\": {\n"
            "\t\t\"pc\": 9025,\n"
            "\t\t\"s\": 240,\n"
            "\t\t\"a\": 37,\n"
            "\t\t\"x\": 162,\n"
            "\t\t\"y\": 170,\n"
            "\t\t\"p\": 40,\n"
            "\t\t\"ram\": [\n"
            "\t\t\t[113, 169],\n"
            "\t\t\t[114, 89]\n"
            "\t\t]\n"
            "\t},\n"
            "\t\"cycles\": [\n"
            "\t\t[9023, 177, \"read\"],\n"
            "\t\t[9024, 113, \"read\"]\n"
            "\t]\n"
            "}]";

    const auto test = Cpu6502TestUtils::readTest(content)[0];

    ASSERT_EQ(test.name, "b1 71 8b");
    ASSERT_EQ(test.cycles.size(), 2);
    if (test.cycles.size() == 2) {
        const auto& firstCycle = test.cycles[0];
        const auto& secondCycle = test.cycles[1];

        ASSERT_EQ(firstCycle.address, 9023);
        ASSERT_EQ(secondCycle.address, 9024);

        ASSERT_EQ(firstCycle.value, 177);
        ASSERT_EQ(secondCycle.value, 113);

        ASSERT_EQ(firstCycle.operation, "read");
        ASSERT_EQ(secondCycle.operation, "read");
    }

    const auto& initialState = test.initial;
    const auto& finalState = test.final;

    ASSERT_EQ(initialState.pc, 9023);
    ASSERT_EQ(finalState.pc, 9025);
    ASSERT_EQ(initialState.s, 240);
    ASSERT_EQ(finalState.s, 240);
    ASSERT_EQ(initialState.a, 47);
    ASSERT_EQ(finalState.a, 37);
    ASSERT_EQ(initialState.x, 162);
    ASSERT_EQ(finalState.x, 162);
    ASSERT_EQ(initialState.y, 170);
    ASSERT_EQ(finalState.y, 170);
    ASSERT_EQ(initialState.p, 170);
    ASSERT_EQ(finalState.p, 40);

    const auto& initialRam = initialState.ram;
    const auto& finalRam = finalState.ram;

    ASSERT_EQ(initialRam.at(9023), 177);
    ASSERT_EQ(initialRam.at(9024), 113);
    ASSERT_EQ(finalRam.at(113), 169);
    ASSERT_EQ(finalRam.at(114), 89);
}
