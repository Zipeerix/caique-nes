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

#ifndef CAIQUE_NES_CPU_HPP
#define CAIQUE_NES_CPU_HPP

#include <optional>
#include <utility>
#include "Core/PPU.hpp"
#include "Core/MMU.hpp"
#include "Utils/Types.hpp"

#ifdef TESTING_ENVIRONMENT_6502
#include "Utils/String.hpp"
#include "../../Tests/External/6502/6502.TestUtils.hpp"
#endif
#ifdef TESTING_ENVIRONMENT_NESTEST
#include "../../Tests/External/NesTest/NesTest.TestUtils.hpp"
#endif

namespace Nes {
    using CycleCount = int;

    namespace Const {
        constexpr Addr stackAddr       = 0x0100;
        constexpr Byte aneMagicNumber  = 0xEE;
#ifdef TESTING_ENVIRONMENT_NESTEST
        constexpr Addr nesTestAutomatedModeAddr = 0xC000;
        constexpr CycleCount cpuInitialCycles   = 7;
#else
        constexpr CycleCount cpuInitialCycles = 0;
#endif
#ifdef TESTING_ENVIRONMENT_BLARGG
        constexpr Addr blarggTextOutputAddr = 0x6004;
#endif
#ifdef TESTING_ENVIRONMENT_6502
        constexpr bool logCpuJam  = false;
        constexpr bool logIllegal = false;
#else
        constexpr bool logCpuJam  = true;
        constexpr bool logIllegal = true;
#endif
        constexpr int nmiTicks = 2;

        namespace DefaultValue {
            constexpr Byte accumulator    = 0;
            constexpr Byte xIndex         = 0;
            constexpr Byte yIndex         = 0;
            constexpr Byte status         = 0x24;
            constexpr Byte stackPointer   = 0xFD;
            constexpr Byte programCounter = 0;
        }
    }

    enum class AddressingMode {
        Implicit,
        Immediate,
        ZeroPage,
        ZeroPageX,
        ZeroPageY,
        Absolute,
        AbsoluteX,
        AbsoluteY,
        IndirectX,
        IndirectY,
        Relative,
        Accumulator,
        JumpIndirect
    };

    enum class CPUFlag {
        Carry,
        Zero,
        InterruptDisable,
        Decimal,
        BFlag,
        Unused,
        Overflow,
        Negative
    };

    template <typename T>
    using BitIndexedRegister = Utils::BitIndexedValue<Byte, T>;

    struct Registers {
        Byte accumulator;
        Byte xIndex;
        Byte yIndex;

        BitIndexedRegister<CPUFlag> status;

        Byte stackPointer;
        Addr programCounter;

        Registers();
    };

    class CPU {
    public:
        CPU(MMU& mmu, PPU& ppu);

        void loadProgramCounter();

        [[nodiscard]] CycleCount tick();

    private:
        MMU& m_mmu;
        PPU& m_ppu;

        Registers m_registers{};

        CycleCount m_cycles = Const::cpuInitialCycles;
        bool m_pageBoundaryCrossed = false;

        void handleNMI();

        /* Utils */
        static Addr normalizeForZeroPage(Addr addr);
        static bool didPageCrossHappen(Addr firstAddr, Addr secondAddr);
        static int howMuchWasProgramCounterIncremented(AddressingMode addressingMode);
        CycleCount cyclesAccountingForPageCross(CycleCount baseCycles) const;

        /* Memory Helpers */
        Byte readOpcode();
        Addr readVector(Addr vectorAddr) const;
        Addr readAddressOperand(AddressingMode addressingMode);
        Byte readOperand(AddressingMode addressingMode);
        void pushToStack(Byte value);
        void pushWordToStack(Word value);
        Byte popFromStack();
        Word popWordFromStack();

        /* Official Instruction Helpers */
        CycleCount executeOpcode(Byte opcode);
        CycleCount branch(bool condition);
        void storeValue(Byte value, AddressingMode addressingMode);
        void loadValue(Byte& target, AddressingMode addressingMode);
        void addWithCarry(AddressingMode addressingMode);
        void subWithCarry(AddressingMode addressingMode);
        void bitwiseAnd(AddressingMode addressingMode);
        void bitwiseOr(AddressingMode addressingMode);
        void bitwiseXor(AddressingMode addressingMode);
        void arithmeticShiftLeft(AddressingMode addressingMode);
        void logicalShiftRight(AddressingMode addressingMode);
        void rotateLeft(AddressingMode addressingMode);
        void rotateRight(AddressingMode addressingMode);
        void testBit(AddressingMode addressingMode);
        void compare(Byte compareWith, AddressingMode addressingMode);
        void increment(Byte& target);
        void decrement(Byte& target);
        void incrementMemory(AddressingMode addressingMode);
        void decrementMemory(AddressingMode addressingMode);
        void jump(AddressingMode addressingMode);
        void call(AddressingMode addressingMode);
        void fnReturn();
        void interruptReturn();
        void transfer(Byte from, Byte& to);
        void pushStatus();
        void popStatus();
        void popAccumulatorFromStack();
        void setXToStackPointer();
        void instructionBreak();

