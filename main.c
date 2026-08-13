#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h>
#include <errno.h>


#define RESET "\x1b[0m"

#define BLACK   "\x1b[30m"
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"

#define BRIGHT_BLACK   "\x1b[90m"
#define BRIGHT_RED     "\x1b[91m"
#define BRIGHT_GREEN   "\x1b[92m"
#define BRIGHT_YELLOW  "\x1b[93m"
#define BRIGHT_BLUE    "\x1b[94m"
#define BRIGHT_MAGENTA "\x1b[95m"
#define BRIGHT_CYAN    "\x1b[96m"
#define BRIGHT_WHITE   "\x1b[97m"


#define INPUT_SIZE 1024
#define MAX_ARGS 32


typedef struct
{
    char *command;
    char *args[MAX_ARGS];
    int argc;
} ParsedCommand;

void enableANSI(void);
void printBanner(void);
void printPrompt(void);

void parseCommand(char *input, ParsedCommand *cmd);

void commandHelp(void);
void commandAbout(void);
void commandVersion(void);
void commandClear(void);
void commandEcho(ParsedCommand *cmd);
void commandMkdir(ParsedCommand *cmd);
void commandRmdir(ParsedCommand *cmd);
void commandPwd(void);
void commandLs(void);
void commandCd(ParsedCommand *cmd);
void commandDel(ParsedCommand *cmd);
void commandTouch(ParsedCommand *cmd);
void commandCat(ParsedCommand *cmd);
void commandCp(ParsedCommand *cmd);
void commandMv(ParsedCommand *cmd);

void listFiles(void);
void changeDirectory(const char *path);


void enableANSI(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    if (hOut == INVALID_HANDLE_VALUE)
        return;

    DWORD dwMode = 0;

    if (!GetConsoleMode(hOut, &dwMode))
        return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);
}

void printBanner(void)
{
    printf(
        GREEN
        "                          _                                             __         ____\n"
        "   __  ______ ___________(_)___     ____  ____ _      _____  __________/ /_  ___  / / /\n"
        "  / / / / __ `/ ___/ ___/ / __ \\   / __ \\/ __ \\ | /| / / _ \\/ ___/ ___/ __ \\/ _ \\/ / / \n"
        " / /_/ / /_/ (__  |__  ) / / / /  / /_/ / /_/ / |/ |/ /  __/ /  (__  ) / / /  __/ / /  \n"
        " \\__, /\\__,_/____/____/_/_/ /_/  / .___/\\____/|__/|__/\\___/_/  /____/_/ /_/\\___/_/_/   \n"
        "/____/                          /_/                                                    \n"
        RESET
    );

    printf(
        CYAN "===================================================\n" RESET
        "              Welcome to YShell v1.0\n"
        CYAN "===================================================\n\n" RESET
    );
}


void printPrompt(void)
{
    char path[MAX_PATH];

    DWORD result = GetCurrentDirectoryA(MAX_PATH, path);

    if (result == 0)
    {
        printf("[yassin" GREEN "@" RESET "localhost "
               RED "~ ?" RESET "] "
               YELLOW "$ " RESET);
        return;
    }

    char *last = strrchr(path, '\\');

    if (last != NULL)
        last++;
    else
        last = path;

    printf(
        "[yassin" GREEN "@" RESET "localhost "
        RED "~ %s" RESET "] "
        YELLOW "$ " RESET,
        last
    );
}


void parseCommand(char *input, ParsedCommand *cmd)
{
    cmd->command = NULL;
    cmd->argc = 0;

    for (int i = 0; i < MAX_ARGS; i++)
        cmd->args[i] = NULL;

    char *token = strtok(input, " \t");

    if (token == NULL)
        return;

    cmd->command = token;

    while ((token = strtok(NULL, " \t")) != NULL)
    {
        if (cmd->argc >= MAX_ARGS)
            break;

        cmd->args[cmd->argc] = token;
        cmd->argc++;
    }
}


