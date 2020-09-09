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

thumb_opcode_t thumb16_opcode(uint16_t opcode) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 = opcode;
  return op;
}

thumb_opcode_t thumb16_opcode_imm7(uint16_t opcode, uint8_t imm7) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 =
    opcode | (imm7 & IMM7_MASK);
  return op;
}

thumb_opcode_t thumb16_opcode_imm8(uint16_t opcode, uint8_t imm8) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 =
    opcode | imm8;
  return op;
}

thumb_opcode_t thumb16_opcode_imm11(uint16_t opcode, uint16_t imm11) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 =
    opcode | (imm11 & IMM11_MASK);
  return op;
}


thumb_opcode_t thumb16_opcode_one_reg_low_imm5(uint16_t opcode, reg_t rd, uint8_t imm5) {
  thumb_opcode_t op;
  if (rd > r7 ) {
    op.kind = register_out_of_range;
    return op;
  }
  op.kind = thumb16;
  op.opcode.thumb16 = opcode |
    ((imm5 & IMM5_MASK) << 3) |
    (rd & REG_LOW_MASK);

  return op;
}

thumb_opcode_t thumb16_opcode_one_reg_low_imm8(uint16_t opcode, reg_t rd, uint8_t imm8) {
  thumb_opcode_t op;
  if (rd > r7 ) {
    op.kind = encode_error;
  } else { 
    op.kind = thumb16;
    op.opcode.thumb16 =
      opcode | ((rd & REG_LOW_MASK) << 8) | imm8;
  }
  return op;
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
  if (rd > r7 || rm > r7 || rn > r7) {
    op.kind = encode_error;
  } else { 
    op.kind = thumb16;
    op.opcode.thumb16 =
      opcode | ((rm & REG_LOW_MASK) << 6)| ((rn & REG_LOW_MASK) << 3) | (rd & REG_LOW_MASK);
  }
  return op;
}

thumb_opcode_t thumb16_opcode_two_regs_low_imm3(uint16_t opcode, reg_t rd, reg_t rm, uint8_t imm3) {
  thumb_opcode_t op;
  if (rd > r7 || rm > r7) {
    op.kind = register_out_of_range;
    return op;
  }
  op.kind = thumb16;
  op.opcode.thumb16 = opcode |
    ((imm3 & IMM3_MASK) << 6) |
    ((rm & REG_LOW_MASK) << 3) |
    (rd & REG_LOW_MASK);
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

thumb_opcode_t thumb16_opcode_one_reg_any(uint16_t opcode, reg_t rm) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 = opcode | ((rm & REG_MASK) << 3);
  return op;
}

thumb_opcode_t thumb16_opcode_two_regs_any(uint16_t opcode, reg_t r1, reg_t r2) {
  thumb_opcode_t op;
  op.kind = thumb16;
  op.opcode.thumb16 = opcode | ((r2 & REG_MASK) << 3) | (r1 & REG_LOW_MASK);
  if (r1 & REG_HIGH_BIT_MASK) 
    op.opcode.thumb16 |= (1 << 7);
  return op;
}

/* ************************************************************
   M0 OpCodes (32bit) (handmade for now) 
   ************************************************************ */
thumb_opcode_t m0_bl(int32_t offset) {
  thumb_opcode_t op;
  op.kind = thumb32;
  uint32_t opcode = 0b11110000000000001101000000000000;
  uint32_t i1 = ((1 << 24) & offset) >> 24;
  uint32_t i2 = ((1 << 23) & offset) >> 23;
  uint32_t s  = ((1 << 25) & offset) >> 25;
  uint32_t j1 = (s == i1) ? (1 << 13) : 0;
  uint32_t j2 = (s == i2) ? (1 << 11) : 0;
  opcode |= (j1 | j2) | (s << 26);
  op.opcode.thumb32.high = (opcode >> 16);
  op.opcode.thumb32.low  = opcode;
  uint16_t imm11 = ((offset >> 1) & IMM11_MASK);
  uint16_t imm10 = ((offset >> 12) & IMM10_MASK);
  op.opcode.thumb32.high |= imm10;
  op.opcode.thumb32.low |= imm11;
  return op;
}

thumb_opcode_t m0_dsb(void) {
  thumb_opcode_t op;
  op.kind = thumb32;
  uint32_t opcode = 0b11110011101111111000111101001111;
  op.opcode.thumb32.high = (opcode >> 16);
  op.opcode.thumb32.low  = opcode;
  return op;  
}

