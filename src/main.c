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
#include <stdlib.h>
#include <unistd.h>

#include <thumb.h>

const char *fn = "out.bin";

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  uint16_t instrs[13];
  instr_seq_t seq;
  seq.size = 10;
  seq.pos  = 0;
  seq.mc = instrs;
  
  emit_opcode(&seq, m0_mov_imm(r0, 2));      
  emit_opcode(&seq, m0_mov_imm(r1, 3));       
  emit_opcode(&seq, m0_add_low(r2, r1, r0)); 
  emit_opcode(&seq, m0_add_any(r0, r2)); 
  emit_opcode(&seq, m0_asr_imm(r0, r0, 1)); 
  emit_opcode(&seq, m0_mov_imm(r2, 0xAD));
  emit_opcode(&seq, m0_mov_imm(r3, 0x0F));
  emit_opcode(&seq, m0_mov_imm(r4, 0xF0));
  emit_opcode(&seq, m0_mov_imm(r5, 0xAB));
  emit_opcode(&seq, m0_mov_imm(r6, 0xDC));
  emit_opcode(&seq, m0_mov_imm(r7, 0x12));
  emit_opcode(&seq, m0_mov_imm(r8, 0x43));
  emit_opcode(&seq, m0_mov_imm(r9, 0x99));

  FILE *fp = fopen(fn, "w");
  if (!fp)  {
    printf("Error opening file %s\n", fn);
    return 0;
  }

  if (fwrite(seq.mc,sizeof(uint16_t),13,fp) < 13) {
    printf("Error writing file\n");
    return 0;
  }

  fclose(fp);
  return 1;
 
}
