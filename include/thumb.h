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

#include <stdbool.h>
#include <stdint.h>
#include <instructions.h>
#include <registers.h>

#define IMM5_MASK (uint8_t)0b00011111
#define IMM3_MASK (uint8_t)0b00000111
#define IMM2_MASK (uint8_t)0b00000011

#define REG_LOW_MASK      (uint8_t)0b00000111
#define REG_MASK          (uint8_t)0b00001111
#define REG_HIGH_BIT_MASK (uint8_t)0b00001000

typedef uint16_t thumb16_opcode_t;

typedef struct {
  uint16_t high;
  uint16_t low;
} thumb32_opcode_t;

typedef enum {
  thumb16,
  thumb32,
  register_out_of_range,
  encode_error
} thumb_opcode_kind;

typedef struct {
  thumb_opcode_kind kind;
  union {
    thumb16_opcode_t thumb16;
    thumb32_opcode_t thumb32;
    int error_code;
  } opcode;
} thumb_opcode_t;

typedef struct {
  uint16_t *mc;
  unsigned int size;
  unsigned int pos;
} instr_seq_t;

extern int emit_opcode(instr_seq_t *seq, thumb_opcode_t op);

extern thumb_opcode_t m0_add_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_add_any(reg_t rd, reg_t rm);
extern thumb_opcode_t m0_asr_imm(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_asr_low(reg_t rd, reg_t rm);
extern thumb_opcode_t m0_lsl_imm(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_lsl_low(reg_t rd, reg_t rm);
extern thumb_opcode_t m0_lsr_imm(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_lsr_low(reg_t rd, reg_t rm);
extern thumb_opcode_t m0_mov_imm(reg_t rd, uint8_t imm8);
extern thumb_opcode_t m0_mov_any(reg_t rd, reg_t rm);
extern thumb_opcode_t m0_mov_low(reg_t rd, reg_t rm);

#endif
