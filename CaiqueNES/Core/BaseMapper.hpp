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

#ifndef CAIQUE_NES_BASEMAPPER_HPP
#define CAIQUE_NES_BASEMAPPER_HPP

#include <functional>
#include <expected>
#include <memory>
#include "Utils/Types.hpp"
#include "Utils/Data.hpp"

namespace Nes {
    class Cartridge;

    using MapperValidationFunction = std::function<bool(std::size_t, std::size_t)>;

    enum class Mapper {
        NROM
    };

    class BaseMapper {
    public:
        static std::unique_ptr<BaseMapper> createFromID(Cartridge& cartridge, Byte id, std::size_t sizeOfPRG,
                                                        std::size_t sizeOfCHR);

        BaseMapper(Cartridge& cartridge, std::size_t sizeOfPRG, std::size_t sizeOfCHR, Byte mapperId,
                   const MapperValidationFunction& validationFn);
        virtual ~BaseMapper() = default;

        virtual Byte readPRG(Addr addr) const = 0;
        virtual void writePRG(Addr addr, Byte value) = 0;

        virtual Byte readCHR(Addr addr) const = 0;
        virtual void writeCHR(Addr addr, Byte value) = 0;

    protected:
        Cartridge& m_cartridge;
    };
}

#endif //CAIQUE_NES_BASEMAPPER_HPP
