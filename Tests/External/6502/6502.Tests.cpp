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

#include <algorithm>
#include <sstream>
#include <gtest/gtest.h>

#define TESTING_ENVIRONMENT_6502 1
#include "Core/MMU.hpp"
#include "Core/CPU.hpp"
#include "Core/PPU.hpp"
#include "6502.TestUtils.hpp"
#undef TESTING_ENVIRONMENT_6502

// Testing is long in debug mode, use release mode or change below setting to >10
#ifndef EVERY_NTH_TEST_6502 // Add to test index every iteration, see above for reasoning
    #define EVERY_NTH_TEST_6502 1
#endif
#ifndef STARTING_TEST_6502 // First test to run
    #define STARTING_TEST_6502 0
#endif
#ifndef EXCLUDE_FROM_COVERAGE_6502 // Tests excluded from coverage check (unstable unofficial)
    #define EXCLUDE_FROM_COVERAGE_6502 0x9C, 0x93, 0x9F, 0x9E, 0x9B, 0xAB, 0x6B
#endif

static std::array<bool, 256> testedOpcodes{false};

std::string getOpcodeCoverage() {
    const auto testedOpcodesCount = std::count_if(testedOpcodes.cbegin(), testedOpcodes.cend(), [](bool tested) {
        return tested;
    });

    std::vector<Nes::Byte> excludedOpcodes = {EXCLUDE_FROM_COVERAGE_6502};
    const auto opcodesToTest = static_cast<double>(testedOpcodes.size() - excludedOpcodes.size());
    return std::to_string(100 * (static_cast<double>(testedOpcodesCount) / opcodesToTest));
}

std::pair<bool, std::string> testOpcodes(const std::vector<Nes::Byte>& opcodes) {
    for (const auto& opcode : opcodes) {
        const auto tests = Cpu6502TestUtils::readTestForInstruction(opcode);
        for (auto testId = STARTING_TEST_6502; testId < tests.size(); testId += EVERY_NTH_TEST_6502) {
            const auto& test = tests[testId];

            Nes::Cartridge mockCartridge;
            Nes::MMU mmu(mockCartridge);
            Nes::PPU ppu(mmu, [](auto){});

            mmu.clearMemoryRegions();
            mmu.addMemoryRegion(Utils::Range<Nes::Addr>{0x0000, 0xFFFF});

            Nes::CPU cpu(mmu, ppu);

            mmu.loadState(test.initial);
            cpu.loadState(test.initial);

            if (!cpu.executeNextInstructionInIsolation()) {
                return {false, "Unhandled opcode: " + Utils::convertToHexString(opcode, true, 2)};
            }

            const auto mmuComparisonResult = mmu.matchesState(test.final);
            const auto cpuComparisonResult = cpu.matchesState(test.final);

            std::stringstream failureStream;
            if (!mmuComparisonResult.first) {
                failureStream << "(MMU Fail) " << "'" + test.name << "'" << " Opcode: "
                   << Utils::convertToHexString(opcode, true, 2)
                   << ", Test: " << std::to_string(testId) << " Error: " << mmuComparisonResult.second << "\n";
            }

            if (!cpuComparisonResult.first) {
                failureStream << "(CPU Fail) " << "'" + test.name << "'" << " Opcode: "
                   << Utils::convertToHexString(opcode, true, 2)
                   << ", Test: " << std::to_string(testId) << " Error: " << cpuComparisonResult.second;
            }

            if (!cpuComparisonResult.first || !mmuComparisonResult.first) {
                return {false, failureStream.str()};
            }
        }

        if (testedOpcodes[opcode]) {
            throw std::logic_error("Duplicate opcode test: " + Utils::convertToHexString(opcode, true, 2));
        }

        testedOpcodes[opcode] = true;
    }

    return {true, ""};
}

