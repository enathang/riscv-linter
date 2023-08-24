Notes:

The linter could be expanded to include: 
- annotations to support stack push/pop

The more I add to the tool, the less I am satisfied with it. Perhaps this has to do with expectation vs reality. As a very simple tool, I'm grateful whenever it points out something I missed. The more I add to it though, the more I expect it to give me stronger guarantees and the more I find it lacking that it does not. Even though it gives me as strong or stronger guarantees than when it was simpler. I;m trying to think on a higher level of abstraction, which is dangerous for an incomplete tool.

The tool has become somewhat chimera-esque in that it does not have a well-defined user or use scenario. Originally, I wrote it for myself, but then kept adding onto it until it had multiple half-ideas.

Bug list:
- Parser
    - The parser will error on (t0) (but not 0(t0))
    - The parser does not recognize `/*` and `*/`
    - Does not support macros with arguments 
- Linter
    - The linter will recognize `mv t0, s0` as a way of copying a register value but not `addi t0, s0, 0` even though they're functionally the same (and `mv` even assembles down to the `addi` instruction). 


Todo:
- Fix above bugs
- Remove hardcoded addition to RiscvDict
- Add annotations to add/remove register values to/from linter stack
