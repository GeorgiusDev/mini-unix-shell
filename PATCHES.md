# All updates, and their history
Here you can view patches, and feature history of this project

# v0.1 Initial version
- Created simple shell with basic built-in commands like: pwd, cd, exit, echo
- Added ls, with flags -l, -a

# v0.2 New built-in commands
- added mkdir, rmdir, touch, rm
- optimised code

# v0.3 Code architecture reconstruction
- reconstructed the code architecture
- better command handling
- fixed small bugs regarding input EOF, and rm
- only "exit" now exits the shell

- *ADDED SUPPORT FOR PROGRAMS AND COMMANDS OUTSIDE THE PROJECT*

# v0.4 Pipeline support
- *Added UNIX pipe support ('|')*
- Optimised code

# v0.5 Cat + Tokenisation rework
- Added cat command
- Complete tokenisation rework (old tokenisation code is commented in case you like it more)
- Support for strings (" ")

# v0.6
- Added grep command

# v0.7
- Added free() at the end of grep.c
- Removed useless code in grep.c
- Commented grep.c, and cat.c

# v0.8
- Added nscmd command
- nscmd searches the system for the specified command instead of using a builtin command or command from this project's /bin directory.