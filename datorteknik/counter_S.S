
; .data
counter:	.quad 0
GPIO:		.quad 0
.text
.global setup
.global interrupt




interrupt:
MOVQ counter, %rax
MOVQ $4, %r10
INCQ counter
binary:
MOVQ $0, %rdx
MOVQ $2, %rdi
DIV %rdi
CMPQ $1, %rdx
MOVQ %r10, %r15
MOVQ %r15, %RCX
MOVQ %rax, %r11
JE LEDon
CMPQ $0, %rdx
MOVQ %r10, %r15
MOVQ %r15, %RCX
MOVQ %rax, %r11
JE LEDoff
stop:
MOVQ %r11, %rax
CMPQ $0, %r11
JNE binary

ret

LEDon:

CALL basminnesadress
MOVQ GPIO, %RDI
ADDQ $3, %RAX
MOVQ %RAX, %RDI
SUBQ $1, %RCX
IMULQ $8, %RCX
MOVQ $49, %RBX
SAL %CL, %RBX
ORQ %RBX, (%RDI)
DECQ %r10
JMP stop


LEDoff:
CALL basminnesadress
ADDQ $3, %RAX
MOVQ %RAX, %R8
SUBQ $1, %RCX
IMULQ $8, %RCX
MOVQ $0xCF, %RBX
SHL %CL, %RBX
NOTQ %RBX
ANDQ %RBX, (%R8)
DECQ %r10
JMP stop


setup:
CALL basminnesadress
MOVQ $0, (GPIO)
CALL input
MOVQ $1, (GPIO)
CALL output
MOVQ $2, (GPIO)
CALL output
MOVQ $3, (GPIO)
CALL output
MOVQ $4, (GPIO)
CALL output
MOVQ $1, counter

ret


basminnesadress:
PUSHQ %RCX
MOVQ $0x60, %RSI
SHLQ $16, %RSI
MOVQ $0xFFFF, %RAX
MOVQ $0x1000, %RCX
ANDQ %RCX, %RAX
MOVQ $0x9E, %RBX
NOTQ %RBX
MOVQ $0xFF, %RDX
ANDQ %RDX, %RBX
ORQ %RAX, %RSI
ORQ %RBX, %RSI
MOV %RSI, %RAX
POPQ %RCX

ret


input:
MOVQ $3, %RDX
SUBQ (GPIO), %RDX
IMULQ $2, %RDX

MOVQ %RDX, %RAX
MOVQ $8, %RBX
MOVQ $0, %RDX
IDIVQ %RBX

ADDQ $8, %RDX
MOVQ %RDX, %RCX
MOVQ $0x3, %RDI
SHL %CL, %RDI

MOVQ (GPIO), %RAX
MOVQ $8, %RBX
MOVQ $0, %RDX
IDIV %RBX

ADDQ %RSI, %RAX

XORQ %RDI, (%RAX)

ret


output:
MOVQ $4, %RCX
SUBQ (GPIO), %RCX
IMULQ $2, %RCX

MOVQ $0x2, %RAX
SHL $6, %RAX

SHL %CL, %RAX

MOVQ %RAX, %RDX
MOVQ %RSI, %RAX

ORQ %RDX, (%RAX)

ret