void commandHelp(void)
{
    printf(
        "\n"
        CYAN "+------------------------------------------------+\n"
        "|                Available Commands              |\n"
        "+------------------------------------------------+\n"
        RESET

        BLUE "  help" RESET "              Show this help menu\n"
        BLUE "  about" RESET "             About YShell\n"
        BLUE "  version" RESET "           Show YShell version\n"
        BLUE "  clear" RESET "             Clear the terminal\n"
        BLUE "  cls" RESET "               Alias for clear\n"
        BLUE "  exit" RESET "              Exit YShell\n"
        BLUE "  echo <text>" RESET "       Print text\n"
        BLUE "  pwd" RESET "               Show current directory\n"
        BLUE "  cd <dir>" RESET "          Change directory\n"
        BLUE "  ls" RESET "               List files\n"
        BLUE "  dir" RESET "              Alias for ls\n"
        BLUE "  mkdir <dir>" RESET "      Create directory\n"
        BLUE "  rmdir <dir>" RESET "      Remove directory\n"
        BLUE "  del <file>" RESET "       Delete file\n"
        BLUE "  touch <file>" RESET "     Create empty file\n"
        BLUE "  cat <file>" RESET "       Display file contents\n"
        BLUE "  cp <src> <dst>" RESET "   Copy file\n"
        BLUE "  mv <src> <dst>" RESET "   Move/rename file\n"

        "\n"
    );
}


void commandAbout(void)
{
    printf(
        "\n"
        GREEN "YShell v1.0\n" RESET
        "A custom Windows shell written in C.\n"
        "Developed by Yassin.\n"
        "Built for learning C, Windows API and cybersecurity.\n\n"
    );
}


void commandVersion(void)
{
    printf(
        "\n"
        CYAN "YShell Version 1.0\n" RESET
        "Build: Development\n\n"
    );
}
void commandClear(void)
{
    system("cls");
    printBanner();
}


void commandEcho(ParsedCommand *cmd)
{
    for (int i = 0; i < cmd->argc; i++)
    {
        printf("%s", cmd->args[i]);

        if (i < cmd->argc - 1)
            printf(" ");
    }

    printf("\n");
}

void commandMkdir(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        printf(
            RED "Usage: mkdir <directory>\n" RESET
        );
        return;
    }

    if (_mkdir(cmd->args[0]) == 0)
    {
        printf(
            GREEN "Directory created successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error: %s\n" RESET,
            strerror(errno)
        );
    }
}


void commandRmdir(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        printf(
            RED "Usage: rmdir <directory>\n" RESET
        );
        return;
    }

    if (_rmdir(cmd->args[0]) == 0)
    {
        printf(
            GREEN "Directory deleted successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error: %s\n" RESET,
            strerror(errno)
        );
    }
}


void commandPwd(void)
{
    char path[MAX_PATH];

    DWORD result = GetCurrentDirectoryA(
        MAX_PATH,
        path
    );

    if (result == 0)
    {
        printf(
            RED "Failed to get current directory.\n" RESET
        );
        return;
    }

    printf(
        "%s\n",
        path
    );
}


void listFiles(void)
{
    WIN32_FIND_DATAA file;
    HANDLE hFind;

    hFind = FindFirstFileA("*", &file);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf(
            RED "Unable to open directory.\n" RESET
        );
        return;
    }

    printf("\n");

    printf(
        "+--------+--------------------------------+\n"
        "| Type   | Name                           |\n"
        "+--------+--------------------------------+\n"
    );

    do
    {
        if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf(
                "|" CYAN " <DIR>  " RESET
                "| %-30s |\n",
                file.cFileName
            );
        }
        else
        {
            printf(
                "|" GREEN " <FILE> " RESET
                "| %-30s |\n",
                file.cFileName
            );
        }

    } while (FindNextFileA(hFind, &file));

    printf(
        "+--------+--------------------------------+\n\n"
    );

    FindClose(hFind);
}

void changeDirectory(const char *path)
{
    if (SetCurrentDirectoryA(path))
    {
        printf(
            GREEN "Directory changed successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error: Directory not found or inaccessible.\n" RESET
        );
    }
}

void commandCd(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        commandPwd();
        return;
    }

    changeDirectory(cmd->args[0]);
}


void commandDel(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        printf(
            RED "Usage: del <file>\n" RESET
        );
        return;
    }

    if (DeleteFileA(cmd->args[0]))
    {
        printf(
            GREEN "File deleted successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error deleting file. Windows error code: %lu\n" RESET,
            GetLastError()
        );
    }
}


