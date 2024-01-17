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

#ifndef CAIQUE_NES_GAMEWIDGET_HPP
#define CAIQUE_NES_GAMEWIDGET_HPP

#include <QWidget>
#include <thread>
#include <atomic>
#include <memory>
#include "Core/VirtualMachine.hpp"
#include "Graphics/Window.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Texture.hpp"
#include "Graphics/Timing.hpp"

namespace UserInterface {
    class GameWidget : public QWidget {
    public:
        explicit GameWidget(const std::string& romPath, QWidget* parent);
        ~GameWidget() override;

    private:
        Nes::VirtualMachine          m_virtualMachine;
        std::unique_ptr<std::thread> m_emulatorThread;
        std::atomic<bool>            m_emulatorRunning = true;

        Graphics::FrameRateBlocker m_frameRateBlocker;
        Graphics::Window           m_window;
        Graphics::Renderer         m_renderer;
        Graphics::Texture          m_screenTexture;

        void emulatorThreadFunction();

        void draw(const Nes::FrameBuffer& frameBuffer);

        void keyPressEvent(QKeyEvent* event) override;
        void keyReleaseEvent(QKeyEvent* event) override;

        QPaintEngine* paintEngine() const override;
    };
}

#endif //CAIQUE_NES_GAMEWIDGET_HPP
