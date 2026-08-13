# YShell

YShell is a lightweight custom command-line shell written in C for Windows.

The project was created to improve my understanding of C programming, Windows APIs, file systems, command parsing, and operating system concepts.

> YShell is currently a learning project and is under active development.

---

## Features

YShell currently supports:

- Directory navigation
- File management
- File creation and reading
- File and directory deletion
- File copying and moving
- ANSI terminal colors
- Command parsing
- Windows API integration
- Basic error handling

---

## Supported Commands

| Command | Description |
|---|---|
| `help` | Display available commands |
| `about` | Display information about YShell |
| `version` | Display the current version |
| `clear` | Clear the terminal |
| `cls` | Alias for `clear` |
| `exit` | Exit YShell |
| `echo <text>` | Print text to the terminal |
| `pwd` | Display the current directory |
| `cd <directory>` | Change the current directory |
| `ls` | List files and directories |
| `dir` | Alias for `ls` |
| `mkdir <directory>` | Create a directory |
| `rmdir <directory>` | Remove a directory |
| `del <file>` | Delete a file |
| `touch <file>` | Create an empty file |
| `cat <file>` | Display file contents |
| `cp <source> <destination>` | Copy a file |
| `mv <source> <destination>` | Move or rename a file |

---

## Technologies

- C
- Windows API
- Windows File System API
- ANSI Escape Codes
- Git
- GitHub

### Windows APIs and Functions

The project currently uses several Windows APIs and functions, including:

```text
GetCurrentDirectory
SetCurrentDirectory
FindFirstFile
FindNextFile
DeleteFile
CreateFile
CopyFile
MoveFile
GetLastError