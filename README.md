# uControl
Repository for UF CpE Design project,  uControl, the universal Bluetooth game controller adapter.

## Andres
## Mason
## Sam
## Juan

# TA: Evan


# BUILD INSTRUCTIONS:
## Firmware: (ATXMEGA128A1U)
Must have Microchip studio installed along with the AVR device support that comes with the installation of Microchip studio.
Download and open the Atmel Solution file in the Microchip branch.
Under Tools>Device make sure to select the XMEGA plugged into a USB slot on your computer.
If you do not have an XMEGA, you can test the build by selecting "Simulator" as your device.
Press the run button and the code will automatically be uploaded to the XMEGA.

## Companion App: (Android Device running Android 11 or lower)
Ensure you have developer mode enabled on your device.  Perform either of the following:
1. (RECOMMENDED) Install .apk file located at the root of this repository directly onto your Android device.  You may need to allow unknown apps in Settings.
2. Install Android Studio on your computer.  Create a new project from Version Control (VSC) and clone it from this repository.  Plug your Android device into your computer via USB.  If Android Studio doesn't recognize your phone you must enable USB Debugging in the Developer Settings on your device.  Build the project and then select your plugged in Android device as the target device.  Hit the Run button and it should install the application and run it on your device.
