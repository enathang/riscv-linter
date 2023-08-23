_increment:
	li t0, 1
	add a0, a0, t0

	ret

_decrement:
	li t0, -1
	add a0, a0, t0

	ret

_func:
	li a0, 5

	call _increment
	call _increment
	call _decrement
