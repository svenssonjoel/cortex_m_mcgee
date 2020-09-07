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

#include <stdio.h>
#include <stdint.h>

typedef enum {
  nothing,
  imm7,
  imm8,
  one_reg_low_imm5,
  one_reg_low_imm8,
  two_regs_low,
  two_regs_low_imm3,
  two_regs_low_imm5,
  one_reg_any,
  two_regs_any,
  three_regs_low
} thumb16_opcode_format;


typedef struct {
  char *name;
  uint16_t opcode;
  thumb16_opcode_format format;
} thumb16_opcode;


/* Special instructions that are left out from here: 
   CPS - Change processor state
*/
   

const thumb16_opcode opcodes[] =
  {{"m0_adc_low"    , 0b0100000101000000, two_regs_low},
   {"m0_add_imm3"   , 0b0001110000000000, two_regs_low_imm3},
   {"m0_add_imm8"   , 0b0011000000000000, one_reg_low_imm8},
   {"m0_add_low"    , 0b0001100000000000, three_regs_low},
   {"m0_add_any"    , 0b0100010000000000, two_regs_any},
   {"m0_add_sp_imm8", 0b1010100000000000, one_reg_low_imm8},
   {"m0_add_sp_imm7", 0b1011000000000000, imm7},
   {"m0_and_low"    , 0b0100000000000000, two_regs_low},
   {"m0_asr_imm"    , 0b0001000000000000, two_regs_low_imm5},
   {"m0_asr_low"    , 0b0100000100000000, two_regs_low},
   {"m0_bic_low"    , 0b0100001110000000, two_regs_low},
   {"m0_bkpt_imm8"  , 0b1011111000000000, imm8},
   {"m0_blx_any"    , 0b0100011110000000, one_reg_any},
   {"m0_bx_any"     , 0b0100011100000000, one_reg_any},
   {"m0_beq_imm8"   , 0b1101000000000000, imm8},
   {"m0_bne_imm8"   , 0b1101000100000000, imm8},
   {"m0_bcs_imm8"   , 0b1101001000000000, imm8},
   {"m0_bcc_imm8"   , 0b1101001100000000, imm8},
   {"m0_bmi_imm8"   , 0b1101010000000000, imm8},
   {"m0_bpl_imm8"   , 0b1101010100000000, imm8},
   {"m0_bvs_imm8"   , 0b1101011000000000, imm8},
   {"m0_bvc_imm8"   , 0b1101011100000000, imm8},
   {"m0_bhi_imm8"   , 0b1101100000000000, imm8},
   {"m0_bls_imm8"   , 0b1101100100000000, imm8},
   {"m0_bge_imm8"   , 0b1101101000000000, imm8},
   {"m0_blt_imm8"   , 0b1101101100000000, imm8},
   {"m0_bgt_imm8"   , 0b1101110000000000, imm8},
   {"m0_ble_imm8"   , 0b1101110100000000, imm8},
   {"m0_bal_imm8"   , 0b1101111000000000, imm8},
   {"m0_cbnz_f_imm5", 0b1011101100000000, one_reg_low_imm5},
   {"m0_cbnz_n_imm5", 0b1011100100000000, one_reg_low_imm5},
   {"m0_cbz_f_imm5" , 0b1011001100000000, one_reg_low_imm5},
   {"m0_cbz_n_imm5" , 0b1011000100000000, one_reg_low_imm5},
   {"m0_cmn_low"    , 0b0100001011000000, two_regs_low},
   {"m0_cmp_imm8"   , 0b0010100000000000, one_reg_low_imm8},
   {"m0_cmp_any"    , 0b0100010100000000, two_regs_any},
   {"m0_eor_low"    , 0b0100000001000000, two_regs_low},
   {"m0_ldr_imm5"   , 0b0110100000000000, two_regs_low_imm5},
   {"m0_ldr_imm8"   , 0b1001100000000000, one_reg_low_imm8},
   {"m0_ldr_lit"    , 0b0100100000000000, one_reg_low_imm8},
   {"m0_ldr_low"    , 0b0101100000000000, three_regs_low},
   {"m0_ldrb_imm5"  , 0b0111100000000000, two_regs_low_imm5},
   {"m0_ldrb_low"   , 0b0101110000000000, three_regs_low},
   {"m0_ldrh_imm5"  , 0b1000100000000000, two_regs_low_imm5},
   {"m0_ldrh_low"   , 0b0101101000000000, three_regs_low},
   {"m0_ldrsb_low"  , 0b0101011000000000, three_regs_low},
   {"m0_ldrsh_low"  , 0b0101111000000000, three_regs_low},
   {"m0_lsl_imm5"   , 0b0000000000000000, two_regs_low_imm5},
   {"m0_lsl_low"    , 0b0100000010000000, two_regs_low},
   {"m0_lsr_imm5"   , 0b0000100000000000, two_regs_low_imm5},
   {"m0_lsr_low"    , 0b0100000011000000, two_regs_low},
   {"m0_mov_imm"    , 0b0010000000000000, one_reg_low_imm8},
   {"m0_mov_any"    , 0b0100011000000000, two_regs_any},
   {"m0_mov_low"    , 0b0000000000000000, two_regs_low},
   {"m0_mul_low"    , 0b0100001101000000, two_regs_low},
   {"m0_mvn_low"    , 0b0100001111000000, two_regs_low},
   {"m0_nop"        , 0b1011111100000000, nothing},
   {"m0_orr_low"    , 0b0100001100000000, two_regs_low},
   {"m0_pop"        , 0b1011110000000000, imm8}, 
   {"m0_pop_lr"     , 0b1011110100000000, imm8},  
   {"m0_push"       , 0b1011010000000000, imm8},
   {"m0_push_lr"    , 0b1011010100000000, imm8},
   {"m0_rev_low"    , 0b1011101000000000, two_regs_low},
   {"m0_rev16_low"  , 0b1011101001000000, two_regs_low},
   {"m0_revsh_low"  , 0b1011101011000000, two_regs_low},
   {"m0_ror_low"    , 0b0100000111000000, two_regs_low},
   {"m0_rsb_low"    , 0b0100001001000000, two_regs_low},
   {"m0_str_imm5"   , 0b0110000000000000, two_regs_low_imm5},
   {"m0_str_imm8"   , 0b1001000000000000, one_reg_low_imm8},
   {"m0_str_low"    , 0b0101000000000000, three_regs_low},
   {"m0_strb_imm5"  , 0b0111000000000000, two_regs_low_imm5},
   {"m0_strb_low"   , 0b0101010000000000, three_regs_low},
   {"m0_strh_imm5"  , 0b1000000000000000, two_regs_low_imm5},
   {"m0_strh_low"   , 0b0101001000000000, three_regs_low},
   {"m0_sub_low"    , 0b0001101000000000, three_regs_low},
   {"m0_sub_imm3"   , 0b0001111000000000, two_regs_low_imm3},
   {"m0_sub_imm8"   , 0b0011100000000000, one_reg_low_imm8},
   {"m0_sub_sp_imm" , 0b1011000010000000, imm7},
   {"m0_svc_imm8"   , 0b1101111100000000, imm8},
   {"m0_sxtb_low"   , 0b1011001001000000, two_regs_low},
   {"m0_sxth_low"   , 0b1011001000000000, two_regs_low},
   {"m0_tst_low"    , 0b0100001000000000, two_regs_low},
   {"m0_udf_imm8"   , 0b1101111000000000, imm8},
   {"m0_uxtb_low"   , 0b1011001011000000, two_regs_low},
   {"m0_uxth_low"   , 0b1011001010000000, two_regs_low},
   {"m0_wfe"        , 0b1011111100100000, nothing},
   {"m0_wfi"        , 0b1011111100110000, nothing},
   {"m0_yield"      , 0b1011111100010000, nothing},
   {NULL, 0, 0}};


