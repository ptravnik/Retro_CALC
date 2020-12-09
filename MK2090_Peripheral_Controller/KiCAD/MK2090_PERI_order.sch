EESchema Schematic File Version 4
LIBS:MK2090_PERI_order-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "MK2090 Peripherals Board"
Date "2020-12-06"
Rev "3"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Conn_01x02_Male J3
U 1 1 5FAB2189
P 1300 1000
F 0 "J3" H 1408 1181 50  0000 C CNN
F 1 "03962a (POS)" H 1408 1090 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1300 1000 50  0001 C CNN
F 3 "~" H 1300 1000 50  0001 C CNN
	1    1300 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J4
U 1 1 5FAB2DF5
P 1300 2400
F 0 "J4" H 1408 2581 50  0000 C CNN
F 1 "03962a (NEG)" H 1350 2200 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1300 2400 50  0001 C CNN
F 3 "~" H 1300 2400 50  0001 C CNN
	1    1300 2400
	1    0    0    -1  
$EndComp
$Comp
L Relay_SolidState:TLP3544 U1
U 1 1 5FAC69DC
P 2400 1600
F 0 "U1" H 2400 1925 50  0000 C CNN
F 1 "TLP3544" H 2400 1834 50  0000 C CNN
F 2 "Housings_DIP:DIP-6_W7.62mm" H 2400 1300 50  0001 C CNN
F 3 "https://toshiba.semicon-storage.com/info/docget.jsp?did=12661&prodName=TLP3544" H 2400 1600 50  0001 C CNN
	1    2400 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 5FADCA5D
P 2400 2300
F 0 "R2" V 2400 2400 50  0000 R CNN
F 1 "4.7M" V 2300 2400 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2330 2300 50  0001 C CNN
F 3 "~" H 2400 2300 50  0001 C CNN
	1    2400 2300
	-1   0    0    1   
$EndComp
$Comp
L Device:R R9
U 1 1 5FB39CE5
P 6000 3650
F 0 "R9" V 6000 3750 50  0000 R CNN
F 1 "15k" V 5900 3750 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 5930 3650 50  0001 C CNN
F 3 "~" H 6000 3650 50  0001 C CNN
	1    6000 3650
	-1   0    0    1   
$EndComp
$Comp
L promicro:ProMicro U3
U 1 1 5FB94D90
P 7700 3550
F 0 "U3" H 7850 4250 60  0000 C CNN
F 1 "ProMicro" H 7700 3200 60  0000 C CNN
F 2 "Housings_DIP:DIP-24_W15.24mm" H 7800 2500 60  0001 C CNN
F 3 "" H 7800 2500 60  0000 C CNN
	1    7700 3550
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR02
U 1 1 5FBAA728
P 2400 3900
F 0 "#PWR02" H 2400 3650 50  0001 C CNN
F 1 "GND" H 2405 3727 50  0000 C CNN
F 2 "" H 2400 3900 50  0001 C CNN
F 3 "" H 2400 3900 50  0001 C CNN
	1    2400 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 5FBB00E4
P 4300 1200
F 0 "R7" V 4300 1300 50  0000 R CNN
F 1 "4.7M" V 4200 1300 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4230 1200 50  0001 C CNN
F 3 "~" H 4300 1200 50  0001 C CNN
	1    4300 1200
	-1   0    0    1   
$EndComp
$Comp
L Device:R R8
U 1 1 5FBB049D
P 4300 1600
F 0 "R8" V 4300 1700 50  0000 R CNN
F 1 "4.7M" V 4200 1700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 4230 1600 50  0001 C CNN
F 3 "~" H 4300 1600 50  0001 C CNN
	1    4300 1600
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FBB7478
P 6200 3900
F 0 "#PWR07" H 6200 3650 50  0001 C CNN
F 1 "GND" H 6205 3727 50  0000 C CNN
F 2 "" H 6200 3900 50  0001 C CNN
F 3 "" H 6200 3900 50  0001 C CNN
	1    6200 3900
	1    0    0    -1  
$EndComp
NoConn ~ 8400 2800
NoConn ~ 8400 2900
$Comp
L Connector:Conn_01x08_Male J8
U 1 1 5FBC8862
P 9700 4400
F 0 "J8" H 9800 3850 50  0000 C CNN
F 1 "Keyboard" H 9800 4850 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x08_Pitch2.54mm" H 9700 4400 50  0001 C CNN
F 3 "~" H 9700 4400 50  0001 C CNN
	1    9700 4400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9350 4800 9500 4800
