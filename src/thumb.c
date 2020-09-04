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

#include <thumb.h>

#include <stdio.h>

const uint16_t m0_opcode_mov_t1 = 0b0010000000000000;

uint16_t byte_reverse(uint16_t in) {
  uint8_t a = in & 0xFF;
  uint8_t b = (in >> 8) & 0xFF;
  uint16_t out = (uint16_t)b | ((uint16_t)a) << 8;
}

int emit_opcode(instr_seq_t *seq, thumb_opcode_t op) {
  if (seq->mc == NULL || seq->pos >= seq->size) return 0;
  
  if (op.is_32bit && seq->pos < (seq->size -2)) {
    seq->mc[seq->pos++] = byte_reverse(op.opcode.thumb32.high);
    seq->mc[seq->pos++] = byte_reverse(op.opcode.thumb32.low);
  } else if (!op.is_32bit && seq->pos < (seq->size -1)) {
    seq->mc[seq->pos++] = byte_reverse(op.opcode.thumb16);
  } else {
    return 0;
  }
  return 1;
}


thumb_opcode_t m0_lsl_imm(reg_t rm, reg_t rd, uint8_t imm5) {
  (void)rm;
  (void)rd;
  (void)imm5;
  thumb_opcode_t op;
  return op;
}

thumb_opcode_t m0_lsr_imm(reg_t rm, reg_t rd, uint8_t imm5) {
  (void)rm;
  (void)rd;
  (void)imm5;
  thumb_opcode_t op;
  return op;
}

thumb_opcode_t m0_mov_imm(reg_t rd, uint8_t imm8) {
  thumb_opcode_t op;
  op.is_32bit = false;
  op.opcode.thumb16 = m0_opcode_mov_t1 | ((rd & REG_MASK) << 8) | imm8;
  return op;
}
  