void print_extern_decl(thumb16_opcode op) {
  switch(op.format) {
  case nothing:
    printf("extern thumb_opcode_t %s(void);\n", op.name);
    break;
  case imm7:
    printf("extern thumb_opcode_t %s(uint8_t imm7);\n", op.name);
    break;
  case imm8:
    printf("extern thumb_opcode_t %s(uint8_t imm8);\n", op.name);
    break;
  case one_reg_any:
    printf("extern thumb_opcode_t %s(reg_t rdn);\n", op.name);
    break;
  case one_reg_low_imm5:
    printf("extern thumb_opcode_t %s(reg_t rdn, uint8_t imm5);\n", op.name);
    break;
  case one_reg_low_imm8:
    printf("extern thumb_opcode_t %s(reg_t rdn, uint8_t imm8);\n", op.name);
    break;
  case two_regs_low:
    printf("extern thumb_opcode_t %s(reg_t rdn, reg_t rm);\n", op.name);
    break;
  case two_regs_low_imm3:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rm, uint8_t imm3);\n", op.name);
    break;
  case two_regs_low_imm5:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rm, uint8_t imm5);\n", op.name);
    break;
  case two_regs_any:
    printf("extern thumb_opcode_t %s(reg_t rdn, reg_t rm);\n", op.name);
    break;
  case three_regs_low:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm);\n", op.name);
    break;
  default:
    printf("Error - unknown\n");
    break;
  }
}

