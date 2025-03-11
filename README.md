# Documentation for 8 bit computer


This is the documentation for a single cycle 8 bit computer design. It is based upon the design of the MIPS microprocessor in the textbook: 
Harris D. M., Harris S. L. - Digital Design and Computer Architecture, 2nd Edition - 2012
## Instruction Set
This computer currently supports 11 different instructions. There are 3 general purpose registers that can be used, and one static register that holds the value 0.


Assembly Commands
### Instruction Table
| INSTR | OpCode | Func | ALUop |
|-------|--------|------|-------|
|add|0000|000010|010|
|sub|0000|000011|110|
|or|0000|000001|001|
|and|0000|000000|000|
|slt|0000|000111|111|
|addi|0001|X|010|
|subi|0010|X|011|
|load|0011|X|010|
|store|0100|X|010|
|beq|0101|X|110|
|jump|0110|X|X|





