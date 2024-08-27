
# Assembly Preprocessing and Parsing Project

## Overview

This project focuses on the preprocessing and parsing stages of an assembly language compiler. The program processes assembly language files, handling preprocessing directives and parsing the code into an intermediate representation.

## Project Structure

The project is divided into several C source and header files, each responsible for a specific aspect of preprocessing and parsing:

- **`pre_assembler1.c`**: The main program file responsible for managing the overall flow of the preprocessing and parsing stages.
- **`pre_assembler_functions.c` & `pre_assembler_functions.h`**: Functions and definitions related to the preprocessing phase of the assembler, handling tasks like removing comments and expanding macros.
- **`part2read.c` & `part2read.h`**: Functions responsible for reading and interpreting assembly language commands and converting them into a format suitable for further processing.
- **`part2_functions.c` & `part2_functions.h`**: Additional functions that assist in the processing and validation of assembly commands.
- **`shared_functions.c` & `shared_functions.h`**: Shared utility functions used across different modules, providing common functionality required during the preprocessing and parsing stages.
- **`makefile`**: The makefile for building the project. It includes rules for compiling and linking the different components of the project.

## Compilation Instructions

To compile the project, ensure you are in the directory containing the source files and `Makefile`. Use the following command:

```bash
make
```

This command will compile the project, generating the necessary object files and the final executable.

## Running the Program

After compiling, you can run the program by providing an assembly language file as input. The program will preprocess the file, handle macros, remove comments, and then parse the code.

### Example Usage:

```bash
./pre_assembler input_file.as
```

The program will output an intermediate file, ready for further stages in the assembly process.

## Key Features

- **Preprocessing**: Handles macros and comments, ensuring the assembly code is clean and expanded before parsing.
- **Parsing**: Identifies and validates instructions, labels, and operands in the assembly code.
- **Modular Design**: The program is organized into several modules, making it easier to maintain and extend.

## Files Included

- **Source Files**:
  - `pre_assembler1.c`
  - `pre_assembler_functions.c`
  - `part2read.c`
  - `part2_functions.c`
  - `shared_functions.c`
  
- **Header Files**:
  - `pre_assembler_functions.h`
  - `part2read.h`
  - `part2_functions.h`
  - `shared_functions.h`

- **Makefile**: For compiling the project.

## Conclusion

This project implements a modular assembly language preprocessing and parsing system. It ensures that assembly code is correctly prepared for the subsequent stages of the compilation process, making it a crucial component in the assembly pipeline.
