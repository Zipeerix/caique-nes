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

#ifndef CAIQUE_NES_FRAMEBUFFER_TPP
#define CAIQUE_NES_FRAMEBUFFER_TPP

#include "Graphics/FrameBuffer.hpp"
#include "Utils/Data.hpp"

constexpr static Graphics::PixelColor createColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = UINT8_MAX) {
    return (static_cast<Graphics::PixelColor>(a << 24)) |
           (static_cast<Graphics::PixelColor>(r << 16)) |
           (static_cast<Graphics::PixelColor>(g << 8))  |
           b;
}

template <int width, int height>
bool Graphics::FrameBuffer<width, height>::withinBounds(int x, int y) const {
    return (x < width && y < height);
}

template <int width, int height>
void Graphics::FrameBuffer<width, height>::updatePixel(int x, int y, PixelColor rawValue) {
    m_pixels.at(Utils::convert2DIndexTo1DIndex(width, x, y)) = rawValue;
}

template <int width, int height>
void Graphics::FrameBuffer<width, height>::copyToTexture(Graphics::Texture& targetTexture) const {
    targetTexture.update(reinterpret_cast<const PixelColor*>(m_pixels.data()), width * sizeof(PixelColor));
}

#endif //CAIQUE_NES_FRAMEBUFFER_TPP
