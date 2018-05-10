![Microchip](microchip_logo.png)

# SAMA5D2 Xplained 9-bit Eval Setup Instructions

Basic instructions for board setup to evaluate 9-bit examples.

## uart3

Port: /dev/ttyS1

| Connector | PIO  | Description |
| --------- | -----| ----------- |
| J18 Pin 5 | PB11 | RX          |
| J18 Pin 6 | PB12 | TX          |
| J18 Pin 2 | GND  | GND         |


## uart5 (FLEXCOM)

Port: /dev/ttyS2

| Connector  | PIO  | Description |
| ---------- | -----| ----------- |
| J18 Pin 28 | PB28 | TX          |
| J18 Pin 29 | PB29 | RX          |
| J18 Pin 20 | PC0  | RTS         |
| J18 Pin 2  | GND  | GND         |

![sama5d2_xplained](sama5d2_xplained.png)


## Instructions

* Follow the instructions to write the *.img SD card image file using Etcher at the following location:
  <http://www.at91.com/linux4sam/bin/view/Linux4SAM/Sama5d2XplainedMainPage#Create_a_SD_card_with_the_demo>
* Connect something to the TX/GND pin a scope of an analyzer for one or both of the UARTs.
* Connect a serial console.
* Login with user root and no password and run one of the examples.

```
    user_example /dev/ttyS1 20000
    user_example /dev/ttyS2 20000
    p9bit_example /dev/ttyS1 20000
    p9bit_example /dev/ttyS2 20000
    senda_example /dev/ttyS2 20000
```

## Requirements

- Standard addressable 9 bit RS485.
- Only the first byte sent has the 9th bit set (address), and all subsequent bytes have this clear (data).
- No interbyte gap in the transmission between the address and data bytes (and between data bytes).
- Enable the RS485 driver control signal (feature of the FLEXCOM module in the SAMA5Dx).
- A way to specify non-standard baud rate, such as the 20K baud we use.