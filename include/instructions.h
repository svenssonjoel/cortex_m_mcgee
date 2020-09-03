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

#ifndef __INSTRUCTIONS_H_
#define __INSTRUCTIONS_H_

typedef enum {
  M0_ADC,
  M0_ADD,
  M0_ADR,
  M0_AND,
  M0_ASR,
  M0_B,
  M0_BIC,
  M0_BKPT,
  M0_BL,
  M0_BLX,
  M0_BX,
  M0_CMN,
  M0_CMP,
  M0_CPS,
  M0_DMB,
  M0_DSB,
  M0_EOR,
  M0_ISB,
  M0_LDM,
  M0_LDR,
  M0_LDRB,
  M0_LDRH,
  M0_LDRSB,
  M0_LDRSH,
  M0_LSB,
  M0_LSL,
  M0_MOV,
  M0_MRS,
  M0_MSR,
  M0_MUL,
  M0_MVN,
  M0_NOP,
  M0_ORR,
  M0_POP,
  M0_PUSH,
  M0_REV,
  M0_REV16,
  M0_REVSH,
  M0_ROR,
  M0_RSB,
  M0_SBC,
  M0_SEV,
  M0_STM,
  M0_STR,
  M0_STRB,
  M0_STRH,
  M0_SUB,
  M0_SVC,
  M0_SXTB,
  M0_SXTH,
  M0_TST,
  M0_UXTB,
  M0_UXTH,
  M0_WFE,
  M0_WFI,
  M0_YIELD
} M0_instr_t;

#endif
