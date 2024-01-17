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

#include "Graphics/Renderer.hpp"
#include "Texture.hpp"

Graphics::Texture::Texture(SDL_Texture* texture) :
    m_sdlTexture(texture, SDL_DestroyTexture)
{
}

Graphics::Texture::Texture(const Graphics::Renderer& renderer, TextureFormat format, TextureAccess access, int width, int height) :
    m_sdlTexture(SDL_CreateTexture(renderer.wrappedObject(), static_cast<std::int32_t>(format),
                                   static_cast<std::int32_t>(access), width, height), SDL_DestroyTexture)
{
}

bool Graphics::Texture::isValid() const {
    return m_sdlTexture != nullptr;
}

SDL_Texture* Graphics::Texture::wrappedObject() const {
    return m_sdlTexture.get();
}

void Graphics::Texture::update(const PixelColor* pixels, int widthBytes) {
    SDL_UpdateTexture(m_sdlTexture.get(), nullptr, pixels, widthBytes);
}
