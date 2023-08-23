Notes:

A segfault will occur if a RiscvDict entry has an empty vector of operandRoles

The parser will error on (t0) but not 0(t0)

The parser does not recognize `/*` and `*/`

Todo:
- Fix above bugs
- Expand test cases to cover other linter edge cases
- Remove hardcoded addition to RiscvDict
