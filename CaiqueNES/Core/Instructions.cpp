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

#include <stdexcept>
#include "Core/CPU.hpp"
#include "Utils/String.hpp"
#include "Utils/Log.hpp"

Nes::CycleCount Nes::CPU::executeOpcode(Byte opcode) {
    switch (opcode) {
        case 0x02:
        case 0x12:
        case 0x22:
        case 0x32:
        case 0x42:
        case 0x52:
        case 0x62:
        case 0x72:
        case 0x92:
        case 0xB2:
        case 0xD2:
        case 0xF2: jam(); return 2;

        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A:
        case 0xDA:
        case 0xFA:
        case 0xEA: nop(1); return 2;
        case 0x80:
        case 0x82:
        case 0x89:
        case 0xC2:
        case 0xE2: nop(2); return 2;
        case 0x04:
        case 0x44:
        case 0x64: nop(2); return 3;
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74:
        case 0xD4:
        case 0xF4: nop(2); return 4;
        case 0x0C: nop(3); return 4;
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC: nop(1, AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);

        case 0x6B:
        case 0xAB: unhandledInstruction(opcode, AddressingMode::Immediate); return 2;

        case 0x87: sax(AddressingMode::ZeroPage);  return 3;
        case 0x97: sax(AddressingMode::ZeroPageY); return 4;
        case 0x8F: sax(AddressingMode::Absolute);  return 4;
        case 0x83: sax(AddressingMode::IndirectX); return 6;

        case 0xA7: lax(AddressingMode::ZeroPage);  return 3;
        case 0xB7: lax(AddressingMode::ZeroPageY); return 4;
        case 0xAF: lax(AddressingMode::Absolute);  return 4;
        case 0xBF: lax(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0xB3: lax(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);
        case 0xA3: lax(AddressingMode::IndirectX); return  6;

        case 0xC7: dcp(AddressingMode::ZeroPage);  return 5;
        case 0xD7: dcp(AddressingMode::ZeroPageX); return 6;
        case 0xCF: dcp(AddressingMode::Absolute);  return 6;
        case 0xDF: dcp(AddressingMode::AbsoluteX); return 7;
        case 0xDB: dcp(AddressingMode::AbsoluteY); return 7;
        case 0xC3: dcp(AddressingMode::IndirectX); return 8;
        case 0xD3: dcp(AddressingMode::IndirectY); return 8;

        case 0xE7: isc(AddressingMode::ZeroPage);  return 5;
        case 0xF7: isc(AddressingMode::ZeroPageX); return 6;
        case 0xEF: isc(AddressingMode::Absolute);  return 6;
        case 0xFF: isc(AddressingMode::AbsoluteX); return 7;
        case 0xFB: isc(AddressingMode::AbsoluteY); return 7;
        case 0xE3: isc(AddressingMode::IndirectX); return 8;
        case 0xF3: isc(AddressingMode::IndirectY); return 8;

        case 0x67: rra(AddressingMode::ZeroPage);  return 5;
        case 0x77: rra(AddressingMode::ZeroPageX); return 6;
        case 0x6F: rra(AddressingMode::Absolute);  return 6;
        case 0x7F: rra(AddressingMode::AbsoluteX); return 7;
        case 0x7B: rra(AddressingMode::AbsoluteY); return 7;
        case 0x63: rra(AddressingMode::IndirectX); return 8;
        case 0x73: rra(AddressingMode::IndirectY); return 8;

        case 0x27: rla(AddressingMode::ZeroPage);  return 5;
        case 0x37: rla(AddressingMode::ZeroPageX); return 6;
        case 0x2F: rla(AddressingMode::Absolute);  return 6;
        case 0x3F: rla(AddressingMode::AbsoluteX); return 7;
        case 0x3B: rla(AddressingMode::AbsoluteY); return 7;
        case 0x33: rla(AddressingMode::IndirectY); return 8;
        case 0x23: rla(AddressingMode::IndirectX); return 8;

        case 0x47: sre(AddressingMode::ZeroPage);  return 5;
        case 0x57: sre(AddressingMode::ZeroPageX); return 6;
        case 0x4F: sre(AddressingMode::Absolute);  return 6;
        case 0x5F: sre(AddressingMode::AbsoluteX); return 7;
        case 0x5B: sre(AddressingMode::AbsoluteY); return 7;
        case 0x43: sre(AddressingMode::IndirectX); return 8;
        case 0x53: sre(AddressingMode::IndirectY); return 8;

        case 0x07: slo(AddressingMode::ZeroPage);  return 5;
        case 0x17: slo(AddressingMode::ZeroPageX); return 6;
        case 0x0F: slo(AddressingMode::Absolute);  return 6;
        case 0x1F: slo(AddressingMode::AbsoluteX); return 7;
        case 0x1B: slo(AddressingMode::AbsoluteY); return 7;
        case 0x03: slo(AddressingMode::IndirectX); return 8;
        case 0x13: slo(AddressingMode::IndirectY); return 8;

        case 0x9F: sha(AddressingMode::AbsoluteY); return 5;
        case 0x93: sha(AddressingMode::IndirectY); return 6;
        case 0x9E: shx(AddressingMode::AbsoluteY); return 5;
        case 0x9C: shy(AddressingMode::AbsoluteX); return 5;

        case 0x0B: anc(AddressingMode::Immediate); return 2;
        case 0x2B: anc(AddressingMode::Immediate); return 2;

        case 0x4B: alr(AddressingMode::Immediate); return 2;

        case 0xCB: sbx(AddressingMode::Immediate); return 2;

        case 0xEB: subWithCarry(AddressingMode::Immediate); return 2;

        case 0x8B: ane(AddressingMode::Immediate); return 2;

        case 0xBB: las(AddressingMode::AbsoluteY); return 4;

        case 0x00: instructionBreak(); return 7;

        case 0x18: m_registers.status.clearBit(CPUFlag::Carry);            return 2;
        case 0x58: m_registers.status.clearBit(CPUFlag::InterruptDisable); return 2;
        case 0xB8: m_registers.status.clearBit(CPUFlag::Overflow);         return 2;
        case 0xD8: m_registers.status.clearBit(CPUFlag::Decimal);          return 2;

        case 0x38: m_registers.status.setBit(CPUFlag::Carry);              return 2;
        case 0x78: m_registers.status.setBit(CPUFlag::InterruptDisable);   return 2;
        case 0xF8: m_registers.status.setBit(CPUFlag::Decimal);            return 2;

        case 0x86: storeValue(m_registers.xIndex, AddressingMode::ZeroPage);       return 3;
        case 0x96: storeValue(m_registers.xIndex, AddressingMode::ZeroPageY);      return 4;
        case 0x8E: storeValue(m_registers.xIndex, AddressingMode::Absolute);       return 4;
        case 0x84: storeValue(m_registers.yIndex, AddressingMode::ZeroPage);       return 3;
        case 0x94: storeValue(m_registers.yIndex, AddressingMode::ZeroPageX);      return 4;
        case 0x8C: storeValue(m_registers.yIndex, AddressingMode::Absolute);       return 4;
        case 0x85: storeValue(m_registers.accumulator, AddressingMode::ZeroPage);  return 3;
        case 0x95: storeValue(m_registers.accumulator, AddressingMode::ZeroPageX); return 4;
        case 0x8D: storeValue(m_registers.accumulator, AddressingMode::Absolute);  return 4;
        case 0x9D: storeValue(m_registers.accumulator, AddressingMode::AbsoluteX); return 5;
        case 0x99: storeValue(m_registers.accumulator, AddressingMode::AbsoluteY); return 5;
        case 0x81: storeValue(m_registers.accumulator, AddressingMode::IndirectX); return 6;
        case 0x91: storeValue(m_registers.accumulator, AddressingMode::IndirectY); return 6;

        case 0xA9: loadValue(m_registers.accumulator, AddressingMode::Immediate); return 2;
        case 0xA5: loadValue(m_registers.accumulator, AddressingMode::ZeroPage);  return 3;
        case 0xB5: loadValue(m_registers.accumulator, AddressingMode::ZeroPageX); return 4;
        case 0xAD: loadValue(m_registers.accumulator, AddressingMode::Absolute);  return 4;
        case 0xA1: loadValue(m_registers.accumulator, AddressingMode::IndirectX); return 6;
        case 0xBD: loadValue(m_registers.accumulator, AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0xB9: loadValue(m_registers.accumulator, AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0xB1: loadValue(m_registers.accumulator, AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0xA2: loadValue(m_registers.xIndex, AddressingMode::Immediate); return 2;
        case 0xA6: loadValue(m_registers.xIndex, AddressingMode::ZeroPage);  return 3;
        case 0xB6: loadValue(m_registers.xIndex, AddressingMode::ZeroPageY); return 4;
        case 0xAE: loadValue(m_registers.xIndex, AddressingMode::Absolute);  return 4;
        case 0xBE: loadValue(m_registers.xIndex, AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);

        case 0xA0: loadValue(m_registers.yIndex, AddressingMode::Immediate); return 2;
        case 0xA4: loadValue(m_registers.yIndex, AddressingMode::ZeroPage);  return 3;
        case 0xB4: loadValue(m_registers.yIndex, AddressingMode::ZeroPageX); return 4;
        case 0xAC: loadValue(m_registers.yIndex, AddressingMode::Absolute);  return 4;
        case 0xBC: loadValue(m_registers.yIndex, AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);

        case 0x69: addWithCarry(AddressingMode::Immediate); return 2;
        case 0x65: addWithCarry(AddressingMode::ZeroPage);  return 3;
        case 0x75: addWithCarry(AddressingMode::ZeroPageX); return 4;
        case 0x6D: addWithCarry(AddressingMode::Absolute);  return 4;
        case 0x61: addWithCarry(AddressingMode::IndirectX); return 6;
        case 0x7D: addWithCarry(AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0x79: addWithCarry(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0x71: addWithCarry(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0xE9: subWithCarry(AddressingMode::Immediate); return 2;
        case 0xE5: subWithCarry(AddressingMode::ZeroPage);  return 3;
        case 0xF5: subWithCarry(AddressingMode::ZeroPageX); return 4;
        case 0xED: subWithCarry(AddressingMode::Absolute);  return 4;
        case 0xE1: subWithCarry(AddressingMode::IndirectX); return 6;
        case 0xFD: subWithCarry(AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0xF9: subWithCarry(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0xF1: subWithCarry(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0x29: bitwiseAnd(AddressingMode::Immediate); return 2;
        case 0x25: bitwiseAnd(AddressingMode::ZeroPage);  return 3;
        case 0x35: bitwiseAnd(AddressingMode::ZeroPageX); return 4;
        case 0x2D: bitwiseAnd(AddressingMode::Absolute);  return 4;
        case 0x21: bitwiseAnd(AddressingMode::IndirectX); return 6;
        case 0x3D: bitwiseAnd(AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0x39: bitwiseAnd(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0x31: bitwiseAnd(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0x49: bitwiseXor(AddressingMode::Immediate); return 2;
        case 0x45: bitwiseXor(AddressingMode::ZeroPage);  return 3;
        case 0x55: bitwiseXor(AddressingMode::ZeroPageX); return 4;
        case 0x4D: bitwiseXor(AddressingMode::Absolute);  return 4;
        case 0x41: bitwiseXor(AddressingMode::IndirectX); return 6;
        case 0x5D: bitwiseXor(AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0x59: bitwiseXor(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0x51: bitwiseXor(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0x09: bitwiseOr(AddressingMode::Immediate); return 2;
        case 0x05: bitwiseOr(AddressingMode::ZeroPage);  return 3;
        case 0x15: bitwiseOr(AddressingMode::ZeroPageX); return 4;
        case 0x0D: bitwiseOr(AddressingMode::Absolute);  return 4;
        case 0x01: bitwiseOr(AddressingMode::IndirectX); return 6;
        case 0x1D: bitwiseOr(AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0x19: bitwiseOr(AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0x11: bitwiseOr(AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0x0A: arithmeticShiftLeft(AddressingMode::Accumulator); return 2;
        case 0x06: arithmeticShiftLeft(AddressingMode::ZeroPage);    return 5;
        case 0x16: arithmeticShiftLeft(AddressingMode::ZeroPageX);   return 6;
        case 0x0E: arithmeticShiftLeft(AddressingMode::Absolute);    return 6;
        case 0x1E: arithmeticShiftLeft(AddressingMode::AbsoluteX);   return 7;

        case 0x4A: logicalShiftRight(AddressingMode::Accumulator); return 2;
        case 0x46: logicalShiftRight(AddressingMode::ZeroPage);    return 5;
        case 0x56: logicalShiftRight(AddressingMode::ZeroPageX);   return 6;
        case 0x4E: logicalShiftRight(AddressingMode::Absolute);    return 6;
        case 0x5E: logicalShiftRight(AddressingMode::AbsoluteX);   return 7;

        case 0x6A: rotateRight(AddressingMode::Accumulator); return 2;
        case 0x66: rotateRight(AddressingMode::ZeroPage);    return 5;
        case 0x76: rotateRight(AddressingMode::ZeroPageX);   return 6;
        case 0x6E: rotateRight(AddressingMode::Absolute);    return 6;
        case 0x7E: rotateRight(AddressingMode::AbsoluteX);   return 7;

        case 0x2A: rotateLeft(AddressingMode::Accumulator); return 2;
        case 0x26: rotateLeft(AddressingMode::ZeroPage);    return 5;
        case 0x36: rotateLeft(AddressingMode::ZeroPageX);   return 6;
        case 0x2E: rotateLeft(AddressingMode::Absolute);    return 6;
        case 0x3E: rotateLeft(AddressingMode::AbsoluteX);   return 7;

        case 0x24: testBit(AddressingMode::ZeroPage); return 3;
        case 0x2C: testBit(AddressingMode::Absolute); return 4;

        case 0xC9: compare(m_registers.accumulator, AddressingMode::Immediate); return 2;
        case 0xC5: compare(m_registers.accumulator, AddressingMode::ZeroPage);  return 3;
        case 0xD5: compare(m_registers.accumulator, AddressingMode::ZeroPageX); return 4;
        case 0xCD: compare(m_registers.accumulator, AddressingMode::Absolute);  return 4;
        case 0xC1: compare(m_registers.accumulator, AddressingMode::IndirectX); return 6;
        case 0xDD: compare(m_registers.accumulator, AddressingMode::AbsoluteX); return cyclesAccountingForPageCross(4);
        case 0xD9: compare(m_registers.accumulator, AddressingMode::AbsoluteY); return cyclesAccountingForPageCross(4);
        case 0xD1: compare(m_registers.accumulator, AddressingMode::IndirectY); return cyclesAccountingForPageCross(5);

        case 0xE0: compare(m_registers.xIndex, AddressingMode::Immediate); return 2;
        case 0xE4: compare(m_registers.xIndex, AddressingMode::ZeroPage);  return 3;
        case 0xEC: compare(m_registers.xIndex, AddressingMode::Absolute);  return 4;

        case 0xC0: compare(m_registers.yIndex, AddressingMode::Immediate); return 2;
        case 0xC4: compare(m_registers.yIndex, AddressingMode::ZeroPage);  return 3;
        case 0xCC: compare(m_registers.yIndex, AddressingMode::Absolute);  return 4;

        case 0xE6: incrementMemory(AddressingMode::ZeroPage);  return 5;
        case 0xF6: incrementMemory(AddressingMode::ZeroPageX); return 6;
        case 0xEE: incrementMemory(AddressingMode::Absolute);  return 6;
        case 0xFE: incrementMemory(AddressingMode::AbsoluteX); return 7;

        case 0xE8: increment(m_registers.xIndex); return 2;
        case 0xC8: increment(m_registers.yIndex); return 2;

        case 0xC6: decrementMemory(AddressingMode::ZeroPage);  return 5;
        case 0xD6: decrementMemory(AddressingMode::ZeroPageX); return 6;
        case 0xCE: decrementMemory(AddressingMode::Absolute);  return 6;
        case 0xDE: decrementMemory(AddressingMode::AbsoluteX); return 7;

        case 0xCA: decrement(m_registers.xIndex); return 2;
        case 0x88: decrement(m_registers.yIndex); return 2;

        case 0x4C: jump(AddressingMode::Absolute);     return 3;
        case 0x6C: jump(AddressingMode::JumpIndirect); return 5;

        case 0x20: call(AddressingMode::Absolute); return 6;

        case 0x60: fnReturn();        return 6;
        case 0x40: interruptReturn(); return 6;

        case 0xAA: transfer(m_registers.accumulator, m_registers.xIndex); return 2;
        case 0x8A: transfer(m_registers.xIndex, m_registers.accumulator); return 2;
        case 0xA8: transfer(m_registers.accumulator, m_registers.yIndex); return 2;
        case 0x98: transfer(m_registers.yIndex, m_registers.accumulator); return 2;

        case 0x48: pushToStack(m_registers.accumulator); return 3;
        case 0x68: popAccumulatorFromStack();            return 4;

        case 0x08: pushStatus(); return 3;
        case 0x28: popStatus();  return 4;

        case 0x9A: m_registers.stackPointer = m_registers.xIndex; return 2;
        case 0xBA: setXToStackPointer(); return 2;

        case 0x10: return branch(!m_registers.status.isBitSet(CPUFlag::Negative));
        case 0x30: return branch(m_registers.status.isBitSet(CPUFlag::Negative));
        case 0x50: return branch(!m_registers.status.isBitSet(CPUFlag::Overflow));
        case 0x70: return branch(m_registers.status.isBitSet(CPUFlag::Overflow));
        case 0x90: return branch(!m_registers.status.isBitSet(CPUFlag::Carry));
        case 0xB0: return branch(m_registers.status.isBitSet(CPUFlag::Carry));
        case 0xD0: return branch(!m_registers.status.isBitSet(CPUFlag::Zero));
        case 0xF0: return branch(m_registers.status.isBitSet(CPUFlag::Zero));

        default:
            throw std::invalid_argument("Unhandled opcode: " + Utils::convertToHexString(opcode, true, 2));
    }
}

void Nes::CPU::instructionBreak() {
    pushWordToStack(m_registers.programCounter + 1);

    Byte statusCopy = m_registers.status.getCombinedValue();
    statusCopy = Utils::setBit(statusCopy, static_cast<int>(CPUFlag::BFlag));
    statusCopy = Utils::setBit(statusCopy, static_cast<int>(CPUFlag::BFlag) + 1);

    pushToStack(statusCopy);

    m_registers.status.setBit(CPUFlag::InterruptDisable);
    m_registers.programCounter = readVector(Const::VectorAddr::irq);
}

Nes::CycleCount Nes::CPU::branch(bool condition) {
    CycleCount cycles = 2;
    if (condition) {
        cycles++;

        const Addr afterPc = m_registers.programCounter + 1;
        const Addr jumpTo = afterPc + static_cast<SByte>(m_mmu.read(m_registers.programCounter));

        if (didPageCrossHappen(afterPc, jumpTo)) {
            cycles++;
        }

        m_registers.programCounter = jumpTo;
    } else {
        m_registers.programCounter++;
    }

    return cycles;
}

void Nes::CPU::storeValue(Byte value, AddressingMode addressingMode) {
    const auto addr = readAddressOperand(addressingMode);
    m_mmu.write(addr, value);
}

void Nes::CPU::loadValue(Byte& target, AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    target = operand;

    m_registers.status.setBitTo(CPUFlag::Zero, target == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(target));
}

void Nes::CPU::addWithCarry(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    const Word simulatedSum = m_registers.accumulator + operand + m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, simulatedSum > Const::maximumByteValue);

    const Byte actualSum = static_cast<Byte>(simulatedSum);

    const auto changeInSign = Utils::isBitSet(operand ^ actualSum, 7) && Utils::isBitSet(m_registers.accumulator ^ actualSum, 7);
    m_registers.status.setBitTo(CPUFlag::Overflow, changeInSign);

    m_registers.accumulator = actualSum;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::subWithCarry(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    const Word simulateDiff = m_registers.accumulator - operand - !m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, simulateDiff <= Const::maximumByteValue);

    const Byte actualDiff = static_cast<Byte>(simulateDiff);

    const auto changeInSign = Utils::isBitSet(~operand ^ actualDiff, 7) && Utils::isBitSet(m_registers.accumulator ^ actualDiff, 7);
    m_registers.status.setBitTo(CPUFlag::Overflow, changeInSign);

    m_registers.accumulator = actualDiff;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::bitwiseAnd(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator &= operand;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::bitwiseOr(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator |= operand;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::bitwiseXor(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator ^= operand;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::arithmeticShiftLeft(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = m_registers.accumulator;
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 7));

    value <<= 1;

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.status.setBitTo(CPUFlag::Zero, value == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(value));
}

void Nes::CPU::logicalShiftRight(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = m_registers.accumulator;
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 0));

    value >>= 1;

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.status.setBitTo(CPUFlag::Zero, value == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(value));
}

void Nes::CPU::rotateLeft(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = m_registers.accumulator;
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    const auto wasCarrySet = m_registers.status.isBitSet(CPUFlag::Carry);

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 7));

    value <<= 1;
    if (wasCarrySet) {
        value = Utils::setBit(value, 0);
    }

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.status.setBitTo(CPUFlag::Zero, value == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(value));
}

void Nes::CPU::rotateRight(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = m_registers.accumulator;
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    const auto wasCarrySet = m_registers.status.isBitSet(CPUFlag::Carry);

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 0));

    value >>= 1;
    if (wasCarrySet) {
        value = Utils::setBit(value, 7);
    }

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.status.setBitTo(CPUFlag::Zero, value == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(value));
}

void Nes::CPU::testBit(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.status.setBitTo(CPUFlag::Zero, (operand & m_registers.accumulator) == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(operand));
    m_registers.status.setBitTo(CPUFlag::Overflow, Utils::isBitSet(operand, 6));
}

void Nes::CPU::compare(Byte compareWith, AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.status.setBitTo(CPUFlag::Carry, compareWith >= operand);
    m_registers.status.setBitTo(CPUFlag::Zero, compareWith == operand);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(static_cast<Byte>(compareWith - operand)));
}

void Nes::CPU::incrementMemory(AddressingMode addressingMode) {
    const auto operandAddress = readAddressOperand(addressingMode);
    const Byte updatedOperand = m_mmu.read(operandAddress) + 1;

    m_mmu.write(operandAddress, updatedOperand);

    m_registers.status.setBitTo(CPUFlag::Zero, updatedOperand == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(updatedOperand));
}

void Nes::CPU::decrementMemory(AddressingMode addressingMode) {
    const auto operandAddress = readAddressOperand(addressingMode);
    const Byte updatedOperand = m_mmu.read(operandAddress) - 1;

    m_mmu.write(operandAddress, updatedOperand);

    m_registers.status.setBitTo(CPUFlag::Zero, updatedOperand == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(updatedOperand));
}

void Nes::CPU::increment(Byte& target) {
    target++;

    m_registers.status.setBitTo(CPUFlag::Zero, target == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(target));
}

void Nes::CPU::decrement(Byte& target) {
    target--;

    m_registers.status.setBitTo(CPUFlag::Zero, target == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(target));
}

void Nes::CPU::jump(AddressingMode addressingMode) {
    m_registers.programCounter = readAddressOperand(addressingMode);
}

void Nes::CPU::call(AddressingMode addressingMode) {
    pushWordToStack(m_registers.programCounter + 1);
    m_registers.programCounter = readAddressOperand(addressingMode);
}

void Nes::CPU::fnReturn() {
    m_registers.programCounter = popWordFromStack() + 1;
}

void Nes::CPU::interruptReturn() {
    const auto newStatusValue = popFromStack();
    m_registers.status.setCombinedValue(newStatusValue);
    m_registers.status.clearBit(CPUFlag::BFlag);
    m_registers.status.setBit(CPUFlag::Unused);

    m_registers.programCounter = popWordFromStack();
}

void Nes::CPU::transfer(Nes::Byte from, Nes::Byte& to) {
    to = from;

    m_registers.status.setBitTo(CPUFlag::Zero, to == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(to));
}

void Nes::CPU::pushStatus() {
    Byte statusCopy = m_registers.status.getCombinedValue();
    statusCopy = Utils::setBit(statusCopy, static_cast<int>(CPUFlag::BFlag));
    statusCopy = Utils::setBit(statusCopy, static_cast<int>(CPUFlag::Unused));

    pushToStack(statusCopy);
}

void Nes::CPU::popStatus() {
    const auto newStatusValue = popFromStack();
    m_registers.status.setCombinedValue(newStatusValue);
    m_registers.status.clearBit(CPUFlag::BFlag);
    m_registers.status.setBit(CPUFlag::Unused);
}

void Nes::CPU::popAccumulatorFromStack() {
    m_registers.accumulator = popFromStack();

    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}


void Nes::CPU::setXToStackPointer() {
    m_registers.xIndex = m_registers.stackPointer;

    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.xIndex == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.xIndex));
}

void Nes::CPU::sax(AddressingMode addressingMode) {
    const auto addr   = readAddressOperand(addressingMode);
    const auto result = m_registers.accumulator & m_registers.xIndex;

    m_mmu.write(addr, result);
}

void Nes::CPU::lax(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.xIndex      = operand;
    m_registers.accumulator = operand;

    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::dcp(AddressingMode addressingMode) {
    const auto addr    = readAddressOperand(addressingMode);
    const auto operand = m_mmu.read(addr);

    const Byte result = operand - 1;

    m_mmu.write(addr, result);
    m_registers.status.setBitTo(CPUFlag::Carry, m_registers.accumulator >= result);

    const Byte toCompare = m_registers.accumulator - result;
    m_registers.status.setBitTo(CPUFlag::Zero, toCompare == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(toCompare));
}

void Nes::CPU::las(AddressingMode addressingMode) {
    const auto addr    = readAddressOperand(addressingMode);
    const auto operand = m_mmu.read(addr);

    const Byte result = operand & m_registers.stackPointer;
    m_registers.accumulator  = result;
    m_registers.xIndex       = result;
    m_registers.stackPointer  = result;

    m_registers.status.setBitTo(CPUFlag::Zero, result == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(result));
}

void Nes::CPU::isc(AddressingMode addressingMode) {
    const auto operandAddress = readAddressOperand(addressingMode);
    const Byte updatedOperand = m_mmu.read(operandAddress) + 1;

    m_mmu.write(operandAddress, updatedOperand);

    const Word simulateDiff = m_registers.accumulator - updatedOperand - !m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, simulateDiff <= Const::maximumByteValue);

    const Byte actualDiff = static_cast<Byte>(simulateDiff);

    const auto changeInSign = Utils::isBitSet(~updatedOperand ^ actualDiff, 7) && Utils::isBitSet(m_registers.accumulator ^ actualDiff, 7);
    m_registers.status.setBitTo(CPUFlag::Overflow, changeInSign);

    m_registers.accumulator = actualDiff;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::rra(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = readOperand(addressingMode);
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    const auto wasCarrySet = m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 0));

    value >>= 1;
    if (wasCarrySet) {
        value = Utils::setBit(value, 7);
    }

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    const Word simulatedSum = m_registers.accumulator + value + m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, simulatedSum > Const::maximumByteValue);

    const Byte actualSum = static_cast<Byte>(simulatedSum);

    const auto changeInSign = Utils::isBitSet(value ^ actualSum, 7) && Utils::isBitSet(m_registers.accumulator ^ actualSum, 7);
    m_registers.status.setBitTo(CPUFlag::Overflow, changeInSign);

    m_registers.accumulator = actualSum;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::rla(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = readOperand(AddressingMode::Accumulator);
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    const auto wasCarrySet = m_registers.status.isBitSet(CPUFlag::Carry);
    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 7));

    value <<= 1;
    if (wasCarrySet) {
        value = Utils::setBit(value, 0);
    }

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.accumulator &= value;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::sre(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = readOperand(addressingMode);
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 0));

    value >>= 1;

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.accumulator ^= value;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::slo(AddressingMode addressingMode) {
    Addr operandAddress;
    Byte value;
    if (addressingMode == AddressingMode::Accumulator) {
        value = readOperand(addressingMode);
    } else {
        operandAddress = readAddressOperand(addressingMode);
        value = m_mmu.read(operandAddress);
    }

    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(value, 7));

    value <<= 1;

    if (addressingMode == AddressingMode::Accumulator) {
        m_registers.accumulator = value;
    } else {
        m_mmu.write(operandAddress, value);
    }

    m_registers.accumulator |= value;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::sha(AddressingMode addressingMode) {
    const auto addr = readAddressOperand(addressingMode);
    const Byte value = (m_registers.accumulator & ((addr >> 8)) + 1) & 0xFF;
    m_mmu.write(addr, value);
}

void Nes::CPU::shx(AddressingMode addressingMode) {
    const auto addr = readAddressOperand(addressingMode);
    const Byte value = (m_registers.xIndex & ((addr >> 8)) + 1) & 0xFF;
    m_mmu.write(addr, value);
}

void Nes::CPU::shy(AddressingMode addressingMode) {
    const auto addr = readAddressOperand(addressingMode);
    const Byte value = (m_registers.yIndex & ((addr >> 8)) + 1) & 0xFF;
    m_mmu.write(addr, value);
}

void Nes::CPU::sbx(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    Byte result = m_registers.accumulator & m_registers.xIndex;
    m_registers.status.setBitTo(CPUFlag::Carry, result >= operand);

    result -= operand;

    m_registers.status.setBitTo(CPUFlag::Zero, result == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(result));

    m_registers.xIndex = result;
}

void Nes::CPU::ane(AddressingMode addressingMode) {
    // This opcode is highly unstable, there are magic numbers possible (0xFF, 0x00, 0xEE etc)
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator = (m_registers.accumulator | Const::aneMagicNumber) & m_registers.xIndex & operand;

    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::anc(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator &= operand;

    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
    m_registers.status.setBitTo(CPUFlag::Carry, m_registers.status.isBitSet(CPUFlag::Negative));
}

void Nes::CPU::alr(AddressingMode addressingMode) {
    const auto operand = readOperand(addressingMode);

    m_registers.accumulator &= operand;
    m_registers.status.setBitTo(CPUFlag::Carry, Utils::isBitSet(m_registers.accumulator, 0));

    m_registers.accumulator >>= 1;
    m_registers.status.setBitTo(CPUFlag::Zero, m_registers.accumulator == 0);
    m_registers.status.setBitTo(CPUFlag::Negative, Utils::isValueNegative(m_registers.accumulator));
}

void Nes::CPU::jam() {
    m_registers.programCounter--;

    if (Const::logCpuJam) {
        Utils::log("CPU is jammed due to opcode at: " + Utils::convertToHexString(m_registers.programCounter, true, 4));
    }
}

void Nes::CPU::nop(std::size_t instructionSize, std::optional<AddressingMode> addressingMode) {
    if (addressingMode.has_value()) {
        (void) readAddressOperand(addressingMode.value());
    }

    m_registers.programCounter += (instructionSize - 1);
}

void Nes::CPU::unhandledInstruction(Byte opcode, AddressingMode addressingMode) {
    if (addressingMode != AddressingMode::Implicit) {
        (void) readOperand(addressingMode); // for side effects
    }

    if (Const::logIllegal) {
        Utils::log("Unhandled illegal opcode executed: " + Utils::convertToHexString(opcode, true, 2));
    }
}
