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

#ifndef CAIQUE_NES_RANGE_TPP
#define CAIQUE_NES_RANGE_TPP

#include <stdexcept>
#include "Utils/Range.hpp"

template <Utils::IntegerType BoundType>
bool Utils::Range<BoundType>::isValueWithin(BoundType value) const {
    return value >= from && value <= to;
}

template <Utils::IntegerType BoundType>
std::size_t Utils::Range<BoundType>::extent() const {
    return static_cast<std::size_t>(to - from) + 1;
}


#endif // CAIQUE_NES_RANGE_TPP
