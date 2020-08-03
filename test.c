//
// Created by Igork on 03/08/2020.
//

typedef unsigned char byte;

void toBinary(int opcode, byte src,int* buffer)
{
    *buffer |= opcode << 18;
    int temp = src;
    *buffer |= temp << 16;

}
