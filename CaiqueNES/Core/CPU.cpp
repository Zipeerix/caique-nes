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

#include "Core/CPU.hpp"

Nes::Registers::Registers() :
    accumulator(Const::DefaultValue::accumulator),
    xIndex(Const::DefaultValue::xIndex),
    yIndex(Const::DefaultValue::yIndex),
    status(Const::DefaultValue::status),
    stackPointer(Const::DefaultValue::stackPointer),
    programCounter(Const::DefaultValue::programCounter)
{
}

Nes::CPU::CPU(MMU& mmu, PPU& ppu) :
    m_mmu(mmu),
    m_ppu(ppu)
{
}

void Nes::CPU::handleNMI() {
    pushWordToStack(m_registers.programCounter);

    Byte statusCopy = m_registers.status.getCombinedValue();
    statusCopy = Utils::clearBit(statusCopy, static_cast<int>(CPUFlag::BFlag));
    statusCopy = Utils::setBit(statusCopy, static_cast<int>(CPUFlag::Unused));

    pushToStack(statusCopy);

    m_registers.status.setBit(CPUFlag::InterruptDisable);

    m_ppu.tick(Const::nmiTicks);
    m_registers.programCounter = readVector(Const::VectorAddr::interrupt);
}

void Nes::CPU::loadProgramCounter() {
    m_registers.programCounter = readVector(Const::VectorAddr::reset);
}

Nes::CycleCount Nes::CPU::tick() {
    if (m_ppu.nmiStatus()) {
        handleNMI();
    }

    const auto opcode = readOpcode();
    const auto cyclesTaken = executeOpcode(opcode);
    m_cycles += cyclesTaken;

    m_ppu.tick(cyclesTaken);

    return cyclesTaken;
}

Nes::Addr Nes::CPU::normalizeForZeroPage(Addr addr) {
    return addr % (Const::maximumByteValue + 1);
}

bool Nes::CPU::didPageCrossHappen(Addr firstAddr, Addr secondAddr) {
    return Utils::getUpperByte(firstAddr) != Utils::getUpperByte(secondAddr);
}

int Nes::CPU::howMuchWasProgramCounterIncremented(AddressingMode addressingMode) {
    switch (addressingMode) {
        case AddressingMode::Absolute:
        case AddressingMode::AbsoluteY:
        case AddressingMode::AbsoluteX:
            return 2;

        case AddressingMode::JumpIndirect:
        case AddressingMode::Implicit:
            throw std::logic_error("Attempting to see how much was PC incremented in implicit/jump addressing mode");

        default:
            return 1;
    }
}

Nes::CycleCount Nes::CPU::cyclesAccountingForPageCross(CycleCount baseCycles) const {
    return m_pageBoundaryCrossed ? baseCycles + 1 : baseCycles;
}

Nes::Byte Nes::CPU::readOpcode() {
    return m_mmu.read(m_registers.programCounter++);
}

Nes::Addr Nes::CPU::readVector(Addr vectorAddr) const {
    return m_mmu.readWord(vectorAddr);
}

Nes::Addr Nes::CPU::readAddressOperand(AddressingMode addressingMode) {
    switch (addressingMode) {
        case AddressingMode::Implicit:
            throw std::logic_error("Attempting to fetch operand address in implicit addressing mode");

        case AddressingMode::Relative:
        case AddressingMode::Immediate:
            return m_registers.programCounter++;

        case AddressingMode::Absolute: {
            const auto addr = m_registers.programCounter;
            m_registers.programCounter += 2;

            return m_mmu.readWord(addr);
        }

        case AddressingMode::AbsoluteX: {
            const auto ptr      = readAddressOperand(AddressingMode::Absolute);
            const Addr finalPtr = ptr + m_registers.xIndex;

            m_pageBoundaryCrossed = didPageCrossHappen(ptr, finalPtr);
            return finalPtr;
        }

        case AddressingMode::AbsoluteY: {
            const auto ptr      = readAddressOperand(AddressingMode::Absolute);
            const Addr finalPtr = ptr + m_registers.yIndex;

            m_pageBoundaryCrossed = didPageCrossHappen(ptr, finalPtr);
            return finalPtr;
        }

        case AddressingMode::ZeroPage: {
            const auto addr = m_mmu.read(m_registers.programCounter++);
            return normalizeForZeroPage(addr);
        }

        case AddressingMode::ZeroPageX: {
            const auto ptr = readAddressOperand(AddressingMode::ZeroPage);
            return normalizeForZeroPage(ptr + m_registers.xIndex);
        }

        case AddressingMode::ZeroPageY: {
            const auto ptr = readAddressOperand(AddressingMode::ZeroPage);
            return normalizeForZeroPage(ptr + m_registers.yIndex);
        }

        case AddressingMode::IndirectX: {
            const Byte zpPtrLower = m_mmu.read(m_registers.programCounter++) + m_registers.xIndex;
            const Byte zpPtrUpper = zpPtrLower + 1;
            return Utils::combineBytes(m_mmu.read(normalizeForZeroPage(zpPtrUpper)), m_mmu.read(normalizeForZeroPage(zpPtrLower)));
        }

        case AddressingMode::IndirectY: {
            const Byte zpPtrLower = m_mmu.read(m_registers.programCounter++);
            const Byte zpPtrUpper = zpPtrLower + 1;

            const auto ptr = Utils::combineBytes(m_mmu.read(normalizeForZeroPage(zpPtrUpper)), m_mmu.read(normalizeForZeroPage(zpPtrLower)));
            const Addr finalPtr = ptr + m_registers.yIndex;

            m_pageBoundaryCrossed = didPageCrossHappen(ptr, finalPtr);
            return finalPtr;
        }

        case AddressingMode::JumpIndirect: {
            const auto address = readAddressOperand(AddressingMode::Absolute);

            if (Utils::getLowerByte(address) == Const::AddrRange::zeroPage.to) {
                const Addr upperByteAddr = Utils::combineBytes(Utils::getUpperByte(address), 0x00);
                const Byte upperByte     = m_mmu.read(upperByteAddr);
                const Byte lowerByte     = m_mmu.read(address);

                return Utils::combineBytes(upperByte, lowerByte);
            } else {
                return m_mmu.readWord(address);
            }
        }

        default:
            throw std::logic_error("Unhandled addressing mode: " + std::to_string(static_cast<int>(addressingMode)));
    }
}

Nes::Byte Nes::CPU::readOperand(AddressingMode addressingMode) {
    if (addressingMode == AddressingMode::Accumulator) {
        return m_registers.accumulator;
    }

    const auto addr = readAddressOperand(addressingMode);
    return m_mmu.read(addr);
}

void Nes::CPU::pushToStack(Byte value) {
    m_mmu.write(m_registers.stackPointer + Const::stackAddr, value);
    m_registers.stackPointer--;
}

void Nes::CPU::pushWordToStack(Word value) {
    const auto upperByte = Utils::getUpperByte(value);
    const auto lowerByte = Utils::getLowerByte(value);

    pushToStack(upperByte);
    pushToStack(lowerByte);
}

Nes::Byte Nes::CPU::popFromStack() {
    m_registers.stackPointer++;
    return m_mmu.read(m_registers.stackPointer + Const::stackAddr);
}

Nes::Word Nes::CPU::popWordFromStack() {
    const Byte lowerByte = popFromStack();
    const Byte upperByte = popFromStack();

    return Utils::combineBytes(upperByte, lowerByte);
}
