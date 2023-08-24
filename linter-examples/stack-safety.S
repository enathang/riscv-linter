.macro push_s0_onto_stack
	sd s0, 8(sp)

	li t0, 8
	add sp, sp, t0
.endm

.macro pop_s0_off_stack
	li t0, -8
	add sp, sp, t0

	ld s0, 8(sp)
.endm

_func_test1:
	li t0, 0
	sd t0, 8(sp)
	call _other_function
	ld t0, 8(sp)

	mv t1, t0
	
	ret

# Should raise warning that t0 is unsafe to read from (because even though we added it to stack, we didn't pop it off before reading)
_func_test2:
	li t0, 0
	sd t0, 8(sp)
	call _other_function
	mv t1, t0

	ret

# Should raise warning that the value pushed onto stack and value popped off stack are different
_func_test3:
	#:+ t0
	#:- t1
