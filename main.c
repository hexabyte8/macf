#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAC_ADDRESS_LENGTH 12

// Function declarations
int is_valid_mac(const char *mac);
void format_mac_address(const char *input, char *output, const char *format);
void process_mac_addresses_from_file(const char *input_file, const char *output_file, const char *error_file, const char *format, int verbose);
void process_single_mac_address(const char *mac, const char *output_file, const char *format, int verbose);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <mac_address|--single> <format> [-f <input_file>] [-o <output_file>] [--output-errors <error_file>] [-v]\n", argv[0]);
        fprintf(stderr, "Formats: cisco, dash, colon, raw\n");
        return EXIT_FAILURE;
    }

    const char *input_file = NULL;
    const char *output_file = NULL; // Default to standard output
    const char *error_file = NULL;
    int verbose = 0;
    int single_mode = 1; // Default mode is single MAC address input
    const char *mac_address = NULL;
    const char *format = NULL;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                input_file = argv[++i];
                single_mode = 0; // File mode
            } else {
                fprintf(stderr, "Error: Missing file name after -f/--file flag.\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "--output-errors") == 0 && i + 1 < argc) {
            error_file = argv[++i];
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--single") == 0) {
            single_mode = 1; // Explicitly enable single mode
        } else if (format == NULL) {
            format = argv[i]; // First non-flag argument is the format
        } else if (mac_address == NULL) {
            mac_address = argv[i]; // Second non-flag argument is the MAC address
        } else {
            fprintf(stderr, "Error: Unrecognized argument: %s\n", argv[i]);
            return EXIT_FAILURE;
        }
    }

    // Ensure format is provided
    if (format == NULL) {
        fprintf(stderr, "Error: No format provided. Supported formats: cisco, dash, colon, raw.\n");
        return EXIT_FAILURE;
    }

    // Process input based on mode
    if (single_mode && mac_address != NULL) {
        process_single_mac_address(mac_address, output_file, format, verbose);
    } else if (!single_mode && input_file != NULL) {
        process_mac_addresses_from_file(input_file, output_file, error_file, format, verbose);
    } else {
        fprintf(stderr, "Error: No input provided. Use a MAC address or specify a file with -f/--file.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// Function to validate if a string is a valid MAC address
int is_valid_mac(const char *mac) {
    int length = 0;
    while (*mac) {
        if (isalnum(*mac)) {
            if (!isxdigit(*mac)) {
                return 0; // Invalid character
            }
            length++;
        }
        mac++;
    }
    return length == MAC_ADDRESS_LENGTH;
}

// Function to format a MAC address
void format_mac_address(const char *input, char *output, const char *format) {
    char clean_mac[MAC_ADDRESS_LENGTH + 1] = {0};
    int j = 0;

    // Remove all non-hexadecimal characters
    for (int i = 0; input[i] != '\0' && j < MAC_ADDRESS_LENGTH; i++) {
        if (isxdigit(input[i])) {
            clean_mac[j++] = tolower(input[i]);
        }
    }

    // Format the MAC address
    if (strcmp(format, "cisco") == 0) {
        snprintf(output, 15, "%c%c%c%c.%c%c%c%c.%c%c%c%c",
                 clean_mac[0], clean_mac[1], clean_mac[2], clean_mac[3],
                 clean_mac[4], clean_mac[5], clean_mac[6], clean_mac[7],
                 clean_mac[8], clean_mac[9], clean_mac[10], clean_mac[11]);
    } else if (strcmp(format, "dash") == 0) {
        snprintf(output, 18, "%c%c-%c%c-%c%c-%c%c-%c%c-%c%c",
                 clean_mac[0], clean_mac[1], clean_mac[2], clean_mac[3],
                 clean_mac[4], clean_mac[5], clean_mac[6], clean_mac[7],
                 clean_mac[8], clean_mac[9], clean_mac[10], clean_mac[11]);
    } else if (strcmp(format, "colon") == 0) {
        snprintf(output, 18, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c",
                 clean_mac[0], clean_mac[1], clean_mac[2], clean_mac[3],
                 clean_mac[4], clean_mac[5], clean_mac[6], clean_mac[7],
                 clean_mac[8], clean_mac[9], clean_mac[10], clean_mac[11]);
    } else if (strcmp(format, "raw") == 0) {
        strncpy(output, clean_mac, MAC_ADDRESS_LENGTH);
        output[MAC_ADDRESS_LENGTH] = '\0';
    } else {
        strcpy(output, "INVALID_FORMAT");
    }
}

// Function to process a single MAC address
void process_single_mac_address(const char *mac, const char *output_file, const char *format, int verbose) {
    char formatted_mac[256];

    if (is_valid_mac(mac)) {
        format_mac_address(mac, formatted_mac, format);

        if (output_file) {
            // Write to output file
            FILE *out_fp = fopen(output_file, "w");
            if (!out_fp) {
                perror("Error opening output file");
                exit(EXIT_FAILURE);
            }
            fprintf(out_fp, "%s\n", formatted_mac);
            fclose(out_fp);
        } else {
            // Print to standard output
            printf("%s\n", formatted_mac);
        }

        if (verbose) {
            printf("Converted: %s -> %s\n", mac, formatted_mac);
        }
    } else {
        fprintf(stderr, "Error: Invalid MAC address: %s\n", mac);
    }
}

// Function to process MAC addresses from a file
void process_mac_addresses_from_file(const char *input_file, const char *output_file, const char *error_file, const char *format, int verbose) {
    FILE *in_fp = fopen(input_file, "r");
    if (!in_fp) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    FILE *out_fp = NULL;
    if (output_file) {
        out_fp = fopen(output_file, "w");
        if (!out_fp) {
            perror("Error opening output file");
            fclose(in_fp);
            exit(EXIT_FAILURE);
        }
    }

    FILE *err_fp = NULL;
    if (error_file) {
        err_fp = fopen(error_file, "w");
        if (!err_fp) {
            perror("Error opening error file");
            fclose(in_fp);
            if (out_fp) fclose(out_fp);
            exit(EXIT_FAILURE);
        }
    }

    char line[256];
    while (fgets(line, sizeof(line), in_fp)) {
        char mac[256];
        char formatted_mac[256];
        sscanf(line, "%s", mac); // Read the MAC address from the line

        if (is_valid_mac(mac)) {
            format_mac_address(mac, formatted_mac, format);

            if (out_fp) {
                fprintf(out_fp, "%s\n", formatted_mac);
            } else {
                printf("%s\n", formatted_mac);
            }

            if (verbose) {
                printf("Converted: %s -> %s\n", mac, formatted_mac);
            }
        } else {
            if (verbose) {
                fprintf(stderr, "Error: Invalid MAC address: %s\n", mac);
            }
            if (err_fp) {
                fprintf(err_fp, "%s\n", mac);
            }
        }
    }

    fclose(in_fp);
    if (out_fp) fclose(out_fp);
    if (err_fp) fclose(err_fp);
}
