EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Amplifier_Operational:LM321 U1
U 1 1 5E15CFED
P 5550 3350
F 0 "U1" H 5894 3396 50  0000 L CNN
F 1 "LM321" H 5894 3305 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 5550 3350 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lm321.pdf" H 5550 3350 50  0001 C CNN
	1    5550 3350
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 5E15E416
P 5900 4000
F 0 "R1" H 5970 4046 50  0000 L CNN
F 1 "R" H 5970 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5830 4000 50  0001 C CNN
F 3 "~" H 5900 4000 50  0001 C CNN
	1    5900 4000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 5E15EEB0
P 4650 3350
F 0 "J2" H 4568 3025 50  0000 C CNN
F 1 "Conn_01x02" H 4568 3116 50  0000 C CNN
F 2 "Connector_JST:JST_PH_B2B-PH-K_1x02_P2.00mm_Vertical" H 4650 3350 50  0001 C CNN
F 3 "~" H 4650 3350 50  0001 C CNN
	1    4650 3350
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5E1606CF
P 4050 3350
F 0 "J1" H 3968 3025 50  0000 C CNN
F 1 "Conn_01x02" H 3968 3116 50  0000 C CNN
F 2 "Connector_JST:JST_EH_B2B-EH-A_1x02_P2.50mm_Vertical" H 4050 3350 50  0001 C CNN
F 3 "~" H 4050 3350 50  0001 C CNN
	1    4050 3350
	-1   0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 5E161282
P 5900 4450
F 0 "R2" H 5970 4496 50  0000 L CNN
F 1 "R" H 5970 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5830 4450 50  0001 C CNN
F 3 "~" H 5900 4450 50  0001 C CNN
	1    5900 4450
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 5E160974
P 6650 3450
F 0 "J3" H 6568 3125 50  0000 C CNN
F 1 "Conn_01x02" H 6568 3216 50  0000 C CNN
F 2 "Connector_JST:JST_PH_B2B-PH-K_1x02_P2.00mm_Vertical" H 6650 3450 50  0001 C CNN
F 3 "~" H 6650 3450 50  0001 C CNN
	1    6650 3450
	1    0    0    1   
$EndComp
Wire Wire Line
	5450 3050 5450 2950
Wire Wire Line
	4850 3250 5250 3250
Wire Wire Line
	4250 2950 4250 3250
Wire Wire Line
	5450 2950 4250 2950
Wire Wire Line
	4250 3350 4250 4650
Wire Wire Line
	4250 4650 4850 4650
Wire Wire Line
	4850 3350 4850 4650
Connection ~ 4850 4650
Wire Wire Line
	4850 4650 5450 4650
Wire Wire Line
	5250 3450 5250 4250
Wire Wire Line
	5450 3650 5450 4650
Wire Wire Line
	5900 4250 5900 4300
Wire Wire Line
	5900 4150 5900 4250
Connection ~ 5900 4250
Wire Wire Line
	5250 4250 5900 4250
Wire Wire Line
	5900 4650 5900 4600
Connection ~ 5450 4650
Wire Wire Line
	5450 4650 5900 4650
Connection ~ 5900 4650
Wire Wire Line
	6450 4650 5900 4650
Wire Wire Line
	6450 3450 6450 4650
Wire Wire Line
	5850 3350 5900 3350
Wire Wire Line
	5900 3350 5900 3850
Connection ~ 5900 3350
Wire Wire Line
	5900 3350 6450 3350
$EndSCHEMATC
