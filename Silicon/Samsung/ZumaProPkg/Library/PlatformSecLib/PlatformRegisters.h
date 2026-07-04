#ifndef _PLATFORM_REGISTERS_H_
#define _PLATFORM_REGISTERS_H_

//
// Decon Registers (verified on stallion: writel(0x3061, 0x19470000 + 0x30))
//
#define HW_SW_TRIG_CONTROL 0x30
#define DECON_TRIG_VALUE   0x3061

//
// Watchdog Registers
// uniLoader clears WTCON bits (1 << 5) | (1 << 2) on both cluster watchdogs
// (0x10060000 and 0x10070000) to stop mid-bring-up resets.
//
#define WATCHDOG_ENABLE ((1 << 5) | (1 << 2))

#endif /* _PLATFORM_REGISTERS_H_ */
