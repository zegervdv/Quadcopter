# STM-Quadcopter

A DIY quadcopter with an STM32F3-Discovery

## Goals

To build a self-stabilizing quadcopter controlled via bluetooth.

## Components

* STM32F3-Discovery
* RN42-I/RM Bluetooth module

### Pin Mapping

| Pin  | Function                       |
| ---  | --------                       |
| PC10 | USART3 TX - Bluetooth comm.    |
| PC11 | USART3 RX - Bluetooth comm.    |
| PC12 | GPIO OUT - Reset Bluetooth     |
| PD0  | GPIO IN - Connection Bluetooth |
| PC6  | PWM - Left Front motor         |
| PC7  | PWM - Left Back motor          |
| PC8  | PWM - Right Back motor         |
| PC9  | PWM - Right Front motor        |

## Communication

### Receiving

The data send from the quadcopter has following formatting:

| Byte  | Name     | Value     |
| ---   | ---      | ---       |
| 0-1   | Sync     | 0x5555    |
| 2-5   | Roll     | [-π, π]   |
| 6-9   | Pitch    | [-π, π]   |
| 10-13 | Yaw      | [-π, π]   |
| 14-17 | Altitude | [0, 10m]  |
| 18-21 | Battery  | [0, 100%] |

### Transmitting

| Byte  | Name      | Value       |
| ---   | ----      | ---         |
| 0     | Indicator |             |
| 1-4   | Roll      | [-π/6, π/6] |
| 5-8   | Pitch     | [-π/6, π/6] |
| 9-12  | Throttle  | [0, 100%]   |
| 13-16 | Yaw       | [-π, π]     |
| 17    | CRC       |             |

| Indicator | Mode            |
| ---       | ---             |
| 00        | Normal Command  |
| FF        | Takeoff/Land    |
| other     | Reserved        |

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