TEST(Cpu6502, Instruction_Illegal_Ane) {
    auto result = testOpcodes({0x8B});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Alr) {
    auto result = testOpcodes({0x4B});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Sax) {
    auto result = testOpcodes({0x87, 0x97, 0x8F, 0x83});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Lax) {
    auto result = testOpcodes({0xA7, 0xB7, 0xAF, 0xBF, 0xA3, 0xB3});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Dcp) {
    auto result = testOpcodes({0xC7, 0xD7, 0xCF, 0xDF, 0xDB, 0xD3, 0xC3});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Anc) {
    auto result = testOpcodes({0x0B, 0x2B});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Sbx) {
    auto result = testOpcodes({0xCB});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Usbc) {
    auto result = testOpcodes({0xEB});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Las) {
    auto result = testOpcodes({0xBB});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Isc) {
    auto result = testOpcodes({0xE7, 0xF7, 0xEF, 0xFF, 0xFB, 0xE3, 0xF3});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Rra) {
    auto result = testOpcodes({0x67, 0x77, 0x6F, 0x7F, 0x7B, 0x63, 0x73});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Sre) {
    auto result = testOpcodes({0x47, 0x57, 0x4F, 0x5F, 0x5B, 0x43, 0x53});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Rla) {
    auto result = testOpcodes({0x27, 0x37, 0x2F, 0x3F, 0x3B, 0x33, 0x23});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Slo) {
    auto result = testOpcodes({0x07, 0x17, 0x0F, 0x1F, 0x1B, 0x03, 0x13});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Illegal_Jam) {
    auto result = testOpcodes({0x02, 0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x92, 0xB2, 0xD2, 0xF2});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Break) {
    auto result = testOpcodes({0x00});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Branch) {
    auto result = testOpcodes({0x10, 0x30, 0x50, 0x70, 0x90, 0xB0, 0xD0, 0xF0});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Stack) {
    auto result = testOpcodes({0x9A, 0xBA, 0x48, 0x68, 0x08, 0x28});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Transfer) {
    auto result = testOpcodes({0xAA, 0x8A, 0xA8, 0x98});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Call) {
    auto result = testOpcodes({0x20});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Returns) {
    auto result = testOpcodes({0x40, 0x60});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Jump) {
    auto result = testOpcodes({0x4C, 0x6C});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Increment) {
    auto result = testOpcodes({0xE6, 0xF6, 0xEE, 0xFE, 0xE8, 0xC8});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Decrement) {
    auto result = testOpcodes({0xC6, 0xD6, 0xCE, 0xDE, 0xCA, 0x88});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Noop) {
    auto result = testOpcodes({
        0x04, 0x44, 0x64, 0x14, 0x34, 0x54, 0x74, 0xD4, 0xF4, 0x0C, 0x1C, 0x3C,
        0x5C, 0x7C, 0xDC, 0xFC, 0xEA, 0x1A, 0x3A, 0x5A, 0x7A, 0xDA, 0xFA, 0x80,
        0x82, 0x89, 0xC2, 0xE2
    });
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Load) {
    auto result = testOpcodes({0xA9, 0xA5, 0xB5, 0xAD, 0xBD, 0xB9, 0xA1, 0xB1, 0xA2, 0xA6, 0xB6, 0xAE, 0xBE,
                               0xA0, 0xA4, 0xB4, 0xAC, 0xBC});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Rotations) {
    auto result = testOpcodes({0x2A, 0x26, 0x36, 0x2E, 0x3E, 0x6A, 0x66, 0x76, 0x6E, 0x7E});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Comparison) {
    auto result = testOpcodes({0xC9, 0xC5, 0xD5, 0xCD, 0xDD, 0xD9, 0xC1, 0xD1, 0xC0, 0xC4, 0xCC, 0xE0, 0xE4, 0xEC});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Bitwise) {
    auto result = testOpcodes({0x29, 0x25, 0x35, 0x2D, 0x3D, 0x39, 0x21, 0x31, 0x49, 0x45, 0x55, 0x4D, 0x5D, 0x59,
                               0x41, 0x51, 0x09, 0x05, 0x15, 0x0D, 0x1D, 0x19, 0x01, 0x11});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Shifts) {
    auto result = testOpcodes({0x0A, 0x06, 0x16, 0x0E, 0x1E, 0x4A, 0x46, 0x56, 0x4E, 0x5E   });
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Bit) {
    auto result = testOpcodes({0x24, 0x2C});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Adc) {
    auto result = testOpcodes({0x69, 0x65, 0x75, 0x6D, 0x7D, 0x79, 0x61, 0x71});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Sbc) {
    auto result = testOpcodes({0xE9, 0xE5, 0xF5, 0xED, 0xFD, 0xF9, 0xE1, 0xF1});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Flag) {
    auto result = testOpcodes({0x18, 0x38, 0x58, 0x78, 0xB8, 0xD8, 0xF8});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, Instruction_Store) {
    auto result = testOpcodes({0x86, 0x96, 0x8E, 0x84, 0x94, 0x8C, 0x85, 0x95, 0x8D, 0x9D, 0x99, 0x81, 0x91});
    ASSERT_TRUE(result.first) << result.second;
}

TEST(Cpu6502, CoverageCheck) {
    std::vector<Nes::Byte> excludedOpcodes{EXCLUDE_FROM_COVERAGE_6502};
    std::vector<Nes::Byte> untestedOpcodes;
    for (auto i = 0; i < testedOpcodes.size(); i++) {
        const auto opcode = i;
        if (!testedOpcodes[opcode]) {
            if (std::find(excludedOpcodes.begin(), excludedOpcodes.end(), opcode) != excludedOpcodes.end()) {
                continue;
            }

            untestedOpcodes.push_back(opcode);
        }
    }

    std::stringstream untestedOpcodesStr;
    for (auto i = 0; i < untestedOpcodes.size(); i++) {
        const auto opcode = untestedOpcodes[i];
        untestedOpcodesStr << Utils::convertToHexString(opcode, true, 2) << " ";

        if (i % 20 == 0) {
            untestedOpcodesStr << "\n";
        }
    }

    ASSERT_TRUE(untestedOpcodes.empty()) << "Coverage: " << getOpcodeCoverage() << "%\n" << untestedOpcodesStr.str();
}

#undef TESTING_ENVIRONMENT_6502
