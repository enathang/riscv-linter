# Should warn that _increment_no_ret has no control flow (even though not invoked)
_increment_no_ret:
	li t0, 1
	add a0, a0, t0

# Should not emit a control flow warning because there's no code in this label
_random_label:
	.skip 4096

_func_jump:
	li t0, 1
	j _increment_no_ret

# Check that the linter still works at the end of the file
_func_no_ret_2:
	li t0, 0
