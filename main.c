#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
#include <direct.h>
#include <errno.h>
// Reset
#define RESET "\x1b[0m"

// Regular Colors
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

// Bright Colors
#define BRIGHT_BLACK "\x1b[90m"
#define BRIGHT_RED "\x1b[91m"
#define BRIGHT_GREEN "\x1b[92m"
#define BRIGHT_YELLOW "\x1b[93m"
#define BRIGHT_BLUE "\x1b[94m"
#define BRIGHT_MAGENTA "\x1b[95m"
#define BRIGHT_CYAN "\x1b[96m"
#define BRIGHT_WHITE "\x1b[97m"

// Background Colors
#define BG_BLACK "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GREEN "\x1b[42m"
#define BG_YELLOW "\x1b[43m"
#define BG_BLUE "\x1b[44m"
#define BG_MAGENTA "\x1b[45m"
#define BG_CYAN "\x1b[46m"
#define BG_WHITE "\x1b[47m"

// Bright Background Colors
#define BG_BRIGHT_BLACK "\x1b[100m"
#define BG_BRIGHT_RED "\x1b[101m"
#define BG_BRIGHT_GREEN "\x1b[102m"
#define BG_BRIGHT_YELLOW "\x1b[103m"
#define BG_BRIGHT_BLUE "\x1b[104m"
#define BG_BRIGHT_MAGENTA "\x1b[105m"
#define BG_BRIGHT_CYAN "\x1b[106m"
#define BG_BRIGHT_WHITE "\x1b[107m"
void listFiles()
{
    WIN32_FIND_DATA file;
    HANDLE hFind;

    hFind = FindFirstFile("*", &file);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("Unable to open directory.\n");
        return;
    }
    printf("\n");
    printf("+------+-----------------------------+\n");
    printf("|Type  | name                        |\n");
    printf("+------+-----------------------------+\n");
    do
    {
        if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            printf("|"CYAN"<DIR> "RESET"| %-28s|\n", file.cFileName);
        }
        else
        {
            printf("|"GREEN"<FILE>"RESET"| %-28s|\n", file.cFileName);
        }

    } while (FindNextFile(hFind, &file));
    printf("+------+-----------------------------+\n");
    printf("\n");
    FindClose(hFind);
}
void enableANSI()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;

    SetConsoleMode(hOut, dwMode);
}

void printBanner()
{
    printf( GREEN

            "                          _                                             __         ____\n"

            "   __  ______ ___________(_)___     ____  ____ _      _____  __________/ /_  ___  / / /\n"

            "  / / / / __ `/ ___/ ___/ / __ \\   / __ \\/ __ \\ | /| / / _ \\/ ___/ ___/ __ \\/ _ \\/ / / \n"

            " / /_/ / /_/ (__  |__  ) / / / /  / /_/ / /_/ / |/ |/ /  __/ /  (__  ) / / /  __/ / /  \n"

            " \\__, /\\__,_/____/____/_/_/ /_/  / .___/\\____/|__/|__/\\___/_/  /____/_/ /_/\\___/_/_/   \n"

            "/____/                          /_/                                                    \n" RESET

            );

    printf(
        CYAN "===================================================\n" RESET
        "          Welcome to YShell v1.0\n"
        CYAN "===================================================\n\n" RESET
    );
}

void printPrompt()
{
    printf("[yassin" GREEN "@" RESET "localhost " RED "~" RESET "]" YELLOW "$ " RESET);
}

int main()
{
    char command[100];
    bool running = true;

    enableANSI();
    printBanner();

    while (running)
    {
        printPrompt();

        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "help") == 0)
        {
            printf(
                "\n\t+------------------------+\n"
                CYAN "\t|   Available Commands   |\n" RESET
                "\t+------------------------+\n"
                "\t[" BLUE "1" RESET "] help\n"
                "\t[" GREEN "2" RESET "] about\n"
                "\t[" CYAN "3" RESET "] version\n"
                "\t[" MAGENTA "4" RESET "] clear\n"
                "\t[" YELLOW "5" RESET "] exit\n\n"
            );
        }
        else if (strcmp(command, "about") == 0)
        {
            printf(
                GREEN "\nYShell v1.0\n" RESET
                "A custom terminal written in C.\n"
                "Developed by Yassin.\n\n"
            );
        }
        else if (strcmp(command, "version") == 0)
        {
            printf("YShell Version 1.0\n\n");
        }
        else if (strcmp(command, "clear") == 0)
        {
            system("cls");
            printBanner();
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf(GREEN "\nGoodbye!\n" RESET);
            running = false;
        }else if(strncmp(command,"echo ",5)==0){
        printf("\t %s\n",command + 5);
        }else if(strncmp(command, "mkdir ",6) == 0){
            if(_mkdir(command + 6) ==0){
                printf(GREEN "Directory created successfully.\n" RESET);
            }else
            {
                printf(RED "Error creating directory.\n" RESET);
            }
        }
        else if(strcmp(command,"pwd") == 0){
            char path[MAX_PATH];
            GetCurrentDirectory(MAX_PATH, path);
            printf("%s\n",path);
        }else if(strcmp(command,"dir") == 0 || strcmp(command,"ls") == 0){
            listFiles();
        }
        else if (strlen(command) == 0)
        {
        }
        else
        {
            printf(RED "Unknown command: %s\n\n" RESET, command);
        }
    }

    return 0;
}
