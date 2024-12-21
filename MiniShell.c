#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_COMMAND_LENGTH 1024

void print_prompt() {
    printf("mini-shell> ");
}

void execute_command(char *command) {
    if (strcmp(command, "exit\n") == 0) {
        exit(0);
    }

    // Set up Windows process creation
    STARTUPINFO si = {0};
    PROCESS_INFORMATION pi = {0};
    si.cb = sizeof(si);

    // Add "cmd.exe /c" to execute commands in Windows Command Prompt
    char full_command[MAX_COMMAND_LENGTH] = "cmd.exe /c ";
    strcat(full_command, command);

    // Create a new process for the command
    if (!CreateProcess(
            NULL,              // Application name
            full_command,      // Command line
            NULL,              // Process security attributes
            NULL,              // Thread security attributes
            FALSE,             // Inherit handles
            0,                 // Creation flags
            NULL,              // Environment
            NULL,              // Current directory
            &si,               // Startup info
            &pi                // Process info
        )) {
        fprintf(stderr, "mini-shell: Failed to execute command\n");
        return;
    }

    // Wait for the process to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        print_prompt();
        if (fgets(command, sizeof(command), stdin) == NULL) {
            perror("mini-shell");
            break;
        }

        // Remove trailing newline character from the command
        command[strcspn(command, "\n")] = '\0';

        execute_command(command);
    }

    return 0;
}

