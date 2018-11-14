# MAD Shell

An alternative to Linux Bash shell written in C.

## Built-in Commands

| Key/Command | Description |
| ----------- | ----------- |
| pwd | Full path to working directory |
| ls | Short listing of directory |
| ls -l | Long listing of directory |
| cd | Change Directory |
| cp [file] [newfile] | Copy file to file |
| mkdir [dir] | Create new directory |
| rmdir [dir] | Remove directory (only operates on empty directories ) |
| help | Shows Help regarding the shell |
| exit | Exit the shell |
| env [var] [new value] | Sets the env variable to the new value | 

## Other Commands

Other commands are run by replacing the current process using execvp and if such command does not exist, then it is run as if it is the name of an executable file ("./a.out")

## Extra Features

Command History   
Multiple Piping (with `|`)  
Changing Environmental Variables  
I/O Redirection (with `<` for input and `>` for output)  
Background Execution (with `&` at the end of command)  