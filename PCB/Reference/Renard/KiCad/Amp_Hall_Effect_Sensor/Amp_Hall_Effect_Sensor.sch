EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "Amplifier Hall Sensor Board"
Date "2020-06-17"
Rev "Rev. 1"
Comp "Nanodyn Systems"
Comment1 "Designed by Renard Senekal"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Amp_Hall_Effect_Sensor:ACS758LCB-050B-PFF-T U1
U 1 1 5EEA2952
P 8450 4400
F 0 "U1" H 8450 4725 50  0000 C CNN
F 1 "ACS758LCB-050B-PFF-T" H 8450 4634 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:ACS758LCB-050B-PFF-T" H 8400 4300 50  0001 C CNN
F 3 "https://docs.rs-online.com/a0f2/0900766b80d885f6.pdf" H 8400 4300 50  0001 C CNN
	1    8450 4400
	1    0    0    -1  
$EndComp
$Comp
L Amp_Hall_Effect_Sensor:C0805 C1
U 1 1 5EEA35DC
P 7050 4150
F 0 "C1" V 7004 4278 50  0000 L CNN
F 1 "1u" V 7095 4278 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7150 4100 50  0001 C CNN
F 3 "" H 7150 4100 50  0001 C CNN
	1    7050 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 4300 7050 4400
Wire Wire Line
	7050 4000 7050 3900
Wire Wire Line
	7950 3900 7950 4300
Wire Wire Line
	7950 4300 8150 4300
$Comp
L Amp_Hall_Effect_Sensor:R0805 R2
U 1 1 5EEA757C
P 7500 4900
F 0 "R2" H 7500 5125 50  0000 C CNN
F 1 "Rlpf" H 7500 5034 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:R0805" H 7650 4800 50  0001 C CNN
F 3 "" H 7650 4800 50  0001 C CNN
	1    7500 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 4500 7950 4500
Wire Wire Line
	7950 4500 7950 4900
Wire Wire Line
	7950 4900 7750 4900
Wire Wire Line
	7050 4900 7250 4900
$Comp
L Amp_Hall_Effect_Sensor:C0805 C5
U 1 1 5EEA3EEE
P 7400 4150
F 0 "C5" V 7354 4278 50  0000 L CNN
F 1 "100n" V 7445 4278 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7500 4100 50  0001 C CNN
F 3 "" H 7500 4100 50  0001 C CNN
	1    7400 4150
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 4800 7050 4900
$Comp
L Amp_Hall_Effect_Sensor:C0805 C2
U 1 1 5EEAB992
P 7050 4650
F 0 "C2" V 7004 4778 50  0000 L CNN
F 1 "Clfp" V 7095 4778 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7150 4600 50  0001 C CNN
F 3 "" H 7150 4600 50  0001 C CNN
	1    7050 4650
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 4400 7400 4400
Connection ~ 7050 4400
Wire Wire Line
	7050 4400 7050 4500
Wire Wire Line
	7050 3900 7400 3900
Wire Wire Line
	7400 4000 7400 3900
Connection ~ 7400 3900
Wire Wire Line
	7400 3900 7950 3900
Wire Wire Line
	7400 4300 7400 4400
Connection ~ 7400 4400
Wire Wire Line
	7400 4400 8150 4400
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J1
U 1 1 5EEAF911
P 6550 3900
F 0 "J1" H 6558 4125 50  0000 C CNN
F 1 "Tab_S" H 6558 4034 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 3900 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 3900 50  0001 C CNN
	1    6550 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 3900 6750 3900
Connection ~ 7050 3900
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J2
U 1 1 5EEB14A2
P 6550 4400
F 0 "J2" H 6558 4625 50  0000 C CNN
F 1 "Tab_S" H 6558 4534 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 4400 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 4400 50  0001 C CNN
	1    6550 4400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 4400 6750 4400
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J3
U 1 1 5EEB1CB2
P 6550 4900
F 0 "J3" H 6558 5125 50  0000 C CNN
F 1 "Tab_S" H 6558 5034 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 4900 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 4900 50  0001 C CNN
	1    6550 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4900 7050 4900
Connection ~ 7050 4900
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2 MH4
U 1 1 5EEB25F7
P 9250 3900
F 0 "MH4" H 9122 3859 50  0000 R CNN
F 1 "Hole_M2" H 9122 3950 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2" H 9250 3900 50  0001 C CNN
F 3 "" H 9250 3900 50  0001 C CNN
	1    9250 3900
	-1   0    0    1   
