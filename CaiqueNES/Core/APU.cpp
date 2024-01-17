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

#include "Core/APU.hpp"

Nes::APU::APU(MMU& mmu) :
    m_mmu(mmu)
{
    m_mmu.addMemoryRegion(Const::AddrRange::apuRegisters,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte byte) {
                              handleAPURegisterWrite(addr, byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr addr) {
                              return handleAPURegisterRead(addr);
                          },
                          false);

    m_mmu.addMemoryRegion(Const::AddrRange::apuRegisters,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte byte) {
                              handleAPURegisterWrite(addr, byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr addr) {
                              return handleAPURegisterRead(addr);
                          },
                          false);

    m_mmu.addMemoryRegion(Const::AddrRange::apuStatus,
                          [&](MemoryRegion*, MMU*, Addr addr, Byte byte) {
                              handleAPURegisterWrite(addr, byte);
                          },
                          [&](MemoryRegion*, MMU*, Addr addr) {
                              return handleAPURegisterRead(addr);
                          },
                          false);

    m_registers.resize(Const::AddrRange::apuRegisters.extent(), 0x00);
}

void Nes::APU::handleAPURegisterWrite(Addr addr, Byte value) {
    m_registers[addr - Const::AddrRange::apuRegisters.from] = value;
}

Nes::Byte Nes::APU::handleAPURegisterRead(Addr addr) {
    return m_registers[addr - Const::AddrRange::apuRegisters.from];
}
