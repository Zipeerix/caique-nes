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

#include "Palette.hpp"

static Graphics::PixelColor systemPalette[Nes::Const::systemPaletteSize] = {
    0xFF808080, 0xFF003DA6, 0xFF0012B0,
    0xFF440096, 0xFFA1005E, 0xFFC70028,
    0xFFBA0600, 0xFF8C1700, 0xFF5C2F00,
    0xFF104500, 0xFF054A00, 0xFF00472E,
    0xFF004166, 0xFF000000, 0xFF050505,
    0xFF050505, 0xFFC7C7C7, 0xFF0077FF,
    0xFF2155FF, 0xFF8237FA, 0xFFEB2FB5,
    0xFFFF2950, 0xFFFF2200, 0xFFD63200,
    0xFFC46200, 0xFF358000, 0xFF058F00,
    0xFF008A55, 0xFF0099CC, 0xFF212121,
    0xFF090909, 0xFF090909, 0xFFFFFFFF,
    0xFF0FD7FF, 0xFF69A2FF, 0xFFD480FF,
    0xFFFF45F3, 0xFFFF618B, 0xFFFF8833,
    0xFFFF9C12, 0xFFFABC20, 0xFF9FE30E,
    0xFF2BF035, 0xFF0CF0A4, 0xFF05FBFF,
    0xFF5E5E5E, 0xFF0D0D0D, 0xFF0D0D0D,
    0xFFFFFFFF, 0xFFA6FCFF, 0xFFB3ECFF,
    0xFFDAABEB, 0xFFFFA8F9, 0xFFFFABB3,
    0xFFFFD2B0, 0xFFFFEFA6, 0xFFFFF79C,
    0xFFD7E895, 0xFFA6EDAF, 0xFFA2F2DA,
    0xFF99FFFC, 0xFFDDDDDD, 0xFF111111,
    0xFF111111,
};

Nes::Palette::Palette(std::array<Byte, Const::colorIndexCount> indexes) :
    m_colorIndexes(std::move(indexes))
{
}

Graphics::PixelColor Nes::Palette::getColor(int id) const {
    return systemPalette[m_colorIndexes[id]];
}
