########################################################################################################################
#
#   Copyright 2023 CaiqueNES
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
########################################################################################################################

cmake_minimum_required(VERSION 3.20)
project(caique-nes-bin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(SDL2 REQUIRED)
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)

include_directories(.)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(caique-nes-bin
        Utils/String.tpp
        Utils/Range.tpp
        Utils/Data.tpp
        Utils/Data.cpp
        Utils/Host.cpp
        Utils/Log.cpp
        Utils/BitIndexedValue.tpp
        UserInterface/GameWidget.cpp
        UserInterface/MainWindow.cpp
        Graphics/Renderer.cpp
        Graphics/FrameBuffer.tpp
        Graphics/Texture.cpp
        Graphics/Timing.cpp
        Graphics/Window.cpp
        Core/VirtualMachine.cpp
        Core/Tile.cpp
        Core/Palette.cpp
        Core/OAMEntry.cpp
        Core/Joypad.cpp
        Core/Instructions.cpp
        Core/Cartridge.cpp
        Core/BaseMapper.cpp
        Core/CPU.cpp
        Core/PPU.cpp
        Core/APU.cpp
        Core/MMU.cpp
        Mappers/NROM.cpp
        Application.cpp
        CaiqueNES.cpp
)

target_link_libraries(caique-nes-bin
        Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets
        ${SDL2_LIBRARIES}
)

