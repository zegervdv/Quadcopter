# STM-Quadcopter

A DIY quadcopter with an STM32F3-Discovery

## Goals

To build a self-stabilizing quadcopter controlled via bluetooth.

## Components

* STM32F3-Discovery
* RN42-I/RM Bluetooth module

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
