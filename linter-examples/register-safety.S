_increment:
	li t0, 1
	add a0, a0, t0

	ret

_decrement:
	li t0, -1
	add a0, a0, t0

	ret

# Should warn saying t0 is unsafe to read from
_func_test1:
	li a0, 0
	li t0, 0
	call _increment
	call _decrement
	mv t1, t0

	ret

# Should not emit a warning because we only read from safe registers post-call
_func_test2:
	li a0, 0
	li t0, 0
	call _increment
	call _decrement
	mv t0, s0
	mv t1, t0

	ret

# Should warn saying a non-volatile register was not restored
_func_test3:
	li t0, 0
	mv s0, t0

	ret

# Should not emit a warning because s0 is restored before returning
_func_test4:
	mv t1, s0
	mv t0, 0
	mv s0, t0
	mv s0, t1

	ret

