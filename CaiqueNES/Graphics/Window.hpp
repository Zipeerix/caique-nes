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

#ifndef CAIQUE_NES_WINDOW_HPP
#define CAIQUE_NES_WINDOW_HPP

#include <SDL2/SDL.h>
#include <string>
#include "Utils/Types.hpp"

namespace Graphics {
    class Window {
    public:
        static Window fromExternalSource(Utils::GenericMemoryPtr ptr);

        explicit Window(SDL_Window* window);
        Window(std::string title, int windowWidth, int windowHeight);

        bool isValid() const;

        SDL_Window* wrappedObject() const;

    private:
        std::string m_title;
        Utils::CTypeUniquePtr<SDL_Window> m_sdlWindow = nullptr;
    };
}

#endif //CAIQUE_NES_WINDOW_HPP
