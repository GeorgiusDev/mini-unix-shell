# All updates, and their history after release
Here you can view patches, and feature history of this project after release

# v1.0 First stable/major version
- First stasble version of this shell
- Built-in commands like cd, pwd, exit, echo, rm, rmdir, touch, mkdir, nscmd
- ls, cat, grep, and also ability to use external commands
- pipeline support

# v1.1 New commands
- Added clear command
- Added whoami command
- Added hostname command

# v1.2 added username and hostname to shell prompt
- instead of shell prompt being just "$: ", now it's "username@hostname$: "

# v1.3 COLORS!
- finally, this shell is no longer souless black and white terminal
- I didn't know where to put colors, so you can always just include "colors.h" and put them wherever you like, it's pretty easy!

# v1.4.0 HISTORY!
- for now added history!
- it is saved in buffer variable, untill 100 commands are typed in one session, or "exit" is used. Then it is saved in shell/bin/history.txt file!

- 1.4.1
    - cleaned up main.c

- 1.4.2
    - added "history" command, which prints the list of all commands ever used on standard output

- 1.4.3
    - added numbered entries to command history