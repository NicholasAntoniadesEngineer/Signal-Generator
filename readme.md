# Arbitrary Waveform Magnetic Field Generator (2020)

## Project Overview

This project focuses on developing an Arbitrary Waveform Generator (AWG) that allows the control of input signal frequency and amplitude through digital logic, interfacing with a CRIO (Compact Reconfigurable Input/Output) system. The system is designed to output a signal within the range of -2V to 2V peak-to-peak and supports a frequency range of 0-20kHz. The AWG is programmable via TTL or other logic interfaces.

This signal is then amplified and fed into a large coil to generate a magnetic field at the desired frequency and intensity.

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