Wire Wire Line
	8400 3700 8750 3700
Wire Wire Line
	8400 3800 8700 3800
Wire Wire Line
	8400 3900 8650 3900
Wire Wire Line
	8400 3600 8800 3600
Wire Wire Line
	8400 3400 8900 3400
Wire Wire Line
	9500 4200 9150 4200
Wire Wire Line
	9100 4300 9500 4300
Wire Wire Line
	9050 4400 9500 4400
Wire Wire Line
	9000 4500 9500 4500
Wire Wire Line
	6700 3500 6700 5100
Wire Wire Line
	6700 3500 7000 3500
Wire Wire Line
	6750 3600 6750 5150
Wire Wire Line
	6750 3600 7000 3600
Wire Wire Line
	6800 3700 6800 5200
Wire Wire Line
	6800 3700 7000 3700
Wire Wire Line
	6800 5200 8200 5200
Wire Wire Line
	6850 3800 6850 5250
Wire Wire Line
	6850 3800 7000 3800
Wire Wire Line
	6850 5250 8400 5250
Wire Wire Line
	6900 3900 7000 3900
Wire Wire Line
	6950 2800 7000 2800
$Comp
L Device:R R11
U 1 1 5FC63249
P 8000 5600
F 0 "R11" V 8000 5700 50  0000 R CNN
F 1 "11K" V 7900 5700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7930 5600 50  0001 C CNN
F 3 "~" H 8000 5600 50  0001 C CNN
	1    8000 5600
	-1   0    0    1   
$EndComp
Wire Wire Line
	6700 5100 7800 5100
$Comp
L Device:R R12
U 1 1 5FC68B65
P 8200 5600
F 0 "R12" V 8200 5700 50  0000 R CNN
F 1 "11K" V 8100 5700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8130 5600 50  0001 C CNN
F 3 "~" H 8200 5600 50  0001 C CNN
	1    8200 5600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R13
U 1 1 5FC68DC9
P 8400 5600
F 0 "R13" V 8400 5700 50  0000 R CNN
F 1 "11K" V 8300 5700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8330 5600 50  0001 C CNN
F 3 "~" H 8400 5600 50  0001 C CNN
	1    8400 5600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R14
U 1 1 5FC6907F
P 8600 5600
F 0 "R14" V 8600 5700 50  0000 R CNN
F 1 "11K" V 8500 5700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 8530 5600 50  0001 C CNN
F 3 "~" H 8600 5600 50  0001 C CNN
	1    8600 5600
	-1   0    0    1   
$EndComp
Wire Wire Line
	8400 5450 8400 5250
Connection ~ 8400 5250
$Comp
L Device:R R10
U 1 1 5FC78A1A
P 7800 5600
F 0 "R10" V 7800 5700 50  0000 R CNN
F 1 "11K" V 7700 5700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 7730 5600 50  0001 C CNN
F 3 "~" H 7800 5600 50  0001 C CNN
	1    7800 5600
	-1   0    0    1   
$EndComp
Wire Wire Line
	7800 5450 7800 5100
Wire Wire Line
	6750 5150 8000 5150
Wire Wire Line
	8000 5450 8000 5150
Wire Wire Line
	8200 5450 8200 5200
$Comp
L Connector:Conn_01x04_Female J9
U 1 1 5FC89B57
P 9750 3300
F 0 "J9" H 9650 3000 50  0000 L CNN
F 1 "RTC" H 9600 3700 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm" H 9750 3300 50  0001 C CNN
F 3 "~" H 9750 3300 50  0001 C CNN
	1    9750 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5FC8AE06
P 9350 3500
F 0 "#PWR012" H 9350 3250 50  0001 C CNN
F 1 "GND" H 9355 3327 50  0000 C CNN
F 2 "" H 9350 3500 50  0001 C CNN
F 3 "" H 9350 3500 50  0001 C CNN
	1    9350 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	9350 3500 9550 3500
$Comp
L Connector:Conn_01x10_Male J7
U 1 1 5FD213CB
P 9700 2100
F 0 "J7" H 9800 1450 50  0000 C CNN
F 1 "MK2090 PROC" H 9600 2650 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Angled_1x10_Pitch2.54mm" H 9700 2100 50  0001 C CNN
F 3 "~" H 9700 2100 50  0001 C CNN
	1    9700 2100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9350 2600 9500 2600
