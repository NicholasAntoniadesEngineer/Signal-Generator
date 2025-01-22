# Arbitrary Waveform Generator (2020)

## Project Overview

This project focuses on developing an Arbitrary Waveform Generator (AWG) that allows the control of input signal frequency and amplitude through digital logic, interfacing with a CRIO (Compact Reconfigurable Input/Output) system. The system is designed to output a signal within the range of -2V to 2V peak-to-peak and supports a frequency range of 0-20kHz. The AWG is programmable via TTL or other logic interfaces.

## Industrial Application: Electromagnetic Scale Prevention

### Overview
The Arbitrary Waveform Generator was successfully deployed in an industrial sugar processing plant to combat scale formation in pipework. The system was integrated with a high-power amplifier to drive electromagnetic coils wrapped around critical pipe sections, implementing an innovative non-chemical scale prevention solution.

### System Configuration
- **Amplification Stage**: The AWG's output feeds into a high-power amplifier capable of driving large inductive loads
- **Electromagnetic Coils**: Custom-designed solenoid coils wrapped around pipe sections
- **Frequency Range**: Optimized to operate between 1-15 kHz for maximum effectiveness
- **Signal Characteristics**: Programmable waveforms with controlled frequency sweeps to target various mineral compositions

### Working Principle
The system generates precisely controlled electromagnetic fields that influence the crystallization process of scale-forming minerals. By affecting nucleation sites within the fluid

## Features

- **STM32 Microcontroller**: Implements the waveform generation using Direct Digital Synthesis (DDS).
- **Real-Time Operating System (RTOS)**: Ensures deterministic behavior for time-sensitive applications.
- **Programmable**: Supports control over frequency and amplitude via digital logic interfaces such as TTL.
- **Power Supply**: Operates with a wide input range and provides regulated outputs for the system.

## Waveform Generation Calculations

The AWG frequency generation is controlled through the following equations:

### 1. Prescaler (PSC):
PSC = ((Fclock / Ns) / (Fsine * (Period + 1))) - 1

Where:
- `Fclock` is the system clock frequency.
- `Ns` is the number of samples.
- `Fsine` is the desired sine wave frequency.
- `Period` is the period of the waveform.

### 2. Sine Wave Frequency (Fsine):
Fsine = (Fclock / Ns) / ((Period + 1) * (PSC + 1))

### Example Calculation:
Given the following values:
- `Fclock = 90MHz`
- `Ns = 100`
- `Period = 5`
- `PSC = 10`

You can calculate the sine wave frequency (`Fsine`) as:
Fsine = (90MHz / 100) / ((5 + 1) * (10 + 1)) = 13.636 kHz

## UART Message Protocol

**8-byte message structure:**

| Byte | Description              |
| ---- | ------------------------ |
| `<`  | Start of message byte     |
| ADDR | Device address byte       |
| CMD  | Command byte              |
| DATA1| Data byte 1               |
| DATA2| Data byte 2               |
| DATA3| Data byte 3               |
| DATA4| Data byte 4               |
| `>`  | End of message byte       |

**Commands between CRIO and STM32**:
- On/off commands
- Frequency and amplitude changes for DAC channels
- Voltage, current, and temperature measurements requests

## Bootloader and Programming

- **UART Bootloader**: Allows programming via USB to serial converter with proper boot pin configurations.
- **ST-Link V2 Programmer**: Provides SWD (Serial Wire Debug) interface for flashing firmware onto the STM32.

## PCB Testing Process

- Slowly ramp up voltage to ensure the correct functionality of power supplies and components.
- Test all communication protocols and waveform generation functions.

## Testing Waveform Generation

The table below shows the appropriate number of samples (Ns) based on the desired output frequency:

| Ns   | Max Output Frequency |
| ---- | -------------------- |
| 40   | <= 20kHz             |
| 75   | <= 10kHz             |
| 180  | <= 5kHz              |
| 240  | <= 2.5kHz            |

Ensure the system scales the output signal by 0.5 and offsets it by 500.

## Videos and Resources

### STM32 Programming and Design

- General STM Programming: [YouTube Playlist](https://www.youtube.com/playlist?list=PLmY3zqJJdVeNIZ8z_yw7Db9ej3FVG0iLy)
- STM32F4 MCU Hardware Development: [ST Application Note](https://www.st.com/resource/en/application_note/dm00115714-getting-started-with-stm32f4xxxx-mcu-hardware-development-stmicroelectronics.pdf)
- STM32 PCB Design Videos: [YouTube Channel](https://www.youtube.com/c/PhilS94/featured)
  
### DAC Implementation

- DAC STM32CubeIDE HAL: [Video](https://www.youtube.com/watch?v=xe7KIdRFRoI&ab_channel=Weblearning)
- Generating a Sine Wave: [Video](https://www.youtube.com/watch?v=6Z1L6ox63j0&t=608s&ab_channel=ControllersTech)

### FreeRTOS

- STM32 FreeRTOS Implementation: [ST Tutorial Series](https://www.youtube.com/playlist?list=PLnMKNibPkDnFeFV4eBfDQ9e5IrGL_dx1Q)
