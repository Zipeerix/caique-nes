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

#ifndef CAIQUE_NES_TIMING_HPP
#define CAIQUE_NES_TIMING_HPP

#include <SDL2/SDL.h>
#include <cstdint>

namespace Graphics {
    using MomentInTime = std::uint64_t;

    class FrameRateBlocker {
    public:
        explicit FrameRateBlocker(int frameRate);

        void startOfLoop();
        void endOfLoop();

    private:
        double m_frameTime = 0;

        MomentInTime m_startTime = 0;
        MomentInTime m_endTime = 0;

        void waitUntilFrameTime() const;
    };
}

#endif //CAIQUE_NES_TIMING_HPP