thumb_opcode_t m0_dmb(void) {
  thumb_opcode_t op;
  op.kind = thumb32;
  uint32_t opcode = 0b11110011101111111000111101011111;
  op.opcode.thumb32.high = (opcode >> 16);
  op.opcode.thumb32.low  = opcode;
  return op;  
}

thumb_opcode_t m0_isb(void) {
  thumb_opcode_t op;
  op.kind = thumb32;
  uint32_t opcode = 0b11110011101111111000111101101111;
  op.opcode.thumb32.high = (opcode >> 16);
  op.opcode.thumb32.low  = opcode;
  return op;  
}


/* ************************************************************ 
   M0 OpCodes   (GENERATED CODE)
   ************************************************************ */

thumb_opcode_t m0_adc_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16704, rdn, rm);
}

thumb_opcode_t m0_add_imm3(reg_t rd, reg_t rm, uint8_t imm3) {
  return thumb16_opcode_two_regs_low_imm3(7168, rd, rm, imm3);
}

thumb_opcode_t m0_add_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(12288, rdn, imm8);
}

thumb_opcode_t m0_add_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(6144, rd, rn, rm);
}

thumb_opcode_t m0_add_any(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_any(17408, rdn, rm);
}

thumb_opcode_t m0_add_sp_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(43008, rdn, imm8);
}

thumb_opcode_t m0_add_sp_imm7(uint8_t imm7) {
  return thumb16_opcode_imm7(45056, imm7);
}

thumb_opcode_t m0_and_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16384, rdn, rm);
}

thumb_opcode_t m0_asr_imm(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(4096, rd, rm, imm5);
}

thumb_opcode_t m0_asr_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16640, rdn, rm);
}

thumb_opcode_t m0_bic_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(17280, rdn, rm);
}

thumb_opcode_t m0_bkpt_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(48640, imm8);
}

thumb_opcode_t m0_blx_any(reg_t rdn) {
  return thumb16_opcode_one_reg_any(18304, rdn);
}

thumb_opcode_t m0_bx_any(reg_t rdn) {
  return thumb16_opcode_one_reg_any(18176, rdn);
}

thumb_opcode_t m0_b_imm11(uint16_t imm11) {
  return thumb16_opcode_imm11(57344, imm11);
}

thumb_opcode_t m0_beq_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(53248, imm8);
}

thumb_opcode_t m0_bne_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(53504, imm8);
}

thumb_opcode_t m0_bcs_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(53760, imm8);
}

thumb_opcode_t m0_bcc_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(54016, imm8);
}

thumb_opcode_t m0_bmi_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(54272, imm8);
}

thumb_opcode_t m0_bpl_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(54528, imm8);
}

thumb_opcode_t m0_bvs_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(54784, imm8);
}

thumb_opcode_t m0_bvc_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(55040, imm8);
}

thumb_opcode_t m0_bhi_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(55296, imm8);
}

thumb_opcode_t m0_bls_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(55552, imm8);
}

thumb_opcode_t m0_bge_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(55808, imm8);
}

thumb_opcode_t m0_blt_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(56064, imm8);
}

thumb_opcode_t m0_bgt_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(56320, imm8);
}

thumb_opcode_t m0_ble_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(56576, imm8);
}

thumb_opcode_t m0_cbnz_f_imm5(reg_t rdn, uint8_t imm5) {
  return thumb16_opcode_one_reg_low_imm5(47872, rdn, imm5);
}

thumb_opcode_t m0_cbnz_n_imm5(reg_t rdn, uint8_t imm5) {
  return thumb16_opcode_one_reg_low_imm5(47360, rdn, imm5);
}

thumb_opcode_t m0_cbz_f_imm5(reg_t rdn, uint8_t imm5) {
  return thumb16_opcode_one_reg_low_imm5(45824, rdn, imm5);
}

thumb_opcode_t m0_cbz_n_imm5(reg_t rdn, uint8_t imm5) {
  return thumb16_opcode_one_reg_low_imm5(45312, rdn, imm5);
}

thumb_opcode_t m0_cmn_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(17088, rdn, rm);
}

thumb_opcode_t m0_cmp_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(10240, rdn, imm8);
}

thumb_opcode_t m0_cmp_any(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_any(17664, rdn, rm);
}

thumb_opcode_t m0_eor_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16448, rdn, rm);
}

thumb_opcode_t m0_ldr_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(26624, rd, rm, imm5);
}

thumb_opcode_t m0_ldr_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(38912, rdn, imm8);
}

thumb_opcode_t m0_ldr_lit(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(18432, rdn, imm8);
}

thumb_opcode_t m0_ldr_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(22528, rd, rn, rm);
}

thumb_opcode_t m0_ldrb_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(30720, rd, rm, imm5);
}

