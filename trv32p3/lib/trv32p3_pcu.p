/*
-- File : trv32p3_pcu.p
--
-- Contents : Definition of the trv32p3 controller in PDG.
--
-- Copyright (c) 2019-2020 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/

#include "trv32p3_define.h"

//#define PRINT_PCU
#ifdef        PRINT_PCU
#define DEBUG_PRINT_PCU(FMT,ARGS...)  DEBUG_PRINT_NAME(); DEBUG_PRINTF(".PCU cyc="); DEBUG_PRINT_CYCLES(); DEBUG_PRINT(" : "FMT, ##ARGS)
#else
#define DEBUG_PRINT_PCU(FMT,ARGS...)
#endif


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Storages
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

pcu_storages {
  reg r_booting         <uint1_t>;      // first cycle after reset

  // user_issue() to user_next_pc()
  trn t_issue_ins       <uint1_t>;
  trn t_issue_len       <uint3_t>;
}

hw_init r_booting = 1;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Issue logic
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void trv32p3::user_issue()
{
  DEBUG_PRINT_PCU("--------------------------------------------------------");

  pcr = PC;
  lnk_id = pcr;
  lnk_ex = pcr;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Kill speculatively issued & fetched instructions (@ID @IF)
  bool br_EX = jump_of13_cd_sig; // only set if branch is taken
  bool flush_IF = br_EX;
  bool flush_ID = br_EX;
  if (flush_ID)
    kill_instr();

  // NOTE: Signals like hw_stall_sig() originating from a killed instruction
  // that is currently in the decode stage are not removed, they must be
  // ignored.

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Determine Issue mode

  bool could_issue = issue_sig() && !(hw_stall_sig() && !flush_ID);
  // issue_sig(): cycles(N) -> false for N-1 cycles
  // hw_stall_sig(): false if valid instr@ID stalled due to hw_stall rule
  //   -> here: instr@ID is not advancing, hence don't issue
  // flush_ID: ignore stall if it comes from an instr. killed by a taken branch

  v4u8 ii = pm_rd;

  // instruction data is available in all but first cycle
  bool instr_avail = !r_booting;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Issue instruction

  bool issue_ins = instr_avail && !flush_IF && could_issue;
  // instr_avail: fetched instruction is available
  // flush_IF: discard speculatively fetched instruction, if any

  if (issue_ins) {
    issue_instr(pcr,1,ii[0],ii[1],ii[2],ii[3]);

    // save addr of instr that we just issued
    pif_PC = tif_PC = pcr;

    DEBUG_PRINT_PCU("II: ii=%x pcr=%d nw=%d ",ii,pcr,4);
  } else {
    DEBUG_PRINT_PCU("II stall: ii %d bf %d hw %d br %d",!issue_sig(),!instr_avail,
                    hw_stall_sig() && !flush_ID,br_EX);
  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // to user_next_pc()
  t_issue_ins   = issue_ins;
  t_issue_len   = 4;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// ~~~ Next PC logic
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void trv32p3::user_next_pc()
{
  addr PC_ID = trPC_ID = pif_PC;
  addr PC_EX = trPC_EX = pid_PC;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Process jumps

  bool br_EX   = jump_of13_cd_sig;
  bool jalr_EX = jump_trgt_sig;
  bool jal_ID  = jump_of21_sig     && !hw_stall_sig() && !br_EX;
  // hw_stall_sig(): ignore ctrl instr @ ID if stage ID is stalled
  // br_EX: ignore jal @ ID if killed by cjump @ EX

  bool jump = br_EX || jal_ID || jalr_EX || r_booting;
  PDG_ASSERT( ((uint8_t)br_EX + jal_ID + jalr_EX) <= 1 );

  // determine jump target
  addr jump_pc;
  if      (br_EX)   jump_pc = PC_EX + of13_cd; // relative to instr
  else if (jal_ID)  jump_pc = PC_ID + of21;    // relative to instr
  else if (jalr_EX) jump_pc = trgt[31:1]::"0"; // force LSB to zero, accord. to RISC-V ISA spec.
  else              jump_pc = pcr;             // r_booting

  DEBUG_PRINT_PCU("jump %d%d%d%d",br_EX,jal_ID,jalr_EX,r_booting);


  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Update PC

  bool    advance = t_issue_ins;

  uint3_t pc_step = advance ? t_issue_len : 0;

  addr    next_pc = (jump) ? jump_pc : (addr)(pcr + pc_step);

  if (jump || advance)
    PC = pcw = next_pc;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Fetch

  DEBUG_PRINT_PCU("fetch @ %d",next_pc);
  pm_rd`1` = PM[pm_addr = next_pc]`1`;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // leave boot cycle after first fetch

  if (r_booting)
    r_booting = 0;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  DEBUG_PRINT_PCU("NPC: sigs=%d%d%d%d, pcr=%d, next_pc=%d",
                  hw_stall_sig(),jump_of13_cd_sig,jump_of21_sig,jump_trgt_sig,
                  pcr,next_pc);
}
