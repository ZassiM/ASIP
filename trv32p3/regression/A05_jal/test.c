/*
-- Copyright (c) 2014 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/

#ifdef __ndl__
inline assembly void test_jal() clobbers(x1)
{
    asm_begin
    asm_text
        ori x4, x0, 100
        ori x5, x0, 1
        ori x6, x0, 2
        ori x7, x0, 3
        ori x8, x0, 4
        add x4, x4, x5   // 101
.call
        jal x1, Fa
.return_address
        add x4, x10, x6
        add x10, x4, x7
.chess_report __sint
        nop
        jal x0, Da
        nop
        nop
Fa:     add x10, x4, x8   // 105
.chess_report __sint
        nop
.rts
        jalr x0, x1, 0
        add x4, x12, x5
        nop
Da:
        lui x1, 0    // clear LR
        nop
    asm_end
}
#else
inline void test_jal()
{
    chess_message(" // test_jal");
    chess_report (105);
    chess_report (110);
}
#endif

#ifdef __ndl__
inline assembly void test_jalr() clobbers(x1)
{
    asm_begin
      .direct_eval >>,&,(,),+
    asm_text
        ori x4, x0, 200
        lui x13, ((La+2048)>>12)
        ori x13, x0, (La&0xfff)
        nop
        ori x5, x0, 1
        ori x6, x0, 2
        ori x7, x0, 3
        ori x8, x0, 4
        ori x9, x0, 5
        ori x11, x0, 6
        add x4, x4, x5
.call
        jalr x15, x13, 0
.return_address
        add x4, x10, x6
        add x4, x4, x7
        add x10, x4, x8
.chess_report __sint
        nop
        jal x0, Db
La:     add x4, x4, x9
        add x10, x4, x11
.chess_report __sint
        nop
.rts
        jalr x0, x15, 0
        nop
        nop
Db:
        nop
        lui x1, 0    // clear LR
    asm_end
}
#else
inline void test_jalr()
{
    chess_message(" // test_jalr");
    chess_report (212);
    chess_report (221);
}
#endif

int main()
{
    test_jal();
    test_jalr();
    return 0;
}