Wire Wire Line
	6650 5050 9200 5050
Wire Wire Line
	9200 5050 9200 4100
Wire Wire Line
	9200 3400 9550 3400
Wire Wire Line
	9200 4100 9500 4100
Wire Wire Line
	6650 3100 6650 5050
$Comp
L Connector:Conn_01x06_Female J10
U 1 1 5FF1B976
P 9950 3200
F 0 "J10" H 9900 2800 50  0000 L CNN
F 1 "DS3231" H 10050 3400 50  0000 L CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x06_Pitch2.54mm" H 9950 3200 50  0001 C CNN
F 3 "~" H 9950 3200 50  0001 C CNN
	1    9950 3200
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8900 1900 9500 1900
Wire Wire Line
	8850 2000 9500 2000
Wire Wire Line
	8800 2100 9500 2100
Wire Wire Line
	8750 2200 9500 2200
Wire Wire Line
	8700 2300 9500 2300
Wire Wire Line
	8650 2400 9500 2400
Wire Wire Line
	6750 3000 6750 2500
Wire Wire Line
	6750 3000 7000 3000
$Comp
L Switch:SW_Push SW1
U 1 1 5FF7DF12
P 8600 1050
F 0 "SW1" H 8400 1150 50  0000 C CNN
F 1 "Reset" H 8600 950 50  0000 C CNN
F 2 "Buttons_Switches_THT:SW_PUSH_6mm" H 8600 1250 50  0001 C CNN
F 3 "~" H 8600 1250 50  0001 C CNN
	1    8600 1050
	1    0    0    -1  
$EndComp
Wire Wire Line
	8650 2400 8650 3900
Wire Wire Line
	8700 2300 8700 3800
Wire Wire Line
	8750 2200 8750 3700
Wire Wire Line
	8800 2100 8800 3600
Wire Wire Line
	8900 1900 8900 3400
NoConn ~ 10150 3000
Wire Wire Line
	8550 4700 9500 4700
Wire Wire Line
	6750 2500 8950 2500
$Comp
L power:GND #PWR05
U 1 1 5FE8B8EC
P 5550 1350
F 0 "#PWR05" H 5550 1100 50  0001 C CNN
F 1 "GND" H 5555 1177 50  0000 C CNN
F 2 "" H 5550 1350 50  0001 C CNN
F 3 "" H 5550 1350 50  0001 C CNN
	1    5550 1350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5FE9903D
P 6500 1350
F 0 "#PWR08" H 6500 1100 50  0001 C CNN
F 1 "GND" H 6505 1177 50  0000 C CNN
F 2 "" H 6500 1350 50  0001 C CNN
F 3 "" H 6500 1350 50  0001 C CNN
	1    6500 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 3100 9150 3100
$Comp
L Device:D_ALT D2
U 1 1 60127A59
P 6650 1000
F 0 "D2" H 6550 1150 50  0000 C CNN
F 1 "1N4004" H 6550 900 50  0000 C CNN
F 2 "Diodes_THT:D_T-1_P10.16mm_Horizontal" H 6650 1000 50  0001 C CNN
F 3 "~" H 6650 1000 50  0001 C CNN
	1    6650 1000
	-1   0    0    -1  
$EndComp
$Comp
L Device:D_ALT D1
U 1 1 60189794
P 5300 3000
F 0 "D1" H 5150 2950 50  0000 C CNN
F 1 "1N5819" H 5550 2950 50  0000 C CNN
F 2 "Diodes_THT:D_T-1_P5.08mm_Horizontal" H 5300 3000 50  0001 C CNN
F 3 "~" H 5300 3000 50  0001 C CNN
	1    5300 3000
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR09
U 1 1 6025577F
P 7800 5950
F 0 "#PWR09" H 7800 5700 50  0001 C CNN
F 1 "GND" H 7805 5777 50  0000 C CNN
F 2 "" H 7800 5950 50  0001 C CNN
F 3 "" H 7800 5950 50  0001 C CNN
	1    7800 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	9150 3100 9150 3750
Wire Wire Line
	9150 3750 10300 3750
Wire Wire Line
	10300 3750 10300 3100
Wire Wire Line
	10300 3100 10150 3100
