.thumb
.syntax unified

.section ".text.1700", "ax"
ReadFlash1700:	@ RdCode[1][0]
	LDR	R5, =0x400FC040	@ Select Flash memory (no remap)
	MOVS	R0, #1
	STR	R0, [R5]
	MOVS	R3, #0		@ Start address
	LDR	R5, =0x4000C000	@ UART0 base address
0:
	LDR	R1, [R5, #20]	@ Read LSR
	MOVS	R6, #0x01	@ Test RDR bit
	TST	R1, R6
	BEQ	0b		@ No data received?
	LDR	R0, [R5, #0]	@ Read RBR
	MOVS	R6, #255
	ANDS	R0, R6
	CMP	R0, #0xAA	@ Trigger byte?
	BNE	0b

	LDR	R2, =1024	@ Number of bytes to send
	MOVS	R4, #0		@ Sum
1:
	LDRB	R0, [R3]	@ Get a byte
	ADDS	R3, #1		@ Next address
	ADDS	R4, R0		@ Add sum
	BL	2f		@ Send byte
	SUBS	R2, #1		@ Repeat for the block
	BNE	1b

	MOVS	R0, R4		@ Send sum
	BL	2f
	MOVS	R4, #8
	LSRS	R0, R4
	BL	2f

	B	0b		@ Next block

2:	@ Send a byte 
	LDR	R1, [R5, #20]	@ Read LSR
	MOVS	R6, #0x20	@ Test THRE bit
	TST	R1, R6
	BEQ	2b
	STR	R0, [R5, #0]	@ Write to THR
	BX	LR
.pool


.section ".text.1100", "ax"
ReadFlash1100:	@ RdCode[2][0]
	LDR	R5, =0x40048000	@ Select Flash memory (no remap)
	MOVS	R0, #2
	STR	R0, [R5]
	LDR	R5, =0x40008000	@ UART0 base address
	MOVS	R3, #0		@ Start address
0:
	LDR	R1, [R5, #20]	@ Read LSR
	MOVS	R6, #0x01	@ Test RDR bit
	TST	R1, R6	
	BEQ	0b
	LDR	R0, [R5, #0]	@ Read RBR
	MOVS	R6, #255
	ANDS	R0, R6
	CMP	R0, #0xAA
	BNE	0b

	LDR	R2, =1024	@ Number of bytes to send
	MOVS	R4, #0		@ Sum
1:
	LDRB	R0, [R3]	@ Get a byte
	ADDS	R3, #1		@ Next address
	ADDS	R4, R0		@ Add sum
	BL	2f		@ Send byte
	SUBS	R2, #1		@ Repeat for the block
	BNE	1b

	MOVS	R0, R4		@ Send sum
	BL	2f
	MOVS	R4, #8
	LSRS	R0, R4
	BL	2f

	B	0b		@ Next block

2:	@ Send a byte 
	LDR	R1, [R5, #20]	@ Read LSR
	MOVS	R6, #0x20	@ Test THRE bit
	TST	R1, R6
	BEQ	2b
	STR	R0, [R5, #0]	@ Write to THR
	BX	LR
.pool


.section ".text.800", "ax"
ReadFlash800:	@ RdCode[3][0]
	LDR	R5, =0x40048000	@ Select Flash memory (no remap)
	MOVS	R0, #2
	STR	R0, [R5]
	LDR	R5, =0x40064000	@ USART0 base address
	MOVS	R3, #0		@ Start address
0:
	LDR	R1, [R5, #8]	@ Read STAT
	MOVS	R6, #0x01	@ Test RXRDY bit
	TST	R1, R6
	BEQ	0b
	LDR	R0, [R5, #20]	@ Read RXDAT
	MOVS	R6, #255
	ANDS	R0, R6
	CMP	R0, #0xAA
	BNE	0b

	LDR	R2, =1024	@ Number of bytes to send
	MOVS	R4, #0		@ Sum
1:
	LDRB	R0, [R3]	@ Get a byte
	ADDS	R3, #1		@ Next address
	ADDS	R4, R0		@ Add sum
	BL	2f		@ Send byte
	SUBS	R2, #1		@ Repeat for the block
	BNE	1b

	MOVS	R0, R4		@ Send sum
	BL	2f
	MOVS	R4, #8
	LSRS	R0, R4
	BL	2f

	B	0b		@ Next block

2:	@ Send a byte 
	LDR	R1, [R5, #8]	@ Read STAT
	MOVS	R6, #0x04
	TST	R1, R6		@ Test TXRDY bit
	BEQ	2b
	STR	R0, [R5, #28]	@ Write to TXDAT
	BX	LR
.pool


.section ".text.1500", "ax"
ReadFlash1500:	@ RdCode[4][0]
	LDR	R5, =0x40074000	@ Select Flash memory (no remap)
	MOVS	R0, #2
	STR	R0, [R5]
	LDR	R5, =0x40040000	@ USART0 base address
	MOVS	R3, #0		@ Start address
0:
	LDR	R1, [R5, #8]	@ Read STAT
	MOVS	R6, #0x01	@ Test RXRDY bit
	TST	R1, R6
	BEQ	0b
	LDR	R0, [R5, #20]	@ Read RXDAT
	MOVS	R6, #255
	ANDS	R0, R6
	CMP	R0, #0xAA
	BNE	0b

	LDR	R2, =1024	@ Number of bytes to send
	MOVS	R4, #0		@ Sum
1:
	LDRB	R0, [R3]	@ Get a byte
	ADDS	R3, #1		@ Next address
	ADDS	R4, R0		@ Add sum
	BL	2f		@ Send byte
	SUBS	R2, #1		@ Repeat for the block
	BNE	1b

	MOVS	R0, R4		@ Send sum
	BL	2f
	MOVS	R4, #8
	LSRS	R0, R4
	BL	2f

	B	0b		@ Next block

2:	@ Send a byte 
	LDR	R1, [R5, #8]	@ Read STAT
	MOVS	R6, #0x04
	TST	R1, R6		@ Test TXRDY bit
	BEQ	2b
	STR	R0, [R5, #28]	@ Write to TXDAT
	BX	LR
.pool


.end