thumb_opcode_t m0_ldrb_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(23552, rd, rn, rm);
}

thumb_opcode_t m0_ldrh_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(34816, rd, rm, imm5);
}

thumb_opcode_t m0_ldrh_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(23040, rd, rn, rm);
}

thumb_opcode_t m0_ldrsb_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(22016, rd, rn, rm);
}

thumb_opcode_t m0_ldrsh_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(24064, rd, rn, rm);
}

thumb_opcode_t m0_lsl_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(0, rd, rm, imm5);
}

thumb_opcode_t m0_lsl_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16512, rdn, rm);
}

thumb_opcode_t m0_lsr_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(2048, rd, rm, imm5);
}

thumb_opcode_t m0_lsr_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16576, rdn, rm);
}

thumb_opcode_t m0_mov_imm(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(8192, rdn, imm8);
}

thumb_opcode_t m0_mov_any(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_any(17920, rdn, rm);
}

thumb_opcode_t m0_mov_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(0, rdn, rm);
}

thumb_opcode_t m0_mul_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(17216, rdn, rm);
}

thumb_opcode_t m0_mvn_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(17344, rdn, rm);
}

thumb_opcode_t m0_nop(void) {
  return thumb16_opcode(48896);
}

thumb_opcode_t m0_orr_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(17152, rdn, rm);
}

thumb_opcode_t m0_pop(uint8_t imm8) {
  return thumb16_opcode_imm8(48128, imm8);
}

thumb_opcode_t m0_pop_lr(uint8_t imm8) {
  return thumb16_opcode_imm8(48384, imm8);
}

thumb_opcode_t m0_push(uint8_t imm8) {
  return thumb16_opcode_imm8(46080, imm8);
}

thumb_opcode_t m0_push_lr(uint8_t imm8) {
  return thumb16_opcode_imm8(46336, imm8);
}

thumb_opcode_t m0_rev_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(47616, rdn, rm);
}

thumb_opcode_t m0_rev16_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(47680, rdn, rm);
}

thumb_opcode_t m0_revsh_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(47808, rdn, rm);
}

thumb_opcode_t m0_ror_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16832, rdn, rm);
}

thumb_opcode_t m0_rsb_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16960, rdn, rm);
}

thumb_opcode_t m0_str_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(24576, rd, rm, imm5);
}

thumb_opcode_t m0_str_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(36864, rdn, imm8);
}

thumb_opcode_t m0_str_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(20480, rd, rn, rm);
}

thumb_opcode_t m0_strb_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(28672, rd, rm, imm5);
}

thumb_opcode_t m0_strb_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(21504, rd, rn, rm);
}

thumb_opcode_t m0_strh_imm5(reg_t rd, reg_t rm, uint8_t imm5) {
  return thumb16_opcode_two_regs_low_imm5(32768, rd, rm, imm5);
}

thumb_opcode_t m0_strh_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(20992, rd, rn, rm);
}

thumb_opcode_t m0_sub_low(reg_t rd, reg_t rn, reg_t rm) {
  return thumb16_opcode_three_regs_low(6656, rd, rn, rm);
}

thumb_opcode_t m0_sub_imm3(reg_t rd, reg_t rm, uint8_t imm3) {
  return thumb16_opcode_two_regs_low_imm3(7680, rd, rm, imm3);
}

thumb_opcode_t m0_sub_imm8(reg_t rdn, uint8_t imm8) {
  return thumb16_opcode_one_reg_low_imm8(14336, rdn, imm8);
}

thumb_opcode_t m0_sub_sp_imm(uint8_t imm7) {
  return thumb16_opcode_imm7(45184, imm7);
}

thumb_opcode_t m0_svc_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(57088, imm8);
}

thumb_opcode_t m0_sxtb_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(45632, rdn, rm);
}

thumb_opcode_t m0_sxth_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(45568, rdn, rm);
}

thumb_opcode_t m0_tst_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(16896, rdn, rm);
}

thumb_opcode_t m0_udf_imm8(uint8_t imm8) {
  return thumb16_opcode_imm8(56832, imm8);
}

thumb_opcode_t m0_uxtb_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(45760, rdn, rm);
}

thumb_opcode_t m0_uxth_low(reg_t rdn, reg_t rm) {
  return thumb16_opcode_two_regs_low(45696, rdn, rm);
}

thumb_opcode_t m0_wfe(void) {
  return thumb16_opcode(48928);
}

thumb_opcode_t m0_wfi(void) {
  return thumb16_opcode(48944);
}

thumb_opcode_t m0_yield(void) {
  return thumb16_opcode(48912);
}

