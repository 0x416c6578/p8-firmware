# Interrupts in ARM and in nRF5x SOCs:
ARM uses NVIC - Nested Vector Interrupt Controller, which boiled down means that interrupts can have a priority, meaning higher priority (lower number) interrupts will be handled before lower priority ones if both are received simultaneously.

In ARM, an interrupt is used to describe a type of _exception_. The exception number is used as the offset in the _vector table_ where "the address of the routine of the exception can be found". Such an address will be loaded into the PC and run by the processor when an interrupt is received.

As mentioned above, exceptions have a priority number. If two exceptions have the same priority and are  received simultaneously, then the one with the lower number will be handled first.

A _synchronous_ exception are fired immediately after some instruction is executed (for example a  supervisor call). An _asynchronous_ exception is not fired knowingly, for example an external interrupt.

There are 15 builtin exceptions in Cortex-M, for example system reset. Another example is the NMI, or  non-maskable interrupt, which is triggered if an error happens in other exception handlers. It has the highest priority besides reset. PendSV & SysTick are to do with RTOSes.

The exception number for an external interrupt starts at offset 16 in the vector table

Exceptions are configured with registers in the SCS (System Control Space)

## The Interrupt Control and State Register - `0xE000ED04`
It provides software control for the NMI, PendSV and SysTick exceptions, as well as (importantly) providing interrupt status information.




### Sources
https://static.docs.arm.com/ddi0403/eb/DDI0403E_B_armv7m_arm.pdf - B3.2.4  
https://interrupt.memfault.com/blog/arm-cortex-m-exceptions-and-nvic
