; Write your own assembly language source program.
; The program should contain at least 10 insructions, 5 labels, 5 directives
; No errors: the assembler should generate successfully all output files:
;     myprog.ob, myprog.ext, myprog.ent


    .entry LIST
    .extern W
MAIN:       add    r3,LIST
LOOP:       prn    #48
lea    W,r6
inc    r6
mov    r3,K
sub    r1,r4
bne    END
cmp    K,#-6
bne    &END
dec    W
    .entry MAIN
jmp    &LOOP
add    L3,L3
END:        stop

STR:       .string "abcd"
LIST:      .data   6,-9
    .data   -100
K:         .data   31
    .extern L3
