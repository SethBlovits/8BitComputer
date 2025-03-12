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

### Register Addresses 
|Register|Code|Address|
|--------|-------|------|
|Reg 0 | $r0 | 00 |
|Reg A | $rA | 01 |
|Reg B | $rB | 10 |
|Reg C | $rC | 11 |

## Assembly Instruction Breakdown
#### ADD
- Adds the value stored in rs to the value stored in rt, and stores the result in the general register rd.<br/>
```
add rs,rt,rd;
```
---
#### SUB
- Subtracts the value stored in rs to the value stored in rt, and stores the result in the general register rd.<br/>
```
sub rs,rt,rd;
```
---
#### OR
- Perform bitwise or operation on the values stored in rs and rt, and stores the result in the general register rd.<br/>
```
or rs,rt,rd;
```
---
#### AND
- Perform bitwise and operation on the values stored in rs and rt, and stores the result in the general register rd.<br/>
```
and rs,rt,rd;
```
---
#### SLT
- If rs<rt then set rd to 1. If not the set rd to 0. <br/>
```
slt rs,rt,rd;
```
---
#### ADDI
- Add an immediate value to the value stored in rs, then store the result in rd<br/>
```
addi rs,rd,immediate;
```
---
#### SUBI
- Subtract and immediate value from the value stored in rs, then store the result in rd.<br/>
```
subi rs,rd,immediate;
```
---
#### LOAD
- Read out the data stored in data memory into register rd. The address being accessed is found by adding rs and the immediate value together.<br/>
```
load rs,rd,immediate;
```
---
#### STORE
- Store the data in rd in data memory. The address being written to is found by adding rs and the immediate value together.<br/>
```
store rs,rd,immediate;
```
---
#### BEQ
- If the data in registers rs and rd are equal, then branch to the relative address designated by the immediate.<br/>
```
beq rs,rd,immediate;
```
#### JUMP
- Jump to the memory address specified by the rs and rd registers. The 16 bit address is set by setting the bottom 8 bits to be the data in rs, and the top 8 bits to be the data in rd.<br/>
```
jump rs,rd; <br/>
```
---
#### JUMPF
- Jumpf allows you to jump to an instruction address of the start of a function. Due to space limitations, jumpf breaks down into 3 sub functions. The address represented by label is split into two 8 bit pieces, which are stored in rs and rd by two addi calls. Then a jump command is executed using these two registers.<br/>
```
jumpf rs,rd,label;
```
Boils down into:
```
addi $r0,rs,label[7:0];
addi $r0,rd,label[15:8];
jump rs,rd;
```





