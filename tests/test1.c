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

#include <test_expect.h>

const char *testname = "test1";
const char *fn = "test1.bin";

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

  if (!test_expect_init(testname)) {
    printf("error starting initializing test_expect\n");
    return 0;
  }
  
  uint16_t instrs[24];
  instr_seq_t seq;
  seq.size = 24;
  seq.pos  = 0;
  seq.mc = instrs;
  
  emit_opcode(&seq, m3_adc_imm(r1, r2, 0xFE, false));
  emit_opcode(&seq, m3_adc_any(r0, r1, r2, 0x3, imm_shift_lsl, true));
  emit_opcode(&seq, m3_add_const(r4, r5, 0xBED, true));
  emit_opcode(&seq, m3_add_imm(r7, r8, 0x12)); 
  emit_opcode(&seq, m3_add_any(r0, r1, r2, 0x0, imm_shift_none, false)); 
  emit_opcode(&seq, m3_add_sp_imm(r0, r1, 0x0, imm_shift_none, true));
  emit_opcode(&seq, m3_add_pc_imm(r0, 10));
  emit_opcode(&seq, m3_sub_pc_imm(r0, 10));
  emit_opcode(&seq, m3_and_imm(r3, r4,0xF,true));
  emit_opcode(&seq, m3_and_any(r3, r4, r5, 0x0, imm_shift_none,true));
  emit_opcode(&seq, m3_asr_imm(r3, r4, 0x3, true));
  emit_opcode(&seq, m3_asr_any(r3, r4, r5, true));

  test_expect_shutdown();
  
  FILE *fp = fopen(fn, "w");
  if (!fp)  {
    printf("Error opening file %s\n", fn);
    return 0;
  }

  if (fwrite(seq.mc,sizeof(uint16_t),24,fp) < 24) {
    printf("Error writing file\n");
    return 0;
  }

  fclose(fp);
  return 1;
 
}
