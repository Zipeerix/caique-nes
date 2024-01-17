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

#ifndef COCKATOO_BOY_DATA_TPP
#define COCKATOO_BOY_DATA_TPP

#include "Utils/Data.hpp"

template <Utils::IntegerType T>
bool Utils::isBitSet(T value, int bitIndex) {
    return value & (static_cast<T>(1) << bitIndex);
}

template <Utils::IntegerType T>
T Utils::setBit(T value, int bitIndex) {
    return value | (static_cast<T>(1) << bitIndex);
}

template <Utils::IntegerType T>
T Utils::clearBit(T value, int bitIndex) {
    return value & ~(static_cast<T>(1) << bitIndex);
}

template <Utils::IntegerType T>
T Utils::setBitTo(T value, int bitIndex, bool set) {
    return set ? setBit(value, bitIndex) : clearBit(value, bitIndex);
}

template <Utils::IntegerType T>
T Utils::isValueNegative(T value) {
    return isBitSet(value, (sizeof(T) * 8) - 1);
}

#endif //COCKATOO_BOY_DATA_TPP
