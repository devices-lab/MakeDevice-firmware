# MakeDevice-firmware

This repository is a fork of [jacdac-msr-modules](https://github.com/jacdac/jacdac-msr-modules), 
with additional targets created for the MakeDevice virtual modules. A [python script](
hexes2bin.py) has also been added, which is used by the makefile to generate `firmware.bin` files for each
target, needed by [MakeDevice-backend](https://github.com/devices-lab/MakeDevice-backend) for flashing

## Optional: Flashing Jacdac modules via Raspberry Pi Pico
[Prior documented methods](https://github.com/microsoft/jacdac-stm32x0/blob/main/README.md) for flashing STM32 based Jacdac modules make use of a black magic probe, blue pill, or an ST-Link programmer. This is an alternative method that turns any rp2040-based device, such as the widespread Raspberry Pi Pico (or Pico W) into a general purpose SWD debug probe (via the CMSIS-DAP interface), allowing you to flash Jacdac modules with your computer. Try the following steps after you have generated a 'combined' `.hex` firmware file (contains both JD bootloader and app).

### Debug probe - one time setup 
1. Grab the `.uf2` file from [free-dap](https://github.com/ataradov/free-dap/raw/refs/heads/master/bin/free_dap_rp2040.uf2)
1. Plug in your pico while holding the BOOTSEL button
2. Drag and drop the `.uf2` file onto the pico
4. Connect the following pico pin numbers to the Jacdac module's SWD nets (exposed via [hackconnect](https://microsoft.github.io/jacdac-docs/ddk/firmware/jac-connect/))
- 38 - GND
- 36 - 3V
- 15 (GPIO11) - SWCLK
- 16 (GPIO12) - SWDIO
- 20 (GPIO15) - nRST
<!-- To flash using the rp2040 brain Jacdac module, you can recompile free-dap to use different GPIO pins accessible by the user (https://github.com/microsoft/jacdac-ddk/blob/main/electronics/altium/brain-designs/JacdacRp2040BrainEc30%20124-1.5/PDF/JacdacRp2040BrainEc30%20124-1.5%20schematic.PDF) -->

<img src="https://github.com/user-attachments/assets/dde2cfd8-527d-42c1-9b88-7a669bd0b1b6" width=400></img>

### Flashing
Once the debug probe is set up and connected to your computer over USB, flashing can be done using [OpenOCD](https://openocd.org/), or other software (such as [pyOCD](https://pyocd.io/), or [edbg](https://github.com/ataradov/edbg)).
The following examples are for the STM32G030 target and the flex sensor service, but can be altered to work for other targets.

#### OpenOCD - Widely used
`openocd -f stm32g0x-custom.cfg -c "program combined-flex.hex verify reset exit"`

Here's the config file `stm32g0x-custom.cfg`:
```tcl
source [find interface/cmsis-dap.cfg]
transport select swd
source [find target/stm32g0x.cfg]
adapter speed 480
reset_config srst_only srst_nogate connect_assert_srst
```

#### pyOCD - Nicer to use but requires 'packs' to support certain targets, written in python
 - `pyocd flash -M under-reset -f 40000 -t stm32g030j6mx --pack Keil.STM32G0xx_DFP.1.2.0.pack combined-flex.hex`


#### edbg - Minimal and portable, written in C, supports very few targets
1. Convert hex to binary (edbg doesn't support hex flashing)
`objcopy --input-target=ihex --output-target=binary combined-flex.hex combined-flex.bin`
2. Flash using:
`edbg.exe -t stm32g0 -c 4000 -pv -f combined-flex.bin`

<!-- 
### Other uses
Since this debug probe implements the generic CMSIS-DAP interface, it can be used for many different SWD/JTAG purposes.
TODO: discuss GDB debugging, rp2040 target, nrf52833 target, microbit debugging? 
-->


# jacdac-msr-modules

This repository defines configuration for experimental [Jacdac](https://aka.ms/jacdac) modules
produced by Microsoft Research:
- https://microsoft.github.io/jacdac-docs/devices/microsoft/research

If you're building your own modules, please follow instructions at
- https://github.com/microsoft/jacdac-stm32x0/blob/main/README.md

In particular, fork the template repo https://github.com/microsoft/jacdac-module-template
**Do not fork this repo.**

## Contributing

This project welcomes contributions and suggestions.  Most contributions require you to agree to a
Contributor License Agreement (CLA) declaring that you have the right to, and actually do, grant us
the rights to use your contribution. For details, visit https://cla.opensource.microsoft.com.

When you submit a pull request, a CLA bot will automatically determine whether you need to provide
a CLA and decorate the PR appropriately (e.g., status check, comment). Simply follow the instructions
provided by the bot. You will only need to do this once across all repos using our CLA.

This project has adopted the [Microsoft Open Source Code of Conduct](https://opensource.microsoft.com/codeofconduct/).
For more information see the [Code of Conduct FAQ](https://opensource.microsoft.com/codeofconduct/faq/) or
contact [opencode@microsoft.com](mailto:opencode@microsoft.com) with any additional questions or comments.

## Trademarks

This project may contain trademarks or logos for projects, products, or services. Authorized use of Microsoft 
trademarks or logos is subject to and must follow 
[Microsoft's Trademark & Brand Guidelines](https://www.microsoft.com/en-us/legal/intellectualproperty/trademarks/usage/general).
Use of Microsoft trademarks or logos in modified versions of this project must not cause confusion or imply Microsoft sponsorship.
Any use of third-party trademarks or logos are subject to those third-party's policies.

# jacdac-firmware-notes - STM32G0

## Build environment
The build environment for STM30G0 needs to be set up as described in the [jacdac-stm32x0](https://github.com/microsoft/jacdac-stm32x0/blob/main/README.md) repo. But that repo itself is not cloned/used directly when building firmware because it's imported as a submodule. The repo that contains the code for all the STM32G0 (and STM32F0) modules from MSR is [jacdac-msr-modules](https://github.com/microsoft/jacdac-msr-modules/).

## Module source code

The source code for a series of modules is in a subfolder of `targets/` in the  [jacdac-msr-modules](https://github.com/microsoft/jacdac-msr-modules/) repo. This subfolder will likely contain the source code for all ***hardware version numbers*** (i.e. revisions) of a particular module design. A suitable folder name would be the words from the ***hardware device name*** of the module (as printed on the silkscreen) with the module's ***hardware design identifier*** (if it has one), e.g. `targets/temp-rh-116/`. It's also possible to group the source code for several different modules in a single sub-folder, which might make sense if they have a lot in common—for example if the MCU has the same pin mapping in each then the submodule could be named `targets/jm-v3.2/`. 

The functionality of any given module is encapsulated in a relatively short `.c` file for that module, usually located in the `targets/<...>/profile/` sub-folder of the repo. This `.c` file defines the following:

- The ***firmware identifier*** (previously known as product identifier). This is a 32 bit number that uniquely identifies the module from an 'over-the-wire' appearance and a visual appearance point of view. If a module is altered and that changes the way it appears on the Jacdac bus and/or the way it looks physically then it should have a unique firmware identifier.

- A short ***device identification string*** that should include the manufacturer name, module name and version number of the module. These should all match what is printed on the silkscreen of the module. This string is returned by the module as the device description in the control packet. If a module ***hardware design identifier*** is included in the device identification string before the version number, that may be used to automatically populate the hardware design identifier field in the device catalog during [device registration](https://microsoft.github.io/jacdac-docs/tools/device-registration/).

- Initialisation code for the module, defined in a function called `app_init_services()`.

- Any other functions uniquely required by that module.

There is also an include file called `targets/<...>/board.h` that specifies the MCU pin mapping. If a particular pin type is not present, use `NO_PIN` or comment out the relevant line completely.

Finally there is a `targets/<...>/config.mk` file that specifies the type of MCU being used.

## Building the firmware

To build the firmware for a particular module edit `Makefile.user` in the root of the repo so that `TRG` is set to the specific `.c` file to be built, e.g. `TRG = targets/developer-rgb-117/profile/developer-rgb-117-v1.0.c`. Then run `make` to generate `.elf`, `.bin` and `.hex` files in the `built/<...>/` folder for that module. There are separate files for the app itself (i.e. the server code that encapsulates the module functionality) and the Jacdac bootloader. There is also a 'combined' `.hex` file that contains both app and bootloader code.

Remember that every uniquely identifiable module, i.e. every revision of every module design, must have a unique ***firmware identifier*** (also known as product identifier). If compiling the code for a new module revision for the first time, change the firware identifer to be `0x00` in the `.c` file for that module and the build process will automatcially generate a valid new firmware identifier and substitute that into the `.c` file. Make sure to check in the updated `.c` file!

In case of unexplained build failures, it might be worth running `make clean` before `make`. 

## Flashing the firmware onto a module

There are two ways to flash the firmware onto a module:
- As part of the `make` process. Based on settings in `Makefile.user` either `openocd` will be used to flash the module, e.g. via an [STLINK/V2](https://www.st.com/en/development-tools/st-link-v2.html) or a [Black Magic Probe](https://github.com/blacksphere/blackmagic/wiki) (or similar) can will be used to flash the module via `gdb`.
- The 'combined' hex file can be flashed onto the module over SWD using traditional flashing tools such as a [Segger J-Link](https://www.segger.com/products/debug-probes/j-link) with [J-Flash software](https://www.segger.com/downloads/flasher/) or an [ST-LINK/V2](https://www.st.com/en/development-tools/st-link-v2.html) with the [ST-LINK utility](https://www.st.com/content/st_com/en/products/development-tools/software-development-tools/stm32-software-development-tools/stm32-programmers/stsw-link004.html).

## Releasing the firmware

Firmware for all the modules in a given repository is released in a single `.uf2` drop file. This can be generated locally (to make sure it won't fail in when built in the cloud) by executing `make drop`. This creates a file called `built/drop.uf2` that can be used for testing firmware updates locally. Because `make drop` compiles the firmware for every module separately it can take several minutes and the resulting `.uf2` could be a few MB (for all the modules in the MSR-modules repo, for example). 

The locally-built `.uf2` file should not be checked into the repo. Instead, a github action will build the final `.uf2` release file in the cloud. This is trigged if  `make check-release`, which automatically runs after all check-ins, detects
releases that are tagged with ***version flags*** of the form `vX.Y.Z`, e.g. `v0.20.7`. 

So to make a new release of firmware for all the modules in a repo, tag the latest check-in with the appropriate version flag.

It's also possible to run `make bump`. This increments the firmware drop version number and generates a new `.uf2` file in the `dist/` folder of the  [jacdac-msr-modules](https://github.com/microsoft/jacdac-msr-modules/) repo. 

## Deploying the latest firmware via the web

Firmware updates can be delivered to an STM-based module via the Jacdac website's [firmware update](https://microsoft.github.io/jacdac-docs/tools/updater/) support. Firmware will be delivered based on a module's ***firmware identifier***.


This `.uf2` file is used by the [Jacdac firmware updater](https://microsoft.github.io/jacdac-docs/tools/updater/).

To check if a firmware update is necessary, the website first gets the device's ***firmware identifier***, then it finds the `.JSON` file for that device which includes a reference to the relevant firmware repository. Next it looks for `.uf2` files named `fw-X.Y.Z.uf2` in the `dist/` folder in that repo and displays them to the user in SemVer order. If a user selects one of these then it's downlaoded at that point and cached in the browswer. If the user then requests to push that firmware to the device the website will do that. 

<!-- We don't automatcially check for updates any more. -->

<!-- github releases would be better in theory for binaries, but we couldn't get our website to download those binaries. -->

<!-- There are two main conventions for the naming of folders containing code for module(s):
- A sub-folder per module, named either with the design ID _and_ the version number in the folder name, e.g. `jm-rgb-led-bar-58-0.1`, or without the version number, e.g. `jm-access-switch-output-1.0` (esp. for non MSR modules such as those from MACH).
