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
### Navigation <a name="navigation"></a>

## Build Quality <a name="quality"></a>
### Known Bugs <a name="bugs"></a>
* Crash if emulated input is sent before actually connecting to Bluetooth. Can be avoided by wating for module to fully connect before sending controller inputs.
* Connection freeze if app is not fully shut down before re-opening a connection.

## Vertical Features <a name="features"></a>
### External Interface <a name="external"></a>
--Supported Controller Systems
* Phone Emulation - v0.2
* </i>Xbox One - COMING SOON</i>
* </i>Playstation 4 (Dualshock) - COMING SOON</i>
* <i>Nintendo Switch -COMING SOON</i>

--Supported Data Emulation Types
* Basic HID gamepad - v0.1
* </i>Xbox One - COMING SOON</i>
* </i>Playstation 4 (Dualshock) - COMING SOON</i>
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


--Microprocessor Specifications (ATXMEGA128A1U Chipset)
* 8 bit core size
* 32 MHz clock speed
* 128kb FLASH program memory
* 2k X 8 EEPROM
* 1.6V ~ 3.6V
* 50MHz internal oscillator
* Microchip Technologies
* AVR XMEGA A1U

# Schematics and Diagrams <a name="diagrams"></a>
Hard-wire Connections
![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1017950729031397386/unknown.jpg)

Current Electronics Housing
![alt text](https://cdn.discordapp.com/attachments/946515441138937876/1017951341244600362/Screenshot_2022-09-09_201455.png)

# Build Instructions <a name="build"></a>
## Firmware: (ATXMEGA128A1U) <a name="firmware"></a>
Must have Microchip studio installed along with the AVR device support that comes with the installation of Microchip studio.
Download and open the Atmel Solution file in the Microchip branch.
Under Tools>Device make sure to select the XMEGA plugged into a USB slot on your computer.
If you do not have an XMEGA, you can test the build by selecting "Simulator" as your device.
Press the run button and the code will automatically be uploaded to the XMEGA.

## Companion App: (Android Device running Android 11 or lower) <a name="app"></a>
Ensure you have developer mode enabled on your device.  Perform either of the following:
* (RECOMMENDED) Install .apk file located at the root of this repository directly onto your Android device.  You may need to allow unknown apps in Settings.
* Install Android Studio on your computer.  Create a new project from Version Control (VSC) and clone it from this repository.  Plug your Android device into your computer via USB.  If Android Studio doesn't recognize your phone you must enable USB Debugging in the Developer Settings on your device.  Build the project and then select your plugged in Android device as the target device.  Hit the Run button and it should install the application and run it on your device.
