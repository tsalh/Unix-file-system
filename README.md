# UNIX-like File System

This project implements the graph structure of a UNIX-like file system. Creates
a basic API for a UNIX-like file system implementing basic UNIX commands. This
project takes commands from an input file and prints the output to a file. When
user quits the program all files and directories are destroyed making no actual
changes to OS.

## Compiling and Running

When I originally did the project I did not use a Makefile.
```bash
$ g++ *.cpp
$ ./a.out <input file> <output file>
```

### Commands Supported
NOTE: Does not support executing multiple commands at once or flags

```bash
$ mkdir <name>  # Make a directory "name"
$ chdir <name>  # Change directory to "name"
$ rmdir <name>  # Remove directory "name"
$ ls 		    # list files in current working directory
$ lsr 		    # Recursively list files in CWD and all subdirectories
$ mkfile <name> # Create a file in the CWD
$ rmfile <name> # Removes a file in the CWD
$ pwd			# Prints full path of current working directory
$ Q				# exit file system
```