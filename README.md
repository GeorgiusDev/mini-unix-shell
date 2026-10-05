# Mini Unix Shell clone v1.0
A simple unix shell clone written in C, with basic commands!
Read version patches in patches.md file

# Features
- Command tokenization
- Built in commands
- External command execution using fork(), execv(), and waitpid()
- Execution of programs, and commands outside this project!
- Unix pipe support ('|'), including multiple chained pipes
- File descriptor manipulation
- Basic error handling for system calls
- Strings support (" ")
- nscmd command which searches the system for the specified command instead of using a builtin command or command from this project's /bin directory.

# Commands
Built in:
- pwd -> shows current directory
- cd DIRECTORY -> change directory to destination
- exit -> exit the shell
- echo TEXT -> TEXT
- mkdir NAME -> creates the directory with name "NAME"
- rmdir NAME -> deletes the directory with name "NAME"
- touch FILE -> creates the file with name "FILE"
- rm FILE -> deletes the file with name "FILE"
- nscmd [COMMAND] [ARGUMENTS] -> searches the system for the specified command instead of using a builtin command or command from this project's /bin directory.

External:
- ls:
  - ls
  - ls DIRECTORY
  - ls -a
  - ls -a DIRECTORY
  - ls -l
  - ls -l DIRECTORY
  
- cat
  - cat
  - cat FILE
  
- grep
  - grep PATTERNS
  - grep PATTERNS [FILE]
  
*PIPES*:
- Commands can be connected using '|':
- ls | cat
- echo Hello | cat
  
# Requirements
- Linux / Unix-like operating system
- GCC
- Make

# Building
- git clone https://github.com/GeorgiusDev/mini-unix-shell
- cd mini-unix-shell
- make
- ./shell

For cleaning compiled files: make clean

# License
This project is licensed under the MIT License You are free to use, modify, and share this project, but you must include credit by keeping the copyright notice in LICENSE.txt file

Copyright (c) 2026 GeorgiusDev

https://www.georgius.dev
