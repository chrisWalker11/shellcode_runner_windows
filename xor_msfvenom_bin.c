#include <stdio.h>
#include <stdlib.h>

// Function to perform XOR encryption or decryption
void xor_encrypt_decrypt(const char *input_file, const char *output_file, unsigned char key) {
    FILE *fp_in = fopen(input_file, "rb"); // Open the input file in binary mode
    if (!fp_in) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *fp_out = fopen(output_file, "wb"); // Open the output file in binary mode
    if (!fp_out) {
        perror("Error opening output file");
        fclose(fp_in);
        exit(EXIT_FAILURE);
    }

    int ch;
    while ((ch = fgetc(fp_in)) != EOF) {
        fputc(ch ^ key, fp_out); // XOR each byte and write to the output file
    }

    fclose(fp_in);
    fclose(fp_out);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <XOR key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *input_file = argv[1];
    const char *output_file = argv[2];
    unsigned char key = (unsigned char)strtoul(argv[3], NULL, 0); // Convert string to unsigned long and then to unsigned char

    xor_encrypt_decrypt(input_file, output_file, key);

    printf("File has been encrypted/decrypted successfully.\n");
    return EXIT_SUCCESS;
}
