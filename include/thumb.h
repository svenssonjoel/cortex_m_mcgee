/**********************************************************************************/
/* MIT License									  */
/* 										  */
/* Copyright (c) 2020 Joel Svensson             				  */
/* 										  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy	  */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights	  */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell	  */
/* copies of the Software, and to permit persons to whom the Software is	  */
/* furnished to do so, subject to the following conditions:			  */
/* 										  */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.				  */
/* 										  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR	  */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,	  */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE	  */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER	  */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.									  */
/**********************************************************************************/

#ifndef __THUMB_H_
#define __THUMB_H_

#include <stdint.h>
#include <instructions.h>
#include <registers.h>

#define IMM5_MASK (uint8_t)0b00011111
#define IMM3_MASK (uint8_t)0b00000111
#define IMM2_MASK (uint8_t)0b00000011

typedef uint16_t thumb16_opcode_t;

typedef struct {
  uint16_t high;
  uint16_t low;
} thumb32_opcode_t;

typedef struct {
  bool is_32bit;
  union {
    thumb16_opcode_t thumb16;
    thumb32_opcode_t thumb32;
  } opcode;
} thumb_opcode_t;

typedef uint16_t* instr_seq_t;


thumb_opcode_t m0_lsl_imm(uint8_t imm, register_t rm, register_t rd);
thumb_opcode_t m0_lsr_imm(uint8_t imm, register_t rm, register_t rd);



#endif
