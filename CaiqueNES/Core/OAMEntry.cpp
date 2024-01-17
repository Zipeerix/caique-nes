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

#include "OAMEntry.hpp"

Nes::OAMEntry::OAMEntry(const std::array<Byte, Const::oamEntrySize>& bytes) :
    m_spriteId(bytes[Const::OAMByteIndex::spriteId]),
    m_attributes(bytes[Const::OAMByteIndex::attributes]),
    m_internalPosition({bytes[Const::OAMByteIndex::xPosition], bytes[Const::OAMByteIndex::yPosition]})
{
}

Nes::Byte Nes::OAMEntry::getSpriteId() const {
    return m_spriteId;
}

int Nes::OAMEntry::getPaletteId() const {
    const auto firstBit  = m_attributes.isBitSet(SpriteAttribute::Palette);
    const auto secondBit = m_attributes.isBitSet(SpriteAttribute::PaletteSecondBit);

    return (2 * secondBit) + firstBit;
}

Graphics::Position Nes::OAMEntry::getPosition(int pixelX, int pixelY) const {
    int x = m_internalPosition.x;
    x += m_attributes.isBitSet(SpriteAttribute::FlipHorizontal) ? -pixelX : pixelX;
    x += m_attributes.isBitSet(SpriteAttribute::FlipHorizontal) ? Const::flipOffset : 0;

    int y = m_internalPosition.y;
    y += m_attributes.isBitSet(SpriteAttribute::FlipVertical) ? Const::flipOffset : 0;
    y += m_attributes.isBitSet(SpriteAttribute::FlipVertical) ? -pixelY : pixelY;

    return {x, y};
}
