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

/* Info and questions

   __ISA compatibility per MX__
   *Cortex-M0   - ARMv6-M    (Thumb and subset of Thumb2)
   *Cortex-M0+  - ARMv6-M    (Thumb and subset of Thumb2)
   *Cortex-M1   - ARMv6-M    (Thumb and subset of Thumb2)
   Cortex-M23   - ARMv8-M    (Thumb and subset of Thumb2)
   *Cortex-M3   - ARMv7-M    (Thumb and subset of Thumb2)
   *Cortex-M4   - ARMv7E-M   (Thumb and Thumb2)
   Cortex-M33   - ARMv8-M    (Thumb and Thumb2) 
   Cortex-M35p  - ARMv8-M    (Thumb and Thumb2)
   Cortex-M55   - ARMv8.1-M  (Thumb and Thumb2)
   Cortex-M7    - ARMv7E-M   (Thumb and Thumb2)
   ( * The ones we care about the most currently )

   __Instruction encodings (T1, T2, T3)__
   The Architecture Reference Manual is a bit vague on what the 
   meaning of instruction encodings T1,T2 and T3 are.

   It does say that "each Thumb instruction encoding has a numbered
   label, T1, T2". 
   By looking at some of the instructions in A7.7 I can only 
   guess that it is just a numbering of the encodings and does 
   not give any hint to which architectures or ISAs they belong to. 

   From the "Definite Guide to the ARM Cortex-M3 and Cortex-M4
   processors" - Joseph Yiu, we can read that M0, M1 and M1+ only 
   supports 16bit instructions + a few 32bit ones.
   That the M3 supports many more 32bit instructions and a few 
   more 16bit ones. 
   That the M4 supports the remaining DSP enhancing and SIMD 
   instructions. 
   Lastly that M4 with FPU supports supports an additional set 
   of floating point instructions that are all 32bit. 
   The "def. guide" also says that M4 runs all M3 that runs all 
   m1 ... instructions. 
   
   So a sensible naming scheme for instruction generators would 
   be to prefix them with the m<n> where their support is introduced. 
*/
 
const uint16_t m0_opcode_add_low = 0b0001100000000000; /* add low_reg low_reg low_reg */
const uint16_t m0_opcode_add_any = 0b0100010000000000; /* add any_reg any_reg */
const uint16_t m0_opcode_asr_imm = 0b0001000000000000;
const uint16_t m0_opcode_asr_low = 0b0100000100000000; 
const uint16_t m0_opcode_lsl_imm = 0b0000000000000000;
const uint16_t m0_opcode_lsl_low = 0b0100000010000000;
const uint16_t m0_opcode_lsr_imm = 0b0000100000000000;
const uint16_t m0_opcode_lsr_low = 0b0100000011000000;
const uint16_t m0_opcode_mov_imm = 0b0010000000000000;
const uint16_t m0_opcode_mov_any = 0b0100011000000000; /* mov any_reg any_reg */
const uint16_t m0_opcode_mov_low = 0b0000000000000000; /* mov low_reg low_reg */



uint16_t byte_reverse(uint16_t in) {
  uint8_t a = in & 0xFF;
  uint8_t b = (in >> 8) & 0xFF;
  uint16_t out = (uint16_t)b | ((uint16_t)a) << 8;
}

int emit_opcode(instr_seq_t *seq, thumb_opcode_t op) {
  if (seq->mc == NULL || seq->pos >= seq->size) return 0;
  
  if (op.kind == thumb32 && seq->pos <= (seq->size -2)) {
    seq->mc[seq->pos++] = op.opcode.thumb32.high;
    seq->mc[seq->pos++] = op.opcode.thumb32.low;
  } else if (op.kind == thumb16 && seq->pos <= (seq->size -1)) {
    seq->mc[seq->pos++] = op.opcode.thumb16;
  } else {
    return 0;
  }
  return 1;
}

