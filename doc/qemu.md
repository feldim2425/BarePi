# Debugging with QEMU

## Using the shell script
You can run the `scripts/emulate.sh` from your projects root directory to start QEMU with some predefined arguments.

### Shellscript options
* ``-h`` ... Shows a list of all options
* ``-d`` ... Enables the debugging interface for gdb (GDB is not started automatically). Also halts the processor at startup.
* ``-p <port>`` ... Changes the tcp port for the GDB interface. The default is set to 1234. This option only works together with ``-d``
* ``-s`` ... Enables the semihost interface. Which is used to let the SDK communicate with QEMU (Exit reasons/codes, Console IO, etc.). See ``includes/semihost.h``.

## Starting QEMU directly
Sometimes the emulate script is not enough and you might want to add additional options for peripherals, graphics etc..

To run QEMU directly just call ``qemu-system-arm -bios <path to binary> -M raspi2``
For AArch64 emulation you can use ``qemu-system-aarch64 -bios <path to binary> -M raspi2``

You can change the emulated machine but ``raspi2`` is currently the only RaspberryPi that is supported by QEMU ARM. You can get the full list by executing ``qemu-system-arm -M help``

On QEMU AARCH64 ``raspi3`` and ``raspi2`` are supported. Again you can run ``qemu-system-aarch64 -M help`` to get the full list

### Other QEMU options
* ``-semihosting`` ... Sets the semihosting mode
* ``-S`` ... Halts the CPU at startup, useful for debugging to wait for the GDB debugger
* ``-gdb tcp::<port>`` ... Opens a GDB debugging interface on a local tcp port
* ``-serial stdio`` ... Connects the Serial interface to the terminal
* ``-monitor stdio`` ... Connects the QEMU monitor to the terminal
* ``-nographic`` ... Disable the window that normally opens with qemu (Monitor,Serial,Video,...)

For more infos about the QEMU arguments/parameters refer to the QEMU help (``qemu-system-arm -help``) or the QEMU wiki https://wiki.qemu.org/Documentation


For more information about the QEMU monitor refer to: https://en.wikibooks.org/wiki/QEMU/Monitor