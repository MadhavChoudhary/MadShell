# MAD Shell

An alternative to Linux Bash shell written in C.

## Built-in Commands
cd  
help    
exit  
pwd  
env  
mkdir  
rmdir  
ls  
cp  

## Other Commands

Other commands are run by replacing the current process using execvp and if such command does not exist, then it is run as if it is the name of an executable file ("./a.out")

## Extra Features

Command History   
Multiple Piping (with `|`)  
Changing Environmental Variables  
I/O Redirection (with `<` for input and `>` for output)  
Background Execution (with `&` at the end of command)  