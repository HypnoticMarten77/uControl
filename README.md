# ![alt text](https://cdn.discordapp.com/attachments/936755058626928735/1017895293787832350/logo.png)
Repository for UF CpE Design project,  uControl, the universal Bluetooth game controller adapter. 

# Hardware/Firmware Instructions and Details


The basic idea here is that we are taking advantage of the Arduino Mega's ATMEL 2560 chipset, just with our own proprietary firmware. There are technically two microprocessors located on the Arduino MEGA and UNO. To save on hardware, we are converting the ATMEL16U2 chip located on the board to a HID controller, rather than a serial port, reducing the stress on the main CPU, all while simplifying the software needed drastically.

Rather than wasting the 16U2 chip on only serial communication (which is useful for debugging and reprogramming), we can instead use the 16U2 chip as a HID controller after the firmware has been finished, allowing us to send the converted data as if it were a HID, which will reduce our CPU load significantly since we don't need serial debugging once the product is finished.

![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1026241686352965742/IMG_4642.jpg)

## Universal Plug and Play

We are designing the uControl firmware to be recognized on Windows machines as a HID device, allowing us to reduce the requirement of writing drivers for every operating system (Linux/Windows/Mac/etc.). As well, this eliminates the need to manually configure external programs such as XInput, or other serial readers/interpreters, making the uControl much more lightweight and simple to use.

The Arduino UNO/MEGA chipsets are not configured as HID devices and therefore must operate with our uControl firmware.


## Flashing the uControl Firmware onto the 16U2
Note** This process works with both the Arduino UNO and the Arduino MEGA2560, however, the MEGA is generally preferred as it has much more program memory available.

To begin, you must first place the device in DFU mode (Device Firmware Update Mode) by shorting the two pins on the ICSP (In Circuit Serial Programmer).

![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1026243657344499722/Screenshot_2022-10-02_172542.png)

Note** Before you flash the new firmware, make sure you upload the sketch to program memory as we will have no access to the 16U2 as a reprogrammer to reflash the program memory after it has been converted to UPNP mode.

Next, you must go to the device manager and locate the Arduino in DFU mode, sometimes the driver does not properly get installed, so you must install the software FLIP located in this repository or [here](https://www.microchip.com/en-us/development-tool/flip).

Right click the unconfigured 16U2 device and update the firmware. Locate the driver in 
[C:\Program Files (x86)\Atmel\Flip 3.4.7\usb]
and make sure to "include subfolders".

Now that the driver is installed you can flash the 16U2 by opening FLIP and select the 16U2:
![alt text](https://cdn.discordapp.com/attachments/960677104620560454/1026246874430177350/Screenshot_2022-10-02_173829.png)

Now open the serial connection via USB.

Next click File -> Load HEX File -> Select the uControl Firmware

Now you can click Run to erase the chip, program it, and verify that the chip is programmable.

Lastly, click "Start Application" and make sure that there is a confirmation at the bottom that the firmware was successfully flashed.

## Revert Firmware to Original Arduino
Follow the same exact steps as in the previous section, only this time, make sure when you select a HEX file, you select the original Arduino firmware.
Once uploaded, you should be able to use the Arduino as it was from the factory.
