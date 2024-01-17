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
#ifndef CAIQUE_NES_STRING_TPP
#define CAIQUE_NES_STRING_TPP

#include <sstream>
#include <iomanip>
#include "Utils/Types.hpp"
#include "Utils/String.hpp"

template<Utils::NumericType T>
std::string Utils::convertToHexString(T value, bool addPrefix, std::optional<int> forcedSize) {
    std::stringstream ss;

    if (addPrefix) {
        ss << "0x";
    }

    if (forcedSize.has_value()) {
        ss << std::setfill('0') << std::setw(forcedSize.value());
    }

    ss << std::uppercase << std::hex << static_cast<uint64_t>(value);

    return ss.str();
}

#endif //CAIQUE_NES_STRING_TPP