$EndComp
Wire Wire Line
	9150 3900 8950 3900
Wire Wire Line
	8950 3900 8950 4300
Wire Wire Line
	8950 4300 8750 4300
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2 MH5
U 1 1 5EEB4C97
P 9250 4900
F 0 "MH5" H 9122 4859 50  0000 R CNN
F 1 "Hole_M2" H 9122 4950 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2" H 9250 4900 50  0001 C CNN
F 3 "" H 9250 4900 50  0001 C CNN
	1    9250 4900
	-1   0    0    1   
$EndComp
Wire Wire Line
	8750 4500 8950 4500
Wire Wire Line
	8950 4500 8950 4900
Wire Wire Line
	8950 4900 9150 4900
$Comp
L Amp_Hall_Effect_Sensor:ACS758LCB-050B-PFF-T U2
U 1 1 5EEBA5A3
P 8450 5900
F 0 "U2" H 8450 6225 50  0000 C CNN
F 1 "ACS758LCB-050B-PFF-T" H 8450 6134 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:ACS758LCB-050B-PFF-T" H 8400 5800 50  0001 C CNN
F 3 "https://docs.rs-online.com/a0f2/0900766b80d885f6.pdf" H 8400 5800 50  0001 C CNN
	1    8450 5900
	1    0    0    -1  
$EndComp
$Comp
L Amp_Hall_Effect_Sensor:C0805 C3
U 1 1 5EEBA5A9
P 7050 5650
F 0 "C3" V 7004 5778 50  0000 L CNN
F 1 "1u" V 7095 5778 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7150 5600 50  0001 C CNN
F 3 "" H 7150 5600 50  0001 C CNN
	1    7050 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 5800 7050 5900
Wire Wire Line
	7050 5500 7050 5400
Wire Wire Line
	7950 5400 7950 5800
Wire Wire Line
	7950 5800 8150 5800
$Comp
L Amp_Hall_Effect_Sensor:R0805 R3
U 1 1 5EEBA5B3
P 7500 6400
F 0 "R3" H 7500 6625 50  0000 C CNN
F 1 "Rlpf" H 7500 6534 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:R0805" H 7650 6300 50  0001 C CNN
F 3 "" H 7650 6300 50  0001 C CNN
	1    7500 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8150 6000 7950 6000
Wire Wire Line
	7950 6000 7950 6400
Wire Wire Line
	7950 6400 7750 6400
Wire Wire Line
	7050 6400 7250 6400
$Comp
L Amp_Hall_Effect_Sensor:C0805 C6
U 1 1 5EEBA5BD
P 7400 5650
F 0 "C6" V 7354 5778 50  0000 L CNN
F 1 "100n" V 7445 5778 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7500 5600 50  0001 C CNN
F 3 "" H 7500 5600 50  0001 C CNN
	1    7400 5650
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 6300 7050 6400
$Comp
L Amp_Hall_Effect_Sensor:C0805 C4
U 1 1 5EEBA5C4
P 7050 6150
F 0 "C4" V 7004 6278 50  0000 L CNN
F 1 "Clfp" V 7095 6278 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:C0805" H 7150 6100 50  0001 C CNN
F 3 "" H 7150 6100 50  0001 C CNN
	1    7050 6150
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 5900 7400 5900
Connection ~ 7050 5900
Wire Wire Line
	7050 5900 7050 6000
Wire Wire Line
	7050 5400 7400 5400
Wire Wire Line
	7400 5500 7400 5400
Connection ~ 7400 5400
Wire Wire Line
	7400 5400 7950 5400
Wire Wire Line
	7400 5800 7400 5900
Connection ~ 7400 5900
Wire Wire Line
	7400 5900 8150 5900
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J4
U 1 1 5EEBA5D4
P 6550 5400
F 0 "J4" H 6558 5625 50  0000 C CNN
F 1 "Tab_S" H 6558 5534 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 5400 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 5400 50  0001 C CNN
	1    6550 5400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 5400 6750 5400
Connection ~ 7050 5400
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J5
U 1 1 5EEBA5DC
P 6550 5900
F 0 "J5" H 6558 6125 50  0000 C CNN
F 1 "Tab_S" H 6558 6034 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 5900 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 5900 50  0001 C CNN
	1    6550 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7050 5900 6750 5900
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J6
U 1 1 5EEBA5E3
P 6550 6400
F 0 "J6" H 6558 6625 50  0000 C CNN
F 1 "Tab_S" H 6558 6534 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 6400 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 6400 50  0001 C CNN
	1    6550 6400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 6400 7050 6400
