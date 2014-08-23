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

| Byte | Name     | Default |
| ---  | ---      | ---     |
| 0    | Sync     | 0x55    |
| 1    | Sync     | 0x55    |
| 2    | GYRO_X_H |         |
| 3    | GYRO_X_L |         |
| 4    | GYRO_Y_H |         |
| 5    | GYRO_Y_L |         |
| 6    | GYRO_Z_H |         |
| 7    | GYRO_Z_L |         |
| 8    | MAG_X_H  |         |
| 9    | MAG_X_L  |         |
| 10   | MAG_Y_H  |         |
| 11   | MAG_Y_L  |         |
| 12   | MAG_Z_H  |         |
| 13   | MAG_Z_L  |         |
| 14   | ACC_X_H  |         |
| 15   | ACC_X_L  |         |
| 16   | ACC_Y_H  |         |
| 17   | ACC_Y_L  |         |
| 18   | ACC_Z_H  |         |
| 19   | ACC_Z_L  |         |

### Transmitting

| Byte | Name     | Value                     |
| ---  | ----     | ---                       |
| 0    | Roll     | Signed byte [-30°, +30°]  |
| 1    | Pitch    | Signed byte [-30°, +30°]  |
| 2    | Throttle | Signed byte []            |
| 3    | Yaw      | Signed byte []            |

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
