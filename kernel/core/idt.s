/* GPLv2 (c) Airbus */
.text

.globl resume_from_intr
.type  resume_from_intr,"function"

.globl idt_trampoline
.type  idt_trampoline,"function"

/*
** send end-of-interrupt to PIC
*/
ack_pic2:
	push	%eax
	movb	$0x20, %al
	outb	%al, $0xa0
	jmp	eoi_pic1
ack_pic1:
	push	%eax
	movb	$0x20, %al
eoi_pic1:
	outb	%al, $0x20
	pop	%eax

/*
** ring0 int stack layout
**
** SS       | if pvl level change
** ESP      | if pvl level change
** EFLAGS
** CS
** EIP
** ERR CODE
** INT NUMBER
** EAX
** ...        
** EDI
*/
idt_common:
        pusha
        mov     %esp, %eax
        call    intr_hdlr

resume_from_intr:
        popa
        add     $8, %esp
        iret

/*
** IDT handlers
*/
.section        .idt_jmp, "ax", @progbits

idt_trampoline:
/* divide error (no) */
        .align  16
        pushl   $-1
        pushl   $0
        jmp     idt_common

/* debug (no) */
        .align  16
        pushl   $-1
        pushl   $1
        jmp     idt_common

/* nmi (no ) */
        .align  16
        pushl   $-1
        pushl   $2
        jmp     idt_common

/* breakpoint (no) */
        .align  16
        pushl   $-1
        pushl   $3
        jmp     idt_common

/* overflow (no) */
        .align  16
        pushl   $-1
        pushl   $4
        jmp     idt_common

/* bound (no) */
        .align  16
        pushl   $-1
        pushl   $5
        jmp     idt_common

/* invalid opcode (no) */
        .align  16
        pushl   $-1
        pushl   $6
        jmp     idt_common

/* device not available (no) */
        .align  16
        pushl   $-1
        pushl   $7
        jmp     idt_common

/* double fault (yes) */
        .align  16
        pushl   $8
        jmp     idt_common

/* copro segment (no) */
        .align  16
        pushl   $-1
        pushl   $9
        jmp     idt_common

/* TSS invalid (yes) */
        .align  16
        pushl   $10
        jmp     idt_common

/* Segment not present (yes) */
        .align  16
        pushl   $11
        jmp     idt_common

/* stack segment fault (yes) */
        .align  16
        pushl   $12
        jmp     idt_common

/* general protection (yes) */
        .align  16
        pushl   $13
        jmp     idt_common

/* page fault (yes) */
        .align  16
        pushl   $14
        jmp     idt_common

/* intel reserved */
        .align  16
        pushl   $-1
        pushl   $15
        jmp     idt_common

/* fpu (no ) */
        .align  16
        pushl   $-1
        pushl   $16
        jmp     idt_common

/* alignment (yes) */
        .align  16
        pushl   $17
        jmp     idt_common

/* machine check (no) */
        .align  16
        pushl   $-1
        pushl   $18
        jmp     idt_common

/* simd (no) */
        .align  16
        pushl   $-1
        pushl   $19
        jmp     idt_common

/* intel reserved 20-31 */
        .rep	12
	.align	16
	iret
	.endr

/* pic 1 irq 32-39 */
	.irp    nr,32,33,34,35,36,37,38,39
	.align	16
	pushl	$-1
 	push	$\nr
	jmp	ack_pic1
	.endr

/* pic 2 irq 40-47 */
	.irp    nr,40,41,42,43,44,45,46,47
	.align	16
	pushl   $-1
	push	$\nr
	jmp	ack_pic2
	.endr

/* available 48-255 */
        .irp    nr, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
        .align  16
        pushl   $-1
        pushl   $\nr
        jmp     idt_common
        .endr
