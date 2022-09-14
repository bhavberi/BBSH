# **BB-SHELL [_BBSH_ ]** (_V1.3_ )

_Made for an assignment/s for OS&N Course Monsoon 2022-23._

_Completely coded in `C`._

## Build & Run
1. `make` / `make bbsh` (For building and running the file)
2. `make clean` (For cleaning the built files)
3. `make full` (For 1. + 2.)

## ABOUT
_BB-SHELL_ is a very ~~advanced~~ noob shell, which can do some work, but very limited. :sweat_smile:

It has some of the following features as of now:- (More may ~~not~~ come later)

- Colourful shell display with username/hostname and current directory.
- Can run some built-in commands such as `cd`, `echo`, `pwd`, `ls` (With -a and -l flags).
- Other Commands - `pinfo` (≈ ps), `discover` (≈ find with -d and -f flags)
- Stores `history` of upto 20, for all the sessions.
- Error Handling
- Can run both `background` qand `foreground` processes. (For any other shell commands)
- Multiple Command Execution in a single line using `;`.
- `exit` is used to exit the shell.
- I/O Redirection using pipes and < & >/>>.

### OTHER COMMANDS & INSTRUCTIONS
- #### `cd`
    ```
    Usage: cd <path_to_directory>
    ```
    Supports the following wildcards:
    - `~` : path to shell's home directory
    - `-` : path to previous directory
    - `.` : path to current directory
    - `..` : path to parent directory
- #### `pwd`
    ```
    Usage: pwd
    ```
- #### `echo`
    ```
    Usage: echo <string>
    ```
- #### `ls`
    ```
    Usage: ls <path_to_directory_1> <path_to_directory_2> ...
    ```
    Flags:-
    - `-a`: do not ignore entries starting with .
    - `-l`: use a long listing format

    Supports all the wildcards as `cd`.
- #### `pinfo`
    ```
    Usage: pinfo <pid>
    ```
    Gives the following info:
    - Process ID
    - Process Status
    - Virtual Memory
    - Path to Executable
- #### `discover`
    ```
    Usage: discover <target_dir> "<file_name>"
    ```
    Flags:-
    - `-d`: searches for all directories
    - `-f`: searches for all files

    All arguments/flags are optional to use.
- #### `exit`
    ```
    Usage: exit
    ```
    Alaias for `exit` -> `quit`
- #### `history`
    ```
    Usage: history
    ```
    Prints the last `10` commands in the history.
- #### `<command>`
    - Runs the command in the foreground.
    - If `&` is added at the end, will run the command in background.


---
## _FOR DEVELOPERS_
---
### FILES

1. `main.c` -> Containing the parent code to run the shell. Just mainly calling different functions.
2. `libraries.h` -> Containing all the imports and in general definitions.
3. `input.c` and `input.h` -> Containing the functions to take input, process it and do tasks based on that. Also, handles the I/O Redirection processes.
4. `print.c` and `print.h` -> Containing the code to print prompt, errors, etc.
5. `signals.c` and `signals.h` -> Containing the functions to handle different basic signals and process them accordingly.
6. `path.c` and `path.h` -> Containing the functions to process different directories and paths.
7. `history.c` and `history.h` -> Reading, Processing and Writing History.
8. `discovery.c` and `discovery.h` -> Handles the code for `discovery` command and its variants.
9. `jobpool.c` and `jobpool.h` -> Manages the current running processes in the shell, along with doing book-keeping for the background processes.
10. `helpers.c` and `helpers.h` -> Some basic functions for using at different places.
11. `commands.c` and `commands.h` -> _Imp. functions_ for different commands and all of their implementation.

### _New Features coming soon...._ (V2.0)