NoConn ~ 10150 3200
NoConn ~ 10150 3300
NoConn ~ 10150 3400
NoConn ~ 10150 3500
Text Notes 9500 1400 0    50   ~ 0
1 - Raw battery (+3.0 to +4.2V)\n2 - Regulated +4.3 V\n3 - Serial activity LED (Arduino D4)\n4 - Piezo PWM Out (Arduino D10)\n5 - LCD PWM Out (Arduino D6)\n6 - Software Serial HALT (Arduino D7)\n7 - Software Serial TX (Arduino D8)\n8 - Software Serial RX (Arduino D9)\n9 - Arduino Reset\n10 - GND
Connection ~ 8950 2500
Wire Wire Line
	8950 2500 9500 2500
Text Notes 10400 3450 0    50   ~ 0
1 - 32KHz\n2 - SQW\n3 - SCL\n4 - SDA\n5 - VCC +3.3V\n6 - GND
Text Notes 9600 5700 0    50   ~ 0
1 - VCC +3.3V\n2 - KBD RST (Arduino D15)\n3 - KBD CLK (Arduino D16)\n4 - KBD READ (Arduino A0)\n5 - KBD LED RST (Arduino D14)\n6 - KBD LED CLK (Arduino D5)\n7 - KBD POWER ON/OFF\n8 - GND
Wire Wire Line
	8600 5750 8600 5800
Wire Wire Line
	8600 5800 8400 5800
Wire Wire Line
	8400 5750 8400 5800
Connection ~ 8400 5800
Wire Wire Line
	8400 5800 8200 5800
Wire Wire Line
	8200 5750 8200 5800
Connection ~ 8200 5800
Wire Wire Line
	8200 5800 8000 5800
Wire Wire Line
	8000 5750 8000 5800
Connection ~ 8000 5800
Wire Wire Line
	8000 5800 7800 5800
Wire Wire Line
	7800 5750 7800 5800
Wire Wire Line
	8600 5800 9350 5800
Wire Wire Line
	9350 5800 9350 4800
Connection ~ 8600 5800
Wire Wire Line
	9100 4300 9100 5250
Wire Wire Line
	8400 5250 9100 5250
Wire Wire Line
	9050 4400 9050 5100
Wire Wire Line
	9050 5100 7800 5100
Connection ~ 7800 5100
Wire Wire Line
	9000 4500 9000 5200
Wire Wire Line
	9000 5200 8200 5200
Connection ~ 8200 5200
Wire Wire Line
	8000 5150 9150 5150
Wire Wire Line
	9150 4200 9150 5150
Connection ~ 8000 5150
Wire Wire Line
	8400 3500 8600 3500
Wire Wire Line
	8850 2000 8850 4050
Wire Wire Line
	8850 4050 6900 4050
Wire Wire Line
	6900 4050 6900 3900
Wire Wire Line
	8400 3200 9550 3200
Wire Wire Line
	8400 3300 9550 3300
$Comp
L Transistor_FET:2N7000 Q1
U 1 1 5FCFBAD9
P 2200 2050
F 0 "Q1" H 2406 2096 50  0000 L CNN
F 1 "2N7000" H 2406 2005 50  0000 L CNN
F 2 "TO_SOT_Packages_THT:TO-92_Molded_Narrow_Oval" H 2400 1975 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7000.pdf" H 2200 2050 50  0001 L CNN
	1    2200 2050
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5FD1581F
P 2100 1200
F 0 "R1" V 2100 1300 50  0000 R CNN
F 1 "470*" V 2000 1300 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2030 1200 50  0001 C CNN
F 3 "~" H 2100 1200 50  0001 C CNN
	1    2100 1200
	-1   0    0    1   
$EndComp
Wire Wire Line
	2100 1050 2100 1000
Connection ~ 2100 1000
Wire Wire Line
	2100 1500 2100 1350
Wire Wire Line
	2100 1850 2100 1700
Wire Wire Line
	2100 1000 2700 1000
Wire Wire Line
	2700 1500 2700 1000
Wire Wire Line
	2700 1700 2850 1700
NoConn ~ 2700 1600
Wire Wire Line
	6400 1000 6500 1000
Wire Wire Line
	6800 1000 6950 1000
$Comp
L power:GND #PWR03
U 1 1 5FFCA89E
P 3900 3900
F 0 "#PWR03" H 3900 3650 50  0001 C CNN
F 1 "GND" H 3905 3727 50  0000 C CNN
F 2 "" H 3900 3900 50  0001 C CNN
F 3 "" H 3900 3900 50  0001 C CNN
	1    3900 3900
	1    0    0    -1  
