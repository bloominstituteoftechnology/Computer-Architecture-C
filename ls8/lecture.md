10000010 # LDI R0,8     r[0] = 8                // store value
00000000 # 0 in decimal
00001000 # 8 in decimal
01000111 # PRN R0        printf("%d\n", r[0]);   // print
00000000 # 0 in decimal
00000001 # HLT           exit();                  // exit

10100000 # ADD
10100010 # MUL

<!-- multiply -->
10000010 # LDI R0,8      r[0] = 8;
00000000
00001000 
10000010 # LDI R1,9      r[1] = 9;
00000001
00001001 
10100010 # MUL R0,R1     r[0] = r[0] * r[1];
00000000
00000001
01000111 # PRN R0        printf("%d\n", r[0]);
00000000
00000001 # HLT           exit();