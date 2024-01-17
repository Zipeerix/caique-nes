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

#ifndef CAIQUE_NES_DATA_HPP
#define CAIQUE_NES_DATA_HPP

#include <type_traits>
#include "Utils/Types.hpp"

namespace Utils {
    constexpr uint64_t kilobytesToBytes(uint64_t kiloBytes) {
        return kiloBytes * 1024;
    }

    constexpr int convert2DIndexTo1DIndex(int width, int x, int y) {
        return (y * width) + x;
    }

    template <IntegerType T>
    bool isBitSet(T value, int bitIndex);

    template <IntegerType T>
    T setBit(T value, int bitIndex);

    template <IntegerType T>
    T clearBit(T value, int bitIndex);

    template <IntegerType T>
    T setBitTo(T value, int bitIndex, bool set);

    template <IntegerType T>
    T isValueNegative(T value);

    Nes::Byte getLowerByte(Nes::Word value);
    Nes::Byte getUpperByte(Nes::Word value);

    Nes::Word combineBytes(Nes::Byte upper, Nes::Byte lower);

    Nes::Byte combineBits(bool firstBit, bool secondBit);
}

#include "Utils/Data.tpp"

#endif // CAIQUE_NES_DATA_HPP
