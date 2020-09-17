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

#include <test_expect.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


FILE* out = NULL;


void output_startup(char *fn_bin) {
  fprintf(out, "#!/usr/bin/expect -f\n");
  fprintf(out, "set timeout 60\n");
  fprintf(out, "spawn telnet -e ~ localhost 4444\n");
  fprintf(out, "expect -re \"> ?$\"\n");
  fprintf(out, "send -- \"reset init\\n\"\n");
  fprintf(out, "expect -re \"> ?$\"\n");
  fprintf(out, "send -- \"reg pc 0x20000000\\n\"\n");
  fprintf(out,
	  "expect {\n"
	  "    \"pc*0x20000000*\\n\" {\n"
	  "        puts \"PC set successfully\\n\"\n"
	  "    }\n"
	  "    \"pc*0x*\\n\" {\n"
	  "        puts \"Error setting pc\\n\"\n"
	  "        exit 1\n"
	  "    }\n"
	  "}\n");
  fprintf(out, "expect -re \"> ?$\"\n");
  fprintf(out, "send -- \"load_image %s 0x20000000 bin\\n\"\n", fn_bin);
  fprintf(out, "expect -re \"> ?$\"\n");
}

void test_step(void) {
  fprintf(out, "send -- \"step\\n\"\n");
  fprintf(out, "expect -re \"> ?$\"\n");
}
  
void test_assert_reg(char *reg, uint32_t value) {
  fprintf(out, "send -- \"reg %s\\n\"\n", reg);
  fprintf(out,
	  "expect {\n"
	  "    \"%s*0x%08x*\\n\" {\n"
	  "        puts \"%s set successfully\\n\"\n"
	  "    }\n"
	  "    \"%s*0x*\\n\" {\n"
	  "        puts \"Error setting %s\\n\"\n"
	  "        exit 1\n"
	  "    }\n"
	  "}\n", reg, value, reg, reg, reg);
  fprintf(out, "expect -re \"> ?$\"\n");
}


int test_expect_init(const char *testname) {
  size_t n = 0;

  const char *exp_ending = ".expect";
  const char *bin_ending = ".bin";
  
  n = strlen(testname);
  char fn_exp[256];
  char fn_bin[256];
  
  strncpy(fn_exp, testname, 256);
  strncpy(fn_bin, testname, 256); 

  if (n + 8 < 256) {
    strncat(fn_exp, exp_ending, 8);
    strncat(fn_bin, bin_ending, 4);
  } else {
    return 0;
  }
   
  out = fopen(fn_exp, "w");
  if (!out) return 0;

  output_startup(fn_bin);
  
  return 1;
}

void test_expect_shutdown(void) {

  fprintf(out, "send -- \"~\" \n");
  fprintf(out, "expect \"telnet>\"\n");
  fprintf(out, "send -- \"close\\n\"\n");
  fprintf(out, "expect eof\n");
  fprintf(out, "wait\n");

  if(out)
    fclose(out);
}

