# Interrupts

At a point in p8-firmware::setup() interrupts::initInterrupts() is called.

More information about tasks and events can be found in the [Nordic Semiconductor documentation](https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.nrf52832.ps.v1.1%2Fgpiote.html).

The following steps are executed:
- NRF_GPIOTE->INTENCLR = GPIOTE_INTENSET_PORT_Msk disable interrupt
- NVIC_DisableIRQ(GPIOTE_IRQn)
- NVIC_ClearPendingIRQ(GPIOTE_IRQn)
- NVIC_SetPriority(GPIOTE_IRQn, 1)
- NVIC_EnableIRQ(GPIOTE_IRQn)

The last 4 steps seem to clean and initialize the IRQ at GPIOTE_IRQn

Now that interrupts are disabled, port-based interrupts are turned on by:
```
  NRF_GPIOTE->EVENTS_PORT = 1;
  NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;

```
The hardware button and the touch screen booleans are then set via digitalRead()ing the correct input. The NRF_GPIO->PIN_CNF[index] values are then set.
