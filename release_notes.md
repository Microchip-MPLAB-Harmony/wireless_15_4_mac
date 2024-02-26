![Microchip logo](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_logo.png)
![Harmony logo small](https://raw.githubusercontent.com/wiki/Microchip-MPLAB-Harmony/Microchip-MPLAB-Harmony.github.io/images/microchip_mplab_harmony_logo_small.png)

# Microchip PIC32CX-BZ2 Device Standalone IEEE 802.15.4 Mac Layer Release Notes

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