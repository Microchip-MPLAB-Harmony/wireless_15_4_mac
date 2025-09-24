![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip PIC32CX-BZx Device Standalone IEEE 802.15.4 Mac Layer Release Notes

## Release v1.2.0

### Features/Bug Fixes
-  Added support for PIC32CXBZ3 device.
-  Added support for PIC32CXBZ6 device.
-  General Enhancements to MAC MCC Component.

### Known Issues
-  None

## Development Tools
-   [MPLAB X v6.25 or higher](https://www.microchip.com/mplab/mplab-x-ide)
-   [MPLAB® XC32 C/C++ Compiler v4.60 or higher](https://www.microchip.com/mplab/compilers)
-   MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.5.3 and above
-   Device Pack: PIC32CX-BZ2-DFP (1.4.243), PIC32CX-BZ2-DFP (1.3.216), PIC32CX-BZ6-DFP (1.3.38)

________________________________

## Release v1.1.0

### New Features
-	Support for WBZ451H high power module

### Known Issues
- None

## Development Tools
- [MPLAB X v6.20 or later](https://www.microchip.com/mplab/mplab-x-ide)
- [MPLAB® XC32 C/C++ Compiler v4.40 or later](https://www.microchip.com/mplab/compilers)
-  MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.5.0 and above
-  Device Pack: PIC32CX-BZ2-DFP (1.4.243)

________________________________

## Release v1.0.1

### Bug fixes

- Fix of dependency version string in package.yml

________________________________

## Release v1.0.0

The MAC core layer abstracts and implements IEEE 802.15.4 compliant functionalities for Non-Beacon enabled network support.
MAC should be responsible for following functional requirement of a standalone IEEE 802.15.4. 

-	Transmission of data
-	Purge Data
-	Reception of data
-	Promiscuous mode
-	control of PHY receiver
-	Timestamp of incoming data
-	Beacon management (Transmitting/receiving beacons)
-	Channel access mechanism
-	Frame validation
-	Acknowledged frame delivery
-	Association and dissociation
-	Security (Unsecured/secured mode/Frame integrity/Data encryption)
-	Scanning(Active/Passive/ED)
-	Store at least on transaction in indirect message queue

## Known Issues / Limitations

-	No issues

## Development Tools
-	MPLAB X v6.15
-	MPLAB® XC32 C/C++ Compiler v4.21
-	MPLAB® X IDE plug-ins: MPLAB® Code Configurator (MCC) v5.3.7 and above
-	Device Pack: PIC32CX-BZ2-DFP (1.2.230)

## Notes
-	None