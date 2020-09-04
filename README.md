
# Cortex M. McGee

ARM Cortex M0-4 machine code generator

_WORK IN PROGRESS_








# Debug on stm32f4-discovery

- debug.sh script starts up OpenOCD and connects to the board

- Start gdb (`arm-none-eabi-gdb`)
  - Issue command `target extended-remote localhost:3333` or short version `tar ext :3333`
  
- Telnet `telnet localhost 4444`
