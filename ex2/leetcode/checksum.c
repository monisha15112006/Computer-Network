#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

// Performs One's Complement Addition on two 16-bit binary strings
void ones_complement_add(const char *a, const char *b, char *result) {
    int carry = 0;
    int sum[16];
   
    // Core addition from right to left
    for (int i = 15; i >= 0; i--) {
        int bit_a = a[i] - '0';
        int bit_b = b[i] - '0';
        int current_sum = bit_a + bit_b + carry;
       
        sum[i] = current_sum % 2;
        carry = current_sum / 2;
    }
   
    // Wrap-around carry (End-around carry rule for One's Complement)
    if (carry > 0) {
        for (int i = 15; i >= 0; i--) {
            int current_sum = sum[i] + carry;
            sum[i] = current_sum % 2;
            carry = current_sum / 2;
            if (carry == 0) break;
        }
    }
   
    for (int i = 0; i < 16; i++) {
        result[i] = sum[i] + '0';
    }
    result[16] = '\0';
}

int main() {
    char input_string[100];
    char raw_binary_stream[1000] = "";
    char transmitted_stream[1200] = "";
    char error_choice;
    int bit_pos;

    // 1. Capture dynamic text input safely
    printf("Enter text string input (e.g., hi): ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0';
    printf("\n");

    // Convert the full text string into one continuous raw binary block
    for (int i = 0; i < strlen(input_string); i++) {
        char temp_bin[9];
        char_to_binary(input_string[i], temp_bin);
        strcat(raw_binary_stream, temp_bin);
    }

    // Pad raw stream with an extra zero-byte if character count is odd (to make 16-bit blocks)
    char padded_stream[1000];
    strcpy(padded_stream, raw_binary_stream);
    if (strlen(padded_stream) % 16 != 0) {
        strcat(padded_stream, "00000000");
    }

    // ==========================================
    // 2. SENDER SIDE (COMPUTE CHECKSUM)
    // ==========================================
    printf("=================================================================================\n");
    printf("                                   SENDER SIDE                                   \n");
    printf("=================================================================================\n");
    printf("Input Text         : %s\n", input_string);
    printf("Raw Binary Stream  : %s\n", raw_binary_stream);

    char current_sum[17] = "0000000000000000";
    int stream_len = strlen(padded_stream);

    // Sum all 16-bit chunks sequentially
    for (int i = 0; i < stream_len; i += 16) {
        char next_word[17];
        strncpy(next_word, &padded_stream[i], 16);
        next_word[16] = '\0';
       
        char temp_res[17];
        ones_complement_add(current_sum, next_word, temp_res);
        strcpy(current_sum, temp_res);
    }

    // Invert the final sum to generate the checksum bits
    char checksum[17];
    for (int i = 0; i < 16; i++) {
        checksum[i] = (current_sum[i] == '0') ? '1' : '0';
    }
    checksum[16] = '\0';

    // Assemble continuous transmitted stream: Data bits + Checksum bits tightly packed
    sprintf(transmitted_stream, "%s%s", raw_binary_stream, checksum);
   
    printf("Computed Checksum  : %s\n", checksum);
    printf("Transmitted Stream : %s (Length: %d bits)\n\n", transmitted_stream, (int)strlen(transmitted_stream));

    // ==========================================
    // 3. INTERACTIVE ERROR INJECTION PROMPT
    // ==========================================
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &error_choice);

    char receiver_stream[1200];
    strcpy(receiver_stream, transmitted_stream);

    if (error_choice == 'y' || error_choice == 'Y') {
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
    // 4. RECEIVER SIDE (VERIFY CHECKSUM)
    // ==========================================
    printf("=================================================================================\n");
    printf("                                  RECEIVER SIDE                                  \n");
    printf("=================================================================================\n");
    printf("Processing Received Continuous Binary Stream...\n\n");
    printf("Received Stream    : %s\n", receiver_stream);

    // Prepare receiver stream layout for validation (re-apply padding to data if original data length was odd)
    char rx_padded[1200] = "";
    int rx_main_data_len = strlen(raw_binary_stream);
   
    strncpy(rx_padded, receiver_stream, rx_main_data_len);
    rx_padded[rx_main_data_len] = '\0';
   
    if (rx_main_data_len % 16 != 0) {
        strcat(rx_padded, "00000000"); // Pad data block out
    }
   
    // Append the received checksum block onto the working math track
    char rx_checksum[17];
    strcpy(rx_checksum, &receiver_stream[rx_main_data_len]);
    strcat(rx_padded, rx_checksum);

    char rx_sum[17] = "0000000000000000";
    int rx_len = strlen(rx_padded);

    // Add all received blocks together including the checksum block
    for (int i = 0; i < rx_len; i += 16) {
        char next_word[17];
        strncpy(next_word, &rx_padded[i], 16);
        next_word[16] = '\0';
       
        char temp_res[17];
        ones_complement_add(rx_sum, next_word, temp_res);
        strcpy(rx_sum, temp_res);
    }

    // Verify if the sum inversion equals 0 (meaning all sum bits are 1)
    int stream_corrupted = 0;
    for (int i = 0; i < 16; i++) {
        if (rx_sum[i] != '1') {
            stream_corrupted = 1; // Any '0' means an error occurred
            break;
        }
    }

    printf("Receiver Final Sum : %s\n", rx_sum);

    // Decode the clean segments back into text characters
    char decoded_output[100] = "";
    int decoded_idx = 0;
    for (int i = 0; i < strlen(raw_binary_stream); i += 8) {
        char chunk[9];
        strncpy(chunk, &receiver_stream[i], 8);
        chunk[8] = '\0';
        decoded_output[decoded_idx++] = binary_to_char(chunk);
    }
    decoded_output[decoded_idx] = '\0';

    // ==========================================
    // 5. FINAL VERDICT WITH TEXT DECODING
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
