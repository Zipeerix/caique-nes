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

#include "Window.hpp"

Graphics::Window Graphics::Window::fromExternalSource(Utils::GenericMemoryPtr ptr) {
    return Window(SDL_CreateWindowFrom(ptr));
}

Graphics::Window::Window(SDL_Window* window) :
    m_sdlWindow(window, SDL_DestroyWindow)
{
}

Graphics::Window::Window(std::string title, int windowWidth, int windowHeight) :
    m_title(std::move(title)),
    m_sdlWindow(SDL_CreateWindow(m_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0), SDL_DestroyWindow)
{
}

bool Graphics::Window::isValid() const {
    return m_sdlWindow != nullptr;
}

SDL_Window* Graphics::Window::wrappedObject() const {
    return m_sdlWindow.get();
}