thumb_opcode_t thumb16_opcode_two_regs_low(uint16_t opcode, reg_t rd, reg_t rm) {
  thumb_opcode_t op;
  if (rd > r7 || rm > r7) {
    op.kind = encode_error;
  } else { 
    op.kind = thumb16;
    op.opcode.thumb16 =
      opcode | ((rm & REG_LOW_MASK) << 3) | (rd & REG_LOW_MASK);
  }
  return op;
}

thumb_opcode_t thumb16_opcode_three_regs_low(uint16_t opcode, reg_t rd, reg_t rn, reg_t rm) {
  thumb_opcode_t op;
  if (rd > r7 || rm > r7 || rn > 7) {
    op.kind = encode_error;
  } else { 
    op.kind = thumb16;
    op.opcode.thumb16 =
      opcode | ((rm & REG_LOW_MASK) << 6)| ((rn & REG_LOW_MASK) << 3) | (rd & REG_LOW_MASK);
  }
  return op;
}

thumb_opcode_t thumb16_opcode_two_regs_low_imm5(uint16_t opcode, reg_t rd, reg_t rm, uint8_t imm5) {
  thumb_opcode_t op;
  if (rd > r7 || rm > r7) {
    op.kind = register_out_of_range;
    return op;
  }
  op.kind = thumb16;
  op.opcode.thumb16 = opcode |
    ((imm5 & IMM5_MASK) << 6) |
    ((rm & REG_LOW_MASK) << 3) |
    (rd & REG_LOW_MASK);
  return op;
}

/* ************************************************************ 
   M0 OpCodes
   ************************************************************ */ 

thumb_opcode_t m0_add_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(m0_opcode_add_low, rd, rn, rm);
}

thumb_opcode_t m0_add_any(reg_t rd, reg_t rm) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 = m0_opcode_add_any | ((rm << 3) & REG_MASK) | (rd & REG_LOW_MASK);
  if (rd & REG_HIGH_BIT_MASK) 
    op.opcode.thumb16 |= (1 << 7);
  return op;
}

thumb_opcode_t m0_asr_imm(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(m0_opcode_asr_imm, rd, rm, imm5);
}

thumb_opcode_t m0_asr_low(reg_t rd, reg_t rm) {
  return thumb16_opcode_two_regs_low(m0_opcode_asr_low, rd, rm);
}

thumb_opcode_t m0_lsl_imm(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(m0_opcode_lsl_imm, rd, rm, imm5);
}

thumb_opcode_t m0_lsl_low(reg_t rd, reg_t rm) {
  return thumb16_opcode_two_regs_low(m0_opcode_lsl_low, rd, rm);
}

thumb_opcode_t m0_lsr_imm(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(m0_opcode_lsr_imm, rd, rm, imm5);
}

thumb_opcode_t m0_lsr_low(reg_t rd, reg_t rm) {
  return thumb16_opcode_two_regs_low(m0_opcode_lsr_low, rd, rm);
}

thumb_opcode_t m0_mov_imm(reg_t rd, uint8_t imm8) {
  thumb_opcode_t op;
  if (rd > r7) {
    op.kind = register_out_of_range;
    return op;
  }
  op.kind = thumb16;
  op.opcode.thumb16 = m0_opcode_mov_imm | ((rd & REG_LOW_MASK) << 8) | imm8;
  return op;
}
  
thumb_opcode_t m0_mov_any(reg_t rd, reg_t rm) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 = m0_opcode_mov_any | ((rm & REG_MASK) << 3) | (rd & REG_LOW_MASK);
  if (rd & REG_HIGH_BIT_MASK) op.opcode.thumb16 = op.opcode.thumb16 | (1 << 7);
  return op;
}

thumb_opcode_t m0_mov_low(reg_t rd, reg_t rm) {
  return thumb16_opcode_two_regs_low(m0_opcode_mov_low, rd, rm); 
}