$EndComp
$Comp
L 4xxx:4023 U2
U 1 1 60123414
P 3150 2050
F 0 "U2" H 3150 2375 50  0000 C CNN
F 1 "4023" H 3150 2284 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 3150 2050 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4011bms-12bms-23bms.pdf" H 3150 2050 50  0001 C CNN
	1    3150 2050
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 601350AB
P 3550 1600
F 0 "R4" V 3550 1700 50  0000 R CNN
F 1 "15k" V 3450 1700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3480 1600 50  0001 C CNN
F 3 "~" H 3550 1600 50  0001 C CNN
	1    3550 1600
	-1   0    0    1   
$EndComp
$Comp
L Device:R R5
U 1 1 60135977
P 3850 1600
F 0 "R5" V 3850 1700 50  0000 R CNN
F 1 "15k" V 3750 1700 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3780 1600 50  0001 C CNN
F 3 "~" H 3850 1600 50  0001 C CNN
	1    3850 1600
	-1   0    0    1   
$EndComp
Wire Wire Line
	3450 2050 3850 2050
$Comp
L 4xxx:4023 U2
U 2 1 60177C6F
P 3150 2850
F 0 "U2" H 3150 3175 50  0000 C CNN
F 1 "4023" H 3150 3084 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 3150 2850 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4011bms-12bms-23bms.pdf" H 3150 2850 50  0001 C CNN
	2    3150 2850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2800 2250 3500 2250
Wire Wire Line
	3500 2250 3500 2150
Wire Wire Line
	3500 2150 3450 2150
$Comp
L 4xxx:4023 U2
U 3 1 601FB832
P 3900 2450
F 0 "U2" H 3900 2775 50  0000 C CNN
F 1 "4023" H 3900 2684 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 3900 2450 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4011bms-12bms-23bms.pdf" H 3900 2450 50  0001 C CNN
	3    3900 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2850 2850 2850
Wire Wire Line
	2800 2250 2800 2850
Connection ~ 3850 2050
Wire Wire Line
	3450 1950 3550 1950
Connection ~ 3550 1950
Wire Wire Line
	3550 1950 3550 2350
Wire Wire Line
	3550 2350 3600 2350
Wire Wire Line
	3550 2350 3550 2450
Wire Wire Line
	3550 2450 3600 2450
Connection ~ 3550 2350
Wire Wire Line
	3550 2450 3550 2550
Wire Wire Line
	3550 2550 3600 2550
Connection ~ 3550 2450
Wire Wire Line
	3450 2750 3500 2750
Wire Wire Line
	3450 2950 3500 2950
Wire Wire Line
	3450 2850 3500 2850
Wire Wire Line
	3500 2850 3500 2950
Wire Wire Line
	3500 2750 3500 2850
Connection ~ 3500 2750
Wire Wire Line
	3500 2750 3900 2750
Connection ~ 3500 2850
$Comp
L Device:R R6
U 1 1 6019DBEA
P 3900 3000
F 0 "R6" V 3900 3100 50  0000 R CNN
F 1 "15k" V 3800 3100 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 3830 3000 50  0001 C CNN
F 3 "~" H 3900 3000 50  0001 C CNN
	1    3900 3000
	-1   0    0    1   
$EndComp
Wire Wire Line
	3900 2750 3900 2850
Wire Wire Line
	1500 1000 1700 1000
$Comp
L Connector:Conn_01x01_Male J1
U 1 1 6042A6E9
P 650 2850
F 0 "J1" H 758 3031 50  0000 C CNN
F 1 "+Raw" H 758 2940 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 650 2850 50  0001 C CNN
F 3 "~" H 650 2850 50  0001 C CNN
	1    650  2850
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J2
U 1 1 6042B31E
P 650 3500
F 0 "J2" H 758 3681 50  0000 C CNN
F 1 "-Raw" H 758 3590 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 650 3500 50  0001 C CNN
F 3 "~" H 650 3500 50  0001 C CNN
	1    650  3500
	1    0    0    -1  
$EndComp
$Comp
L 4xxx:4023 U2
U 4 1 604916A2
P 1700 3200
F 0 "U2" H 1930 3246 50  0000 L CNN
F 1 "4023" H 1930 3155 50  0000 L CNN
F 2 "Housings_DIP:DIP-14_W7.62mm" H 1700 3200 50  0001 C CNN
F 3 "http://www.intersil.com/content/dam/Intersil/documents/cd40/cd4011bms-12bms-23bms.pdf" H 1700 3200 50  0001 C CNN
	4    1700 3200
	1    0    0    -1  
