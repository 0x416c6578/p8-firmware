# Interrupts in ARM and in nRF5x SOCs:
## Intro into Exceptions and Interrupts
ARM uses NVIC - Nested Vector Interrupt Controller, which boiled down means that interrupts can have a priority, meaning higher priority (lower number) interrupts will be handled before lower priority ones if both are received simultaneously.

In ARM, an interrupt is used to describe a type of _exception_. The exception number is used as the offset in the _vector table_ where "the address of the routine of the exception can be found". Such an address will be loaded into the PC and run by the processor when an interrupt is received.

As mentioned above, exceptions have a priority number. If two exceptions have the same priority and are  received simultaneously, then the one with the lower number will be handled first.

A _synchronous_ exception are fired immediately after some instruction is executed (for example a  supervisor call). An _asynchronous_ exception is not fired knowingly, for example an external interrupt.

Exceptions can be
- Pending = handler not yet invoked
- Active = handler running
- Pending and active (only for _asynchronous_ exceptions) = exception was detected and handler is running, but another of the same exception was detected
- Inactive = nether pending nor active

There are 15 builtin exceptions in Cortex-M, for example system reset. Another example is the NMI, or  non-maskable interrupt, which is triggered if an error happens in other exception handlers. It has the highest priority besides reset. PendSV & SysTick are to do with RTOSes.

The exception number for an external interrupt starts at offset 16 in the vector table

Exceptions are configured with registers in the SCS (System Control Space)

## The Interrupt Control and State Register (ICSR) - `0xE000ED04`
It provides software control for the NMI, PendSV and SysTick exceptions, as well as (importantly) providing interrupt status information.
- Bit 22 indicates whether an external NVIC interrupt is pending [1]
- Bits 20-12 indicate the exception number of the highest priority pending exception
- Bits 8-0 indicate the exception number of the currently executing exception. A value of 0 indicates that the processor is in _thread mode_ which basically means the processor isn't running exception handling code (remember exception numbers start from 1)

## The Application Interrupt and Reset Control Register (AIRCR) - `0xE000ED0C`
It sets and returns interrupt control data
- Bits 31-16 is the vector key, writes to this register must write 0x05FA to this field otherwise they are ignored
- Bits 10-8 indicate _priority grouping_ which is a mechanism to divide priority levels by sub-priorities, meaning that if two exceptions of the same priority are received simultaneously, the logic will pick the one with the lowest sub-priority rather than the lowest exception number
- Bit 2 is SYSRESETREQ. Writing a 1 to this bit will request a local reset
- Bit 1 is VECTCLRACTIVE, which writing 1 to it will clear all active state information for exceptions, including setting ICSR to 0

## System Handler Priority Registers (SHRPn) - `0xE000ED18`
- Exceptions 1-3 (Reset, NMI, Hardfault) have fixed priorities, so SHRP1 starts from exception 4 in bits 0-7, then 5 in bits 8-15 and so on. SHRP2 and 3 follow this pattern up to exception 15 (B3.2.9 - B3.2.12)

## Interrupt Controller Type Register
- The first 4 bits of this register denote the total number of interrupt lines in an implementation, with the total being 32*(numberInFirst4Bits+1)

## The NVIC Interrupt Controller
- Both level-interrupts and pulse-interrupts can be handled
- Interrupts can be enabled and disabled by writing to their corresponding Interrupt Set-Enable or Interrupt Clear-Enable register bit-field
  - When an interrupt is disabled, triggering that interrupt will make it pending, but never active
  - 



### Sources
https://static.docs.arm.com/ddi0403/eb/DDI0403E_B_armv7m_arm.pdf - B3.2.4 onwards  
https://interrupt.memfault.com/blog/arm-cortex-m-exceptions-and-nvic - Largely based off this
