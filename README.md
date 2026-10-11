# Linux Commands Implementation in C

A C-based project to understand how common Linux command-line utilities work internally. This project started with implementing basic Linux commands from scratch and was later extended into a simple MiniShell that executes these custom commands.

**Repository:** [linux-commands-Implementation-](https://github.com/KrishChawla2006/linux-commands-Implementation-)

## About the Project

Linux provides many command-line utilities that we use regularly to work with files, directories, permissions, and text. Instead of only using these commands, I wanted to understand how they work behind the scenes.

So, I implemented several commonly used Linux commands in C using standard C libraries and POSIX system calls wherever needed.

After implementing the commands, I took the project a step further by building a **MiniShell** that allows users to execute these custom implementations through a command-line interface.

This project helped me gain practical experience with C programming, file handling, process creation, system calls, and basic shell implementation.

## Implemented Commands

| Command | Description |
|---|---|
| `echo` | Displays text on the terminal. |
| `cat` | Reads and displays file contents. |
| `wc` | Counts lines, words, and bytes in a file. |
| `cp` | Copies files from one location to another. |
| `ls` | Lists files and directories. |
| `chmod` | Changes file permissions. |
| `pwd` | Displays the current working directory. |
| `date` | Displays the current date and time. |
| `grep` | Searches for matching text in files. |

These are custom implementations written in C, rather than simply forwarding commands to the original Linux utilities.

## MiniShell

Once the individual commands were working, I built a simple MiniShell to bring them together into one interface.

The shell accepts user input, parses commands and arguments, and executes the corresponding custom programs.

### Features

- **Command execution:** Runs the implemented commands through their corresponding executables.
- **Built-in commands:** Supports `cd` to change directories and `exit` to close the shell.
- **Quoted arguments:** Handles single and double quotes for inputs containing spaces.
- **Output redirection:** Supports `>` to write output to a file and `>>` to append output to an existing file.
- **Command restrictions:** Executes only the supported custom commands instead of allowing arbitrary system commands.
- **Process management:** Uses `fork()`, `execv()`, and `waitpid()` to execute and manage child processes.

The MiniShell is intentionally kept simple and is not intended to be a complete replacement for Bash. Features such as pipelines, input redirection, and background execution are outside its current scope.

## Project Structure

```text
linux-commands-Implementation-/
│
├── src/
│   ├── my_echo.c
│   ├── my_cat.c
│   ├── my_wc.c
│   ├── my_cp.c
│   ├── my_ls.c
│   ├── my_chmod.c
│   ├── my_pwd.c
│   ├── my_date.c
│   └── my_grep.c
│
├── minishell/
│   └── main.c
│
├── bin/          # Compiled executables
├── tests/        # Test files
├── Makefile
└── README.md
```

The `src/` directory contains the individual command implementations, while `minishell/` contains the shell program. The `bin/` directory holds the compiled executables generated during the build process.

## Requirements

- Linux or WSL with Ubuntu
- GCC compiler
- GNU Make
- Basic knowledge of C programming

## Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/KrishChawla2006/linux-commands-Implementation-.git
cd linux-commands-Implementation-
```

### 2. Compile the project

Build the commands and MiniShell using the Makefile:

```bash
make
```

If you want to rebuild the project from scratch:

```bash
make clean
make
```

### 3. Run individual commands

After compilation, you can run the custom executables from the project root:

```bash
./bin/my_echo "Hello World"
./bin/my_pwd
./bin/my_date
./bin/my_wc README.md
./bin/my_grep "MiniShell" README.md
```

### 4. Start the MiniShell

```bash
./bin/minishell
```

You can now execute the supported commands through the shell.

For example:

```text
myshell> echo "Hello World"
Hello World

myshell> pwd

myshell> date

myshell> echo "First line" > tests/example.txt
myshell> echo "Second line" >> tests/example.txt
myshell> cat tests/example.txt
First line
Second line

myshell> wc tests/example.txt
myshell> grep -n Second tests/example.txt

myshell> cd tests
myshell> pwd

myshell> exit
```

The exact output of commands such as `pwd`, `date`, and `wc` depends on your environment and the files you use.

**Note:** The MiniShell supports a limited set of commands and basic output redirection. It does not support all Bash syntax or every option available in the original Linux utilities.

## Concepts Learned

While building this project, I worked with:

- C programming and command-line arguments (`argc` and `argv`)
- File operations and error handling
- Directory traversal and file permissions
- POSIX system calls
- Process creation and execution using `fork()`, `execv()`, and `waitpid()`
- File descriptor management using `open()` and `dup2()`
- Input parsing, quoted arguments, and output redirection
- Compilation and build automation using GCC and Makefiles

## Future Improvements

Some features I would like to explore next include:

- Adding input redirection using `<`
- Implementing pipes using `|`
- Supporting more command-line options
- Improving argument parsing and error handling
- Adding automated tests for individual commands and MiniShell
- Supporting background processes using `&`

## Author

**Krish Chawla**

Computer Science Engineering — Artificial Intelligence and Machine Learning

GitHub: [@KrishChawla2006](https://github.com/KrishChawla2006)

---

This project is part of my learning journey to understand Linux, C programming, and how command-line tools and shells work internally.
