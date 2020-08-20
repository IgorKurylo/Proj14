; Write your own assembly language source program.
; The program should contain at least 10 insructions, 5 labels, 5 directives
; No errors: the assembler should generate successfully all output files:
;     myprog.ob, myprog.ext, myprog.ent


    .entry ARRAY
    .extern Y
START:       add    r3,LIST
LOOP:        prn     #32
lea    Y,r3
inc    r5
dec    r4
mov    r1,K
sub    r2,r7
bne    END
cmp    K,#10
bne    &FINAL
dec    W
    .entry START
jmp    &LOOP
add    L3,L3
FINAL:        stop

POLYDROM:       .string "abba"
ARRAY:      .data   11,-56
    .data   0,1,2,3,4,5
FIB:         .data   1,1,2,3,5,8,13,21
    .extern L3
