#include <stdio.h>
#include <string.h>

// Converts an ASCII character into an 8-bit binary string
void char_to_binary(char ch, char *binary_str) {
    for (int i = 7; i >= 0; i--) {
        binary_str[7 - i] = ((ch >> i) & 1) ? '1' : '0';
    }
    binary_str[8] = '\0';
}

// Converts an 8-bit binary string back into a plain text character
char binary_to_char(const char *binary_str) {
    char ch = 0;
    for (int i = 0; i < 8; i++) {
        ch |= (binary_str[i] - '0') << (7 - i);
    }
    return ch;
}

// Counts the total number of '1' bits inside a binary string
int count_ones(const char *str) {
    int count = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '1') count++;
    }
    return count;
}

int main() {
    char input_string[100];
    int parity_choice;

    // 1. Capture dynamic text inputs safely
    printf("Enter text string input (e.g., hi): ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0'; // Strip trailing newline

    printf("Choose Parity Type (1 for Even, 2 for Odd): ");
    scanf("%d", &parity_choice);
    printf("\n");

    char raw_binary_stream[1000] = "";
    char transmitted_stream[1000] = "";

    // ==========================================
    // 2. SENDER SIDE
    // ==========================================
    printf("=================================================================================\n");
    printf("                                   SENDER SIDE                                   \n");
    printf("=================================================================================\n");
    printf("Input Text         : %s\n", input_string);

    // Build the continuous binary stream, appending 1 parity bit to each 8-bit character block
    for (int i = 0; i < strlen(input_string); i++) {
        char temp_bin[9];
        char transmitted_packet[10];
        char parity_bit;

        char_to_binary(input_string[i], temp_bin);
        strcat(raw_binary_stream, temp_bin);

        int ones = count_ones(temp_bin);
        if (parity_choice == 1) {
            parity_bit = (ones % 2 == 0) ? '0' : '1';
        } else {
            parity_bit = (ones % 2 != 0) ? '0' : '1';
        }

        // Attach parity bit tightly to the end of the 8 data bits (9 bits total)
        sprintf(transmitted_packet, "%s%c", temp_bin, parity_bit);
        strcat(transmitted_stream, transmitted_packet);
    }

    printf("Raw Binary Stream  : %s\n", raw_binary_stream);
    printf("Transmitted Stream : %s (Length: %d bits)\n\n", transmitted_stream, (int)strlen(transmitted_stream));

    // ==========================================
    // 3. INTERACTIVE ERROR INJECTION PROMPT
    // ==========================================
    char error_choice;
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &error_choice);

    char receiver_stream[1000];
    strcpy(receiver_stream, transmitted_stream);

    if (error_choice == 'y' || error_choice == 'Y') {
        int bit_pos;
        printf("Enter bit position to flip (0 to %d): ", (int)strlen(transmitted_stream) - 1);
        scanf("%d", &bit_pos);
       
        if (bit_pos >= 0 && bit_pos < strlen(receiver_stream)) {
            receiver_stream[bit_pos] = (receiver_stream[bit_pos] == '0') ? '1' : '0'; // Flip single bit
            printf("\n[CHANNEL LOG] Noise injected successfully! Flipped bit at position %d.\n", bit_pos);
        } else {
            printf("\n[CHANNEL LOG] Invalid position. No noise injected.\n");
        }
    } else {
        printf("\n[CHANNEL LOG] Clear channel transmission. No noise applied.\n");
    }
    printf("\n");

    // ==========================================
    // 4. RECEIVER SIDE
    // ==========================================
    printf("=================================================================================\n");
    printf("                                  RECEIVER SIDE                                  \n");
    printf("=================================================================================\n");
    printf("Processing Received Continuous Binary Stream...\n\n");
    printf("Received Stream    : %s\n", receiver_stream);

    int stream_corrupted = 0;
    char decoded_output[100] = "";
    int decoded_idx = 0;

    // Validate the received continuous stream internally in 9-bit structural segments
    int block_len = 9;
    for (int i = 0; i < strlen(receiver_stream); i += block_len) {
        char segment[10];
        char extracted_data[9];
       
        strncpy(segment, &receiver_stream[i], block_len);
        segment[block_len] = '\0';
       
        int segment_ones = count_ones(segment);
       
        // Check the structural parity configuration rules
        if (parity_choice == 1) {
            if (segment_ones % 2 != 0) {
                stream_corrupted = 1;
            }
        } else {
            if (segment_ones % 2 == 0) {
                stream_corrupted = 1;
            }
        }

        // Isolate the original 8 data bits and convert them back to a text letter
        strncpy(extracted_data, segment, 8);
        extracted_data[8] = '\0';
        decoded_output[decoded_idx++] = binary_to_char(extracted_data);
    }
    decoded_output[decoded_idx] = '\0';

    // ==========================================
    // 5. FINAL VERDICT WITH MESSAGE DECODING
    // ==========================================
    printf("\n=================================================================================\n");
    printf("                                 FINAL VERDICT                                   \n");
    printf("=================================================================================\n");
    if (stream_corrupted) {
        printf("Result       : ERROR DETECTED! Frame has been DISCARDED.\n");
        printf("Received Text: [DATA CORRUPTED - CANNOT DISPLAY]\n");
    } else {
        printf("Result       : SUCCESS! Frame accepted cleanly.\n");
        printf("Received Text: %s\n", decoded_output); // PRINTS THE RECEIVED TEXT
    }
    printf("=================================================================================\n");

    return 0;
}
