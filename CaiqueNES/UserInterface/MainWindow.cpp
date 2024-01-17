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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"

UserInterface::MainWindow::MainWindow(const std::string& romPath, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_graphicsWidget = std::make_unique<GameWidget>(romPath, this);

    ui->setupUi(this);
    ui->centralwidget->layout()->addWidget(m_graphicsWidget.get());

    setWindowTitle(QString::fromStdString("CaiqueNES: " + romPath));
}

UserInterface::MainWindow::~MainWindow() {
    delete ui;
}
