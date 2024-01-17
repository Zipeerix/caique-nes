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

#include "Renderer.hpp"

Graphics::Renderer::Renderer(SDL_Renderer* renderer) :
    m_sdlRenderer(renderer, SDL_DestroyRenderer)
{
}

Graphics::Renderer::Renderer(const Window& window, const std::vector<RendererFlags>& flags) :
    m_sdlRenderer(SDL_CreateRenderer(window.wrappedObject(), -1, converToSDLFlags(flags)), SDL_DestroyRenderer)
{
}

bool Graphics::Renderer::isValid() const {
    return m_sdlRenderer != nullptr;
}

SDL_Renderer* Graphics::Renderer::wrappedObject() const {
    return m_sdlRenderer.get();
}

void Graphics::Renderer::setRenderDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) {
    SDL_SetRenderDrawColor(m_sdlRenderer.get(), r, g, b, a);
}

void Graphics::Renderer::setRenderDrawColor(Graphics::PixelColor pixelColor) {
    const std::uint8_t b = (pixelColor & 0x000000FF);
    const std::uint8_t g = (pixelColor & 0x0000FF00) >> 8;
    const std::uint8_t r = (pixelColor & 0x00FF0000) >> 16;
    const std::uint8_t a = (pixelColor & 0xFF000000) >> 24;

    setRenderDrawColor(r, g, b, a);
}

void Graphics::Renderer::copyTexture(const Graphics::Texture& texture) {
    SDL_RenderCopy(m_sdlRenderer.get(), texture.wrappedObject(), nullptr, nullptr);
}

void Graphics::Renderer::present() {
    SDL_RenderPresent(m_sdlRenderer.get());
}

void Graphics::Renderer::clear() {
    SDL_RenderClear(m_sdlRenderer.get());
}

std::int32_t Graphics::Renderer::converToSDLFlags(const std::vector<RendererFlags>& flags) {
    std::int32_t rawFlags = 0;

    for (const auto& flag : flags) {
        rawFlags |= static_cast<std::int32_t>(flag);
    }

    return rawFlags;
}
