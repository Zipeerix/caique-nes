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

#include "Tile.hpp"
#include "Utils/Data.hpp"

Nes::Tile::Tile(std::array<Byte, Const::tileSize> bytes, const Palette& palette) :
    m_bytes(std::move(bytes)),
    m_palette(palette)
{
}

bool Nes::Tile::skipDrawingSpritePixel(int x, int y) const {
    return getColorId(x, y) == 0;
}

Graphics::PixelColor Nes::Tile::getColor(int x, int y) const {
    const auto colorId = getColorId(x, y);
    return m_palette.getColor(colorId);
}

int Nes::Tile::getColorId(int x, int y) const {
    const auto shiftMagnitude = (Const::tileDimension - 1) - x;

    const Byte lower = m_bytes[y]                        >> shiftMagnitude;
    const Byte upper = m_bytes[y + Const::tileDimension] >> shiftMagnitude;

    const auto colorId = Utils::combineBits(Utils::isBitSet(upper, 0), Utils::isBitSet(lower, 0));

    return colorId;
}
