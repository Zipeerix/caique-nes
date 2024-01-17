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

#ifndef CAIQUE_NES_MAINWINDOW_HPP
#define CAIQUE_NES_MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include "UserInterface/GameWidget.hpp"

namespace UserInterface {
    namespace Ui {
        class MainWindow;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        explicit MainWindow(const std::string& romPath, QWidget* parent = nullptr);
        ~MainWindow() override;

    private:
        Ui::MainWindow* ui;

        std::unique_ptr<GameWidget> m_graphicsWidget;
    };
}

#endif //CAIQUE_NES_MAINWINDOW_HPP