Connection ~ 7050 6400
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2 MH6
U 1 1 5EEBA5EB
P 9250 5400
F 0 "MH6" H 9122 5359 50  0000 R CNN
F 1 "Hole_M2" H 9122 5450 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2" H 9250 5400 50  0001 C CNN
F 3 "" H 9250 5400 50  0001 C CNN
	1    9250 5400
	-1   0    0    1   
$EndComp
Wire Wire Line
	9150 5400 8950 5400
Wire Wire Line
	8950 5400 8950 5800
Wire Wire Line
	8950 5800 8750 5800
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2 MH7
U 1 1 5EEBA5F4
P 9250 6400
F 0 "MH7" H 9122 6359 50  0000 R CNN
F 1 "Hole_M2" H 9122 6450 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2" H 9250 6400 50  0001 C CNN
F 3 "" H 9250 6400 50  0001 C CNN
	1    9250 6400
	-1   0    0    1   
$EndComp
Wire Wire Line
	8750 6000 8950 6000
Wire Wire Line
	8950 6000 8950 6400
Wire Wire Line
	8950 6400 9150 6400
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2.5 MH1
U 1 1 5EEC4805
P 7600 7850
F 0 "MH1" V 7654 7772 50  0000 R CNN
F 1 "Hole_M2.5" V 7563 7772 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2.5" H 7600 7850 50  0001 C CNN
F 3 "" H 7600 7850 50  0001 C CNN
	1    7600 7850
	0    -1   -1   0   
$EndComp
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2.5 MH2
U 1 1 5EEC6071
P 8150 7850
F 0 "MH2" V 8204 7772 50  0000 R CNN
F 1 "Hole_M2.5" V 8113 7772 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2.5" H 8150 7850 50  0001 C CNN
F 3 "" H 8150 7850 50  0001 C CNN
	1    8150 7850
	0    -1   -1   0   
$EndComp
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2.5 MH3
U 1 1 5EEC6548
P 8700 7850
F 0 "MH3" V 8754 7772 50  0000 R CNN
F 1 "Hole_M2.5" V 8663 7772 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2.5" H 8700 7850 50  0001 C CNN
F 3 "" H 8700 7850 50  0001 C CNN
	1    8700 7850
	0    -1   -1   0   
$EndComp
$Comp
L Amp_Hall_Effect_Sensor:Hole_M2.5 MH8
U 1 1 5EEC6A40
P 9250 7850
F 0 "MH8" V 9304 7772 50  0000 R CNN
F 1 "Hole_M2.5" V 9213 7772 50  0000 R CNN
F 2 "Amp_Hall_Effect_Sensor:Hole_M2.5" H 9250 7850 50  0001 C CNN
F 3 "" H 9250 7850 50  0001 C CNN
	1    9250 7850
	0    -1   -1   0   
$EndComp
$Comp
L power:+5V #PWR01
U 1 1 5EECDC8B
P 6750 3800
F 0 "#PWR01" H 6750 3650 50  0001 C CNN
F 1 "+5V" H 6765 3973 50  0000 C CNN
F 2 "" H 6750 3800 50  0001 C CNN
F 3 "" H 6750 3800 50  0001 C CNN
	1    6750 3800
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR02
U 1 1 5EED27A4
P 6750 4500
F 0 "#PWR02" H 6750 4250 50  0001 C CNN
F 1 "GNDA" H 6755 4327 50  0000 C CNN
F 2 "" H 6750 4500 50  0001 C CNN
F 3 "" H 6750 4500 50  0001 C CNN
	1    6750 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 3800 6750 3900
Connection ~ 6750 3900
Wire Wire Line
	6750 3900 7050 3900
Wire Wire Line
	6750 4500 6750 4400
Connection ~ 6750 4400
Wire Wire Line
	6750 4400 6650 4400
$Comp
L power:+5V #PWR03
U 1 1 5EED618F
P 6750 5300
F 0 "#PWR03" H 6750 5150 50  0001 C CNN
F 1 "+5V" H 6765 5473 50  0000 C CNN
F 2 "" H 6750 5300 50  0001 C CNN
F 3 "" H 6750 5300 50  0001 C CNN
	1    6750 5300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDA #PWR04
