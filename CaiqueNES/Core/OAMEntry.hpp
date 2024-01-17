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

#ifndef CAIQUE_NES_OAMENTRY_HPP
#define CAIQUE_NES_OAMENTRY_HPP

#include <array>
#include "Utils/BitIndexedValue.hpp"
#include "Utils/Types.hpp"

namespace Nes {
    namespace Const {
        constexpr int oamEntrySize = 4;
        constexpr int flipOffset   = 7;

        namespace OAMByteIndex {
            constexpr int yPosition  = 0;
            constexpr int spriteId   = 1;
            constexpr int attributes = 2;
            constexpr int xPosition  = 3;
        }
    }

    enum class SpriteAttribute {
        Palette,
        PaletteSecondBit,
        Priority = 5,
        FlipHorizontal,
        FlipVertical
    };

    class OAMEntry {
    public:
        explicit OAMEntry(const std::array<Byte, Const::oamEntrySize>& bytes);

        Byte getSpriteId() const;
        int getPaletteId() const;
        Graphics::Position getPosition(int pixelX, int pixelY) const;

    private:
        Byte m_spriteId;
        Utils::BitIndexedValue<Byte, SpriteAttribute> m_attributes;
        Graphics::Position m_internalPosition{};
    };
}

#endif //CAIQUE_NES_OAMENTRY_HPP
