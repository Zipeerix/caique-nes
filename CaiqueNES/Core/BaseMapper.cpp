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

#include "Mappers/NROM.hpp"

std::unique_ptr<Nes::BaseMapper> Nes::BaseMapper::createFromID(Cartridge& cartridge, Byte id, std::size_t sizeOfPRG,
                                                               std::size_t sizeOfCHR)
{
    switch (static_cast<Mapper>(id)) {
        case Mapper::NROM: return std::make_unique<NROM>(cartridge, sizeOfPRG, sizeOfCHR);
        default: return nullptr;
    }
}

Nes::BaseMapper::BaseMapper(Cartridge& cartridge, std::size_t sizeOfPRG, std::size_t sizeOfCHR, Byte mapperId,
                            const MapperValidationFunction& validationFn) :
    m_cartridge(cartridge)
{
    if (!validationFn(sizeOfPRG, sizeOfCHR)) {
        throw std::invalid_argument("Sanity check failed for mapper with ID " + std::to_string(mapperId));
    }
}
