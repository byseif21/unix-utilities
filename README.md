# Unix Utilities in C

This repository contains simple implementations of basic Unix utilities in C.

## Included Utilities:
1. **pwd** - Prints the current working directory.
2. **echo** - Prints user input.
3. **cp** - Copies a file from source to destination.
4. **mv** - Moves/renames a file.
5. **FemtoShell** - A minimal shell that supports `echo` and `exit`.

## Compilation
Compile each program separately using `gcc`:
```sh
gcc -o pwd pwd.c
gcc -o echo echo.c
gcc -o cp cp.c
gcc -o mv mv.c
gcc -o femtoshell femtoshell.c
