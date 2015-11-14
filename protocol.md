# Rx/Tx Protocol

## Reception

The quadcopter is by default in Rx mode, accepting all incoming data.
RF module asserts an interrupt to signal the controller a new packet is available.

An option is available in the header to indicate if a response is expected from the
drone.

A CRC is performed in the RF module; if the check fails, the data is dropped.

## Transmission

The quadcopter switches to Tx mode for a status update every 0.5 seconds.
The Tx mode will be indicated internally with a status flag.

## Packet

Every packet (both Rx/Tx) has a length indicator in first byte.

### Rx

Packets sent from the remote controller require 1 length byte and 1 header byte.

| Byte | Name   | Value   | Size           | Remarks                                |
| ---  | ---    | ---     | ---            | ---                                    |
| 0    | Length | [1, 64] | 8 bit          | Length of packet excluding Length byte |
| 1    | Header |         | 8 bit          | See Header                             |
| 2-63 | Data   |         | up to 62 bytes | n/a                                    |

Upon receiving an interrupt on IRQ0 the handler will deassert the valid bit of the command, copy the length byte to the length field, copy type to type field and data bytes to
the data array.
After completing the transfer, the valid bit will be asserted.

#### Header

| Bits | Name     | Remarks                     |
| ---  | ---      | ---                         |
| 0-3  | Reserved | n/a                         |
| 4-6  | Type     | Type indicator              |
| 7    | Return   | Set if response is expected |

Type definitions
| Type | Value | Description                                               |
| ---  | ---   | ---                                                       |
| RC   | 000   | Remote control message with roll, pitch, yaw and throttle |
| TO   | 001   | Automated take off/land                                   |
| PID  | 100   | PID parameter update                                      |
| CONF | 101   | RF module reconfigure                                     |
| PASS | 110   | Pass through mode to/from serial                          |

##### Remote Control (RC)

| Byte  | Name     | Value       |
| ---   | ----     | ---         |
| 0     | Length   | 0x12        |
| 1     | Header   | 0x80        |
| 2-5   | Roll     | [-π/6, π/6] |
| 6-7   | Pitch    | [-π/6, π/6] |
| 10-13 | Throttle | [-100, 100] |
| 14-17 | Yaw      | [-π, π]     |

##### Take-off/landing (TO)

TODO: TBD


##### PID

Allows to update PID controller values (non-persistent).
Valid values are indicated with corresponding bit in VALID bytes.
All values are 32 bit floats

| Byte  | Name        |
| ---   | ---         |
| 0-1   | VALID       |
| 2-5   | KP THROTTLE |
| 6-9   | KI THROTTLE |
| 10-13 | KD THROTTLE |
| 14-17 | KP PITCH    |
| 18-21 | KI PITCH    |
| 22-25 | KD PITCH    |
| 26-29 | KP ROLL     |
| 30-33 | KI ROLL     |
| 34-37 | KD ROLL     |
| 38-41 | KP YAW      |
| 42-45 | KI YAW      |
| 46-49 | KD YAW      |


##### RF module configuration update (CONF)

Allows reconfiguration of RF module parameters on the fly
Data to be formatted as address, value bytes, which will be passed to the RF module.

##### Pass through mode (PASS)

Send data to/from RF module from/to Serial connection.
TODO: TBD

### Tx

Status update sent from the quadcopter contains:

| Byte  | Name     | Value     | Size   | Remarks                          |
| ---   | ---      | ---       | ---    | ---                              |
| n/a   | Sync     | 0x5555    | 16 bit | Internal to RF module            |
| 0     | Length   | 0x15      | 8 bit  | Fixed when sending status update |
| 1-4   | Roll     | [-π, π]   | 32 bit | float                            |
| 5-8   | Pitch    | [-π, π]   | 32 bit | float                            |
| 9-12  | Yaw      | [-π, π]   | 32 bit | float                            |
| 13-16 | Altitude | [0, 10m]  | 32 bit | float                            |
| 17-20 | Battery  | [0, 100%] | 32 bit | float                            |


*) If other Tx messages are required, a type indicator must be included in every Tx packet