$EndComp
Connection ~ 1700 1000
Wire Wire Line
	1700 1000 2100 1000
$Comp
L power:GND #PWR01
U 1 1 604B566B
P 1700 3900
F 0 "#PWR01" H 1700 3650 50  0001 C CNN
F 1 "GND" H 1705 3727 50  0000 C CNN
F 2 "" H 1700 3900 50  0001 C CNN
F 3 "" H 1700 3900 50  0001 C CNN
	1    1700 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 1000 2850 1700
Connection ~ 3900 2750
Wire Wire Line
	5000 2750 5000 3400
Wire Wire Line
	5000 3400 7000 3400
Wire Wire Line
	5300 3150 5300 3300
Wire Wire Line
	5300 3300 6000 3300
Wire Wire Line
	5300 2450 5300 2850
Wire Wire Line
	4200 2450 5300 2450
Wire Wire Line
	3900 2750 5000 2750
Text Notes 4500 2850 0    50   ~ 0
Power Hold
Text Notes 4500 2550 0    50   ~ 0
Power Sense
Wire Wire Line
	2850 1000 3550 1000
Wire Wire Line
	4300 1050 4300 1000
Connection ~ 4300 1000
Wire Wire Line
	5450 1400 5450 3200
$Comp
L power:GND #PWR06
U 1 1 605CF923
P 6000 3900
F 0 "#PWR06" H 6000 3650 50  0001 C CNN
F 1 "GND" H 6005 3727 50  0000 C CNN
F 2 "" H 6000 3900 50  0001 C CNN
F 3 "" H 6000 3900 50  0001 C CNN
	1    6000 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 3800 6000 3900
$Comp
L power:GND #PWR04
U 1 1 605DD65C
P 4300 3900
F 0 "#PWR04" H 4300 3650 50  0001 C CNN
F 1 "GND" H 4305 3727 50  0000 C CNN
F 2 "" H 4300 3900 50  0001 C CNN
F 3 "" H 4300 3900 50  0001 C CNN
	1    4300 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1750 4300 3900
Wire Wire Line
	3900 3150 3900 3900
Wire Wire Line
	4300 1350 4300 1400
Connection ~ 4300 1400
Wire Wire Line
	4300 1400 4300 1450
Wire Wire Line
	4300 1400 5450 1400
Wire Wire Line
	5450 3200 7000 3200
Wire Wire Line
	6950 1000 6950 1800
Wire Wire Line
	6650 3100 7000 3100
Connection ~ 6000 3300
Wire Wire Line
	6000 3300 7000 3300
Wire Wire Line
	6000 3500 6000 3300
Wire Wire Line
	6200 3900 6200 2900
Wire Wire Line
	6200 2900 7000 2900
Wire Wire Line
	3550 1950 8550 1950
Wire Wire Line
	8550 1950 8550 4700
Wire Wire Line
	8500 3100 8500 2050
Wire Wire Line
	3850 2050 8500 2050
Text Notes 4500 1900 0    50   ~ 0
Power Button
Text Notes 4500 2150 0    50   ~ 0
RTC Wake-up
Wire Wire Line
	3550 1450 3550 1000
Connection ~ 3550 1000
Wire Wire Line
	3550 1000 3850 1000
Wire Wire Line
	3850 1450 3850 1000
Connection ~ 3850 1000
Wire Wire Line
	3850 1000 4300 1000
Wire Wire Line
	3550 1750 3550 1950
Wire Wire Line
	3850 1750 3850 2050
Connection ~ 9200 4100
Wire Wire Line
	9200 3400 9200 4100
Wire Wire Line
	1700 1000 1700 750 
Wire Wire Line
	1700 750  9250 750 
Wire Wire Line
	9250 750  9250 1700
Wire Wire Line
	9250 1700 9500 1700
Wire Wire Line
	7800 5800 7800 5950
Connection ~ 7800 5800
$Comp
L power:GND #PWR010
U 1 1 6007A4B2
P 8350 1100
F 0 "#PWR010" H 8350 850 50  0001 C CNN
F 1 "GND" H 8355 927 50  0000 C CNN
F 2 "" H 8350 1100 50  0001 C CNN
F 3 "" H 8350 1100 50  0001 C CNN
	1    8350 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8350 1100 8350 1050
