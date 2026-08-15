# Mini Unix Shell clone v0.2
A simple unix shell clone written in C, with basic commands!
Read version patches in patches.md file

# Features
- Command tokenization
- Built in commands
- External command execution using fork(), execv(), and waitpid()

# Commands
Built in:
- pwd -> shows current directory
- cd DIRECTORY -> change directory to destination
- exit, break or close -> exit the shell
- echo -> echo
External:
- ls:
  - ls
  - ls DIRECTORY
  - ls -a
  - ls -a DIRECTORY
  - ls -l
  - ls -l DIRECTORY
#Requirements
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
