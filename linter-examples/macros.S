.macro macro_name
	mv t0, t0
.endm

# This function will raise a warning that the instruction 'macro_name' cannot be found.
# In order to lint macros, one needs to preprocess the assembly file to have macro-replacement 
_func:
	li t0, 0
	li t1, 1
	macro_name t0, t1

	ret

