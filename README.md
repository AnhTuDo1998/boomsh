# boomsh
A miniature shell in C

## Lifetime of a shell
The shell do 3 things in it lifetime:
- Initialize: read and execute configuration files.
- Interpret: read in commands and executes them
- Terminate: Clean up and exit the shell program.

## Build
Currently using readline library and header, we must get the header file through our package manager and link it together with binary during compilation:
```
sudo apt install readline-dev
mkdir build && cd build
gcc -g -Wall ../src/boomsh.c -L/usr/include -lreadline -o boomsh
```

Execute the shell by
```
./boomsh
```