Wire Wire Line
	8350 1050 8400 1050
Wire Wire Line
	8800 1050 8950 1050
Wire Wire Line
	8950 1050 8950 2500
Connection ~ 8600 4600
Wire Wire Line
	8600 4600 8600 3500
Wire Wire Line
	8600 4600 8600 5450
Wire Wire Line
	8600 4600 9500 4600
Wire Wire Line
	2100 2500 2400 2500
Wire Wire Line
	2400 2500 2400 3900
Wire Wire Line
	2100 2250 2100 2400
Wire Wire Line
	2400 2500 2400 2450
Connection ~ 2400 2500
Wire Wire Line
	2400 2150 2400 2050
$Comp
L Device:R R3
U 1 1 607EFF86
P 2600 2050
F 0 "R3" V 2600 2150 50  0000 R CNN
F 1 "1.1k" V 2500 2150 50  0000 R CNN
F 2 "Resistors_THT:R_Axial_DIN0204_L3.6mm_D1.6mm_P7.62mm_Horizontal" V 2530 2050 50  0001 C CNN
F 3 "~" H 2600 2050 50  0001 C CNN
	1    2600 2050
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2400 2050 2450 2050
Connection ~ 2400 2050
Wire Wire Line
	2750 2050 2850 2050
Connection ~ 6950 1800
Wire Wire Line
	6950 1800 6950 2800
Wire Wire Line
	6950 1800 9500 1800
Wire Wire Line
	1700 3700 1700 3900
Wire Wire Line
	1700 1000 1700 2700
$Comp
L Connector:Conn_01x01_Male J5
U 1 1 6086C43E
P 5850 1000
F 0 "J5" H 5822 1024 50  0000 R CNN
F 1 "3296 IN" H 6050 1150 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 5850 1000 50  0001 C CNN
F 3 "~" H 5850 1000 50  0001 C CNN
	1    5850 1000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4300 1000 5650 1000
$Comp
L Connector:Conn_01x01_Male J6
U 1 1 60876316
P 5850 1350
F 0 "J6" H 5800 1450 50  0000 R CNN
F 1 "GND" H 5900 1250 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 5850 1350 50  0001 C CNN
F 3 "~" H 5850 1350 50  0001 C CNN
	1    5850 1350
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5550 1350 5650 1350
$Comp
L Connector:Conn_01x01_Male J11
U 1 1 60881AE6
P 6200 1000
F 0 "J11" H 6172 1024 50  0000 R CNN
F 1 "3296 OUT" H 6400 1150 50  0000 R CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 6200 1000 50  0001 C CNN
F 3 "~" H 6200 1000 50  0001 C CNN
	1    6200 1000
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J12
U 1 1 6088C2C9
P 6200 1350
F 0 "J12" H 6100 1450 50  0000 C CNN
F 1 "GND" H 6250 1250 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x01_Pitch2.54mm" H 6200 1350 50  0001 C CNN
F 3 "~" H 6200 1350 50  0001 C CNN
	1    6200 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 1350 6500 1350
$Comp
L Connector:Conn_01x02_Male J13
U 1 1 5FCE5B93
P 1150 3150
F 0 "J13" H 1258 3331 50  0000 C CNN
F 1 "Ext Power" H 1150 2950 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1150 3150 50  0001 C CNN
F 3 "~" H 1150 3150 50  0001 C CNN
	1    1150 3150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	850  3500 900  3500
Wire Wire Line
	900  3500 900  3250
Wire Wire Line
	900  3250 950  3250
Wire Wire Line
	850  2850 900  2850
Wire Wire Line
	900  2850 900  3150
Wire Wire Line
	900  3150 950  3150
$Comp
L Connector:Conn_01x02_Male J14
U 1 1 5FD13713
P 1000 1300
F 0 "J14" H 1108 1481 50  0000 C CNN
F 1 "Bat+" H 1000 1100 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1000 1300 50  0001 C CNN
F 3 "~" H 1000 1300 50  0001 C CNN
	1    1000 1300
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J15
U 1 1 5FD1F786
P 1000 1800
F 0 "J15" H 1108 1981 50  0000 C CNN
F 1 "Bat -" H 1000 1600 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x02_Pitch2.54mm" H 1000 1800 50  0001 C CNN
F 3 "~" H 1000 1800 50  0001 C CNN
	1    1000 1800
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 1900 1600 1900
Wire Wire Line
	1550 1100 1500 1100
