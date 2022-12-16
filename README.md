COMP 1500 Assembly Emulator
===========================

"COMP 1500: Computing Ideas and Innovation" is a course offered in the Department of Computer Science at the University of Manitoba.

In Fall 2017, we offered a unit where students wrote programs in a fictional assembly language. This is an emulator for that assembly language.

Building
--------

```bash
make c1500
```

Running
-------

The emulator expects programs to be written in plain-text files. Pass the plain-text file as the first argument to the emulator. Input for the program is provided on `STDIN` and output is written to `STDOUT`.

```bash
$ echo "1 2 3 4 -1" | ./c1500 test-data/prog.txt
10
$ 
```

Assembly
========

The assembly language has a very small number of instructions.

| Instruction            | Effect                                                      |
| ---------------------- | ----------------------------------------------------------- |
| `SAVE address`         | Saves the ALU output to the specified memory address.       |
| `STORE value, address` | Stores the literal integer `value` at the specified address |
| `GETA address`         | Load the value from `address` into ALU register A           |
| `GETB address`         | Load the value from `address` into ALU register B           |
| `ADD`                  | Instruct the ALU to add its operands                        |
| `SUB`                  | Instruct the ALU to subtract its operands                   |
| `MULT`                 | Instruct the ALU to multiply its operands                   |
| `GETINPUT`             | Get input from the input device (the keyboard, `STDIN`)     |
| `OUTPUT`               | Write to the output device (`STDOUT`)                       |
| `JUMP address`         | Unconditional jump to instruction at `address`              |
| `JUMPIFZERO address`   | Jump to instruction at `address` if ALU output is zero      |

The address space for instructions is separate from the data address space. That is, instructions that read and write from memory cannot read and write instructions.
