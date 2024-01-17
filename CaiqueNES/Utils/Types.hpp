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

#ifndef CAIQUE_NES_TYPES_HPP
#define CAIQUE_NES_TYPES_HPP

#include <functional>
#include <concepts>
#include <cstdint>
#include <string>
#include <memory>

namespace Nes {
    using Byte  = std::uint8_t;
    using SByte = std::int8_t;
    using Word  = std::uint16_t;
    using Addr  = std::uint16_t;

    namespace Const {
        constexpr Word maximumByteValue = UINT8_MAX;
    }

    class Module {};

    template<typename ModuleType> requires std::is_base_of_v<Module, ModuleType>
    using ModuleLink = ModuleType* const;
}

namespace Utils {
    template <typename T>
    concept IntegerType = std::is_integral_v<T>;

    template <typename T>
    concept NumericType = std::is_arithmetic_v<T>;

    template <typename CType>
    using CTypeUniquePtr = std::unique_ptr<CType, std::function<void(CType*)>>;

    using GenericMemoryPtr = void*;

    template <typename T>
    constexpr inline GenericMemoryPtr genericMemoryCast(T* value) {
        return reinterpret_cast<GenericMemoryPtr>(value);
    }

    template <typename T>
    constexpr inline GenericMemoryPtr genericMemoryCast(T value) {
        return (GenericMemoryPtr)value;
    }

    using ErrorString = std::string;
}

namespace Graphics {
    using PixelColor = std::uint32_t;

    struct Position {
        int x;
        int y;
    };
}

#endif // CAIQUE_NES_TYPES_HPP