void print_opcode(thumb16_opcode op) {

  switch(op.format) {
  case nothing:
    printf("thumb_opcode_t %s(void) {\n", op.name);
    printf("  return thumb16_opcode(%u);\n", op.opcode);
    printf("}\n\n");
    break;
  case imm7:
    printf("thumb_opcode_t %s(uint8_t imm7) {\n", op.name);
    printf("  return thumb16_opcode_imm7(%u, imm7);\n", op.opcode);
    printf("}\n\n");
    break;
  case imm8:
    printf("thumb_opcode_t %s(uint8_t imm8) {\n", op.name);
    printf("  return thumb16_opcode_imm8(%u, imm8);\n", op.opcode);
    printf("}\n\n");
    break;
  case one_reg_any:
    printf("thumb_opcode_t %s(reg_t rdn) {\n", op.name);
    printf("  return thumb16_opcode_one_reg_any(%u, rdn);\n", op.opcode);
    printf("}\n\n");
    break;
  case one_reg_low_imm5:
    printf("thumb_opcode_t %s(reg_t rdn, uint8_t imm5) {\n", op.name);
    printf("  return thumb16_opcode_one_reg_low_imm5(%u, rdn, imm5);\n", op.opcode);
    printf("}\n\n");
    break;
  case one_reg_low_imm8:
    printf("thumb_opcode_t %s(reg_t rdn, uint8_t imm8) {\n", op.name);
    printf("  return thumb16_opcode_one_reg_low_imm8(%u, rdn, imm8);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_low:
    printf("thumb_opcode_t %s(reg_t rdn, reg_t rm) {\n", op.name);
    printf("  return thumb16_opcode_two_regs_low(%u, rdn, rm);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_low_imm3:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rm, uint8_t imm3) {\n", op.name);
    printf("  return thumb16_opcode_two_regs_low_imm3(%u, rd, rm, imm3);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_low_imm5:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rm, uint8_t imm5) {\n", op.name);
    printf("  return thumb16_opcode_two_regs_low_imm5(%u, rd, rm, imm5);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_any:
    printf("thumb_opcode_t %s(reg_t rdn, reg_t rm) {\n", op.name);
    printf("  return thumb16_opcode_two_regs_any(%u, rdn, rm);\n", op.opcode);
    printf("}\n\n");
    break;
  case three_regs_low:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm) {\n", op.name);
    printf("  return thumb16_opcode_three_regs_low(%u, rd, rn, rm);\n", op.opcode);
    printf("}\n\n");
    break;
  default:
    printf("Error - unknown\n");
    break;
  } 
}


void unique_ops() {

  int i = 0;

  while (opcodes[i].name) {
    int j = i+1;
    while (opcodes[j].name) {
      if (i != j && opcodes[i].opcode == opcodes[j].opcode) {
	printf("WARNING!\n");
	printf("opcodes at indices %d and %d are the same\n", i, j);
	printf("%d: %s :  %u \n", i, opcodes[i].name, opcodes[i].opcode);
	printf("%d: %s :  %u \n", j, opcodes[j].name, opcodes[j].opcode);
	printf("\n");
      }
      j++;
    }
    i++;
  }
}


int main(int argc, char **argv) {

  int i = 0;

  unique_ops();
  
  i = 0;
  while (opcodes[i].name) {
    print_extern_decl(opcodes[i++]);
  }

  printf("\n\n\n");
  
  i = 0;
  while (opcodes[i].name) {
    print_opcode(opcodes[i++]);
  }
  
  return 0;
}
