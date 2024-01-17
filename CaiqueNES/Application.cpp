/***********************************************************************************************************************
*
*   Copyright 2023 CockatooBoy
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

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <vector>
#include "UserInterface/MainWindow.hpp"
#include "Core/VirtualMachine.hpp"
#include "Utils/Types.hpp"
#include "Utils/Log.hpp"
#include "Application.hpp"

Application::Application(int argc, char** argv) :
    m_argc(argc),
    m_argv(argv)
{
    std::vector<std::string> args(argv + 1, argv + argc);

    if (args.empty()) {
        throw std::invalid_argument("No ROM path provided");
    }

    m_romPath = args.at(0);

    std::for_each(args.cbegin() + 1, args.cend(), [&](const auto& arg) {
        // placeholder for future args..
        Utils::log("Unknown command line argument: " + arg);
    });
}

int Application::run() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        throw std::runtime_error("Unable to initialise SDL2 Library");
    }

    QApplication app(m_argc, m_argv);

    UserInterface::MainWindow window(m_romPath);
    window.show();

    const auto appResult = app.exec();
    SDL_Quit();

    return appResult;
}
