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
#include "Core/MMU.hpp"

TEST(Core_MMU, ReadWrite_DefaultAccessors_WithBuiltInMemory) {
    Nes::Cartridge mockCart;
    Nes::MMU mmu(mockCart);
    mmu.clearMemoryRegions();

    Utils::Range<Nes::Addr> newAddrRange = {0xB000, 0xBFFF};

    mmu.addMemoryRegion(newAddrRange);

    mmu.write(0xB000, 0xAB);
    mmu.write(0xBFFF, 0xCD);

    ASSERT_EQ(mmu.read(0xB000), 0xAB);
    ASSERT_EQ(mmu.read(0xBFFF), 0xCD);
}

TEST(Core_MMU, ReadWrite_16bit) {
    Nes::Cartridge mockCart;
    Nes::MMU mmu(mockCart);
    mmu.clearMemoryRegions();

    Utils::Range<Nes::Addr> newAddrRange = {0xB000, 0xBFFF};

    mmu.addMemoryRegion(newAddrRange);

    mmu.writeWord(0xB000, 0xABCD);

    ASSERT_EQ(mmu.read(0xB000), 0xCD);
    ASSERT_EQ(mmu.read(0xB001), 0xAB);
    ASSERT_EQ(mmu.readWord(0xB000), 0xABCD);
}

TEST(Core_MMU, ReadWrite_CustomAccessors_WithBuiltInMemory) {
    Nes::Cartridge mockCart;
    Nes::MMU mmu(mockCart);
    mmu.clearMemoryRegions();

    Utils::Range<Nes::Addr> newAddrRange = {0xB000, 0xBFFF};

    mmu.addMemoryRegion(newAddrRange,
                        [&](Nes::MemoryRegion* region, Nes::MMU*,  Nes::Addr addr,  Nes::Byte value) {
                            region->memory.at(addr - region->addrRange.from) = value;
                        },
                        [&]( Nes::MemoryRegion* region, const  Nes::MMU*,  Nes::Addr addr) {
                            return region->memory.at(addr - region->addrRange.from);
                        },
                        true);

    mmu.write(0xB000, 0xAB);
    mmu.write(0xBFFF, 0xCD);

    ASSERT_EQ(mmu.read(0xB000), 0xAB);
    ASSERT_EQ(mmu.read(0xBFFF), 0xCD);
}

TEST(Core_MMU, ReadWrite_CustomAccessors_WithBuiltInMemory_OutOfRange) {
    Nes::Cartridge mockCart;
    Nes::MMU mmu(mockCart);
    mmu.clearMemoryRegions();

    Utils::Range<Nes::Addr> newAddrRange = {0xB000, 0xBFFF};

    mmu.addMemoryRegion(newAddrRange,
                        [&](Nes::MemoryRegion* region, Nes::MMU*,  Nes::Addr addr,  Nes::Byte value) {
                            region->memory.at(addr - region->addrRange.from) = value;
                        },
                        [&]( Nes::MemoryRegion* region, const  Nes::MMU*,  Nes::Addr addr) {
                            return region->memory.at(addr - region->addrRange.from);
                        },
                        true);
}

TEST(Core_MMU, ReadWrite_CustomAccessors_WithExternalMemory) {
    Nes::Cartridge mockCart;
    Nes::MMU mmu(mockCart);
    mmu.clearMemoryRegions();

    Utils::Range<Nes::Addr> newAddrRange = {0xB000, 0xBFFF};

    Nes::Byte mockMemory = 0x00;
    mmu.addMemoryRegion(newAddrRange,
                        [&](Nes::MemoryRegion*, Nes::MMU*,  Nes::Addr,  Nes::Byte value) {
                            mockMemory = value;
                        },
                        [&]( Nes::MemoryRegion*, const Nes::MMU*,  Nes::Addr) {
                            return mockMemory;
                        },
                        false);

    mmu.write(0xB000, 0xAB);

    ASSERT_EQ(mockMemory, 0xAB);
    ASSERT_EQ(mmu.read(0xB000), 0xAB);
}
