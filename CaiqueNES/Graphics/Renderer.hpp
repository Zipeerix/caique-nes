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

#ifndef CAIQUE_NES_RENDERER_HPP
#define CAIQUE_NES_RENDERER_HPP

#include <SDL2/SDL.h>
#include "Graphics/FrameBuffer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Window.hpp"
#include "Utils/Types.hpp"

namespace Graphics {
    enum class RendererFlags {
        Accelerated = SDL_RENDERER_ACCELERATED
    };

    class Renderer {
    public:
        explicit Renderer(SDL_Renderer* renderer);
        Renderer(const Window& window, const std::vector<RendererFlags>& flags);

        bool isValid() const;
        SDL_Renderer* wrappedObject() const;

        void setRenderDrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = UINT8_MAX);
        void setRenderDrawColor(PixelColor pixelColor);

        void copyTexture(const Graphics::Texture& texture);

        void present();
        void clear();

    private:
        Utils::CTypeUniquePtr<SDL_Renderer> m_sdlRenderer;

        static std::int32_t converToSDLFlags(const std::vector<RendererFlags>& flags);
    };
}

#endif //CAIQUE_NES_RENDERER_HPP
