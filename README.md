
# Cortex M. McGee

ARM Cortex M0-4 machine code generator

_WORK IN PROGRESS_




# Debug on stm32f4-discovery

- debug.sh script starts up OpenOCD and connects to the board

- Start gdb (`arm-none-eabi-gdb`)
  - Issue command `target extended-remote localhost:3333` or short version `tar ext :3333`
  
- Telnet `telnet localhost 4444`
  - load binary code into memory with command: `load_image filename address`
    For example: `load_image out.bin 0x20000000`
  - Disassemble loaded code: 
    ```
	> arm disassemble 0x20000000 10
	0x20000000  0x20ff    	MOVS	r0, #0xff
	0x20000002  0x21de    	MOVS	r1, #0xde
	0x20000004  0x22ad    	MOVS	r2, #0xad
	0x20000006  0x230f    	MOVS	r3, #0x0f
	0x20000008  0x24f0    	MOVS	r4, #0xf0
	0x2000000a  0x25ab    	MOVS	r5, #0xab
	0x2000000c  0x26dc    	MOVS	r6, #0xdc
	0x2000000e  0x2712    	MOVS	r7, #0x12
	0x20000010  0x2043    	MOVS	r0, #0x43
	0x20000012  0x2199    	MOVS	r1, #0x99
    ```
  - Display registers contents: `reg`
  - Set register contents: `reg name value`
    Example: `reg pc 0x20000000`
	
  - Set breakpoint `bp address length`
    Example: `bp 0x20000012 2`

  - Run: `resume`
  - Step through code: `step`
	
# To figure out

- Byte ordering for 32bit thumb
- Generation of elf header and sections.
