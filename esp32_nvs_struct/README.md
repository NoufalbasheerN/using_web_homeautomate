# Non-Volatile Storage (NVS) Read and Write Example

This example demonstrates how to read and write a c struct with multiple variables in it using NVS.

In this example, value which is saved holds a c struct. Since it is written to NVS, the value is preserved between restarts.

Example also shows how to check if read / write operation was successful, or certain value is not initialized in NVS. Diagnostic is provided in plain text to help track program flow and capture any issues on the way.

Detailed functional description of NVS and API is provided in [documentation](https://docs.espressif.com/projects/esp-idf/en/latest/api-reference/storage/nvs_flash.html).

## How to use example

The code for this example is available at:

https://codeberg.org/macpczone/ESP32_NVS_Struct

### Hardware required

This example does not require any special hardware, and can be run on any common development board.

### Configure the project

Here are some samples of the commands that I use to set up the environment variables, so that the sdk commands can be found in the command path:

export IDF_PATH=/big/src/embedded/esp-idf-v3.3.4
export PATH=${IDF_PATH}/tools:/usr/local/esp32/crosstool-NG/builds/xtensa-esp32-elf/bin:$PATH
modprobe cp210x

The first two commands contain the path to where I have unpacked the ESP-IDF SDK files and the compiler utilities. The third command is just to load the kernel module for the usb-serial chip, that is used to connect the micro usb connector on the board, to the ESP32 microcontroller.

If using Make based build system, run `make menuconfig` and set serial port under Serial Flasher Options.

If using CMake based build system, no configuration is required.

### Build and flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
make -j4 flash monitor
```

Or, for CMake based build system (replace PORT with serial port name):

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)

See the Getting Started Guide for full steps to configure and use ESP-IDF to build projects.

## Example Output

First run:
```
Opening Non-Volatile Storage (NVS) handle... Done
Reading string from NVS ... The value is not initialized yet!
Buffer is 0 characters in size
Adding text to NVS ... Done
Committing updates in NVS ... Done

Restarting in 4 seconds...
Restarting in 3 seconds...
Restarting in 2 seconds...
Restarting in 1 seconds...
Restarting in 0 seconds...
Restarting now.
```

Subsequent runs:

```
Opening Non-Volatile Storage (NVS) handle... Done
Reading string from NVS ... Done
Buffer = Here is a load of text to test the NVS string storage. 

Buffer is 55 characters in size
Adding text to NVS ... Done
Committing updates in NVS ... Done

Restarting in 4 seconds...
Restarting in 3 seconds...
Restarting in 2 seconds...
Restarting in 1 seconds...
Restarting in 0 seconds...
Restarting now.
```

The text in the buffer will increase in size and the variables will have their values changed on each run.

To reset buffer text, erase the contents of flash memory using `make erase_flash` (or `idf.py erase_flash`, if using CMake build system), then upload the program again as described above.