Wire Wire Line
	1200 1300 1550 1300
Wire Wire Line
	1550 1300 1550 1100
Connection ~ 2100 2400
Wire Wire Line
	2100 2400 2100 2500
Wire Wire Line
	1500 2400 2100 2400
Wire Wire Line
	1600 1900 1600 2500
Wire Wire Line
	1600 2500 1500 2500
Wire Wire Line
	8400 3000 9350 3000
Wire Wire Line
	9350 3000 9350 3500
Connection ~ 9350 3500
Wire Wire Line
	9350 2600 9350 3000
Connection ~ 9350 3000
Wire Wire Line
	8400 3100 8400 3000
Connection ~ 8400 3000
Wire Wire Line
	1200 1400 1550 1400
Wire Wire Line
	1550 1400 1550 1300
Connection ~ 1550 1300
Wire Wire Line
	1600 1800 1600 1900
Wire Wire Line
	1200 1800 1600 1800
Connection ~ 1600 1900
$Comp
L Switch:SW_Push SW2
U 1 1 5FCEE2D3
P 2350 5300
F 0 "SW2" H 2150 5400 50  0000 C CNN
F 1 "E" H 2350 5200 50  0000 C CNN
F 2 "keebs:Mx_Alps_100" H 2350 5500 50  0001 C CNN
F 3 "~" H 2350 5500 50  0001 C CNN
	1    2350 5300
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW3
U 1 1 5FCEEDD6
P 2400 5750
F 0 "SW3" H 2200 5850 50  0000 C CNN
F 1 "A" H 2400 5650 50  0000 C CNN
F 2 "keebs:Mx_Alps_100" H 2400 5950 50  0001 C CNN
F 3 "~" H 2400 5950 50  0001 C CNN
	1    2400 5750
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW4
U 1 1 5FCEF22B
P 2400 6200
F 0 "SW4" H 2200 6300 50  0000 C CNN
F 1 "L" H 2400 6100 50  0000 C CNN
F 2 "keebs:Mx_Alps_100" H 2400 6400 50  0001 C CNN
F 3 "~" H 2400 6400 50  0001 C CNN
	1    2400 6200
	1    0    0    -1  
$EndComp
$Comp
L Switch:SW_Push SW5
U 1 1 5FCEF961
P 2400 6650
F 0 "SW5" H 2200 6750 50  0000 C CNN
F 1 "Spec" H 2400 6550 50  0000 C CNN
F 2 "keebs:Mx_Alps_100" H 2400 6850 50  0001 C CNN
F 3 "~" H 2400 6850 50  0001 C CNN
	1    2400 6650
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J16
U 1 1 5FCF0823
P 1300 5500
F 0 "J16" H 1408 5881 50  0000 C CNN
F 1 "Ext" H 1408 5790 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_1x05_Pitch2.54mm" H 1300 5500 50  0001 C CNN
F 3 "~" H 1300 5500 50  0001 C CNN
	1    1300 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	1500 5300 2150 5300
Wire Wire Line
	1500 5400 2100 5400
Wire Wire Line
	2100 5400 2100 5750
Wire Wire Line
	2100 5750 2200 5750
Wire Wire Line
	1500 5500 2050 5500
Wire Wire Line
	2050 5500 2050 6200
Wire Wire Line
	2050 6200 2200 6200
Wire Wire Line
	1500 5600 2000 5600
Wire Wire Line
	2000 5600 2000 6650
Wire Wire Line
	2000 6650 2200 6650
Wire Wire Line
	1500 5700 1550 5700
Wire Wire Line
	2550 5300 2650 5300
Wire Wire Line
	2650 5300 2650 5750
Wire Wire Line
	2650 6850 1550 6850
Wire Wire Line
	1550 5700 1550 6850
Wire Wire Line
	2600 5750 2650 5750
Connection ~ 2650 5750
Wire Wire Line
	2650 5750 2650 6200
Wire Wire Line
	2600 6200 2650 6200
Connection ~ 2650 6200
Wire Wire Line
	2650 6200 2650 6650
Wire Wire Line
	2600 6650 2650 6650
Connection ~ 2650 6650
Wire Wire Line
	2650 6650 2650 6850
$EndSCHEMATC
