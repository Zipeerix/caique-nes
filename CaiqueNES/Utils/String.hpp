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

#ifndef CAIQUE_NES_STRING_HPP
#define CAIQUE_NES_STRING_HPP

#include <type_traits>
#include <optional>
#include <string>
#include "Utils/Types.hpp"

namespace Utils {
    template<NumericType T>
    std::string convertToHexString(T value, bool addPrefix = false, std::optional<int> forcedSize = std::nullopt);
}

#include "Utils/String.tpp"

#endif //CAIQUE_NES_STRING_HPP