        /* Illegal Instruction Helpers */
        void shx(AddressingMode addressingMode);
        void shy(AddressingMode addressingMode);
        void sha(AddressingMode addressingMode);
        void sax(AddressingMode addressingMode);
        void lax(AddressingMode addressingMode);
        void dcp(AddressingMode addressingMode);
        void las(AddressingMode addressingMode);
        void isc(AddressingMode addressingMode);
        void rra(AddressingMode addressingMode);
        void rla(AddressingMode addressingMode);
        void sre(AddressingMode addressingMode);
        void slo(AddressingMode addressingMode);
        void anc(AddressingMode addressingMode);
        void sbx(AddressingMode addressingMode);
        void ane(AddressingMode addressingMode);
        void alr(AddressingMode addressingMode);

        /* Other instruction Helpers */
        void jam();
        void nop(std::size_t instructionSize, std::optional<AddressingMode> addressingMode = std::nullopt);
        void unhandledInstruction(Byte opcode, AddressingMode addressingMode);

#ifdef TESTING_ENVIRONMENT_BLARGG
    public:
        std::string textOutput() {
            std::string str;

            Addr addr = Const::blarggTextOutputAddr;
            while (m_mmu.read(addr) != 0x00) {
                str += (char)m_mmu.read(addr);
                addr++;
            }

            return str;
        }
#endif
#ifdef TESTING_ENVIRONMENT_6502
    public:
        void loadState(const Cpu6502TestUtils::CpuState& state) {
            m_registers.programCounter = state.pc;
            m_registers.stackPointer   = state.s;
            m_registers.yIndex         = state.y;
            m_registers.xIndex         = state.x;
            m_registers.accumulator    = state.a;

            m_registers.status.setCombinedValue(state.p);
        };

        std::pair<bool, std::string> matchesState(const Cpu6502TestUtils::CpuState& state) const {
            if (m_registers.programCounter != state.pc) {
                return {false, "(PC) Expected: " + Utils::convertToHexString(state.pc, true, 4) +
                        " Actual: " + Utils::convertToHexString(m_registers.programCounter, true, 4)};
            }

            if (m_registers.stackPointer != state.s) {
                return {false, "(SP) Expected: " + Utils::convertToHexString(state.s, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.stackPointer, true, 2)};
            }

            if (m_registers.status.getCombinedValue() != state.p) {
                return {false, "(Status) Expected: " + Utils::convertToHexString(state.p, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.status.getCombinedValue(), true, 2)};
            }

            if (m_registers.yIndex != state.y) {
                return {false, "(Y) Expected: " + Utils::convertToHexString(state.y, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.yIndex, true, 2)};
            }

            if (m_registers.xIndex != state.x) {
                return {false, "(X) Expected: " + Utils::convertToHexString(state.x, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.xIndex, true, 2)};
            }

            if (m_registers.accumulator != state.a) {
                return {false, "(A) Expected: " + Utils::convertToHexString(state.a, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.accumulator, true, 2)};
            }

            return {true, ""};
        }

        bool executeNextInstructionInIsolation() {
            try {
                const auto opcode = readOpcode();
                m_cycles += executeOpcode(opcode);
                return true;
            } catch (const std::invalid_argument&) {
                return false;
            }
        }
#endif
#ifdef TESTING_ENVIRONMENT_NESTEST
    public:
            std::pair<bool, std::string> matchesEntry(const NesTestUtils::Entry& state) const {
            if (m_registers.programCounter != state.programCounter) {
                return {false, "(PC) Expected: " + Utils::convertToHexString(state.programCounter, true, 4) +
                        " Actual: " + Utils::convertToHexString(m_registers.programCounter, true, 4)};
            }

            if (m_registers.stackPointer != state.stackPointer) {
                return {false, "(SP) Expected: " + Utils::convertToHexString(state.stackPointer, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.stackPointer, true, 2)};
            }

            if (m_registers.status.getCombinedValue() != state.status) {
                return {false, "(Status) Expected: " + Utils::convertToHexString(state.status, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.status.getCombinedValue(), true, 2)};
            }

            if (m_registers.yIndex != state.yIndex) {
                return {false, "(Y) Expected: " + Utils::convertToHexString(state.yIndex, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.yIndex, true, 2)};
            }

            if (m_registers.xIndex != state.xIndex) {
                return {false, "(X) Expected: " + Utils::convertToHexString(state.xIndex, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.xIndex, true, 2)};
            }

            if (m_registers.accumulator != state.accumulator) {
                return {false, "(A) Expected: " + Utils::convertToHexString(state.accumulator, true, 2) +
                               " Actual: " + Utils::convertToHexString(m_registers.accumulator, true, 2)};
            }

            if (state.cpuCycles != m_cycles) {
               return {false, "(CPU Cycles) Expected: " + std::to_string(state.cpuCycles) +
               " Actual: " + std::to_string(m_cycles)};
            }

            return {true, ""};
        }

        void setAutomatedMode() {
            m_registers.programCounter = Const::nesTestAutomatedModeAddr;
        }
#endif
    };
}

#endif //CAIQUE_NES_CPU_HPP
