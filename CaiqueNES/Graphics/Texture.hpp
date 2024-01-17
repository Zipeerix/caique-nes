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

#ifndef CAIQUE_NES_TEXTURE_HPP
#define CAIQUE_NES_TEXTURE_HPP

#include <SDL2/SDL.h>
#include <cstdint>
#include "Utils/Types.hpp"

namespace Graphics {
    class Renderer;

    enum class TextureFormat {
        ARGBBytes = SDL_PIXELFORMAT_ARGB8888
    };

    enum class TextureAccess {
        Static = SDL_TEXTUREACCESS_STATIC
    };

    class Texture {
    public:
        explicit Texture(SDL_Texture* texture);
        Texture(const Graphics::Renderer& renderer, TextureFormat format, TextureAccess access, int width, int height);

        bool isValid() const;
        SDL_Texture* wrappedObject() const;

        void update(const PixelColor* pixels, int widthBytes);

    private:
        Utils::CTypeUniquePtr<SDL_Texture> m_sdlTexture;
    };
}

#endif //CAIQUE_NES_TEXTURE_HPP
