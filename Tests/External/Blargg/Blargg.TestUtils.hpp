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

#ifndef CAIQUE_NES_BLARGG_TESTUTILS_HPP
#define CAIQUE_NES_BLARGG_TESTUTILS_HPP

#include <expected>
#include <string>

#define TESTING_ENVIRONMENT_BLARGG 1
#include "Core/VirtualMachine.hpp"
#undef TESTING_ENVIRONMENT_BLARGG

namespace BlarggUtils {
    static std::expected<std::string, std::string> runTest(const std::string& category, const std::string& name) {
        const auto filePath = std::string(TEST_DIR_BLARGG) + category + "/Roms/" + name + ".nes";

        Nes::VirtualMachine vm([&](auto) {});

        const auto loadRomResult = vm.loadRom(filePath);
        if (!loadRomResult.has_value()) {
            return std::unexpected("Unable to load rom due to: " + loadRomResult.error());
        }

        for (int i = 0; i < 10000000; i++) {
            (void) vm.accessCPU()->tick();
        }

        return vm.accessCPU()->textOutput();
    }
}

#endif //CAIQUE_NES_BLARGG_TESTUTILS_HPP
