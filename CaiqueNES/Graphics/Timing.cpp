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

#include <cmath>
#include "Timing.hpp"

Graphics::FrameRateBlocker::FrameRateBlocker(int frameRate) :
    m_frameTime(1000.0 / frameRate)
{
}

void Graphics::FrameRateBlocker::startOfLoop() {
    m_startTime = SDL_GetPerformanceCounter();
}

void Graphics::FrameRateBlocker::endOfLoop() {
    m_endTime = SDL_GetPerformanceCounter();

    waitUntilFrameTime();
}

void Graphics::FrameRateBlocker::waitUntilFrameTime() const {
    const float elapsed = (float) (m_endTime - m_startTime) / (float) SDL_GetPerformanceFrequency() * 1000.0f;
    SDL_Delay(static_cast<std::uint32_t>(std::floor(m_frameTime - elapsed)));
}
