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

#ifndef CAIQUE_NES_APPLICATION_HXX
#define CAIQUE_NES_APPLICATION_HXX

#include <QApplication>
#include <string>

class Application {
public:
    explicit Application(int argc, char** argv);

    [[nodiscard]] int run();

private:
    // There are here so that their lifetime is valid for QApplication
    int    m_argc;
    char** m_argv;

    std::string m_romPath;
};

#endif // CAIQUE_NES_APPLICATION_HXX
