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

#include <algorithm>
#include <iostream>
#include "Log.hpp"

void Utils::log(const std::string& message) {
    const time_t tm = time(nullptr);
    struct tm* now  = localtime (&tm);

    std::string timeString = asctime(now);
    timeString.erase(std::remove(timeString.begin(), timeString.end(), '\n'), timeString.cend());

    std::cout << "[" << timeString << "]: " << message << std::endl;
}
