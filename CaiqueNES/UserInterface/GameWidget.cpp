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

#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QKeySequence>
#include <QKeyEvent>
#include "GameWidget.hpp"

UserInterface::GameWidget::GameWidget(const std::string& romPath, QWidget* parent) :
    QWidget(parent),
    m_virtualMachine(std::bind(&GameWidget::draw, this, std::placeholders::_1)),
    m_frameRateBlocker(Nes::Const::frameRate),
    m_window(Graphics::Window::fromExternalSource(Utils::genericMemoryCast(winId()))),
    m_renderer(nullptr), // Render can only be created after m_window is confirmed to be valid later in constructor,
    m_screenTexture(nullptr) // Texture can only be created after m_renderer is confirmed to be valid later in constructor,
{
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);

    if (!m_window.isValid()) {
        throw std::runtime_error("Unable to initialise SDL2 window");
    }

    m_renderer = Graphics::Renderer(m_window, {Graphics::RendererFlags::Accelerated});
    if (!m_renderer.isValid()) {
        throw std::runtime_error("Unable to initialise SDL2 renderer");
    }

    m_screenTexture = Graphics::Texture(m_renderer, Graphics::TextureFormat::ARGBBytes, Graphics::TextureAccess::Static,
                                        Nes::Const::screenWidth, Nes::Const::screenHeight);
    if (!m_screenTexture.isValid()) {
        throw std::runtime_error("Unable to initialise SDL2 texture");
    }

    m_renderer.clear();
    m_renderer.present();

    auto romLoadResult = m_virtualMachine.loadRom(romPath);
    if (!romLoadResult.has_value()) {
        QMessageBox::critical(this, "Error", QString::fromStdString("Unable to load ROM file at: " + romPath +
                              " due to " + romLoadResult.error()));
        QApplication::exit();
    }

    m_emulatorThread = std::make_unique<std::thread>(&GameWidget::emulatorThreadFunction, this);
}

UserInterface::GameWidget::~GameWidget() {
    m_emulatorRunning = false;
    m_emulatorThread->join();
}

void UserInterface::GameWidget::emulatorThreadFunction() {
    while (m_emulatorRunning) {
        m_frameRateBlocker.startOfLoop();
        m_virtualMachine.tick();
        m_frameRateBlocker.endOfLoop();
    }
}

void UserInterface::GameWidget::draw(const Nes::FrameBuffer& frameBuffer) {
    frameBuffer.copyToTexture(m_screenTexture);

    m_renderer.clear();
    m_renderer.copyTexture(m_screenTexture);
    m_renderer.present();
}

void UserInterface::GameWidget::keyPressEvent(QKeyEvent* event) {
    const auto key = QKeySequence(event->key()).toString().toStdString();

    if (key == "A") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::A);
    } else if (key == "S") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::B);
    } else if (key == "O") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Select);
    } else if (key == "P") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Start);
    } else if (key == "Up") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Up);
    } else if (key == "Down") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Down);
    } else if (key == "Left") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Left);
    } else if (key == "Right") {
        m_virtualMachine.handleKeyPress(Nes::JoypadButton::Right);
    }
}

void UserInterface::GameWidget::keyReleaseEvent(QKeyEvent* event)  {
    const auto key = QKeySequence(event->key()).toString().toStdString();

    if (key == "A") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::A);
    } else if (key == "S") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::B);
    } else if (key == "O") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Select);
    } else if (key == "P") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Start);
    } else if (key == "Up") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Up);
    } else if (key == "Down") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Down);
    } else if (key == "Left") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Left);
    } else if (key == "Right") {
        m_virtualMachine.handleKeyRelease(Nes::JoypadButton::Right);
    }
}

QPaintEngine* UserInterface::GameWidget::paintEngine() const {
    return nullptr;
}