void commandTouch(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        printf(
            RED "Usage: touch <file>\n" RESET
        );
        return;
    }

    HANDLE hFile = CreateFileA(
        cmd->args[0],
        GENERIC_WRITE,
        0,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf(
            RED "Error creating file. Code: %lu\n" RESET,
            GetLastError()
        );
        return;
    }

    CloseHandle(hFile);

    printf(
        GREEN "File created successfully.\n" RESET
    );
}

void commandCat(ParsedCommand *cmd)
{
    if (cmd->argc < 1)
    {
        printf(
            RED "Usage: cat <file>\n" RESET
        );
        return;
    }

    FILE *file = fopen(
        cmd->args[0],
        "r"
    );

    if (file == NULL)
    {
        printf(
            RED "Error: %s\n" RESET,
            strerror(errno)
        );
        return;
    }

    char buffer[512];

    while (fgets(buffer, sizeof(buffer), file))
    {
        printf("%s", buffer);
    }

    fclose(file);

    printf("\n");
}
void commandCp(ParsedCommand *cmd)
{
    if (cmd->argc < 2)
    {
        printf(
            RED "Usage: cp <source> <destination>\n" RESET
        );
        return;
    }

    if (CopyFileA(
            cmd->args[0],
            cmd->args[1],
            FALSE
        ))
    {
        printf(
            GREEN "File copied successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error copying file. Code: %lu\n" RESET,
            GetLastError()
        );
    }
}


void commandMv(ParsedCommand *cmd)
{
    if (cmd->argc < 2)
    {
        printf(
            RED "Usage: mv <source> <destination>\n" RESET
        );
        return;
    }

    if (MoveFileA(
            cmd->args[0],
            cmd->args[1]
        ))
    {
        printf(
            GREEN "File moved successfully.\n" RESET
        );
    }
    else
    {
        printf(
            RED "Error moving file. Code: %lu\n" RESET,
            GetLastError()
        );
    }
}


bool executeCommand(ParsedCommand *cmd)
{
    if (cmd->command == NULL)
        return true;

    if (strcmp(cmd->command, "help") == 0)
    {
        commandHelp();
    }

    else if (strcmp(cmd->command, "about") == 0)
    {
        commandAbout();
    }

    else if (strcmp(cmd->command, "version") == 0)
    {
        commandVersion();
    }

    else if (
        strcmp(cmd->command, "clear") == 0 ||
        strcmp(cmd->command, "cls") == 0
    )
    {
        commandClear();
    }

    else if (strcmp(cmd->command, "echo") == 0)
    {
        commandEcho(cmd);
    }

    else if (strcmp(cmd->command, "mkdir") == 0)
    {
        commandMkdir(cmd);
    }

    else if (strcmp(cmd->command, "rmdir") == 0)
    {
        commandRmdir(cmd);
    }

    else if (strcmp(cmd->command, "pwd") == 0)
    {
        commandPwd();
    }

    else if (
        strcmp(cmd->command, "ls") == 0 ||
        strcmp(cmd->command, "dir") == 0
    )
    {
        listFiles();
    }

    else if (strcmp(cmd->command, "cd") == 0)
    {
        commandCd(cmd);
    }

    else if (strcmp(cmd->command, "del") == 0)
    {
        commandDel(cmd);
    }

    else if (strcmp(cmd->command, "touch") == 0)
    {
        commandTouch(cmd);
    }

    else if (strcmp(cmd->command, "cat") == 0)
    {
        commandCat(cmd);
    }

    else if (strcmp(cmd->command, "cp") == 0)
    {
        commandCp(cmd);
    }

    else if (strcmp(cmd->command, "mv") == 0)
    {
        commandMv(cmd);
    }

    else if (strcmp(cmd->command, "exit") == 0)
    {
        printf(
            GREEN "\nGoodbye!\n" RESET
        );

        return false;
    }

    else
    {
        printf(
            RED "Unknown command: %s\n" RESET,
            cmd->command
        );
    }

    return true;
}

int main(void)
{
    char input[INPUT_SIZE];

    bool running = true;

    enableANSI();

    printBanner();

    while (running)
    {
        printPrompt();

        if (fgets(
                input,
                sizeof(input),
                stdin
            ) == NULL)
        {
            break;
        }

        input[strcspn(input, "\r\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        ParsedCommand cmd;

        parseCommand(
            input,
            &cmd
        );

        running = executeCommand(
            &cmd
        );
    }

    return 0;
}
