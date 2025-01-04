# UART Communications Protocol

## Message Structure (8 Bytes)

| Byte       | Description                |
|------------|----------------------------|
| `<`        | Start of message byte.      |
| `ADDR`     | Device Address byte.        |
| `CMD`      | Command byte.               |
| `DATA1`    | Data byte 1.                |
| `DATA2`    | Data byte 2.                |
| `DATA3`    | Data byte 3.                |
| `DATA4`    | Data byte 4.                |
| `>`        | End of message byte.        |

## List of Commands from CRIO to STM

1. **On/Off Command**
2. **Change** - Frequency of DAC channel 1.
3. **Change** - Frequency of DAC channel 2.
4. **Change** - Amplitude of DAC channel 1.
5. **Change** - Amplitude of DAC channel 2.
6. **Request** - Voltage and Current measurement of channel 1 output.
7. **Request** - Voltage and Current measurement of channel 2 output.
8. **Request** - Temperature sensor 1 and 2 output.
9. **Acknowledge** - Message received.
10. **Bad Message** - Indication of an invalid message received.
11. **Request** - Current system state.

## List of Commands from STM to CRIO

1. **Return** - Frequency of DAC channel 1.
2. **Return** - Frequency of DAC channel 2.
3. **Return** - Amplitude of DAC channel 1.
4. **Return** - Amplitude of DAC channel 2.
5. **Return** - Voltage and Current measurement of channel 1 output.
6. **Return** - Voltage and Current measurement of channel 2 output.
7. **Return** - Temperature sensors 1 and 2 output.
8. **Acknowledge** - Message received.
9. **Bad Message** - Indication of an invalid message received.
10. **Low Power Mode** - Indication of low power mode state.
