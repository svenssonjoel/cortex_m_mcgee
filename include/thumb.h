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


#define IMM7_MASK (uint8_t)0b01111111
#define IMM5_MASK (uint8_t)0b00011111
#define IMM3_MASK (uint8_t)0b00000111
#define IMM2_MASK (uint8_t)0b00000011

#define IMM10_MASK (uint16_t)0b0000001111111111
#define IMM11_MASK (uint16_t)0b0000011111111111
#define IMM12_MASK (uint16_t)0b0000111111111111

#define IMM25_MASK (uint32_t)0x1FFFFFF

#define REG_LOW_MASK      (uint8_t)0b00000111
#define REG_MASK          (uint8_t)0b00001111
#define REG_HIGH_BIT_MASK (uint8_t)0b00001000

#define IMM_SHIFT_LSL  (uint32_t)0b00
#define IMM_SHIFT_LSR  (uint32_t)0b01
#define IMM_SHIFT_ASR  (uint32_t)0b10
#define IMM_SHIFT_RRX  (uint32_t)0b11    //RRX is IMM5 = 0
#define IMM_SHIFT_ROR  (uint32_t)0b11    //ROR or RRX depends on IMM5 being 0 or n > 0

typedef enum {
  imm_shift_lsl,
  imm_shift_lsr,
  imm_shift_asr,
  imm_shift_rrx,
  imm_shift_ror,
  imm_shift_none
} imm_shift_t;

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

/* handcoded */
extern thumb_opcode_t m3_bfc(reg_t rd, uint8_t lsb, uint8_t width);
extern thumb_opcode_t m3_bfi(reg_t rd, reg_t rn, uint8_t lsb, uint8_t width);

