@17
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M+1
A=A-1
MD=M-D
@IFEQ3
D;JEQ
@SP
A=M-1
M=0
(IFEQ3)
@17
D=A
@SP
M=M+1
A=M-1
M=D
@16
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M+1
A=A-1
MD=M-D
@IFEQ6
D;JEQ
@SP
A=M-1
M=0
(IFEQ6)
@16
D=A
@SP
M=M+1
A=M-1
M=D
@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M+1
A=A-1
MD=M-D
@IFEQ9
D;JEQ
@SP
A=M-1
M=0
(IFEQ9)
@892
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNLT12
D;JGE
@SP
A=M-1
M=-1
(IFNLT12)
@891
D=A
@SP
M=M+1
A=M-1
M=D
@892
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNLT15
D;JGE
@SP
A=M-1
M=-1
(IFNLT15)
@891
D=A
@SP
M=M+1
A=M-1
M=D
@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNLT18
D;JGE
@SP
A=M-1
M=-1
(IFNLT18)
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNGT21
D;JLE
@SP
A=M-1
M=-1
(IFNGT21)
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32767
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNGT24
D;JLE
@SP
A=M-1
M=-1
(IFNGT24)
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
D=M-D
M=0
@IFNGT27
D;JLE
@SP
A=M-1
M=-1
(IFNGT27)
@57
D=A
@SP
M=M+1
A=M-1
M=D
@31
D=A
@SP
M=M+1
A=M-1
M=D
@53
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M+D
@112
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M-D
@SP
A=M-1
M=!M
@SP
AM=M-1
D=M
A=A-1
M=M&D
@82
D=A
@SP
M=M+1
A=M-1
M=D
@SP
AM=M-1
D=M
A=A-1
M=M|D
@SP
A=M-1
M=!M
