/**
  Google Pixel 10a (stallion) -- Tensor G4 / zumapro UFS board init.

  Register bases from the live DTB (ufs@13200000, phy@13204000) and the
  mainline Linux ufs-exynos.c / phy-gs101-ufs.c gs101/zuma paths.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/UfsHostBridge.h>

//
// UFS register bases (DTB: reg-names hci/vs_hci/unipro/ufsp, phy-pma)
//
#define ZUMA_UFS_HCI_BASE       0x13200000UL
#define ZUMA_UFS_VS_BASE        0x13201100UL
#define ZUMA_UFS_UNIPRO_BASE    0x13280000UL
#define ZUMA_UFS_UFSP_BASE      0x132A0000UL
#define ZUMA_UFS_PHY_PMA_BASE   0x13204000UL

//
// PMU (google,gs101-pmu @ 0x15460000) UFS PHY isolation control.
// TENSOR_GS101_PHY_CTRL = 0x3ec8, bit0 = 1 -> isolation bypassed / MPHY on.
// (UfsDxe's UfsPreSetup releases isolation via Ufs->PhyIsoAddr.)
//
#define ZUMA_PMU_BASE           0x15460000UL
#define ZUMA_UFS_PHY_CTRL       (ZUMA_PMU_BASE + 0x3EC8)

//
// HSI sysreg (syscon@13020000) IO coherency / shareability.
// UFS_SHAREABILITY_OFFSET = 0x710; UFS_GS101_SHARABLE = RD|WR = 0x3.
//
#define ZUMA_SYSREG_HSI_BASE    0x13020000UL
#define ZUMA_UFS_SHAREABILITY   (ZUMA_SYSREG_HSI_BASE + 0x710)
#define UFS_GS101_SHARABLE      0x3

//
// HCI vendor reg: HCI_IOP_ACG_DISABLE = 0x100, bit0 = ACG disable enable.
//
#define HCI_IOP_ACG_DISABLE     0x100
#define HCI_IOP_ACG_DISABLE_EN  (1U << 0)

//
// sclk_unipro_main rate -- drives the UniPro PCS period / line-reset DME
// writes. 166 MHz matches the Exynos UFS family; VERIFY against the live
// clock if HS link-up misbehaves (PWM should still work if slightly off).
//
#define ZUMA_UFS_MCLK_RATE      166000000UL

//
// Device power on zuma is supplied by PMIC regulators (vcc/vccq) that the
// bootloader already enabled. UfsDxe unconditionally pokes Ufs->DevPwrAddr,
// so point it at a harmless DRAM scratch to make that a no-op.
//
STATIC UINT32 mUfsDevPwrScratch = 0;

EFI_STATUS
UfsBoardInit (struct UfsHost *Ufs)
{
  UINT32 Reg;

  DEBUG ((EFI_D_INFO, "UFS: zuma board init\n"));

  // MMIO bases
  Ufs->IoAddr     = (VOID *)(UINTN)ZUMA_UFS_HCI_BASE;
  Ufs->VsAddr     = (VOID *)(UINTN)ZUMA_UFS_VS_BASE;
  Ufs->UniProAddr = (VOID *)(UINTN)ZUMA_UFS_UNIPRO_BASE;
  Ufs->UfsPaddr   = (VOID *)(UINTN)ZUMA_UFS_UFSP_BASE;
  Ufs->PhyPma     = (VOID *)(UINTN)ZUMA_UFS_PHY_PMA_BASE;

  // PHY isolation control (released later by UfsPreSetup)
  Ufs->PhyIsoAddr = (VOID *)(UINTN)ZUMA_UFS_PHY_CTRL;

  // Device power: harmless scratch (regulators already on)
  Ufs->DevPwrAddr  = (VOID *)&mUfsDevPwrScratch;
  Ufs->DevPwrShift = 0;

  Ufs->MclkRate = ZUMA_UFS_MCLK_RATE;
  Ufs->GearMode = 4;   // HS-G4 max

  // IO coherency / shareability (gs101: set RD|WR sharable)
  Reg  = MmioRead32 (ZUMA_UFS_SHAREABILITY);
  Reg |= UFS_GS101_SHARABLE;
  MmioWrite32 (ZUMA_UFS_SHAREABILITY, Reg);

  // Let ACG be controlled by UFS_ACG_DISABLE (clear the *_EN bit)
  Reg = MmioRead32 ((UINTN)(Ufs->IoAddr + HCI_IOP_ACG_DISABLE));
  MmioWrite32 ((UINTN)(Ufs->IoAddr + HCI_IOP_ACG_DISABLE), Reg & ~HCI_IOP_ACG_DISABLE_EN);

  return EFI_SUCCESS;
}