/* Generated code */
extern thumb_opcode_t m0_adc_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_add_imm3(reg_t rd, reg_t rm, uint8_t imm3);
extern thumb_opcode_t m0_add_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_add_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_add_any(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_add_sp_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_add_sp_imm7(uint8_t imm7);
extern thumb_opcode_t m0_and_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_asr_imm(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_asr_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_bic_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_bkpt_imm8(uint8_t imm8);
extern thumb_opcode_t m0_blx_any(reg_t rdn);
extern thumb_opcode_t m0_bx_any(reg_t rdn);
extern thumb_opcode_t m0_b_imm11(uint16_t imm11);
extern thumb_opcode_t m0_beq_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bne_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bcs_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bcc_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bmi_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bpl_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bvs_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bvc_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bhi_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bls_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bge_imm8(uint8_t imm8);
extern thumb_opcode_t m0_blt_imm8(uint8_t imm8);
extern thumb_opcode_t m0_bgt_imm8(uint8_t imm8);
extern thumb_opcode_t m0_ble_imm8(uint8_t imm8);
extern thumb_opcode_t m0_cbnz_f_imm5(reg_t rdn, uint8_t imm5);
extern thumb_opcode_t m0_cbnz_n_imm5(reg_t rdn, uint8_t imm5);
extern thumb_opcode_t m0_cbz_f_imm5(reg_t rdn, uint8_t imm5);
extern thumb_opcode_t m0_cbz_n_imm5(reg_t rdn, uint8_t imm5);
extern thumb_opcode_t m0_cmn_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_cmp_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_cmp_any(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_eor_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_ldr_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_ldr_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_ldr_lit(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_ldr_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_ldrb_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_ldrb_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_ldrh_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_ldrh_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_ldrsb_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_ldrsh_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_lsl_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_lsl_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_lsr_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_lsr_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_mov_imm(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_mov_any(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_mov_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_mul_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_mvn_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_nop(void);
extern thumb_opcode_t m0_orr_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_pop(uint8_t imm8);
extern thumb_opcode_t m0_pop_lr(uint8_t imm8);
extern thumb_opcode_t m0_push(uint8_t imm8);
extern thumb_opcode_t m0_push_lr(uint8_t imm8);
extern thumb_opcode_t m0_rev_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_rev16_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_revsh_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_ror_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_rsb_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_str_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_str_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_str_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_strb_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_strb_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_strh_imm5(reg_t rd, reg_t rm, uint8_t imm5);
extern thumb_opcode_t m0_strh_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_sub_low(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m0_sub_imm3(reg_t rd, reg_t rm, uint8_t imm3);
extern thumb_opcode_t m0_sub_imm8(reg_t rdn, uint8_t imm8);
extern thumb_opcode_t m0_sub_sp_imm(uint8_t imm7);
extern thumb_opcode_t m0_svc_imm8(uint8_t imm8);
extern thumb_opcode_t m0_sxtb_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_sxth_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_tst_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_udf_imm8(uint8_t imm8);
extern thumb_opcode_t m0_uxtb_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_uxth_low(reg_t rdn, reg_t rm);
extern thumb_opcode_t m0_wfe(void);
extern thumb_opcode_t m0_wfi(void);
extern thumb_opcode_t m0_yield(void);

/* M0 - M3 32bit Instructions */

extern thumb_opcode_t m0_dsb(void);
extern thumb_opcode_t m0_dmb(void);
extern thumb_opcode_t m0_isb(void);
extern thumb_opcode_t m0_bl(int32_t offset);
extern thumb_opcode_t m3_adc_imm(reg_t rd, reg_t rn, uint16_t imm12, bool sf);
extern thumb_opcode_t m3_adc_any(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf);
extern thumb_opcode_t m3_add_const(reg_t rd, reg_t rn, uint16_t imm12, bool sf);
extern thumb_opcode_t m3_add_imm(reg_t rd, reg_t rn, uint16_t imm12);
extern thumb_opcode_t m3_add_any(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf);
extern thumb_opcode_t m3_add_sp_imm(reg_t rd, reg_t rn, uint8_t imm5, imm_shift_t shift, bool sf);
extern thumb_opcode_t m3_add_pc_imm(reg_t rd, uint16_t imm12);
extern thumb_opcode_t m3_sub_pc_imm(reg_t rd, uint16_t imm12);
extern thumb_opcode_t m3_and_imm(reg_t rd, reg_t rn, uint16_t imm12, bool sf);
extern thumb_opcode_t m3_and_any(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf);
extern thumb_opcode_t m3_asr_imm(reg_t rd, reg_t rn, uint8_t imm5, bool sf);
extern thumb_opcode_t m3_asr_any(reg_t rd, reg_t rn, reg_t rm, bool sf);
extern thumb_opcode_t m3_beq(int32_t offset);
extern thumb_opcode_t m3_bne(int32_t offset);
extern thumb_opcode_t m3_bcs(int32_t offset);
extern thumb_opcode_t m3_bcc(int32_t offset);
extern thumb_opcode_t m3_bmi(int32_t offset);
extern thumb_opcode_t m3_bpl(int32_t offset);
extern thumb_opcode_t m3_bvs(int32_t offset);
extern thumb_opcode_t m3_bvc(int32_t offset);
extern thumb_opcode_t m3_bhi(int32_t offset);
extern thumb_opcode_t m3_bls(int32_t offset);
extern thumb_opcode_t m3_bge(int32_t offset);
extern thumb_opcode_t m3_blt(int32_t offset);
extern thumb_opcode_t m3_bgt(int32_t offset);
extern thumb_opcode_t m3_ble(int32_t offset);
extern thumb_opcode_t m3_bal(int32_t offset);
extern thumb_opcode_t m3_b(int32_t offset);
extern thumb_opcode_t m3_bic_imm(reg_t rd, reg_t rn, uint16_t imm12, bool sf);
extern thumb_opcode_t m3_bid_any(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf);
extern thumb_opcode_t m3_clrex(void);
extern thumb_opcode_t m3_clz(reg_t rd, reg_t rn, reg_t rm);
extern thumb_opcode_t m3_cmn_imm(reg_t rd, uint16_t imm12);
extern thumb_opcode_t m3_cmn_any(reg_t rd, reg_t rn, uint8_t imm5, imm_shift_t shift);

#endif
