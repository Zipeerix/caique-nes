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

#ifndef CAIQUE_NES_NESTEST_TESTUTILS_HPP
#define CAIQUE_NES_NESTEST_TESTUTILS_HPP

#include <fstream>
#include <vector>
#include <string>
#include "Utils/Types.hpp"

namespace NesTestUtils {
    struct Entry {
        Nes::Addr programCounter;
        Nes::Byte stackPointer;
        Nes::Byte accumulator;
        Nes::Byte xIndex;
        Nes::Byte yIndex;
        Nes::Byte status;

        int scanline;
        int ppuCycles;
        int cpuCycles;
    };

    static std::vector<std::string> splitStringByDelim(const std::string& s, char delim) {
        std::vector<std::string> result;
        std::stringstream stream(s);
        std::string item;

        while (getline(stream, item, delim)) {
            result.push_back(item);
        }

        return result;
    }

    static std::vector<Entry> readTestLog() {
        const auto filePath = TEST_LOG_FILE_NESTEST;
        std::ifstream file(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        const std::string fileContent = buffer.str();

        std::vector<std::string> lines;
        std::stringstream stream(fileContent);

        for (std::string line; std::getline(stream, line, '\n');)
            lines.push_back(line);

        std::vector<Entry> entries;
        for (const auto& line : lines) {
            Entry entry{};

            /* CPU */
            std::string pcString          = line.substr(0, 4);
            std::string accumulatorString = line.substr(50, 2);
            std::string xString           = line.substr(55, 2);
            std::string yString           = line.substr(60, 2);
            std::string statusString      = line.substr(65, 2);
            std::string spString          = line.substr(71, 2);
            std::string cpuCyclesString   = line.substr(90);

            entry.programCounter = std::stoi(pcString, nullptr, 16);
            entry.stackPointer   = std::stoi(spString, nullptr, 16);
            entry.accumulator    = std::stoi(accumulatorString, nullptr, 16);
            entry.xIndex         = std::stoi(xString, nullptr, 16);
            entry.yIndex         = std::stoi(yString, nullptr, 16);
            entry.status         = std::stoi(statusString, nullptr, 16);
            entry.cpuCycles       = std::stoi(cpuCyclesString);

            /* PPU */
            std::string ppuValuesString = line.substr(78, 7);
            std::vector<std::string> ppuValues = splitStringByDelim(ppuValuesString, ',');

            entry.scanline     = std::stoi(ppuValues.at(0));
            entry.ppuCycles    = std::stoi(ppuValues.at(1));

            entries.push_back(entry);
        }

        return entries;
    }
}

#endif //CAIQUE_NES_NESTEST_TESTUTILS_HPP
