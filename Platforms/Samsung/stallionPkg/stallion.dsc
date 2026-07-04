##
#  Google Pixel 10a (stallion) -- Tensor G4 / zumapro
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

################################################################################
#
# Defines Section - statements that will be processed to create a Makefile.
#
################################################################################
[Defines]
  PLATFORM_NAME                  = stallion
  PLATFORM_GUID                  = 5A7A1140-0A10-49C0-9E10-7E10A5741000
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/stallionPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = stallionPkg/stallion.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = Google Tensor G4 (zumapro)
  #
  SOC_TYPE                       = 0

!include ZumaProPkg/ZumaProPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xF4B00000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Google"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Pixel 10a"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"stallion"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"Pixel_10a_stallion"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosBoardModel|"Pixel 10a"

  #
  # Simple Frame Buffer (verified: 1080x2424 ARGB8888 @ 0xFAC00000)
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1080
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2424
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|stallionPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # Input Libraries
  #
  KeypadDeviceLib|stallionPkg/Library/KeypadDeviceLib/KeypadDeviceLib.inf

  #
  # UEFI Shell Libraries
  #
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  HiiLib|MdeModulePkg/Library/UefiHiiLib/UefiHiiLib.inf
  UefiHiiServicesLib|MdeModulePkg/Library/UefiHiiServicesLib/UefiHiiServicesLib.inf
  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
  BcfgCommandLib|ShellPkg/Library/UefiShellBcfgCommandLib/UefiShellBcfgCommandLib.inf

[Components]
  #
  # Input
  #
  SiliciumPkg/Drivers/KeypadDxe/KeypadDxe.inf
  SiliciumPkg/Drivers/KeypadDeviceDxe/KeypadDeviceDxe.inf

  #
  # Storage stack (UFS -> BlockIo -> Partition -> DiskIo -> FAT)
  #
  SamsungPkg/Drivers/UfsDxe/UfsDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf

  #
  # UEFI Shell -- built under PcdShellFile's GUID so the BDS shell boot
  # option (MsBootOptionsLib) resolves it from the FV. Network/Install
  # command profiles are omitted (no NetworkPkg in this tree).
  #
  ShellPkg/Application/Shell/Shell.inf {
    <Defines>
      FILE_GUID = C57AD6B7-0515-40A8-9D21-551652854E37
    <PcdsFixedAtBuild>
      gEfiShellPkgTokenSpaceGuid.PcdShellLibAutoInitialize|FALSE
    <LibraryClasses>
      NULL|ShellPkg/Library/UefiShellLevel1CommandsLib/UefiShellLevel1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel2CommandsLib/UefiShellLevel2CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellLevel3CommandsLib/UefiShellLevel3CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDriver1CommandsLib/UefiShellDriver1CommandsLib.inf
      NULL|ShellPkg/Library/UefiShellDebug1CommandsLib/UefiShellDebug1CommandsLib.inf
  }
