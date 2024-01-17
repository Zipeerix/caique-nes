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

#include "Utils/Data.hpp"

Nes::Byte Utils::getUpperByte(Nes::Word value) {
    return (value >> 8) & 0xFF;
}

Nes::Byte Utils::getLowerByte(Nes::Word value) {
    return value & 0xFF;
}

Nes::Word Utils::combineBytes(Nes::Byte upper, Nes::Byte lower) {
    return (upper << 8) | lower;
}

Nes::Byte Utils::combineBits(bool firstBit, bool secondBit) {
    return secondBit | firstBit << 1;
}
