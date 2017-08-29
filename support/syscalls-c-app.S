.syntax unified
.cpu cortex-m0plus
.fpu softvfp
.eabi_attribute 20, 1
.eabi_attribute 21, 1
.eabi_attribute 23, 3
.eabi_attribute 24, 1
.eabi_attribute 25, 1
.eabi_attribute 26, 1
.eabi_attribute 30, 4
.eabi_attribute 34, 0
.eabi_attribute 18, 4
.thumb
.syntax unified
.file   "syscalls-app-c.S"
.text

.cfi_sections   .debug_frame
.section	.text.__aeabi_atexit,"ax",%progbits
.align	1
.global	__aeabi_atexit
.code	16
.thumb_func
.type	__aeabi_atexit, %function
__aeabi_atexit:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_atexit, .-__aeabi_atexit
.section	.text.__cxa_atexit,"ax",%progbits
.align	1
.global	__cxa_atexit
.code	16
.thumb_func
.type	__cxa_atexit, %function
__cxa_atexit:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__cxa_atexit, .-__cxa_atexit
.section	.text.atexit,"ax",%progbits
.align	1
.global	atexit
.code	16
.thumb_func
.type	atexit, %function
atexit:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	atexit, .-atexit
.section	.text._atexit,"ax",%progbits
.align	1
.global	_atexit
.code	16
.thumb_func
.type	_atexit, %function
_atexit:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	_atexit, .-_atexit
.section	.text.__cxa_finalize,"ax",%progbits
.align	1
.global	__cxa_finalize
.code	16
.thumb_func
.type	__cxa_finalize, %function
__cxa_finalize:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__cxa_finalize, .-__cxa_finalize
.section	.text.__aeabi_d2f,"ax",%progbits
.align	1
.global	__aeabi_d2f
.code	16
.thumb_func
.type	__aeabi_d2f, %function
__aeabi_d2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2f, .-__aeabi_d2f
.section	.text.__aeabi_f2d,"ax",%progbits
.align	1
.global	__aeabi_f2d
.code	16
.thumb_func
.type	__aeabi_f2d, %function
__aeabi_f2d:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2d, .-__aeabi_f2d
.section	.text.__aeabi_h2f,"ax",%progbits
.align	1
.global	__aeabi_h2f
.code	16
.thumb_func
.type	__aeabi_h2f, %function
__aeabi_h2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_h2f, .-__aeabi_h2f
.section	.text.__aeabi_h2f_alt,"ax",%progbits
.align	1
.global	__aeabi_h2f_alt
.code	16
.thumb_func
.type	__aeabi_h2f_alt, %function
__aeabi_h2f_alt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_h2f_alt, .-__aeabi_h2f_alt
.section	.text.__aeabi_f2h,"ax",%progbits
.align	1
.global	__aeabi_f2h
.code	16
.thumb_func
.type	__aeabi_f2h, %function
__aeabi_f2h:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2h, .-__aeabi_f2h
.section	.text.__aeabi_f2h_alt,"ax",%progbits
.align	1
.global	__aeabi_f2h_alt
.code	16
.thumb_func
.type	__aeabi_f2h_alt, %function
__aeabi_f2h_alt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2h_alt, .-__aeabi_f2h_alt
.section	.text.__aeabi_d2h,"ax",%progbits
.align	1
.global	__aeabi_d2h
.code	16
.thumb_func
.type	__aeabi_d2h, %function
__aeabi_d2h:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2h, .-__aeabi_d2h
.section	.text.__aeabi_d2h_alt,"ax",%progbits
.align	1
.global	__aeabi_d2h_alt
.code	16
.thumb_func
.type	__aeabi_d2h_alt, %function
__aeabi_d2h_alt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2h_alt, .-__aeabi_d2h_alt
.section	.text.do_nothing,"ax",%progbits
.align	1
.global	do_nothing
.code	16
.thumb_func
.type	do_nothing, %function
do_nothing:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #0
.thumb
.syntax unified
.cfi_endproc
.size	do_nothing, .-do_nothing
.section	.text.getSyscallABI,"ax",%progbits
.align	1
.global	getSyscallABI
.code	16
.thumb_func
.type	getSyscallABI, %function
getSyscallABI:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #1
.thumb
.syntax unified
.cfi_endproc
.size	getSyscallABI, .-getSyscallABI
.section	.text.getSyscallAddr,"ax",%progbits
.align	1
.global	getSyscallAddr
.code	16
.thumb_func
.type	getSyscallAddr, %function
getSyscallAddr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #2
.thumb
.syntax unified
.cfi_endproc
.size	getSyscallAddr, .-getSyscallAddr
.section	.text.memcpy,"ax",%progbits
.align	1
.global	memcpy
.code	16
.thumb_func
.type	memcpy, %function
memcpy:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #3
.thumb
.syntax unified
.cfi_endproc
.size	memcpy, .-memcpy
.section	.text.__aeabi_memcpy8,"ax",%progbits
.align	1
.global	__aeabi_memcpy8
.code	16
.thumb_func
.type	__aeabi_memcpy8, %function
__aeabi_memcpy8:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #3
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memcpy8, .-__aeabi_memcpy8
.section	.text.__aeabi_memcpy4,"ax",%progbits
.align	1
.global	__aeabi_memcpy4
.code	16
.thumb_func
.type	__aeabi_memcpy4, %function
__aeabi_memcpy4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #3
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memcpy4, .-__aeabi_memcpy4
.section	.text.__aeabi_memcpy,"ax",%progbits
.align	1
.global	__aeabi_memcpy
.code	16
.thumb_func
.type	__aeabi_memcpy, %function
__aeabi_memcpy:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #3
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memcpy, .-__aeabi_memcpy
.section	.text.memmove,"ax",%progbits
.align	1
.global	memmove
.code	16
.thumb_func
.type	memmove, %function
memmove:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #4
.thumb
.syntax unified
.cfi_endproc
.size	memmove, .-memmove
.section	.text.__aeabi_memmove8,"ax",%progbits
.align	1
.global	__aeabi_memmove8
.code	16
.thumb_func
.type	__aeabi_memmove8, %function
__aeabi_memmove8:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #4
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memmove8, .-__aeabi_memmove8
.section	.text.__aeabi_memmove4,"ax",%progbits
.align	1
.global	__aeabi_memmove4
.code	16
.thumb_func
.type	__aeabi_memmove4, %function
__aeabi_memmove4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #4
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memmove4, .-__aeabi_memmove4
.section	.text.__aeabi_memmove,"ax",%progbits
.align	1
.global	__aeabi_memmove
.code	16
.thumb_func
.type	__aeabi_memmove, %function
__aeabi_memmove:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #4
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memmove, .-__aeabi_memmove
.section	.text.memset,"ax",%progbits
.align	1
.global	memset
.code	16
.thumb_func
.type	memset, %function
memset:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #5
.thumb
.syntax unified
.cfi_endproc
.size	memset, .-memset
.section	.text.__aeabi_memset8,"ax",%progbits
.align	1
.global	__aeabi_memset8
.code	16
.thumb_func
.type	__aeabi_memset8, %function
__aeabi_memset8:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #5
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memset8, .-__aeabi_memset8
.section	.text.__aeabi_memset4,"ax",%progbits
.align	1
.global	__aeabi_memset4
.code	16
.thumb_func
.type	__aeabi_memset4, %function
__aeabi_memset4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #5
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memset4, .-__aeabi_memset4
.section	.text.__aeabi_memset,"ax",%progbits
.align	1
.global	__aeabi_memset
.code	16
.thumb_func
.type	__aeabi_memset, %function
__aeabi_memset:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #5
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memset, .-__aeabi_memset
.section	.text.memclr,"ax",%progbits
.align	1
.global	memclr
.code	16
.thumb_func
.type	memclr, %function
memclr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #6
.thumb
.syntax unified
.cfi_endproc
.size	memclr, .-memclr
.section	.text.__aeabi_memclr8,"ax",%progbits
.align	1
.global	__aeabi_memclr8
.code	16
.thumb_func
.type	__aeabi_memclr8, %function
__aeabi_memclr8:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #6
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memclr8, .-__aeabi_memclr8
.section	.text.__aeabi_memclr4,"ax",%progbits
.align	1
.global	__aeabi_memclr4
.code	16
.thumb_func
.type	__aeabi_memclr4, %function
__aeabi_memclr4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #6
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memclr4, .-__aeabi_memclr4
.section	.text.__aeabi_memclr,"ax",%progbits
.align	1
.global	__aeabi_memclr
.code	16
.thumb_func
.type	__aeabi_memclr, %function
__aeabi_memclr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #6
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_memclr, .-__aeabi_memclr
.section	.text.__aeabi_uread4,"ax",%progbits
.align	1
.global	__aeabi_uread4
.code	16
.thumb_func
.type	__aeabi_uread4, %function
__aeabi_uread4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #7
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uread4, .-__aeabi_uread4
.section	.text.__aeabi_uwrite4,"ax",%progbits
.align	1
.global	__aeabi_uwrite4
.code	16
.thumb_func
.type	__aeabi_uwrite4, %function
__aeabi_uwrite4:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #8
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uwrite4, .-__aeabi_uwrite4
.section	.text.__aeabi_uread8,"ax",%progbits
.align	1
.global	__aeabi_uread8
.code	16
.thumb_func
.type	__aeabi_uread8, %function
__aeabi_uread8:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #9
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uread8, .-__aeabi_uread8
.section	.text.__aeabi_idiv,"ax",%progbits
.align	1
.global	__aeabi_idiv
.code	16
.thumb_func
.type	__aeabi_idiv, %function
__aeabi_idiv:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #10
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_idiv, .-__aeabi_idiv
.section	.text.__aeabi_uidiv,"ax",%progbits
.align	1
.global	__aeabi_uidiv
.code	16
.thumb_func
.type	__aeabi_uidiv, %function
__aeabi_uidiv:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #11
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uidiv, .-__aeabi_uidiv
.section	.text.__aeabi_idivmod,"ax",%progbits
.align	1
.global	__aeabi_idivmod
.code	16
.thumb_func
.type	__aeabi_idivmod, %function
__aeabi_idivmod:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #12
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_idivmod, .-__aeabi_idivmod
.section	.text.__aeabi_uidivmod,"ax",%progbits
.align	1
.global	__aeabi_uidivmod
.code	16
.thumb_func
.type	__aeabi_uidivmod, %function
__aeabi_uidivmod:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #13
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uidivmod, .-__aeabi_uidivmod
.section	.text.__aeabi_lmul,"ax",%progbits
.align	1
.global	__aeabi_lmul
.code	16
.thumb_func
.type	__aeabi_lmul, %function
__aeabi_lmul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #14
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_lmul, .-__aeabi_lmul
.section	.text.__aeabi_ldivmod,"ax",%progbits
.align	1
.global	__aeabi_ldivmod
.code	16
.thumb_func
.type	__aeabi_ldivmod, %function
__aeabi_ldivmod:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #15
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ldivmod, .-__aeabi_ldivmod
.section	.text.__aeabi_uldivmod,"ax",%progbits
.align	1
.global	__aeabi_uldivmod
.code	16
.thumb_func
.type	__aeabi_uldivmod, %function
__aeabi_uldivmod:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #16
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_uldivmod, .-__aeabi_uldivmod
.section	.text.__aeabi_llsl,"ax",%progbits
.align	1
.global	__aeabi_llsl
.code	16
.thumb_func
.type	__aeabi_llsl, %function
__aeabi_llsl:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #17
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_llsl, .-__aeabi_llsl
.section	.text.__aeabi_llsr,"ax",%progbits
.align	1
.global	__aeabi_llsr
.code	16
.thumb_func
.type	__aeabi_llsr, %function
__aeabi_llsr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #18
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_llsr, .-__aeabi_llsr
.section	.text.__aeabi_lasr,"ax",%progbits
.align	1
.global	__aeabi_lasr
.code	16
.thumb_func
.type	__aeabi_lasr, %function
__aeabi_lasr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #19
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_lasr, .-__aeabi_lasr
.section	.text.__aeabi_lcmp,"ax",%progbits
.align	1
.global	__aeabi_lcmp
.code	16
.thumb_func
.type	__aeabi_lcmp, %function
__aeabi_lcmp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #20
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_lcmp, .-__aeabi_lcmp
.section	.text.__aeabi_ulcmp,"ax",%progbits
.align	1
.global	__aeabi_ulcmp
.code	16
.thumb_func
.type	__aeabi_ulcmp, %function
__aeabi_ulcmp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #21
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ulcmp, .-__aeabi_ulcmp
.section	.text.__aeabi_i2f,"ax",%progbits
.align	1
.global	__aeabi_i2f
.code	16
.thumb_func
.type	__aeabi_i2f, %function
__aeabi_i2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #22
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_i2f, .-__aeabi_i2f
.section	.text.__aeabi_l2f,"ax",%progbits
.align	1
.global	__aeabi_l2f
.code	16
.thumb_func
.type	__aeabi_l2f, %function
__aeabi_l2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #22
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_l2f, .-__aeabi_l2f
.section	.text.qfp_int2float,"ax",%progbits
.align	1
.global	qfp_int2float
.code	16
.thumb_func
.type	qfp_int2float, %function
qfp_int2float:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #22
.thumb
.syntax unified
.cfi_endproc
.size	qfp_int2float, .-qfp_int2float
.section	.text.__aeabi_i2d,"ax",%progbits
.align	1
.global	__aeabi_i2d
.code	16
.thumb_func
.type	__aeabi_i2d, %function
__aeabi_i2d:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #23
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_i2d, .-__aeabi_i2d
.section	.text.__aeabi_l2d,"ax",%progbits
.align	1
.global	__aeabi_l2d
.code	16
.thumb_func
.type	__aeabi_l2d, %function
__aeabi_l2d:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #23
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_l2d, .-__aeabi_l2d
.section	.text.qfp_int2double,"ax",%progbits
.align	1
.global	qfp_int2double
.code	16
.thumb_func
.type	qfp_int2double, %function
qfp_int2double:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #23
.thumb
.syntax unified
.cfi_endproc
.size	qfp_int2double, .-qfp_int2double
.section	.text.__aeabi_ui2f,"ax",%progbits
.align	1
.global	__aeabi_ui2f
.code	16
.thumb_func
.type	__aeabi_ui2f, %function
__aeabi_ui2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #24
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ui2f, .-__aeabi_ui2f
.section	.text.__aeabi_ul2f,"ax",%progbits
.align	1
.global	__aeabi_ul2f
.code	16
.thumb_func
.type	__aeabi_ul2f, %function
__aeabi_ul2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #24
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ul2f, .-__aeabi_ul2f
.section	.text.qfp_uint2float,"ax",%progbits
.align	1
.global	qfp_uint2float
.code	16
.thumb_func
.type	qfp_uint2float, %function
qfp_uint2float:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #24
.thumb
.syntax unified
.cfi_endproc
.size	qfp_uint2float, .-qfp_uint2float
.section	.text.__aeabi_ui2d,"ax",%progbits
.align	1
.global	__aeabi_ui2d
.code	16
.thumb_func
.type	__aeabi_ui2d, %function
__aeabi_ui2d:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #25
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ui2d, .-__aeabi_ui2d
.section	.text.__aeabi_ul2d,"ax",%progbits
.align	1
.global	__aeabi_ul2d
.code	16
.thumb_func
.type	__aeabi_ul2d, %function
__aeabi_ul2d:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #25
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_ul2d, .-__aeabi_ul2d
.section	.text.qfp_uint2double,"ax",%progbits
.align	1
.global	qfp_uint2double
.code	16
.thumb_func
.type	qfp_uint2double, %function
qfp_uint2double:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #25
.thumb
.syntax unified
.cfi_endproc
.size	qfp_uint2double, .-qfp_uint2double
.section	.text.__aeabi_f2iz,"ax",%progbits
.align	1
.global	__aeabi_f2iz
.code	16
.thumb_func
.type	__aeabi_f2iz, %function
__aeabi_f2iz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #26
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2iz, .-__aeabi_f2iz
.section	.text.__aeabi_f2lz,"ax",%progbits
.align	1
.global	__aeabi_f2lz
.code	16
.thumb_func
.type	__aeabi_f2lz, %function
__aeabi_f2lz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #26
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2lz, .-__aeabi_f2lz
.section	.text.qfp_float2int,"ax",%progbits
.align	1
.global	qfp_float2int
.code	16
.thumb_func
.type	qfp_float2int, %function
qfp_float2int:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #26
.thumb
.syntax unified
.cfi_endproc
.size	qfp_float2int, .-qfp_float2int
.section	.text.__aeabi_d2iz,"ax",%progbits
.align	1
.global	__aeabi_d2iz
.code	16
.thumb_func
.type	__aeabi_d2iz, %function
__aeabi_d2iz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #27
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2iz, .-__aeabi_d2iz
.section	.text.__aeabi_d2lz,"ax",%progbits
.align	1
.global	__aeabi_d2lz
.code	16
.thumb_func
.type	__aeabi_d2lz, %function
__aeabi_d2lz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #27
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2lz, .-__aeabi_d2lz
.section	.text.qfp_double2int,"ax",%progbits
.align	1
.global	qfp_double2int
.code	16
.thumb_func
.type	qfp_double2int, %function
qfp_double2int:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #27
.thumb
.syntax unified
.cfi_endproc
.size	qfp_double2int, .-qfp_double2int
.section	.text.__aeabi_f2uiz,"ax",%progbits
.align	1
.global	__aeabi_f2uiz
.code	16
.thumb_func
.type	__aeabi_f2uiz, %function
__aeabi_f2uiz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #28
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2uiz, .-__aeabi_f2uiz
.section	.text.__aeabi_f2ulz,"ax",%progbits
.align	1
.global	__aeabi_f2ulz
.code	16
.thumb_func
.type	__aeabi_f2ulz, %function
__aeabi_f2ulz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #28
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_f2ulz, .-__aeabi_f2ulz
.section	.text.qfp_float2uint,"ax",%progbits
.align	1
.global	qfp_float2uint
.code	16
.thumb_func
.type	qfp_float2uint, %function
qfp_float2uint:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #28
.thumb
.syntax unified
.cfi_endproc
.size	qfp_float2uint, .-qfp_float2uint
.section	.text.__aeabi_d2uiz,"ax",%progbits
.align	1
.global	__aeabi_d2uiz
.code	16
.thumb_func
.type	__aeabi_d2uiz, %function
__aeabi_d2uiz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #29
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2uiz, .-__aeabi_d2uiz
.section	.text.__aeabi_d2ulz,"ax",%progbits
.align	1
.global	__aeabi_d2ulz
.code	16
.thumb_func
.type	__aeabi_d2ulz, %function
__aeabi_d2ulz:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #29
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_d2ulz, .-__aeabi_d2ulz
.section	.text.qfp_double2uint,"ax",%progbits
.align	1
.global	qfp_double2uint
.code	16
.thumb_func
.type	qfp_double2uint, %function
qfp_double2uint:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #29
.thumb
.syntax unified
.cfi_endproc
.size	qfp_double2uint, .-qfp_double2uint
.section	.text.__aeabi_cfcmpeq,"ax",%progbits
.align	1
.global	__aeabi_cfcmpeq
.code	16
.thumb_func
.type	__aeabi_cfcmpeq, %function
__aeabi_cfcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #30
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cfcmpeq, .-__aeabi_cfcmpeq
.section	.text.qfp_cfcmpeq,"ax",%progbits
.align	1
.global	qfp_cfcmpeq
.code	16
.thumb_func
.type	qfp_cfcmpeq, %function
qfp_cfcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #30
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cfcmpeq, .-qfp_cfcmpeq
.section	.text.__aeabi_cdcmpeq,"ax",%progbits
.align	1
.global	__aeabi_cdcmpeq
.code	16
.thumb_func
.type	__aeabi_cdcmpeq, %function
__aeabi_cdcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #31
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cdcmpeq, .-__aeabi_cdcmpeq
.section	.text.qfp_cdcmpeq,"ax",%progbits
.align	1
.global	qfp_cdcmpeq
.code	16
.thumb_func
.type	qfp_cdcmpeq, %function
qfp_cdcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #31
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cdcmpeq, .-qfp_cdcmpeq
.section	.text.__aeabi_cfcmple,"ax",%progbits
.align	1
.global	__aeabi_cfcmple
.code	16
.thumb_func
.type	__aeabi_cfcmple, %function
__aeabi_cfcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #32
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cfcmple, .-__aeabi_cfcmple
.section	.text.qfp_cfcmple,"ax",%progbits
.align	1
.global	qfp_cfcmple
.code	16
.thumb_func
.type	qfp_cfcmple, %function
qfp_cfcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #32
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cfcmple, .-qfp_cfcmple
.section	.text.__aeabi_cdcmple,"ax",%progbits
.align	1
.global	__aeabi_cdcmple
.code	16
.thumb_func
.type	__aeabi_cdcmple, %function
__aeabi_cdcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #33
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cdcmple, .-__aeabi_cdcmple
.section	.text.qfp_cdcmple,"ax",%progbits
.align	1
.global	qfp_cdcmple
.code	16
.thumb_func
.type	qfp_cdcmple, %function
qfp_cdcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #33
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cdcmple, .-qfp_cdcmple
.section	.text.__aeabi_cfrcmple,"ax",%progbits
.align	1
.global	__aeabi_cfrcmple
.code	16
.thumb_func
.type	__aeabi_cfrcmple, %function
__aeabi_cfrcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #34
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cfrcmple, .-__aeabi_cfrcmple
.section	.text.qfp_cfrcmple,"ax",%progbits
.align	1
.global	qfp_cfrcmple
.code	16
.thumb_func
.type	qfp_cfrcmple, %function
qfp_cfrcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #34
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cfrcmple, .-qfp_cfrcmple
.section	.text.__aeabi_cdcrmple,"ax",%progbits
.align	1
.global	__aeabi_cdcrmple
.code	16
.thumb_func
.type	__aeabi_cdcrmple, %function
__aeabi_cdcrmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #35
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_cdcrmple, .-__aeabi_cdcrmple
.section	.text.qfp_cdrcmple,"ax",%progbits
.align	1
.global	qfp_cdrcmple
.code	16
.thumb_func
.type	qfp_cdrcmple, %function
qfp_cdrcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #35
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cdrcmple, .-qfp_cdrcmple
.section	.text.__aeabi_fcmpeq,"ax",%progbits
.align	1
.global	__aeabi_fcmpeq
.code	16
.thumb_func
.type	__aeabi_fcmpeq, %function
__aeabi_fcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #36
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmpeq, .-__aeabi_fcmpeq
.section	.text.qfp_cmpeq,"ax",%progbits
.align	1
.global	qfp_cmpeq
.code	16
.thumb_func
.type	qfp_cmpeq, %function
qfp_cmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #36
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpeq, .-qfp_cmpeq
.section	.text.__aeabi_dcmpeq,"ax",%progbits
.align	1
.global	__aeabi_dcmpeq
.code	16
.thumb_func
.type	__aeabi_dcmpeq, %function
__aeabi_dcmpeq:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #37
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmpeq, .-__aeabi_dcmpeq
.section	.text.qfp_cmpeqd,"ax",%progbits
.align	1
.global	qfp_cmpeqd
.code	16
.thumb_func
.type	qfp_cmpeqd, %function
qfp_cmpeqd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #37
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpeqd, .-qfp_cmpeqd
.section	.text.__aeabi_fcmplt,"ax",%progbits
.align	1
.global	__aeabi_fcmplt
.code	16
.thumb_func
.type	__aeabi_fcmplt, %function
__aeabi_fcmplt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #38
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmplt, .-__aeabi_fcmplt
.section	.text.qfp_cmplt,"ax",%progbits
.align	1
.global	qfp_cmplt
.code	16
.thumb_func
.type	qfp_cmplt, %function
qfp_cmplt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #38
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmplt, .-qfp_cmplt
.section	.text.__aeabi_dcmplt,"ax",%progbits
.align	1
.global	__aeabi_dcmplt
.code	16
.thumb_func
.type	__aeabi_dcmplt, %function
__aeabi_dcmplt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #39
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmplt, .-__aeabi_dcmplt
.section	.text.qfp_cmpltd,"ax",%progbits
.align	1
.global	qfp_cmpltd
.code	16
.thumb_func
.type	qfp_cmpltd, %function
qfp_cmpltd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #39
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpltd, .-qfp_cmpltd
.section	.text.__aeabi_fcmple,"ax",%progbits
.align	1
.global	__aeabi_fcmple
.code	16
.thumb_func
.type	__aeabi_fcmple, %function
__aeabi_fcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #40
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmple, .-__aeabi_fcmple
.section	.text.qfp_cmple,"ax",%progbits
.align	1
.global	qfp_cmple
.code	16
.thumb_func
.type	qfp_cmple, %function
qfp_cmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #40
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmple, .-qfp_cmple
.section	.text.__aeabi_dcmple,"ax",%progbits
.align	1
.global	__aeabi_dcmple
.code	16
.thumb_func
.type	__aeabi_dcmple, %function
__aeabi_dcmple:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #41
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmple, .-__aeabi_dcmple
.section	.text.qfp_cmpled,"ax",%progbits
.align	1
.global	qfp_cmpled
.code	16
.thumb_func
.type	qfp_cmpled, %function
qfp_cmpled:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #41
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpled, .-qfp_cmpled
.section	.text.__aeabi_fcmpge,"ax",%progbits
.align	1
.global	__aeabi_fcmpge
.code	16
.thumb_func
.type	__aeabi_fcmpge, %function
__aeabi_fcmpge:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #42
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmpge, .-__aeabi_fcmpge
.section	.text.qfp_cmpge,"ax",%progbits
.align	1
.global	qfp_cmpge
.code	16
.thumb_func
.type	qfp_cmpge, %function
qfp_cmpge:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #42
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpge, .-qfp_cmpge
.section	.text.__aeabi_dcmpge,"ax",%progbits
.align	1
.global	__aeabi_dcmpge
.code	16
.thumb_func
.type	__aeabi_dcmpge, %function
__aeabi_dcmpge:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #43
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmpge, .-__aeabi_dcmpge
.section	.text.qfp_cmpged,"ax",%progbits
.align	1
.global	qfp_cmpged
.code	16
.thumb_func
.type	qfp_cmpged, %function
qfp_cmpged:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #43
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpged, .-qfp_cmpged
.section	.text.__aeabi_fcmpgt,"ax",%progbits
.align	1
.global	__aeabi_fcmpgt
.code	16
.thumb_func
.type	__aeabi_fcmpgt, %function
__aeabi_fcmpgt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #44
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmpgt, .-__aeabi_fcmpgt
.section	.text.qfp_cmpgt,"ax",%progbits
.align	1
.global	qfp_cmpgt
.code	16
.thumb_func
.type	qfp_cmpgt, %function
qfp_cmpgt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #44
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpgt, .-qfp_cmpgt
.section	.text.__aeabi_dcmpgt,"ax",%progbits
.align	1
.global	__aeabi_dcmpgt
.code	16
.thumb_func
.type	__aeabi_dcmpgt, %function
__aeabi_dcmpgt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #45
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmpgt, .-__aeabi_dcmpgt
.section	.text.qfp_cmpgtd,"ax",%progbits
.align	1
.global	qfp_cmpgtd
.code	16
.thumb_func
.type	qfp_cmpgtd, %function
qfp_cmpgtd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #45
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpgtd, .-qfp_cmpgtd
.section	.text.__aeabi_fcmpun,"ax",%progbits
.align	1
.global	__aeabi_fcmpun
.code	16
.thumb_func
.type	__aeabi_fcmpun, %function
__aeabi_fcmpun:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #46
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fcmpun, .-__aeabi_fcmpun
.section	.text.qfp_cmpun,"ax",%progbits
.align	1
.global	qfp_cmpun
.code	16
.thumb_func
.type	qfp_cmpun, %function
qfp_cmpun:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #46
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpun, .-qfp_cmpun
.section	.text.__aeabi_dcmpun,"ax",%progbits
.align	1
.global	__aeabi_dcmpun
.code	16
.thumb_func
.type	__aeabi_dcmpun, %function
__aeabi_dcmpun:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #47
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dcmpun, .-__aeabi_dcmpun
.section	.text.qfp_cmpund,"ax",%progbits
.align	1
.global	qfp_cmpund
.code	16
.thumb_func
.type	qfp_cmpund, %function
qfp_cmpund:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #47
.thumb
.syntax unified
.cfi_endproc
.size	qfp_cmpund, .-qfp_cmpund
.section	.text.__aeabi_fadd,"ax",%progbits
.align	1
.global	__aeabi_fadd
.code	16
.thumb_func
.type	__aeabi_fadd, %function
__aeabi_fadd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #48
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fadd, .-__aeabi_fadd
.section	.text.qfp_fadd,"ax",%progbits
.align	1
.global	qfp_fadd
.code	16
.thumb_func
.type	qfp_fadd, %function
qfp_fadd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #48
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fadd, .-qfp_fadd
.section	.text.__aeabi_dadd,"ax",%progbits
.align	1
.global	__aeabi_dadd
.code	16
.thumb_func
.type	__aeabi_dadd, %function
__aeabi_dadd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #49
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dadd, .-__aeabi_dadd
.section	.text.qfp_dadd,"ax",%progbits
.align	1
.global	qfp_dadd
.code	16
.thumb_func
.type	qfp_dadd, %function
qfp_dadd:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #49
.thumb
.syntax unified
.cfi_endproc
.size	qfp_dadd, .-qfp_dadd
.section	.text.__aeabi_fdiv,"ax",%progbits
.align	1
.global	__aeabi_fdiv
.code	16
.thumb_func
.type	__aeabi_fdiv, %function
__aeabi_fdiv:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #50
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fdiv, .-__aeabi_fdiv
.section	.text.qfp_fdiv_fast,"ax",%progbits
.align	1
.global	qfp_fdiv_fast
.code	16
.thumb_func
.type	qfp_fdiv_fast, %function
qfp_fdiv_fast:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #50
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fdiv_fast, .-qfp_fdiv_fast
.section	.text.__adabi_ddiv,"ax",%progbits
.align	1
.global	__adabi_ddiv
.code	16
.thumb_func
.type	__adabi_ddiv, %function
__adabi_ddiv:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #51
.thumb
.syntax unified
.cfi_endproc
.size	__adabi_ddiv, .-__adabi_ddiv
.section	.text.qfp_ddiv_fast,"ax",%progbits
.align	1
.global	qfp_ddiv_fast
.code	16
.thumb_func
.type	qfp_ddiv_fast, %function
qfp_ddiv_fast:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #51
.thumb
.syntax unified
.cfi_endproc
.size	qfp_ddiv_fast, .-qfp_ddiv_fast
.section	.text.__aeabi_fmul,"ax",%progbits
.align	1
.global	__aeabi_fmul
.code	16
.thumb_func
.type	__aeabi_fmul, %function
__aeabi_fmul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #52
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fmul, .-__aeabi_fmul
.section	.text.qfp_fmul,"ax",%progbits
.align	1
.global	qfp_fmul
.code	16
.thumb_func
.type	qfp_fmul, %function
qfp_fmul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #52
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fmul, .-qfp_fmul
.section	.text.__aeabi_dmul,"ax",%progbits
.align	1
.global	__aeabi_dmul
.code	16
.thumb_func
.type	__aeabi_dmul, %function
__aeabi_dmul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #53
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dmul, .-__aeabi_dmul
.section	.text.qfp_dmul,"ax",%progbits
.align	1
.global	qfp_dmul
.code	16
.thumb_func
.type	qfp_dmul, %function
qfp_dmul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #53
.thumb
.syntax unified
.cfi_endproc
.size	qfp_dmul, .-qfp_dmul
.section	.text.__aeabi_frsub,"ax",%progbits
.align	1
.global	__aeabi_frsub
.code	16
.thumb_func
.type	__aeabi_frsub, %function
__aeabi_frsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #54
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_frsub, .-__aeabi_frsub
.section	.text.qfp_frsub,"ax",%progbits
.align	1
.global	qfp_frsub
.code	16
.thumb_func
.type	qfp_frsub, %function
qfp_frsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #54
.thumb
.syntax unified
.cfi_endproc
.size	qfp_frsub, .-qfp_frsub
.section	.text.__aeabi_drsub,"ax",%progbits
.align	1
.global	__aeabi_drsub
.code	16
.thumb_func
.type	__aeabi_drsub, %function
__aeabi_drsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #55
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_drsub, .-__aeabi_drsub
.section	.text.qfp_drsub,"ax",%progbits
.align	1
.global	qfp_drsub
.code	16
.thumb_func
.type	qfp_drsub, %function
qfp_drsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #55
.thumb
.syntax unified
.cfi_endproc
.size	qfp_drsub, .-qfp_drsub
.section	.text.__aeabi_fsub,"ax",%progbits
.align	1
.global	__aeabi_fsub
.code	16
.thumb_func
.type	__aeabi_fsub, %function
__aeabi_fsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #56
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_fsub, .-__aeabi_fsub
.section	.text.qfp_fsub,"ax",%progbits
.align	1
.global	qfp_fsub
.code	16
.thumb_func
.type	qfp_fsub, %function
qfp_fsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #56
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fsub, .-qfp_fsub
.section	.text.__aeabi_dsub,"ax",%progbits
.align	1
.global	__aeabi_dsub
.code	16
.thumb_func
.type	__aeabi_dsub, %function
__aeabi_dsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #57
.thumb
.syntax unified
.cfi_endproc
.size	__aeabi_dsub, .-__aeabi_dsub
.section	.text.qfp_dsub,"ax",%progbits
.align	1
.global	qfp_dsub
.code	16
.thumb_func
.type	qfp_dsub, %function
qfp_dsub:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #57
.thumb
.syntax unified
.cfi_endproc
.size	qfp_dsub, .-qfp_dsub
.section	.text.float2str,"ax",%progbits
.align	1
.global	float2str
.code	16
.thumb_func
.type	float2str, %function
float2str:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #58
.thumb
.syntax unified
.cfi_endproc
.size	float2str, .-float2str
.section	.text.qfp_float2str,"ax",%progbits
.align	1
.global	qfp_float2str
.code	16
.thumb_func
.type	qfp_float2str, %function
qfp_float2str:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #58
.thumb
.syntax unified
.cfi_endproc
.size	qfp_float2str, .-qfp_float2str
.section	.text.str2float,"ax",%progbits
.align	1
.global	str2float
.code	16
.thumb_func
.type	str2float, %function
str2float:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #59
.thumb
.syntax unified
.cfi_endproc
.size	str2float, .-str2float
.section	.text.qfp_str2float,"ax",%progbits
.align	1
.global	qfp_str2float
.code	16
.thumb_func
.type	qfp_str2float, %function
qfp_str2float:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #59
.thumb
.syntax unified
.cfi_endproc
.size	qfp_str2float, .-qfp_str2float
.section	.text.cos,"ax",%progbits
.align	1
.global	cos
.code	16
.thumb_func
.type	cos, %function
cos:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #60
.thumb
.syntax unified
.cfi_endproc
.size	cos, .-cos
.section	.text.cosf,"ax",%progbits
.align	1
.global	cosf
.code	16
.thumb_func
.type	cosf, %function
cosf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #60
.thumb
.syntax unified
.cfi_endproc
.size	cosf, .-cosf
.section	.text.qfp_fcos,"ax",%progbits
.align	1
.global	qfp_fcos
.code	16
.thumb_func
.type	qfp_fcos, %function
qfp_fcos:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #60
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fcos, .-qfp_fcos
.section	.text.sin,"ax",%progbits
.align	1
.global	sin
.code	16
.thumb_func
.type	sin, %function
sin:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #61
.thumb
.syntax unified
.cfi_endproc
.size	sin, .-sin
.section	.text.sinf,"ax",%progbits
.align	1
.global	sinf
.code	16
.thumb_func
.type	sinf, %function
sinf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #61
.thumb
.syntax unified
.cfi_endproc
.size	sinf, .-sinf
.section	.text.qfp_fsin,"ax",%progbits
.align	1
.global	qfp_fsin
.code	16
.thumb_func
.type	qfp_fsin, %function
qfp_fsin:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #61
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fsin, .-qfp_fsin
.section	.text.tan,"ax",%progbits
.align	1
.global	tan
.code	16
.thumb_func
.type	tan, %function
tan:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #62
.thumb
.syntax unified
.cfi_endproc
.size	tan, .-tan
.section	.text.tanf,"ax",%progbits
.align	1
.global	tanf
.code	16
.thumb_func
.type	tanf, %function
tanf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #62
.thumb
.syntax unified
.cfi_endproc
.size	tanf, .-tanf
.section	.text.qfp_ftan,"ax",%progbits
.align	1
.global	qfp_ftan
.code	16
.thumb_func
.type	qfp_ftan, %function
qfp_ftan:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #62
.thumb
.syntax unified
.cfi_endproc
.size	qfp_ftan, .-qfp_ftan
.section	.text.atan2,"ax",%progbits
.align	1
.global	atan2
.code	16
.thumb_func
.type	atan2, %function
atan2:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #63
.thumb
.syntax unified
.cfi_endproc
.size	atan2, .-atan2
.section	.text.atan2f,"ax",%progbits
.align	1
.global	atan2f
.code	16
.thumb_func
.type	atan2f, %function
atan2f:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #63
.thumb
.syntax unified
.cfi_endproc
.size	atan2f, .-atan2f
.section	.text.qfp_fatan2,"ax",%progbits
.align	1
.global	qfp_fatan2
.code	16
.thumb_func
.type	qfp_fatan2, %function
qfp_fatan2:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #63
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fatan2, .-qfp_fatan2
.section	.text.exp,"ax",%progbits
.align	1
.global	exp
.code	16
.thumb_func
.type	exp, %function
exp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #64
.thumb
.syntax unified
.cfi_endproc
.size	exp, .-exp
.section	.text.fexp,"ax",%progbits
.align	1
.global	fexp
.code	16
.thumb_func
.type	fexp, %function
fexp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #64
.thumb
.syntax unified
.cfi_endproc
.size	fexp, .-fexp
.section	.text.qfp_fexp,"ax",%progbits
.align	1
.global	qfp_fexp
.code	16
.thumb_func
.type	qfp_fexp, %function
qfp_fexp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #64
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fexp, .-qfp_fexp
.section	.text.log,"ax",%progbits
.align	1
.global	log
.code	16
.thumb_func
.type	log, %function
log:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #65
.thumb
.syntax unified
.cfi_endproc
.size	log, .-log
.section	.text.logf,"ax",%progbits
.align	1
.global	logf
.code	16
.thumb_func
.type	logf, %function
logf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #65
.thumb
.syntax unified
.cfi_endproc
.size	logf, .-logf
.section	.text.qfp_fln,"ax",%progbits
.align	1
.global	qfp_fln
.code	16
.thumb_func
.type	qfp_fln, %function
qfp_fln:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #65
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fln, .-qfp_fln
.section	.text.sqrt,"ax",%progbits
.align	1
.global	sqrt
.code	16
.thumb_func
.type	sqrt, %function
sqrt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #66
.thumb
.syntax unified
.cfi_endproc
.size	sqrt, .-sqrt
.section	.text.sqrtf,"ax",%progbits
.align	1
.global	sqrtf
.code	16
.thumb_func
.type	sqrtf, %function
sqrtf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #66
.thumb
.syntax unified
.cfi_endproc
.size	sqrtf, .-sqrtf
.section	.text.qfp_fsqrt_fast,"ax",%progbits
.align	1
.global	qfp_fsqrt_fast
.code	16
.thumb_func
.type	qfp_fsqrt_fast, %function
qfp_fsqrt_fast:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #66
.thumb
.syntax unified
.cfi_endproc
.size	qfp_fsqrt_fast, .-qfp_fsqrt_fast
.section	.text.strncpy,"ax",%progbits
.align	1
.global	strncpy
.code	16
.thumb_func
.type	strncpy, %function
strncpy:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #67
.thumb
.syntax unified
.cfi_endproc
.size	strncpy, .-strncpy
.section	.text.strcpy,"ax",%progbits
.align	1
.global	strcpy
.code	16
.thumb_func
.type	strcpy, %function
strcpy:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #68
.thumb
.syntax unified
.cfi_endproc
.size	strcpy, .-strcpy
.section	.text.strcmp,"ax",%progbits
.align	1
.global	strcmp
.code	16
.thumb_func
.type	strcmp, %function
strcmp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #69
.thumb
.syntax unified
.cfi_endproc
.size	strcmp, .-strcmp
.section	.text.strncmp,"ax",%progbits
.align	1
.global	strncmp
.code	16
.thumb_func
.type	strncmp, %function
strncmp:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #70
.thumb
.syntax unified
.cfi_endproc
.size	strncmp, .-strncmp
.section	.text.strchr,"ax",%progbits
.align	1
.global	strchr
.code	16
.thumb_func
.type	strchr, %function
strchr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #71
.thumb
.syntax unified
.cfi_endproc
.size	strchr, .-strchr
.section	.text.strlen,"ax",%progbits
.align	1
.global	strlen
.code	16
.thumb_func
.type	strlen, %function
strlen:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #72
.thumb
.syntax unified
.cfi_endproc
.size	strlen, .-strlen
.section	.text.snprintf,"ax",%progbits
.align	1
.global	snprintf
.code	16
.thumb_func
.type	snprintf, %function
snprintf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #73
.thumb
.syntax unified
.cfi_endproc
.size	snprintf, .-snprintf
.section	.text.chsnprintf,"ax",%progbits
.align	1
.global	chsnprintf
.code	16
.thumb_func
.type	chsnprintf, %function
chsnprintf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #73
.thumb
.syntax unified
.cfi_endproc
.size	chsnprintf, .-chsnprintf
.section	.text.ltoa,"ax",%progbits
.align	1
.global	ltoa
.code	16
.thumb_func
.type	ltoa, %function
ltoa:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #74
.thumb
.syntax unified
.cfi_endproc
.size	ltoa, .-ltoa
.section	.text.utoa,"ax",%progbits
.align	1
.global	utoa
.code	16
.thumb_func
.type	utoa, %function
utoa:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #75
.thumb
.syntax unified
.cfi_endproc
.size	utoa, .-utoa
.section	.text.ultoa,"ax",%progbits
.align	1
.global	ultoa
.code	16
.thumb_func
.type	ultoa, %function
ultoa:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #76
.thumb
.syntax unified
.cfi_endproc
.size	ultoa, .-ultoa
.section	.text.itoa,"ax",%progbits
.align	1
.global	itoa
.code	16
.thumb_func
.type	itoa, %function
itoa:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #77
.thumb
.syntax unified
.cfi_endproc
.size	itoa, .-itoa
.section	.text.strtol,"ax",%progbits
.align	1
.global	strtol
.code	16
.thumb_func
.type	strtol, %function
strtol:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #78
.thumb
.syntax unified
.cfi_endproc
.size	strtol, .-strtol
.section	.text.simple_strtol,"ax",%progbits
.align	1
.global	simple_strtol
.code	16
.thumb_func
.type	simple_strtol, %function
simple_strtol:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #78
.thumb
.syntax unified
.cfi_endproc
.size	simple_strtol, .-simple_strtol
.section	.text.strtoul,"ax",%progbits
.align	1
.global	strtoul
.code	16
.thumb_func
.type	strtoul, %function
strtoul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #79
.thumb
.syntax unified
.cfi_endproc
.size	strtoul, .-strtoul
.section	.text.simple_strtoul,"ax",%progbits
.align	1
.global	simple_strtoul
.code	16
.thumb_func
.type	simple_strtoul, %function
simple_strtoul:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #79
.thumb
.syntax unified
.cfi_endproc
.size	simple_strtoul, .-simple_strtoul
.section	.text.printf,"ax",%progbits
.align	1
.global	printf
.code	16
.thumb_func
.type	printf, %function
printf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #80
.thumb
.syntax unified
.cfi_endproc
.size	printf, .-printf
.section	.text.tfp_printf,"ax",%progbits
.align	1
.global	tfp_printf
.code	16
.thumb_func
.type	tfp_printf, %function
tfp_printf:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #80
.thumb
.syntax unified
.cfi_endproc
.size	tfp_printf, .-tfp_printf
.section	.text.serialPutChar,"ax",%progbits
.align	1
.global	serialPutChar
.code	16
.thumb_func
.type	serialPutChar, %function
serialPutChar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #81
.thumb
.syntax unified
.cfi_endproc
.size	serialPutChar, .-serialPutChar
.section	.text.putchar,"ax",%progbits
.align	1
.global	putchar
.code	16
.thumb_func
.type	putchar, %function
putchar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #81
.thumb
.syntax unified
.cfi_endproc
.size	putchar, .-putchar
.section	.text.serialGetChar,"ax",%progbits
.align	1
.global	serialGetChar
.code	16
.thumb_func
.type	serialGetChar, %function
serialGetChar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #82
.thumb
.syntax unified
.cfi_endproc
.size	serialGetChar, .-serialGetChar
.section	.text.getchar,"ax",%progbits
.align	1
.global	getchar
.code	16
.thumb_func
.type	getchar, %function
getchar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #82
.thumb
.syntax unified
.cfi_endproc
.size	getchar, .-getchar
.section	.text.serialCanGetChar,"ax",%progbits
.align	1
.global	serialCanGetChar
.code	16
.thumb_func
.type	serialCanGetChar, %function
serialCanGetChar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #83
.thumb
.syntax unified
.cfi_endproc
.size	serialCanGetChar, .-serialCanGetChar
.section	.text.cangetchar,"ax",%progbits
.align	1
.global	cangetchar
.code	16
.thumb_func
.type	cangetchar, %function
cangetchar:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #83
.thumb
.syntax unified
.cfi_endproc
.size	cangetchar, .-cangetchar
.section	.text.free,"ax",%progbits
.align	1
.global	free
.code	16
.thumb_func
.type	free, %function
free:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #84
.thumb
.syntax unified
.cfi_endproc
.size	free, .-free
.section	.text.malloc,"ax",%progbits
.align	1
.global	malloc
.code	16
.thumb_func
.type	malloc, %function
malloc:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #85
.thumb
.syntax unified
.cfi_endproc
.size	malloc, .-malloc
.section	.text.realloc,"ax",%progbits
.align	1
.global	realloc
.code	16
.thumb_func
.type	realloc, %function
realloc:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #86
.thumb
.syntax unified
.cfi_endproc
.size	realloc, .-realloc
.section	.text.ledShow,"ax",%progbits
.align	1
.global	ledShow
.code	16
.thumb_func
.type	ledShow, %function
ledShow:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #87
.thumb
.syntax unified
.cfi_endproc
.size	ledShow, .-ledShow
.section	.text.attachInterruptChibi,"ax",%progbits
.align	1
.global	attachInterruptChibi
.code	16
.thumb_func
.type	attachInterruptChibi, %function
attachInterruptChibi:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #94
.thumb
.syntax unified
.cfi_endproc
.size	attachInterruptChibi, .-attachInterruptChibi
.section	.text.attachInterrupt,"ax",%progbits
.align	1
.global	attachInterrupt
.code	16
.thumb_func
.type	attachInterrupt, %function
attachInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #94
.thumb
.syntax unified
.cfi_endproc
.size	attachInterrupt, .-attachInterrupt
.section	.text.attachFastInterrupt,"ax",%progbits
.align	1
.global	attachFastInterrupt
.code	16
.thumb_func
.type	attachFastInterrupt, %function
attachFastInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #95
.thumb
.syntax unified
.cfi_endproc
.size	attachFastInterrupt, .-attachFastInterrupt
.section	.text.detachInterruptChibi,"ax",%progbits
.align	1
.global	detachInterruptChibi
.code	16
.thumb_func
.type	detachInterruptChibi, %function
detachInterruptChibi:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #96
.thumb
.syntax unified
.cfi_endproc
.size	detachInterruptChibi, .-detachInterruptChibi
.section	.text.detachInterrupt,"ax",%progbits
.align	1
.global	detachInterrupt
.code	16
.thumb_func
.type	detachInterrupt, %function
detachInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #96
.thumb
.syntax unified
.cfi_endproc
.size	detachInterrupt, .-detachInterrupt
.section	.text.detachFastInterrupt,"ax",%progbits
.align	1
.global	detachFastInterrupt
.code	16
.thumb_func
.type	detachFastInterrupt, %function
detachFastInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #97
.thumb
.syntax unified
.cfi_endproc
.size	detachFastInterrupt, .-detachFastInterrupt
.section	.text.createThread,"ax",%progbits
.align	1
.global	createThread
.code	16
.thumb_func
.type	createThread, %function
createThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #118
.thumb
.syntax unified
.cfi_endproc
.size	createThread, .-createThread
.section	.text.exitThread,"ax",%progbits
.align	1
.global	exitThread
.code	16
.thumb_func
.type	exitThread, %function
exitThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #119
.thumb
.syntax unified
.cfi_endproc
.size	exitThread, .-exitThread
.section	.text.suspendThread,"ax",%progbits
.align	1
.global	suspendThread
.code	16
.thumb_func
.type	suspendThread, %function
suspendThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #120
.thumb
.syntax unified
.cfi_endproc
.size	suspendThread, .-suspendThread
.section	.text.suspendThreadTimeout,"ax",%progbits
.align	1
.global	suspendThreadTimeout
.code	16
.thumb_func
.type	suspendThreadTimeout, %function
suspendThreadTimeout:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #121
.thumb
.syntax unified
.cfi_endproc
.size	suspendThreadTimeout, .-suspendThreadTimeout
.section	.text.resumeThread,"ax",%progbits
.align	1
.global	resumeThread
.code	16
.thumb_func
.type	resumeThread, %function
resumeThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #122
.thumb
.syntax unified
.cfi_endproc
.size	resumeThread, .-resumeThread
.section	.text.yieldThread,"ax",%progbits
.align	1
.global	yieldThread
.code	16
.thumb_func
.type	yieldThread, %function
yieldThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #123
.thumb
.syntax unified
.cfi_endproc
.size	yieldThread, .-yieldThread
.section	.text.threadSleep,"ax",%progbits
.align	1
.global	threadSleep
.code	16
.thumb_func
.type	threadSleep, %function
threadSleep:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #124
.thumb
.syntax unified
.cfi_endproc
.size	threadSleep, .-threadSleep
.section	.text.threadSleepUntil,"ax",%progbits
.align	1
.global	threadSleepUntil
.code	16
.thumb_func
.type	threadSleepUntil, %function
threadSleepUntil:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #125
.thumb
.syntax unified
.cfi_endproc
.size	threadSleepUntil, .-threadSleepUntil
.section	.text.waitThread,"ax",%progbits
.align	1
.global	waitThread
.code	16
.thumb_func
.type	waitThread, %function
waitThread:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #126
.thumb
.syntax unified
.cfi_endproc
.size	waitThread, .-waitThread
.section	.text.setTimer,"ax",%progbits
.align	1
.global	setTimer
.code	16
.thumb_func
.type	setTimer, %function
setTimer:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #127
.thumb
.syntax unified
.cfi_endproc
.size	setTimer, .-setTimer
.section	.text.resetTimer,"ax",%progbits
.align	1
.global	resetTimer
.code	16
.thumb_func
.type	resetTimer, %function
resetTimer:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #128
.thumb
.syntax unified
.cfi_endproc
.size	resetTimer, .-resetTimer
.section	.text.runCallbacks,"ax",%progbits
.align	1
.global	runCallbacks
.code	16
.thumb_func
.type	runCallbacks, %function
runCallbacks:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #129
.thumb
.syntax unified
.cfi_endproc
.size	runCallbacks, .-runCallbacks
.section	.text.lockSystem,"ax",%progbits
.align	1
.global	lockSystem
.code	16
.thumb_func
.type	lockSystem, %function
lockSystem:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #130
.thumb
.syntax unified
.cfi_endproc
.size	lockSystem, .-lockSystem
.section	.text.unlockSystem,"ax",%progbits
.align	1
.global	unlockSystem
.code	16
.thumb_func
.type	unlockSystem, %function
unlockSystem:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #131
.thumb
.syntax unified
.cfi_endproc
.size	unlockSystem, .-unlockSystem
.section	.text.lockSystemFromISR,"ax",%progbits
.align	1
.global	lockSystemFromISR
.code	16
.thumb_func
.type	lockSystemFromISR, %function
lockSystemFromISR:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #132
.thumb
.syntax unified
.cfi_endproc
.size	lockSystemFromISR, .-lockSystemFromISR
.section	.text.unlockSystemFromISR,"ax",%progbits
.align	1
.global	unlockSystemFromISR
.code	16
.thumb_func
.type	unlockSystemFromISR, %function
unlockSystemFromISR:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #133
.thumb
.syntax unified
.cfi_endproc
.size	unlockSystemFromISR, .-unlockSystemFromISR
.section	.text.mutexInit,"ax",%progbits
.align	1
.global	mutexInit
.code	16
.thumb_func
.type	mutexInit, %function
mutexInit:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #134
.thumb
.syntax unified
.cfi_endproc
.size	mutexInit, .-mutexInit
.section	.text.mutexLock,"ax",%progbits
.align	1
.global	mutexLock
.code	16
.thumb_func
.type	mutexLock, %function
mutexLock:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #135
.thumb
.syntax unified
.cfi_endproc
.size	mutexLock, .-mutexLock
.section	.text.mutexTryLock,"ax",%progbits
.align	1
.global	mutexTryLock
.code	16
.thumb_func
.type	mutexTryLock, %function
mutexTryLock:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #136
.thumb
.syntax unified
.cfi_endproc
.size	mutexTryLock, .-mutexTryLock
.section	.text.mutexUnlock,"ax",%progbits
.align	1
.global	mutexUnlock
.code	16
.thumb_func
.type	mutexUnlock, %function
mutexUnlock:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #137
.thumb
.syntax unified
.cfi_endproc
.size	mutexUnlock, .-mutexUnlock
.section	.text.flashErase,"ax",%progbits
.align	1
.global	flashErase
.code	16
.thumb_func
.type	flashErase, %function
flashErase:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #138
.thumb
.syntax unified
.cfi_endproc
.size	flashErase, .-flashErase
.section	.text.flashWrite,"ax",%progbits
.align	1
.global	flashWrite
.code	16
.thumb_func
.type	flashWrite, %function
flashWrite:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #139
.thumb
.syntax unified
.cfi_endproc
.size	flashWrite, .-flashWrite
.section	.text.__gnu_thumb1_case_sqi,"ax",%progbits
.align	1
.global	__gnu_thumb1_case_sqi
.code	16
.thumb_func
.type	__gnu_thumb1_case_sqi, %function
__gnu_thumb1_case_sqi:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #140
.thumb
.syntax unified
.cfi_endproc
.size	__gnu_thumb1_case_sqi, .-__gnu_thumb1_case_sqi
.section	.text.__gnu_thumb1_case_uqi,"ax",%progbits
.align	1
.global	__gnu_thumb1_case_uqi
.code	16
.thumb_func
.type	__gnu_thumb1_case_uqi, %function
__gnu_thumb1_case_uqi:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #141
.thumb
.syntax unified
.cfi_endproc
.size	__gnu_thumb1_case_uqi, .-__gnu_thumb1_case_uqi
.section	.text.__gnu_thumb1_case_uhi,"ax",%progbits
.align	1
.global	__gnu_thumb1_case_uhi
.code	16
.thumb_func
.type	__gnu_thumb1_case_uhi, %function
__gnu_thumb1_case_uhi:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #142
.thumb
.syntax unified
.cfi_endproc
.size	__gnu_thumb1_case_uhi, .-__gnu_thumb1_case_uhi
.section	.text.doSudo,"ax",%progbits
.align	1
.global	doSudo
.code	16
.thumb_func
.type	doSudo, %function
doSudo:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #143
.thumb
.syntax unified
.cfi_endproc
.size	doSudo, .-doSudo
.section	.text.setSerialSpeed,"ax",%progbits
.align	1
.global	setSerialSpeed
.code	16
.thumb_func
.type	setSerialSpeed, %function
setSerialSpeed:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #144
.thumb
.syntax unified
.cfi_endproc
.size	setSerialSpeed, .-setSerialSpeed
.section	.text.setThreadName,"ax",%progbits
.align	1
.global	setThreadName
.code	16
.thumb_func
.type	setThreadName, %function
setThreadName:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #145
.thumb
.syntax unified
.cfi_endproc
.size	setThreadName, .-setThreadName
.section	.text.hookSysTick,"ax",%progbits
.align	1
.global	hookSysTick
.code	16
.thumb_func
.type	hookSysTick, %function
hookSysTick:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #146
.thumb
.syntax unified
.cfi_endproc
.size	hookSysTick, .-hookSysTick
.section	.text.resumeThreadI,"ax",%progbits
.align	1
.global	resumeThreadI
.code	16
.thumb_func
.type	resumeThreadI, %function
resumeThreadI:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #147
.thumb
.syntax unified
.cfi_endproc
.size	resumeThreadI, .-resumeThreadI
.section	.text.suspendThreadS,"ax",%progbits
.align	1
.global	suspendThreadS
.code	16
.thumb_func
.type	suspendThreadS, %function
suspendThreadS:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #148
.thumb
.syntax unified
.cfi_endproc
.size	suspendThreadS, .-suspendThreadS
.section	.text.suspendThreadTimeoutS,"ax",%progbits
.align	1
.global	suspendThreadTimeoutS
.code	16
.thumb_func
.type	suspendThreadTimeoutS, %function
suspendThreadTimeoutS:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #149
.thumb
.syntax unified
.cfi_endproc
.size	suspendThreadTimeoutS, .-suspendThreadTimeoutS
.section	.text.hookSoftPwm,"ax",%progbits
.align	1
.global	hookSoftPwm
.code	16
.thumb_func
.type	hookSoftPwm, %function
hookSoftPwm:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #150
.thumb
.syntax unified
.cfi_endproc
.size	hookSoftPwm, .-hookSoftPwm
.section	.text.getOsVersion,"ax",%progbits
.align	1
.global	getOsVersion
.code	16
.thumb_func
.type	getOsVersion, %function
getOsVersion:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #151
.thumb
.syntax unified
.cfi_endproc
.size	getOsVersion, .-getOsVersion
.section	.text.getHwVersion,"ax",%progbits
.align	1
.global	getHwVersion
.code	16
.thumb_func
.type	getHwVersion, %function
getHwVersion:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #152
.thumb
.syntax unified
.cfi_endproc
.size	getHwVersion, .-getHwVersion
.section	.text.getGitVersion,"ax",%progbits
.align	1
.global	getGitVersion
.code	16
.thumb_func
.type	getGitVersion, %function
getGitVersion:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #153
.thumb
.syntax unified
.cfi_endproc
.size	getGitVersion, .-getGitVersion
.section	.text.digitalPinToInterrupt,"ax",%progbits
.align	1
.global	digitalPinToInterrupt
.code	16
.thumb_func
.type	digitalPinToInterrupt, %function
digitalPinToInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #154
.thumb
.syntax unified
.cfi_endproc
.size	digitalPinToInterrupt, .-digitalPinToInterrupt
.section	.text.analogReadResolution,"ax",%progbits
.align	1
.global	analogReadResolution
.code	16
.thumb_func
.type	analogReadResolution, %function
analogReadResolution:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #155
.thumb
.syntax unified
.cfi_endproc
.size	analogReadResolution, .-analogReadResolution
.section	.text.canonicalizePin,"ax",%progbits
.align	1
.global	canonicalizePin
.code	16
.thumb_func
.type	canonicalizePin, %function
canonicalizePin:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #156
.thumb
.syntax unified
.cfi_endproc
.size	canonicalizePin, .-canonicalizePin
.section	.text.canonicalisePin,"ax",%progbits
.align	1
.global	canonicalisePin
.code	16
.thumb_func
.type	canonicalisePin, %function
canonicalisePin:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #156
.thumb
.syntax unified
.cfi_endproc
.size	canonicalisePin, .-canonicalisePin
.section	.text.pinToPort,"ax",%progbits
.align	1
.global	pinToPort
.code	16
.thumb_func
.type	pinToPort, %function
pinToPort:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #157
.thumb
.syntax unified
.cfi_endproc
.size	pinToPort, .-pinToPort
.section	.text.MurmurHash3_x86_32,"ax",%progbits
.align	1
.global	MurmurHash3_x86_32
.code	16
.thumb_func
.type	MurmurHash3_x86_32, %function
MurmurHash3_x86_32:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #158
.thumb
.syntax unified
.cfi_endproc
.size	MurmurHash3_x86_32, .-MurmurHash3_x86_32
.section	.text.murmur3,"ax",%progbits
.align	1
.global	murmur3
.code	16
.thumb_func
.type	murmur3, %function
murmur3:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #158
.thumb
.syntax unified
.cfi_endproc
.size	murmur3, .-murmur3
.section	.text.enableLptmr,"ax",%progbits
.align	1
.global	enableLptmr
.code	16
.thumb_func
.type	enableLptmr, %function
enableLptmr:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #159
.thumb
.syntax unified
.cfi_endproc
.size	enableLptmr, .-enableLptmr
.section	.text.enableTimer,"ax",%progbits
.align	1
.global	enableTimer
.code	16
.thumb_func
.type	enableTimer, %function
enableTimer:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #159
.thumb
.syntax unified
.cfi_endproc
.size	enableTimer, .-enableTimer
.section	.text.enableInterrupt,"ax",%progbits
.align	1
.global	enableInterrupt
.code	16
.thumb_func
.type	enableInterrupt, %function
enableInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #160
.thumb
.syntax unified
.cfi_endproc
.size	enableInterrupt, .-enableInterrupt
.section	.text.disableInterrupt,"ax",%progbits
.align	1
.global	disableInterrupt
.code	16
.thumb_func
.type	disableInterrupt, %function
disableInterrupt:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #161
.thumb
.syntax unified
.cfi_endproc
.size	disableInterrupt, .-disableInterrupt
.section	.text.setInterruptPriority,"ax",%progbits
.align	1
.global	setInterruptPriority
.code	16
.thumb_func
.type	setInterruptPriority, %function
setInterruptPriority:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #162
.thumb
.syntax unified
.cfi_endproc
.size	setInterruptPriority, .-setInterruptPriority
.section	.text.chThdCreateFromHeap,"ax",%progbits
.align	1
.global	chThdCreateFromHeap
.code	16
.thumb_func
.type	chThdCreateFromHeap, %function
chThdCreateFromHeap:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #163
.thumb
.syntax unified
.cfi_endproc
.size	chThdCreateFromHeap, .-chThdCreateFromHeap
.section	.text.errorCondition,"ax",%progbits
.align	1
.global	errorCondition
.code	16
.thumb_func
.type	errorCondition, %function
errorCondition:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #164
.thumb
.syntax unified
.cfi_endproc
.size	errorCondition, .-errorCondition
/* void pinMode(int pin, enum pin_mode mode) { */
.section	.text.pinMode,"ax",%progbits
.align	1
.global	pinMode
.code	16
.thumb_func
.type	pinMode, %function
pinMode:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #88
.thumb
.syntax unified
.cfi_endproc
.size	pinMode, .-pinMode
/* void digitalWrite(int pin, int value) { */
.section	.text.digitalWrite,"ax",%progbits
.align	1
.global	digitalWrite
.code	16
.thumb_func
.type	digitalWrite, %function
digitalWrite:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #89
.thumb
.syntax unified
.cfi_endproc
.size	digitalWrite, .-digitalWrite
/* int digitalRead_c(int pin) { */
.section	.text.digitalRead_c,"ax",%progbits
.align	1
.global	digitalRead_c
.code	16
.thumb_func
.type	digitalRead_c, %function
digitalRead_c:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #90
.thumb
.syntax unified
.cfi_endproc
.size	digitalRead_c, .-digitalRead_c
/* int digitalRead(int pin) { */
.section	.text.digitalRead,"ax",%progbits
.align	1
.global	digitalRead
.code	16
.thumb_func
.type	digitalRead, %function
digitalRead:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #90
.thumb
.syntax unified
.cfi_endproc
.size	digitalRead, .-digitalRead
/* void analogWrite(int pin, int value) { */
.section	.text.analogWrite,"ax",%progbits
.align	1
.global	analogWrite
.code	16
.thumb_func
.type	analogWrite, %function
analogWrite:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #91
.thumb
.syntax unified
.cfi_endproc
.size	analogWrite, .-analogWrite
/* void analogReference(enum analog_reference_type type) { */
.section	.text.analogReference,"ax",%progbits
.align	1
.global	analogReference
.code	16
.thumb_func
.type	analogReference, %function
analogReference:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #92
.thumb
.syntax unified
.cfi_endproc
.size	analogReference, .-analogReference
/* int analogRead(int pin) { */
.section	.text.analogRead,"ax",%progbits
.align	1
.global	analogRead
.code	16
.thumb_func
.type	analogRead, %function
analogRead:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #93
.thumb
.syntax unified
.cfi_endproc
.size	analogRead, .-analogRead
/* void tone(int pin, unsigned int frequency, unsigned long duration) { */
.section	.text.tone,"ax",%progbits
.align	1
.global	tone
.code	16
.thumb_func
.type	tone, %function
tone:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #98
.thumb
.syntax unified
.cfi_endproc
.size	tone, .-tone
/* void noTone(int pin) { */
.section	.text.noTone,"ax",%progbits
.align	1
.global	noTone
.code	16
.thumb_func
.type	noTone, %function
noTone:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #99
.thumb
.syntax unified
.cfi_endproc
.size	noTone, .-noTone
/* unsigned long millis(void) { */
.section	.text.millis,"ax",%progbits
.align	1
.global	millis
.code	16
.thumb_func
.type	millis, %function
millis:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #104
.thumb
.syntax unified
.cfi_endproc
.size	millis, .-millis
/* unsigned long micros(void) { */
.section	.text.micros,"ax",%progbits
.align	1
.global	micros
.code	16
.thumb_func
.type	micros, %function
micros:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #105
.thumb
.syntax unified
.cfi_endproc
.size	micros, .-micros
/* void delay_c(unsigned long msecs) { */
.section	.text.delay_c,"ax",%progbits
.align	1
.global	delay_c
.code	16
.thumb_func
.type	delay_c, %function
delay_c:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #106
.thumb
.syntax unified
.cfi_endproc
.size	delay_c, .-delay_c
/* void delay(unsigned long msecs) { */
.section	.text.delay,"ax",%progbits
.align	1
.global	delay
.code	16
.thumb_func
.type	delay, %function
delay:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #106
.thumb
.syntax unified
.cfi_endproc
.size	delay, .-delay
/* void delayMicroseconds_c(unsigned int usecs) { */
.section	.text.delayMicroseconds_c,"ax",%progbits
.align	1
.global	delayMicroseconds_c
.code	16
.thumb_func
.type	delayMicroseconds_c, %function
delayMicroseconds_c:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #107
.thumb
.syntax unified
.cfi_endproc
.size	delayMicroseconds_c, .-delayMicroseconds_c
/* void delayMicroseconds(unsigned int usecs) { */
.section	.text.delayMicroseconds,"ax",%progbits
.align	1
.global	delayMicroseconds
.code	16
.thumb_func
.type	delayMicroseconds, %function
delayMicroseconds:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #107
.thumb
.syntax unified
.cfi_endproc
.size	delayMicroseconds, .-delayMicroseconds
/* long random_c(long min, long max) { */
.section	.text.random_c,"ax",%progbits
.align	1
.global	random_c
.code	16
.thumb_func
.type	random_c, %function
random_c:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #109
.thumb
.syntax unified
.cfi_endproc
.size	random_c, .-random_c
/* long random(long min, long max) { */
.section	.text.random,"ax",%progbits
.align	1
.global	random
.code	16
.thumb_func
.type	random, %function
random:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #109
.thumb
.syntax unified
.cfi_endproc
.size	random, .-random
/* long randomSeed(unsigned long seed) { */
.section	.text.randomSeed,"ax",%progbits
.align	1
.global	randomSeed
.code	16
.thumb_func
.type	randomSeed, %function
randomSeed:
.cfi_startproc
@ Naked Function: prologue and epilogue provided by programmer.
@ args = 0, pretend = 0, frame = 0
@ frame_needed = 0, uses_anonymous_args = 0
.syntax divided
svc #110
.thumb
.syntax unified
.cfi_endproc
.size	randomSeed, .-randomSeed
