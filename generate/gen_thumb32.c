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
  two_regs_any_imm12_sf
} thumb32_opcode_format;


typedef struct {
  char *name;
  uint32_t opcode;
  thumb32_opcode_format format;
} thumb32_opcode;


const thumb32_opcode opcodes[] =
  {{"m3_adc_imm"    ,0b11110001010000000000000000000000, two_regs_any_imm12_sf},
   {NULL, 0, 0}};


void print_extern_decl(thumb32_opcode op) {
  switch(op.format) {
  case nothing:
    printf("extern thumb_opcode_t %s(void);\n", op.name);
    break;
  default:
    printf("Error - unknown\n");
    break;
  }
}

void print_opcode(thumb32_opcode op) {

  switch(op.format) {
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
