/**
  Copyright (c) 2011-2012, ARM Limited. All rights reserved.
  Copyright (c) 2026, Pixel 10a (stallion) port.
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/IoLib.h>
#include <Library/PlatformSecLib.h>
#include <Library/MemoryMapHelperLib.h>

#include "PlatformRegisters.h"

STATIC
ARM_CORE_INFO
mArmPlatformMpCoreInfoTable[] = {
  // Mpidr, MailboxSetAddress, MailboxGetAddress, MailboxClearAddress, MailboxClearValue
  // Tensor G4 (zumapro): MPIDR Aff1 = logical core (verified from DTB cpu@ reg)

  // LITTLE cluster - Cortex-A520
  { 0x000, 0, 0, 0, 0xFFFFFFFF },
  { 0x100, 0, 0, 0, 0xFFFFFFFF },
  { 0x200, 0, 0, 0, 0xFFFFFFFF },
  { 0x300, 0, 0, 0, 0xFFFFFFFF },

  // big cluster - Cortex-A720
  { 0x400, 0, 0, 0, 0xFFFFFFFF },
  { 0x500, 0, 0, 0, 0xFFFFFFFF },
  { 0x600, 0, 0, 0, 0xFFFFFFFF },

  // prime - Cortex-X4
  { 0x700, 0, 0, 0, 0xFFFFFFFF }
};

VOID
GetPlatformCoreTable (
  OUT ARM_CORE_INFO **ArmCoreTable,
  OUT UINTN          *CoreCount)
{
  // Pass Data
  *ArmCoreTable = mArmPlatformMpCoreInfoTable;
  *CoreCount    = ARRAY_SIZE (mArmPlatformMpCoreInfoTable);
}

STATIC
VOID
DisableWatchdog (
  IN CHAR8 *RegionName)
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR WatchdogTimerRegion;

  // Locate Watchdog Timer Memory Region
  Status = LocateMemoryRegionByName (RegionName, &WatchdogTimerRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Disable Watchdog Timer (clear enable + reset-request bits)
  MmioAnd32 (WatchdogTimerRegion.Address, ~WATCHDOG_ENABLE);
}

VOID
DisableWatchdogTimer ()
{
  // Tensor G4 has two cluster watchdogs -- disable both.
  DisableWatchdog ("Watchdog Timer");
  DisableWatchdog ("Watchdog Timer CL1");
}

VOID
EnableFrameBufferWrites ()
{
  EFI_STATUS                   Status;
  EFI_MEMORY_REGION_DESCRIPTOR DrmDeconRegion;

  // Locate "DRM Decon" Memory Region
  Status = LocateMemoryRegionByName ("DRM Decon", &DrmDeconRegion);
  if (EFI_ERROR (Status)) {
    return;
  }

  // Unblank / allow framebuffer writes (DECON keeps the splash FB
  // write-protected until this poke -- verified value 0x3061).
  MmioWrite32 (DrmDeconRegion.Address + HW_SW_TRIG_CONTROL, DECON_TRIG_VALUE);
}

VOID
PlatformInitialize ()
{
  // Disable Watchdog Timers
  DisableWatchdogTimer ();

  // Enable Frame Buffer Writes
  EnableFrameBufferWrites ();
}
