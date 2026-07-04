/**
  Google Pixel 10a (stallion) -- zuma USB PHY register definitions.

  Ported faithfully from the mainline Linux drivers used on this device:
    - drivers/phy/phy-snps-eusb2.c        (google,zuma-eusb2-phy   @ 0x11110000)
    - drivers/phy/samsung/phy-exynos5-usbdrd.c (zuma-usb31drd-phy   @ 0x11100000)

  PHY chain (from the live DTB):
    dwc3 (usb@11210000) -> usb31drd-phy (0x11100000) -> eUSB2-phy (0x11110000)

  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef _DWC3_PHY_H_
#define _DWC3_PHY_H_

//
// eUSB2 HS PHY (phy-snps-eusb2.c, EXYNOS register block) @ 0x11110000
//
#define ZUMA_EUSB2_PHY_BASE                 0x11110000UL

#define EXYNOS_USB_PHY_HS_PHY_CTRL_RST      0x00
#define   USB_PHY_RST_MASK                  (0x3 << 0)   // GENMASK(1,0)
#define   UTMI_PORT_RST_MASK                (0x3 << 4)   // GENMASK(5,4)

#define EXYNOS_USB_PHY_HS_PHY_CTRL_COMMON   0x04
#define   RPTR_MODE                         (1U << 10)
#define   PHY_ENABLE                        (1U << 0)
#define   FSEL_MASK                         (0x7 << 4)   // GENMASK(6,4)

#define EXYNOS_USB_PHY_CFG_PLLCFG0          0x08
#define   PHY_CFG_PLL_FB_DIV_19_8_MASK      (0xFFFU << 8) // GENMASK(19,8)

#define EXYNOS_USB_PHY_CFG_PLLCFG1          0x0C
#define   EXYNOS_PHY_CFG_PLL_FB_DIV_11_8_MASK (0xFU << 8) // GENMASK(11,8)

#define EXYNOS_PHY_CFG_TX                   0x14
#define   EXYNOS_PHY_CFG_TX_FSLS_VREF_TUNE_MASK (0x3 << 1) // GENMASK(2,1)

#define EXYNOS_USB_PHY_UTMI_TESTSE          0x20
#define   TEST_IDDQ                         (1U << 6)

//
// eUSB2 ref-clk PLL config values (FSEL / fb-div). Indexed by ref clock.
// Default zuma ref clock assumed 24 MHz -- VERIFY against the live "ref" clk.
//
#define FSEL_24_MHZ_VAL                     0x2
#define DIV_19_8_24_MHZ_VAL                 0x120
#define EXYNOS_DIV_11_8_24_MHZ_VAL          0x0

#define FSEL_26_MHZ_VAL                     0x3
#define DIV_19_8_26_MHZ_VAL                 0x107
#define EXYNOS_DIV_11_8_26_MHZ_VAL          0x0

//
// usb31drd link PHY (phy-exynos5-usbdrd.c) @ 0x11100000
// Link control registers (EXYNOS850/2200 layout).
//
#define ZUMA_USBDRD_PHY_BASE                0x11100000UL

VOID
Dwc3PhyInit (
  VOID
  );

#endif /* _DWC3_PHY_H_ */
