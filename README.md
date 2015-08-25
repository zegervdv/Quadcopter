# STM-Quadcopter

A DIY quadcopter with an STM32F3-Discovery

## Goals

To build a self-stabilizing quadcopter controlled via bluetooth or RF-module.

## Components

* STM32F3-Discovery
* RN42-I/RM Bluetooth module OR MRF89XAM8A

### Pin Mapping

| Pin  | MCU Name  | Function             |
| ---  | --------  | --------             |
| PC10 | USART3 TX | Bluetooth comm.      |
| PC11 | USART3 RX | Bluetooth comm.      |
| PC12 | GPIO OUT  | Reset Bluetooth      |
| PD0  | GPIO IN   | Connection Bluetooth |
| PC6  | PWM       | Left Back motor      |
| PC7  | PWM       | Left Front motor     |
| PC8  | PWM       | Right Front motor    |
| PC9  | PWM       | Right Back motor     |
| PC1  | ADC1      | Battery Voltage      |
| PB10 | GPIO OUT  | RF Reset             |
| PB11 | GPIO OUT  | RF Data select       |
| PD9  | GPIO OUT  | RF Configure select  |
| PD8  | EXTI      | RF IRQ0              |
| PB12 | EXTI      | RF IRQ1              |
| PB13 | SPI2 SCK  | RF SCK               |
| PB14 | SPI2 MISO | RF SDO               |
| PB15 | SPI2 MOSI | RF SDI               |

## Communication

See [protocol](protocol.md)

## Getting Started

### Compiling and programming
  - Download and install Virtualbox and Vagrant
  - Download the virtual machine `vagrant up && vagrant ssh`
  - In the folder `source`: run `make` to build and `make burn` to program the
    device

### Controllers
  - Connect to the bluetooth device
  - Start the application and select the correct device

## License
See `LICENSE`
