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

#include <gtest/gtest.h>
#define TESTING_ENVIRONMENT_PPU 1
#include "Core/Cartridge.hpp"
#include "Core/MMU.hpp"
#include "Core/PPU.hpp"
#undef TESTING_ENVIRONMENT_PPU

TEST(Core_PPU, Nametables_Write) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x23);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::data, 0x66);

    ASSERT_EQ(ppu.accessNametables()[0x0305], 0x66);
}

TEST(Core_PPU, Nametables_Read) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::control, 0x00);
    ppu.accessNametables()[0x0305] = 0x66;

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x23);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.getAddrRegister(), 0x2306);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);
}

TEST(Core_PPU, Nametables_Read_CrossPage) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::control, 0x00);
    ppu.accessNametables()[0x01FF] = 0x66;
    ppu.accessNametables()[0x0200] = 0x77;

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x21);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0xFF);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x77);
}

TEST(Core_PPU, Nametables_Read_Step) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::control, 0b100);
    ppu.accessNametables()[0x01FF] = 0x66;
    ppu.accessNametables()[0x01FF + 32] = 0x77;
    ppu.accessNametables()[0x01FF + 64] = 0x88;

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x21);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0xFF);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x77);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x88);
}

TEST(Core_PPU, Nametables_Mirror_Horizontal) {
    Nes::Cartridge cartridge;
    cartridge.forceMirroring(Nes::Mirroring::Horizontal);

    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x24);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::data, 0x66);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x28);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::data, 0x77);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x20);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x2C);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x77);
}

TEST(Core_PPU, Nametables_Mirror_Vertical) {
    Nes::Cartridge cartridge;
    cartridge.forceMirroring(Nes::Mirroring::Vertical);

    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x20);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::data, 0x66);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x2C);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::data, 0x77);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x28);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x24);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);
    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);

    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x77);
}

TEST(Core_PPU, Nametables_Mirroring) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::control, 0x00);
    ppu.accessNametables()[0x0305] = 0x66;

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x63);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);
}

TEST(Core_PPU, OAM_ReadWrite) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::oamAddr, 0x10);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::oamData, 0x66);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::oamData, 0x77);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::oamAddr, 0x10);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::oamData), 0x66);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::oamAddr, 0x11);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::oamData), 0x77);
}

TEST(Core_PPU, Status_ResetsVBlank) {
    Nes::Cartridge cartridge;
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.accesssStatus().setBit(Nes::StatusRegisterFlag::VBlank);

    const auto statusFromRead = ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::status);

    ASSERT_EQ(statusFromRead >> 7, 1);
    ASSERT_EQ(ppu.accesssStatus().getCombinedValue() >> 7, 0);
}

TEST(Core_PPU, Status_LatchReset) {
    Nes::Cartridge cartridge;
    cartridge.loadEmptyTestCartridge();
    cartridge.forceMirroring(Nes::Mirroring::Horizontal);
    Nes::MMU mmu(cartridge);
    Nes::PPU ppu(mmu, [&](auto){});

    ppu.accessNametables()[0x0305] = 0x66;
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x21);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x23);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);
    ASSERT_NE(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::status);

    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x23);
    ppu.handlePPURegisterWrite(Nes::Const::RegisterAddress::addr, 0x05);

    ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data);
    ASSERT_EQ(ppu.handlePPURegisterRead(Nes::Const::RegisterAddress::data), 0x66);
}
