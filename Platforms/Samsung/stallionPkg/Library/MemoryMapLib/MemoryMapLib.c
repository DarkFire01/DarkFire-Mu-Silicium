/**
  Google Pixel 10a (stallion) -- Tensor G4 / zumapro memory map.

  Addresses VERIFIED on a live device (postmarketOS, /proc/iomem + DTB
  /memory + uniLoader board code) -- see Pixel10A/hardwaredoc.md.

  Low-bank usable System RAM fragments:
    0x80000000 - 0x901FFFFF
    0x97000000 - 0xFABFFFFF   (UEFI runs here)
    0xFD900000 - 0xFFFFFFFF
  Secure / firmware carveouts (GSA, TPU, AOC, secure DRAM) live in the gaps
  and are deliberately left unmapped.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryMapLib.h>

STATIC
EFI_MEMORY_REGION_DESCRIPTOR
gMemoryDescriptor[] = {
  // Name, Address, Length, HobOption, ResourceType, ResourceAttribute, MemoryType, ArmAttribute

  // ----- DDR (low bank) -----
  {"RAM Partition",      0x80000000, 0x10200000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // 0x80000000 -> 0x90200000

  // Carveouts 0x90200000 -> 0x97000000 (GSA / TPU / AOC / secure DRAM) left unmapped

  {"RAM Partition",      0x97000000, 0x5D800000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // 0x97000000 -> 0xF4800000
  {"UEFI FD",            0xF4800000, 0x00200000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"RAM Partition",      0xF4A00000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"UEFI Stack",         0xF4B00000, 0x00040000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
  {"DXE Heap",           0xF4B40000, 0x03C00000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK},
  {"RAM Partition",      0xF8740000, 0x024C0000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // -> 0xFAC00000
  {"Display Reserved",   0xFAC00000, 0x00A00000, AddMem, MEM_RES, SYS_MEM_CAP, Reserv, WRITE_THROUGH}, // splash FB 0x9FC900

  // Carveouts 0xFB600000 -> 0xFD900000 (dss_log / ramoops / bootloader log) left unmapped

  {"RAM Partition",      0xFD900000, 0x02700000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // -> 0x100000000

  // ----- DDR (high bank) -----
  {"RAM Partition",     0x880000000, 0x80000000, AddMem, SYS_MEM, SYS_MEM_CAP, Conv,   WRITE_BACK}, // 0x8_80000000 (2 GiB)

  // ----- Register / device regions -----
  {"Watchdog Timer",     0x10060000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // cluster0 WDT
  {"Watchdog Timer CL1", 0x10070000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // cluster1 WDT
  {"Gic Distributor",    0x10400000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // GICv3 GICD
  {"Gic Redistributors", 0x10440000, 0x00100000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // GICv3 GICR
  {"USI UART",           0x10870000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // Samsung USI UART (gs101)
  {"PMU",                0x15460000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // incl. UFS PHY control @ +0x3ec8
  {"DRM Decon",          0x19470000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // DECON_F (FB unblank poke)

  // ----- UFS (google,zuma-ufs / gs101-ufs) -----
  {"UFS HCI",            0x13200000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // hci 0x13200000, vs_hci 0x13201100, phy 0x13204000
  {"UFS UniPro",         0x13280000, 0x00008000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE},
  {"UFS Protect",        0x132A0000, 0x0000B000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // ufsp (0xa014)
  {"UFS Sysreg HSI",     0x13020000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // IO coherency @ +0x710

  // ----- USB dwc3 (google,zuma-dwusb3 / snps,dwc3) -----
  {"USB DWC3",           0x11210000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // dwc3 core (globals @ +0xC100)
  {"USB DRD PHY",        0x11100000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // usb31drd link phy (reg 0x200)
  {"USB eUSB2 PHY",      0x11110000, 0x00001000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}, // snps eusb2 HS phy (reg 0x200)
  {"USB HSI0 Sysreg",    0x11020000, 0x00010000, AddDev, MMAP_IO, UNCACHEABLE, MmIO,   NS_DEVICE}  // gs101-hsi0-sysreg
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                         *MemoryDescriptorCount)
{
  // Pass Data
  *MemoryDescriptor      = gMemoryDescriptor;
  *MemoryDescriptorCount = ARRAY_SIZE (gMemoryDescriptor);
}
