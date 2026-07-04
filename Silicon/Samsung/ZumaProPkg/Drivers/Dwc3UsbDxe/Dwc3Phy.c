/**
  Google Pixel 10a (stallion) -- zuma USB eUSB2 HS PHY bring-up.

  Faithful port of exynos_snps_eusb2_hsphy_init() from the mainline
  drivers/phy/phy-snps-eusb2.c that runs on this device under pmOS.

  NOTE: clocks/regulators and the PMU power-ungate that Linux performs
  (regulator_bulk_enable + exynos5_usbdrd_phy_isol) are assumed already
  satisfied by the bootloader chain for first bring-up; if PMA/PHY access
  SErrors, the isolation ungate must be added here.

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

#include "Dwc3Phy.h"

//
// Pick the ref-clk PLL config. Default 24 MHz (see Dwc3Phy.h).
//
#define ZUMA_EUSB2_FSEL      FSEL_24_MHZ_VAL
#define ZUMA_EUSB2_DIV_19_8  DIV_19_8_24_MHZ_VAL
#define ZUMA_EUSB2_DIV_11_8  EXYNOS_DIV_11_8_24_MHZ_VAL

STATIC
VOID
PhyWriteMask (
  IN UINTN  Base,
  IN UINT32 Offset,
  IN UINT32 Mask,
  IN UINT32 Value
  )
{
  UINT32 Reg = MmioRead32 (Base + Offset);
  Reg &= ~Mask;
  Reg |= (Value & Mask);
  MmioWrite32 (Base + Offset, Reg);
}

//
// exynos_eusb2_ref_clk_init(): program FSEL + PLL feedback dividers.
//
STATIC
VOID
Eusb2RefClkInit (
  IN UINTN Base
  )
{
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_COMMON,
                FSEL_MASK, (ZUMA_EUSB2_FSEL << 4) & FSEL_MASK);

  PhyWriteMask (Base, EXYNOS_USB_PHY_CFG_PLLCFG0,
                PHY_CFG_PLL_FB_DIV_19_8_MASK, (ZUMA_EUSB2_DIV_19_8 << 8) & PHY_CFG_PLL_FB_DIV_19_8_MASK);

  PhyWriteMask (Base, EXYNOS_USB_PHY_CFG_PLLCFG1,
                EXYNOS_PHY_CFG_PLL_FB_DIV_11_8_MASK, (ZUMA_EUSB2_DIV_11_8 << 8) & EXYNOS_PHY_CFG_PLL_FB_DIV_11_8_MASK);
}

//
// exynos_snps_eusb2_hsphy_init(): reset -> ref clk -> tune -> enable.
//
STATIC
VOID
Eusb2PhyInit (
  VOID
  )
{
  UINTN Base = (UINTN)ZUMA_EUSB2_PHY_BASE;

  DEBUG ((EFI_D_INFO, "USB: eUSB2 PHY init @ 0x%lx\n", (UINT64)Base));

  // Assert PHY + UTMI port reset
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_RST,
                USB_PHY_RST_MASK | UTMI_PORT_RST_MASK,
                USB_PHY_RST_MASK | UTMI_PORT_RST_MASK);
  MicroSecondDelay (50);   // required while held in reset

  // Repeater mode
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_COMMON, RPTR_MODE, RPTR_MODE);

  // Ref-clk PLL dividers
  Eusb2RefClkInit (Base);

  // Default tx fsls-vref tune = 0
  PhyWriteMask (Base, EXYNOS_PHY_CFG_TX, EXYNOS_PHY_CFG_TX_FSLS_VREF_TUNE_MASK, 0);

  // Release test IDDQ
  PhyWriteMask (Base, EXYNOS_USB_PHY_UTMI_TESTSE, TEST_IDDQ, 0);
  MicroSecondDelay (10);

  // Deassert PHY reset, enable PHY, deassert UTMI port reset
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_RST, USB_PHY_RST_MASK, 0);
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_COMMON, PHY_ENABLE, PHY_ENABLE);
  PhyWriteMask (Base, EXYNOS_USB_PHY_HS_PHY_CTRL_RST, UTMI_PORT_RST_MASK, 0);

  DEBUG ((EFI_D_INFO, "USB: eUSB2 PHY enabled\n"));
}

VOID
Dwc3PhyInit (
  VOID
  )
{
  // For HS-only operation the eUSB2 transceiver is the data-path PHY.
  // The usb31drd "link" PHY (0x11100000) wrapper init is added in a
  // later stage; for HS bring-up the bootloader's link setup is reused.
  Eusb2PhyInit ();
}
