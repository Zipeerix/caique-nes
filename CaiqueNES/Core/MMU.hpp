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

#ifndef CAIQUE_NES_MMU_HPP
#define CAIQUE_NES_MMU_HPP

#include <functional>
#include "Core/Cartridge.hpp"
#include "Utils/Range.hpp"
#include "Utils/Types.hpp"

#ifdef TESTING_ENVIRONMENT_6502
#include <set>
#include "../../Tests/External/6502/6502.TestUtils.hpp"
#endif

namespace Nes {
    namespace Const {
        namespace AddrRange {
            constexpr Utils::Range<Addr> zeroPage            = {0x0000, 0x00FF};
            constexpr Utils::Range<Addr> internalRAM         = {0x0000, 0x07FF};
            constexpr Utils::Range<Addr> mirror              = {0x0800, 0x1FFF};
            constexpr Utils::Range<Addr> ppuRegisters        = {0x2000, 0x2007};
            constexpr Utils::Range<Addr> ppuRegistersMirror  = {0x2008, 0x3FFF};
            constexpr Utils::Range<Addr> oamDmaRequest       = {0x4014, 0x4014};
            constexpr Utils::Range<Addr> apuRegisters        = {0x4000, 0x4013};
            constexpr Utils::Range<Addr> apuStatus           = {0x4015, 0x4015};
            constexpr Utils::Range<Addr> joypadIO            = {0x4016, 0x4016};
            constexpr Utils::Range<Addr> joypad2IO           = {0x4017, 0x4017};
            constexpr Utils::Range<Addr> unmapped            = {0x4020, 0x5FFF};
            constexpr Utils::Range<Addr> workRAM             = {0x6000, 0x7FFF};
            constexpr Utils::Range<Addr> cartridgePRG        = {0x8000, 0xFFFF};
        }

        namespace VectorAddr {
            constexpr Addr interrupt = 0xFFFA;
            constexpr Addr reset     = 0xFFFC;
            constexpr Addr irq       = 0xFFFE;
        }
    }

    class MMU;
    struct MemoryRegion;

    using MemoryWriteFunction = std::function<void(MemoryRegion*, MMU*, Addr, Byte)>;
    using MemoryReadFunction  = std::function<Byte(MemoryRegion*, MMU*, Addr)>;

    struct MemoryRegion {
        MemoryRegion(Utils::Range<Addr> addrRange, MemoryWriteFunction writeFunction, MemoryReadFunction readFunction,
                     bool needItsOwnMemory = false);
        explicit MemoryRegion(Utils::Range<Addr> addrRange);

        std::vector<Byte> memory;

        Utils::Range<Addr> addrRange;

        MemoryWriteFunction writeFunction;
        MemoryReadFunction  readFunction;
    };

    class MMU : Module {
    public:
        explicit MMU(Cartridge& cartridge);

        const Cartridge& accessCartridge();

        void addMemoryRegion(Utils::Range<Addr> addrRange, const MemoryWriteFunction& writeFunction,
                             const MemoryReadFunction& readFunction, bool needItsOwnMemory);
        void addMemoryRegion(Utils::Range<Addr> addrRange);

        void clearMemoryRegions();

        void write(Addr addr, Byte value);
        void writeWord(Addr addr, Word value);

        Byte read(Addr addr);
        Word readWord(Addr addr);

    private:
        Cartridge& m_cartridge;

        std::vector<MemoryRegion> m_memoryRegions;

#ifdef TESTING_ENVIRONMENT_6502
        public:
            std::set<Addr> m_writtenToAddresses{};

            void loadState(const Cpu6502TestUtils::CpuState& state) {
                clearMemoryRegions();
                addMemoryRegion(Utils::Range<Nes::Addr>{0x0000, UINT16_MAX},
                                [&](MemoryRegion* region, MMU* mmu, Addr addr, Byte value) {
                                    mmu->m_writtenToAddresses.insert(addr);
                                    region->memory.at(addr) = value;
                                },
                                [&](MemoryRegion* region, const MMU*, Addr addr) {
                                   return region->memory.at(addr);
                                },
                             true);

                for (const auto& ramEntry : state.ram) {
                    write(ramEntry.first, ramEntry.second);
                }
            };

            static std::string generateRamMismatchLog(const std::map<Addr, Byte>& changedMemory, const Cpu6502TestUtils::CpuState& finalState) {
                std::string expected = "\nExpected:\n";
                for (const auto& entry : finalState.ram) {
                    expected += "{ " + Utils::convertToHexString(entry.first, true, 4) + " : " + Utils::convertToHexString(entry.second, true, 2) + "}\n";
                }
                std::string actual = "\nActual:\n";
                for (const auto& entry : changedMemory) {
                    actual += "{ " + Utils::convertToHexString(entry.first, true, 4) + " : " + Utils::convertToHexString(entry.second, true, 2) + "}\n";
                }

                return expected + actual;
            }

            std::pair<bool, std::string> matchesState(const Cpu6502TestUtils::CpuState& finalState) {
                std::map<Addr, Byte> changedMemory;

                for (const auto& addr : m_writtenToAddresses) {
                    changedMemory[addr] = read(addr);
                }

                if (changedMemory.size() != finalState.ram.size()) {
                    return {false, "RAM Size mismatch\n" + generateRamMismatchLog(changedMemory, finalState)};
                }

                if (!std::equal(changedMemory.begin(), changedMemory.end(), finalState.ram.begin())) {
                    return {false, "RAM Value mismatch\n" + generateRamMismatchLog(changedMemory, finalState)};
                }

                return {true, ""};
            }
#endif
#ifdef TESTING_ENVIRONMENT_PPU
        public:
            void fillForOamDMA(Byte upperAddrByte, Byte lastValue) {
                const auto from = Utils::combineBytes(upperAddrByte, 0x00);
                const auto to   = Utils::combineBytes(upperAddrByte, 0xFF);

                for (auto i = from; i <= to; i++) {
                    write(i, i - from);
                }

                write(to, lastValue);
            };
#endif
    };
}

#endif //CAIQUE_NES_MMU_HPP