U 1 1 5EED753F
P 6750 6000
F 0 "#PWR04" H 6750 5750 50  0001 C CNN
F 1 "GNDA" H 6755 5827 50  0000 C CNN
F 2 "" H 6750 6000 50  0001 C CNN
F 3 "" H 6750 6000 50  0001 C CNN
	1    6750 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6750 5300 6750 5400
Connection ~ 6750 5400
Wire Wire Line
	6750 5400 7050 5400
Wire Wire Line
	6750 6000 6750 5900
Connection ~ 6750 5900
Wire Wire Line
	6750 5900 6650 5900
$Comp
L Amp_Hall_Effect_Sensor:Tab_S J7
U 1 1 5EEDBE90
P 6550 6900
F 0 "J7" H 6558 7125 50  0000 C CNN
F 1 "Tab_S" H 6558 7034 50  0000 C CNN
F 2 "Amp_Hall_Effect_Sensor:Tab_S" H 6550 6900 50  0001 C CNN
F 3 "https://za.rs-online.com/web/p/solder-tab-terminals/1177246/" H 6550 6900 50  0001 C CNN
	1    6550 6900
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 6900 7600 7750
Wire Wire Line
	7600 6900 8150 6900
Wire Wire Line
	8150 6900 8150 7750
Connection ~ 7600 6900
Wire Wire Line
	8150 6900 8700 6900
Wire Wire Line
	8700 6900 8700 7750
Connection ~ 8150 6900
Wire Wire Line
	8700 6900 9250 6900
Wire Wire Line
	9250 6900 9250 7750
Connection ~ 8700 6900
$Comp
L power:GNDA #PWR05
U 1 1 5EEE3D48
P 7050 7750
F 0 "#PWR05" H 7050 7500 50  0001 C CNN
F 1 "GNDA" H 7055 7577 50  0000 C CNN
F 2 "" H 7050 7750 50  0001 C CNN
F 3 "" H 7050 7750 50  0001 C CNN
	1    7050 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 6900 7050 6900
$Comp
L Amp_Hall_Effect_Sensor:R0805 R1
U 1 1 5EEE7862
P 7050 7350
F 0 "R1" V 7004 7438 50  0000 L CNN
F 1 "0" V 7095 7438 50  0000 L CNN
F 2 "Amp_Hall_Effect_Sensor:R0805" H 7200 7250 50  0001 C CNN
F 3 "" H 7200 7250 50  0001 C CNN
	1    7050 7350
	0    1    1    0   
$EndComp
Wire Wire Line
	7050 7100 7050 6900
Connection ~ 7050 6900
Wire Wire Line
	7050 6900 7600 6900
Wire Wire Line
	7050 7600 7050 7750
Text Notes 6250 3950 0    50   ~ 0
+5V
Text Notes 6250 4450 0    50   ~ 0
GND
Text Notes 6250 5000 0    50   ~ 0
Ch1\nIout
Text Notes 6250 5450 0    50   ~ 0
+5V
Text Notes 6250 5950 0    50   ~ 0
GND
Text Notes 6250 6500 0    50   ~ 0
Ch2\nIout
Text Notes 9700 5000 0    50   ~ 0
Ch1+\nCurrent
Text Notes 9700 4000 0    50   ~ 0
Ch1-\nCurrent
Text Notes 9700 5500 0    50   ~ 0
Ch2-\nCurrent
Text Notes 9700 6500 0    50   ~ 0
Ch2+\nCurrent
Text Notes 6250 6950 0    50   ~ 0
Earth
Text Notes 7200 3600 0    50   ~ 0
Hall Effect Current Measurement Circuits
Wire Notes Line
	6200 3500 10050 3500
Wire Notes Line
	10050 3500 10050 8050
Wire Notes Line
	10050 8050 6200 8050
Wire Notes Line
	6200 8050 6200 3500
$Comp
L power:Earth #PWR?
U 1 1 5EEC1F83
P 9800 7750
F 0 "#PWR?" H 9800 7500 50  0001 C CNN
F 1 "Earth" H 9800 7600 50  0001 C CNN
F 2 "" H 9800 7750 50  0001 C CNN
F 3 "~" H 9800 7750 50  0001 C CNN
	1    9800 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 6900 9800 6900
Wire Wire Line
	9800 6900 9800 7750
Connection ~ 9250 6900
$EndSCHEMATC
