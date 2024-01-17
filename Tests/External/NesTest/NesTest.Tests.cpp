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
#include "NesTest.TestUtils.hpp"

#define TESTING_ENVIRONMENT_NESTEST 1
#include "Core/VirtualMachine.hpp"
#undef TESTING_ENVIRONMENT_NESTEST

TEST(NesTest, Standard) {
    const auto logEntries = NesTestUtils::readTestLog();
    ASSERT_FALSE(logEntries.empty());

    Nes::VirtualMachine virtualMachine{[](auto){}};
    const auto romLoadResult = virtualMachine.loadRom(TEST_ROM_FILE_NESTEST);
    ASSERT_TRUE(romLoadResult.has_value()) << romLoadResult.error();

    virtualMachine.accessCPU()->setAutomatedMode();

    for (auto i = 0; i < logEntries.size(); i++) {
        const auto& entry = logEntries.at(i);

        const auto cpuComparisonResult = virtualMachine.accessCPU()->matchesEntry(entry);
        ASSERT_TRUE(cpuComparisonResult.first) << "(CPU) In entry at line: " << std::to_string(i + 1) << " -> "
                                                << cpuComparisonResult.second;

        const auto ppuComparisonResult = virtualMachine.accessPPU()->matchesEntry(entry);
        ASSERT_TRUE(ppuComparisonResult.first) << "(PPU) In entry at line: " << std::to_string(i + 1) << " -> "
                                               << ppuComparisonResult.second;

        (void) virtualMachine.accessCPU()->tick();
    }
}
