##
#  Google Tensor G4 (zumapro) Silicon Package
#
#  Derived from S5E9945Pkg (Exynos 2400) -- the Tensor G4 shares the same
#  Samsung USI/DECON/PMIC IP and the same CPU topology (Cortex-X4 + A720 + A520).
#  Register bases below are VERIFIED on a live Pixel 10a (stallion) running
#  postmarketOS -- see Pixel10A/hardwaredoc.md.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  #
  # Timer
  #
  USE_PHYSICAL_TIMER                         = 1

!include SamsungPkg/SamsungPkg.dsc.inc

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv9.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  #
  # CPU -- Tensor G4: 4x A520 (LITTLE) + 3x A720 (big) + 1x X4 (prime) = 8 cores
  #
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|3

  #
  # GIC -- GICv3 (verified: GICD @ 0x10400000, GICR @ 0x10440000)
  # NOTE: Tensor G4 is GICv3, NOT the GICv2-style CPU interface used by
  # Exynos 2400. ArmGicDxe auto-detects v3 via GICD_PIDR2 and uses the
  # redistributor base below.
  #
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x10400000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x10440000

  #
  # DDR Info (offsets inherited from Exynos 2400 -- UNVERIFIED on zumapro,
  # informational only; revisit during bring-up)
  #
  gSamsungPkgTokenSpaceGuid.PcdDdrInfoOffset|0x4F000
  gSamsungPkgTokenSpaceGuid.PcdDdrSizeInfoOffset|0x848

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"Google Tensor G4 @ 3.10 GHz"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"zumapro"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMaxCpuFreq|3100
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1InstCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel1DataCacheSize|0x20
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel2CacheSize|0x400
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosLevel3CacheSize|0x2000                              # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemorySpeed|4200                                    # Unofficial Value
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosMemoryType|35

[LibraryClasses]
  #
  # SEC Libraries
  #
  PlatformSecLib|ZumaProPkg/Library/PlatformSecLib/PlatformSecLib.inf

  #
  # Custom Libraries (GPIO bank layout reused from Exynos 2400 for now --
  # zumapro pinctrl differs; revisit before relying on GPIO/keypad)
  #
  GpioBankLib|S5E9945Pkg/Library/GpioBankLib/GpioBankLib.inf

  #
  # UFS -- real zumapro platform lib (overrides SamsungPkg's Null stub).
  # Provides UfsBoardInit + the gs101/zuma UFS CAL.
  #
  PlatformUfsLib|ZumaProPkg/Library/PlatformUfsLib/PlatformUfsLib.inf
