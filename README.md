# ![alt text](https://cdn.discordapp.com/attachments/936755058626928735/1017895293787832350/logo.png)
Repository for UF CpE Design project,  uControl, the universal Bluetooth game controller adapter. 

# Table of contents
1. [Contributors](#contributors)
    * [Individual Contributions](#indivcontribs)
3. [Stakeholders](#stakeholders)
   <!-- 1. [Sub paragraph](#subparagraph1) -->
3. [Project Description](#description)
4. [Usability](#usability)
      * [Interface](#interface)
      * [Navigation](#navigation)
5. [Build Quality](#quality)
      * [Known Bugs](#bugs)
7. [Vertical Features](#features)
      * [External Interface](#external)
      * [Internal Systems](#internal)
9. [Schematics and Diagrams](#diagrams)
10. [Build Instructions](#build)
    * [Firmware](#firmware)
    * [Companion App](#app)


# Contributors <a name="contributors"></a>
* Andres Maldonado-Martin
* Mason Anderson
* Juan Pablo Lancheros
* Sam Fleischer

<a name="indivcontribs"></a>
A detailed and comprehensive list and dates of individual contributions can be found [here](https://github.com/amaldonadomartin77/uControl/blob/main/Documentation/Contributions.txt).

# Stakeholders <a name="stakeholders"></a>
* TA: Evan Rocha

# Project Description <a name="description"></a>
uControl is a all-in-one wireless game controller emulator, allowing you to pair controllers to your PC and emulating controller inputs. Using the uControl Companion App, you are able to manage the devices that are connected to the uControl module and select which (if any) emulation you want, allowing users to have quality emulated controllers for consoles they do not own, all while reducing the need for external input emulators such as XInput, and combining them into a single, wireless package. Using a PS2 emulator but don't have a PS2 controller? uControl will solve all these issues with a single product.

Use the uControl Companion App:
* Make sure your uControl is plugged in and has adequate power.
* Launch the uControl Companion App.
* For first time setups, click "Scan for uControl".
* Once the uControl device has been found, click "Change Controller".
* Select the controller type you are trying to pair with (Nintendo Switch/Playstation/Xbox/Phone)
* Click the back button to "Change Console".
* Select the controller input emulation of your choice (Nintendo Switch/Playstation/Xbox).
* Send the configuration to the uControl using the "Send Configuration" button.


## Usability <a name="usability"></a>
### Interface <a name="interface"></a>
Wireframe for app faces
![alt text](https://cdn.discordapp.com/attachments/960677626811404430/1035675346512646194/Screenshot_2022-10-28_180346.png)
### Navigation <a name="navigation"></a>
![alt text](https://cdn.discordapp.com/attachments/904954102377771010/1017964646147297300/Screenshot_2022-09-09_210752.png)

## Build Quality <a name="quality"></a>
### Known Bugs <a name="bugs"></a>
* (FIXED) Crash if emulated input is sent before actually connecting to Bluetooth. Can be avoided by wating for module to fully connect before sending controller inputs.
* (FIXED) Connection freeze if app is not fully shut down before re-opening a connection.
* App will sometimes crash when pressing "Send Configuration" too quickly if left running in the background for long periods of time.

## Vertical Features <a name="features"></a>
### External Interface <a name="external"></a>
--Supported Controller Systems
* Phone Emulated Controller - DEPRECATED
* </i>Xbox One WIRED - v0.1</i>
* </i>Xbox One S Bluetooth - COMING SOON</i>
* </i>Playstation 4 (Dualshock) - v0.2>
* </i>Playstation 5 (Dualsense) - v0.1>
* <i>Nintendo Switch -COMING SOON</i>

--Supported Data Emulation Types
* HID gamepad - v0.1
* </i>Xbox One - v0.1</i>
* </i>Playstation (Dualshock/Dualsense) - v0.2</i>
* <i>Nintendo Switch -COMING SOON</i>

### Internal Systems <a name="internal"></a>
--Bluetooth Specifications
* ARM Cortex M0 core running at 16MHz
* 256KB flash memory
* 32KB SRAM
* Transport: SPI at up to 4MHz clock speed
* 5V-safe inputs (Arduino Uno friendly, etc.)
* On-board 3.3V voltage regulation
* Bootloader with support for safe OTA firmware updates
* AT command set tunneled over SPI protocol
* 23mm x 26mm x 5mm / 0.9" x 1" x 0.2"
* Weight: 3g


--Microprocessor Specifications (ATMEGA2560 Chipset) USED FOR PROGRAM
* Arduino MEGA 2560 (Can also use Arduino UNO *specs vary*)
* 16 MHz clock speed
* 256KB FLASH MEMORY (8KB used by bootloader)
* 8KB SRAM
* 4KB EEPROM
* 5V Operating Voltage
* 7-12V Input Voltage

--HID Controller Specifications (ATMEGA16U2 Chipset) (2nd processor located on the Arduino)
* 8 bit AVR microcontroller
* 32 x 8 General Purpose Working Registers
* 16K Bytes of In-System Self-Programmable Flash
* 512 EEPROM
* 512 Internal SRAM


# Schematics and Diagrams <a name="diagrams"></a>
### Hard-wire Connections
![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1017950729031397386/unknown.jpg)

### Redesigned 3D Printed Housing
![alt text](https://cdn.discordapp.com/attachments/904954102377771010/1035680377810984960/Screenshot_2022-10-28_182350.png)
![alt text](https://cdn.discordapp.com/attachments/904954102377771010/1035679525729730650/Screenshot_2022-10-28_182015.png)

# Build Instructions <a name="build"></a>
## Firmware: (ATMEGA16U2 and ATMEGA2560) <a name="firmware"></a>
You will need to install custom firmware onto the Arduino to take full advantage of our proprietary systems.
Please follow the README locaed in the Arduino Mega 2560 branch.

![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1026241686352965742/IMG_4642.jpg)

## INSTRUCTIONS FOR FLASHING CUSTOM FIRMWARE TO THE 16U2
[ARDUINO MEGA 2560 FIRMWARE](https://github.com/amaldonadomartin77/uControl/tree/Arduino-Mega-2560)

## Companion App: (Android Device running Android 12 or lower) <a name="app"></a>
Ensure you have developer mode enabled on your device.  Perform either of the following:
* (RECOMMENDED) Install .apk file located at the root of this repository directly onto your Android device.  You may need to allow unknown apps in Settings.
* Install Android Studio on your computer.  Create a new project from Version Control (VSC) and clone it from this repository.  Plug your Android device into your computer via USB.  If Android Studio doesn't recognize your phone you must enable USB Debugging in the Developer Settings on your device.  Build the project and then select your plugged in Android device as the target device.  Hit the Run button and it should install the application and run it on your device.
