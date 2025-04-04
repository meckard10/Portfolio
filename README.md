# Portfolio

Melody Eckard

## Overview

A collection of projects and assignments. The coding projects are programed using C, design 
projects utilize EAGLE CAD, KiCAD, and SOLIDWORKS.

## Project Folders

1. /BiBOP Memory Allocator
	- This project is an implementation of the standard C allocation API as a shared
	  library that can be utilized by any existing program.

2. /RLE Codec
	- This project is a compression decompression pair that utilizes a Run Length 
	  Encoding(RLE) scheme.

## Design

1. PCB Designs
	- [Wireless Remote]{https://github.com/meckard10/ZippyWheels/blob/main/Remote_Design/remote.kicad_pcb} for electric roller skates
		- Includes headers to attach two Raspberry Pi PICO Ws, a voltage regulation 
		  circut to power the PICOs off of a 9V battery, a trigger button, and a 
		  speed wheel. Originally designed using EAGLE and modified in KiCAD for 
		  fabrication.
	- [Curtain Opener]{https://github.com/meckard10/CurtainOpener/blob/main/schematics/windowV2.brd}
		- Includes headers to attach a MSP430 Launchpad, a voltage regulation circuit
		  to power the MSP430 and motor off of a 9V battery, a light sensor, a 
		  photoresistor, a CC1101 transceiver, and a SN754410 motor driver. Designed 
		  using EAGLE.

2. SOLIDWORKS Designs
	- [Wireless Remote Housing]{https://github.com/meckard10/ZippyWheels/blob/main/Remote_Design/remote.STL} for electric roller skates
		- Lower compartment fits a 9V battery and upper compartment holds the 
		  microcontrollers and PCB. Cutouts for trigger button, on/off switch, 
		  and speed wheel.
	- [Alarm Clock Housing]{https://github.com/meckard10/CurtainOpener/blob/main/original%20material/Clock_Box.STL}
		- Holds the internals of a microcontroller clock. Cutouts for buttons, 
		  power, and digital display. Snooze button covering fits over a 
		  commercial button.
