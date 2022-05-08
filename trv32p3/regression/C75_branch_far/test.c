/*
-- Copyright (c) 2019 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/


[[gnu::noinline]]
int test(int a) [[chess::disable_instruction_class(branch)]] // force far branch
{
    int chess_storage(x15) b = a;
    if( a < 16 ) {
        for(int i = 0; i < 128; ++i) chess_flatten_loop {
            b = 1 + chess_copy(b);
        }
    }
    chess_report(b);
    return b;
}

int main()
{
    test(10);
    test(20);
    chess_exit(0);
    return 0;
}
