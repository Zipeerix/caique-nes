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

#ifndef CAIQUE_NES_6502_TESTUTILS_HPP
#define CAIQUE_NES_6502_TESTUTILS_HPP

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "rapidjson/document.h"
#include "Utils/String.hpp"
#include "Utils/Types.hpp"

namespace Cpu6502TestUtils {
    struct CpuState {
        Nes::Addr pc;
        Nes::Byte s;
        Nes::Byte a;
        Nes::Byte x;
        Nes::Byte y;
        Nes::Byte p;
        std::map<Nes::Addr, Nes::Byte> ram;
    };

    struct Cycle {
        Nes::Addr address;
        Nes::Byte value;
        std::string operation;
    };

    struct Cpu6502Test {
        std::string name;
        CpuState initial;
        CpuState final;
        std::vector<Cycle> cycles;
    };

    static CpuState parseState(const auto& json) {
        CpuState state;

        state.pc = json["pc"].GetInt();
        state.s  = json["s"].GetInt();
        state.a  = json["a"].GetInt();
        state.x  = json["x"].GetInt();
        state.y  = json["y"].GetInt();
        state.p  = json["p"].GetInt();

        for (const auto& jsonRamEntry : json["ram"].GetArray()) {
            state.ram.insert({jsonRamEntry[0].GetInt(), jsonRamEntry[1].GetInt()});
        }

        return state;
    }

    static std::vector<Cpu6502Test> readTest(const std::string& fileContent) {
        rapidjson::Document json;
        json.Parse(fileContent.c_str());

        std::vector<Cpu6502Test> tests;
        assert(json.IsArray());
        for (const auto& testJson: json.GetArray())
        {
            Cpu6502Test testStructure{};

            // Name
            testStructure.name = testJson["name"].GetString();

            // Cycles
            assert(testJson["cycles"].IsArray());
            for (const auto& jsonCycle: testJson["cycles"].GetArray()) {
                auto jsonCycleArray = jsonCycle.GetArray();

                Cycle cycle{};
                cycle.address = jsonCycleArray[0].GetInt();
                cycle.value = jsonCycleArray[1].GetInt();
                cycle.operation = jsonCycleArray[2].GetString();

                testStructure.cycles.push_back(cycle);
            }

            // States
            testStructure.initial = parseState(testJson["initial"]);
            testStructure.final   = parseState(testJson["final"]);

            tests.push_back(testStructure);
        }

        return tests;
    }

    static std::vector<Cpu6502Test> readTestForInstruction(std::uint8_t opcode) {
        const std::string filePath = TEST_DIR_6502 + Utils::convertToHexString(opcode, false, 2) + ".json";
        std::ifstream file(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        const std::string fileContent = buffer.str();

        return readTest(fileContent);
    }
}

#endif //CAIQUE_NES_6502_TESTUTILS_HPP
