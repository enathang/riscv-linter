# riscv-linter

This is a linter for RISC-V assembly. It builds off the RISC-V parser written by Autin Henley (see [Parsing RISC-V](https://web.eecs.utk.edu/~azh/blog/parsingriscv.html).)

You can read more about the linter here: once I add the link ;)


## Motivation
This linter is a result of three things:
- I've been writing an OS and learning the RISC-V ABI at the same time. As a result, most of the early assembly code I've written does not comply 100% with the ABI. I'd like a tool that can point out to me exactly *where* and *why* my assembly does not comply with the ABI. I'd also like to verify my fixes do comply with the ABI.
- I've spent an embarassing amount of time debugging my assembly code to realize I forgot to add a `jump` or `return` statement at the end. Since I want to always be explicit about my control flow and never fall through, I'd like a linter to check and quickly point out when I have forgotten to be explicit.
- I was surprised that I could not readily find an existing tool to lint assembly. So maybe this helps the next poor soul writing an OS. 


## Try it out
You should be able to build it with `make`. It builds with C++20 for convenience.
You can then run the executable with the path to a file.


## High level structure of code
Building off the existing parser, we define a few new components:
- Linter
- RiscvDict(ionary)
- CodeBlock

Basically, as the parser parses the code, once it has a construct (ex. full instruction) we pass the instruction to the linter to...well, lint it. The linter maintains a "code block" state. A code block is simply the state of the registers and some other high level concepts (such as 'have we seen a return statement yet'). Whenever we enter a code block (as defined by parsing a label with some instructions in/below it) we reset the code block to a state guaranteed by the RISC-V ABI. We update the state of the code block as we parse the assembly, keeping track of which registers are safe to read from vs not (per the ABI) and the values registers contain when we "leave the code block" (start parsing next label). 

Example:
```
_func:                # Step 0: Per ABI, t0 is considered unsafe to read from because we cannot guarantee what value it will hold
    li t0, 5          # Step 1: We now consider t0 to be safe to read from, because we can guarantee the value
    call _other_func  # Step 2: Per ABI, we can no longer guarantee t0 was not modified by _other_func, so it's considered unsafe again
    mv a0, t0         # Step 3: Here, we will throw a linting error saying it's unsafe to read from t0 because the ABI cannot guarantee the value
                      
_other_func:          # Step 4: Now that we're ending our code block here because we see a new label, we do any final code-block checks (warn that there's no ret)
...
```

The RiscvDict contains a dictionary for parsing the RISC-V assembly instructions from documentation (because there are a lot). The instruction definitions were taken from (https://github.com/michaeljclark/riscv-meta/blob/master/opcodes), which is pretty out of date. Pseudo-ops are not included in the file, so I add a few common ones in the code.

## Limitations
First, the linter is somewhat brittle. I've tried to make it flexible enough to handle the basics of assembly. It works on my code (which is akin to "it works on my machine" of tool development) with some minor modifications. Some limitations and bugs are noted in `src/linter/notes.md`. 

The linter tries to provide some of the higher level guarantees of safety. But not fully guaranteed. For example, pushing/popping the stack is assumed to always be valid, but that may not be true in practice (I started implementing such a check but have not finished it). Another example is `a0/a1` are not checked for safety. They cannot be checked because they are volatile registers but also used for return values, so it's unclear is reading from them is a bug or intended. 

The linter also lints for a specific assembly style (hint: mine) and will raise warnings on assembly that may be perfectly valid. I've tried to make the checks easy-ish to remove from the code. However, since the linting logic is state-machine based (as opposed to rule-based) it may not be super easy to add/remove linting rules.

## Other thoughts
I wonder if reverse-compiling the assembly to C code would perform the same checks as my linter? Idle thoughts.
