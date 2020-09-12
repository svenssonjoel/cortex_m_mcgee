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
  one_reg_any_imm12,
  two_regs_any_imm12,
  two_regs_any_imm12_sf,
  two_regs_any_imm5_sf,
  two_regs_any_imm5_shift_sf,
  three_regs_any_sf,
  three_regs_any_imm5_shift_sf
} thumb32_opcode_format;


typedef struct {
  char *name;
  uint32_t opcode;
  thumb32_opcode_format format;
} thumb32_opcode;


const thumb32_opcode opcodes[] =
  { {"m0_dsb"        , 0b11110011101111111000111101001111, nothing},
    {"m0_dmb"        , 0b11110011101111111000111101011111, nothing},
    {"m0_isb"        , 0b11110011101111111000111101101111, nothing},
    {"m3_adc_imm"    , 0b11110001010000000000000000000000, two_regs_any_imm12_sf},
    {"m3_adc_any"    , 0b11101011010000000000000000000000, three_regs_any_imm5_shift_sf},
    {"m3_add_const"  , 0b11110001000000000000000000000000, two_regs_any_imm12_sf},
    {"m3_add_imm"    , 0b11110010000000000000000000000000, two_regs_any_imm12},
    {"m3_add_any"    , 0b11101011000000000000000000000000, three_regs_any_imm5_shift_sf},
    {"m3_add_sp_imm" , 0b11101011000011010000000000000000, two_regs_any_imm5_shift_sf},
    {"m3_add_pc_imm" , 0b11110010000011110000000000000000, one_reg_any_imm12},
    {"m3_sub_pc_imm" , 0b11110010101011110000000000000000, one_reg_any_imm12},
    {"m3_and_imm"    , 0b11110000000000000000000000000000, two_regs_any_imm12_sf},
    {"m3_and_any"    , 0b11101010000000000000000000000000, three_regs_any_imm5_shift_sf},
    {"m3_asr_imm"    , 0b11101010010011110000000000100000, two_regs_any_imm5_sf},
    {"m3_asr_any"    , 0b11111010010000001111000000000000, three_regs_any_sf},
    {NULL, 0, 0}};


void print_extern_decl(thumb32_opcode op) {
  switch(op.format) {
  case nothing:
    printf("extern thumb_opcode_t %s(void);\n", op.name);
    break;
  case one_reg_any_imm12:
    printf("extern thumb_opcode_t %s(reg_t rd, uint16_t imm12);\n", op.name);
    break;
  case two_regs_any_imm12:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, uint16_t imm12);\n", op.name);
    break;
  case two_regs_any_imm12_sf:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, uint16_t imm12, bool sf);\n", op.name);
    break;
  case two_regs_any_imm5_sf:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, uint8_t imm5, bool sf);\n", op.name);
    break;
  case two_regs_any_imm5_shift_sf:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, uint8_t imm5, imm_shift_t shift, bool sf);\n", op.name);
    break;
  case three_regs_any_sf:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm, bool sf);\n", op.name);
    break;
  case three_regs_any_imm5_shift_sf:
    printf("extern thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf);\n", op.name);
    break;
  default:
    printf("Error - unknown\n");
    break;
  }
}

void print_opcode(thumb32_opcode op) {

  switch(op.format) {
  case nothing:
    printf("thumb_opcode_t %s(void) {\n", op.name);
    printf("  return thumb32_opcode(%u);\n", op.opcode);
    printf("}\n\n");
    break;
  case one_reg_any_imm12:
    printf("thumb_opcode_t %s(reg_t rd,uint16_t imm12) {\n", op.name);
    printf("  return thumb32_opcode_one_reg_any_imm12(%u, rd, imm12);\n", op.opcode);
    printf("}\n\n");
    break;  
  case two_regs_any_imm12:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, uint16_t imm12) {\n", op.name);
    printf("  return thumb32_opcode_two_regs_any_imm12(%u, rd, rn, imm12);\n", op.opcode);
    printf("}\n\n");
    break;  
  case two_regs_any_imm12_sf:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, uint16_t imm12, bool sf) {\n", op.name);
    printf("  return thumb32_opcode_two_regs_any_imm12_sf(%u, rd, rn, imm12, sf);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_any_imm5_sf:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, uint8_t imm5, bool sf) {\n", op.name);
    printf("  return thumb32_opcode_two_regs_any_imm5_sf(%u, rd, rn, imm5, sf);\n", op.opcode);
    printf("}\n\n");
    break;
  case two_regs_any_imm5_shift_sf:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, uint8_t imm5, imm_shift_t shift, bool sf) {\n", op.name);
    printf("  return thumb32_opcode_two_regs_any_imm5_shift_sf(%u, rd, rn, imm5, shift, sf);\n", op.opcode);
    printf("}\n\n");
    break;
  case three_regs_any_sf:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm, bool sf) {\n", op.name);
    printf("  return thumb32_opcode_three_regs_any_sf(%u, rd, rn, rm, sf); \n", op.opcode);
    printf("}\n\n");
    break;  
  case three_regs_any_imm5_shift_sf:
    printf("thumb_opcode_t %s(reg_t rd, reg_t rn, reg_t rm, uint8_t imm5, imm_shift_t shift, bool sf) {\n", op.name);
    printf("  return thumb32_opcode_three_regs_any_imm5_shift_sf(%u, rd, rn, rm, imm5, shift, sf); \n", op.opcode);
    printf("}\n\n");
    break;
  default:
    printf("Error - unknown  (%s)", op.name);
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
