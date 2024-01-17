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

#ifndef CAIQUE_NES_BITINDEXEDVALUE_TPP
#define CAIQUE_NES_BITINDEXEDVALUE_TPP

#include "Utils/BitIndexedValue.hpp"
#include "Utils/Data.hpp"

template <typename ValueType, typename IndexEnum>
Utils::BitIndexedValue<ValueType, IndexEnum>::BitIndexedValue(ValueType initialValue) :
    m_rawValue(initialValue)
{
}

template <typename ValueType, typename IndexEnum>
bool Utils::BitIndexedValue<ValueType, IndexEnum>::isBitSet(IndexEnum index) const {
    return Utils::isBitSet(m_rawValue, static_cast<int>(index));
}

template <typename ValueType, typename IndexEnum>
void Utils::BitIndexedValue<ValueType, IndexEnum>::setBit(IndexEnum bitIndex) {
    m_rawValue = Utils::setBit(m_rawValue, static_cast<int>(bitIndex));
}

template <typename ValueType, typename IndexEnum>
void Utils::BitIndexedValue<ValueType, IndexEnum>::setBitTo(IndexEnum bitIndex, bool value) {
    m_rawValue = Utils::setBitTo(m_rawValue, static_cast<int>(bitIndex), value);
}

template <typename ValueType, typename IndexEnum>
void Utils::BitIndexedValue<ValueType, IndexEnum>::clearBit(IndexEnum bitIndex) {
    m_rawValue = Utils::clearBit(m_rawValue, static_cast<int>(bitIndex));
}

template <typename ValueType, typename IndexEnum>
ValueType Utils::BitIndexedValue<ValueType, IndexEnum>::getCombinedValue() const {
    return m_rawValue;
}

template <typename ValueType, typename IndexEnum>
void Utils::BitIndexedValue<ValueType, IndexEnum>::setCombinedValue(ValueType newValue) {
    m_rawValue = newValue;
}

#endif //CAIQUE_NES_BITINDEXEDVALUE_TPP
