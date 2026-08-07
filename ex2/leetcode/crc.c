#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Simple function to do CRC Binary XOR division
void crc_division(char *dividend, const char *divisor, char *remainder) {
    int data_len = strlen(dividend);
    int div_len = strlen(divisor);
    char temp[2000];
   
    strcpy(temp, dividend);
   
    for (int i = 0; i <= data_len - div_len; i++) {
        if (temp[i] == '1') {
            for (int j = 0; j < div_len; j++) {
                temp[i + j] = (temp[i + j] == divisor[j]) ? '0' : '1';
            }
        }
    }
    // Extract the final remainder bits
    strcpy(remainder, &temp[data_len - (div_len - 1)]);
}

// Converts a text character into an 8-bit binary string
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

// Parses an algebraic polynomial string (e.g., "x^4+x^2+x+1") into a binary string
void parse_polynomial_to_binary(const char *poly_str, char *binary_out) {
    int max_degree = 0;
    int degrees[100] = {0};
    int len = strlen(poly_str);
    int i = 0;

    while (i < len) {
        if (poly_str[i] == ' ' || poly_str[i] == '+') {
            i++;
            continue;
        }

        if (poly_str[i] == 'x' || poly_str[i] == 'X') {
            i++;
            if (poly_str[i] == '^') {
                i++;
                int deg = 0;
                while (i < len && isdigit(poly_str[i])) {
                    deg = deg * 10 + (poly_str[i] - '0');
                    i++;
                }
                degrees[deg] = 1;
                if (deg > max_degree) max_degree = deg;
            } else {
                degrees[1] = 1;
                if (1 > max_degree) max_degree = 1;
            }
        } else if (isdigit(poly_str[i])) {
            int val = 0;
            while (i < len && isdigit(poly_str[i])) {
                val = val * 10 + (poly_str[i] - '0');
                i++;
            }
            if (val == 1) {
                degrees[0] = 1;
            }
        } else {
            i++;
        }
    }

    int idx = 0;
    for (int d = max_degree; d >= 0; d--) {
        binary_out[idx++] = degrees[d] ? '1' : '0';
    }
    binary_out[idx] = '\0';
}

int main() {
    char input_string[100];
    char poly_expr[100];
    char generator_poly[100] = "";
    char raw_binary_stream[1000] = "";
    char dividend[2000] = "", remainder[100] = "", transmitted[2000] = "";
    char error_choice;
    int bit_pos;

    // 1. Capture user inputs
    printf("Enter text string input (e.g., hi): ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0';

    printf("Enter polynomial divisor expression (e.g., x^4+x^2+x+1): ");
    fgets(poly_expr, sizeof(poly_expr), stdin);
    poly_expr[strcspn(poly_expr, "\n")] = '\0';
    printf("\n");

    // Convert polynomial expression into its binary equivalent string
    parse_polynomial_to_binary(poly_expr, generator_poly);
    int poly_len = strlen(generator_poly);

    // Convert the data string into its continuous binary representation
    for (int i = 0; i < strlen(input_string); i++) {
        char temp_bin[9];
        char_to_binary(input_string[i], temp_bin);
        strcat(raw_binary_stream, temp_bin);
    }

    // Count the number of ones in the generator polynomial
    int ones_count = 0;
    for (int i = 0; i < poly_len; i++) {
        if (generator_poly[i] == '1') {
            ones_count++;
        }
    }

    // ==========================================
    // 2. DISPLAY PARSED BINARY VALUES & RULES CHECK
    // ==========================================
    printf("=================================================================================\n");
    printf("                               INPUT BINARY VALUES                               \n");
    printf("=================================================================================\n");
    printf("String Text Binary    : %s\n", raw_binary_stream);
    printf("Polynomial Expression : %s\n", poly_expr);
    printf("Generator Poly Binary : %s (Length: %d bits, Ones: %d)\n", generator_poly, poly_len, ones_count);
    printf("=================================================================================\n\n");

    // Strict Good Divisor Check
    if (poly_len <= 2) {
        printf("Verdict: BAD DIVISOR! The divisor must be above 2 bits long.\n");
        return 0;
    }
    if (generator_poly[0] != '1' || generator_poly[poly_len - 1] != '1') {
        printf("Verdict: BAD DIVISOR! Binary stream must start and end with 1.\n");
        return 0;
    }
    if (ones_count % 2 != 0) {
        printf("Verdict: BAD DIVISOR! Must contain an EVEN number of ones.\n");
        return 0;
    }

    printf("Verdict: GOOD DIVISOR! All criteria passed. Proceeding to sender...\n\n");

    // ==========================================
    // 3. SENDER SIDE
    // ==========================================
    strcpy(dividend, raw_binary_stream);
    for (int i = 0; i < poly_len - 1; i++) {
        strcat(dividend, "0"); // Pad with zeros
    }

    crc_division(dividend, generator_poly, remainder);
    sprintf(transmitted, "%s%s", raw_binary_stream, remainder); // Concat data + remainder

    printf("--- SENDER ---\n");
    printf("Transmitted Stream : %s (Length: %d bits)\n\n", transmitted, (int)strlen(transmitted));

    // ==========================================
    // 4. INTERACTIVE ERROR PROMPT
    // ==========================================
    printf("Do you want to inject an error? (y/n): ");
    scanf(" %c", &error_choice);

    char receiver_stream[2000];
    strcpy(receiver_stream, transmitted);

    if (error_choice == 'y' || error_choice == 'Y') {
        printf("Enter bit position to flip (0 to %d): ", (int)strlen(transmitted) - 1);
        scanf("%d", &bit_pos);
       
        if (bit_pos >= 0 && bit_pos < strlen(receiver_stream)) {
            receiver_stream[bit_pos] = (receiver_stream[bit_pos] == '0') ? '1' : '0'; // Flip single bit
            printf("\n[CHANNEL LOG] Noise injected successfully!\n");
        } else {
            printf("\n[CHANNEL LOG] Invalid position. No noise injected.\n");
        }
    } else {
        printf("\n[CHANNEL LOG] Clear channel transmission. No noise applied.\n");
    }
    printf("\n");

    // ==========================================
    // 5. RECEIVER SIDE
    // ==========================================
    printf("=================================================================================\n");
    printf("                                  RECEIVER SIDE                                  \n");
    printf("=================================================================================\n");
    printf("Processing Received Continuous Binary Stream...\n\n");
    printf("Received Stream    : %s\n", receiver_stream);

    char check_remainder[100] = "";
    crc_division(receiver_stream, generator_poly, check_remainder);
    printf("Division Remainder : %s\n", check_remainder);

    // Evaluate if remainder is entirely zeros
    int corrupted = 0;
    for (int i = 0; i < strlen(check_remainder); i++) {
        if (check_remainder[i] != '0') {
            corrupted = 1;
            break;
        }
    }

    // Decode the data part (the original raw binary length) back to letters
    char decoded_output[100] = "";
    int decoded_idx = 0;
   
    // We step through the stream 8 bits at a time to reconstruct the characters
    for (int i = 0; i < strlen(raw_binary_stream); i += 8) {
        char chunk[9];
        strncpy(chunk, &receiver_stream[i], 8);
        chunk[8] = '\0';
        decoded_output[decoded_idx++] = binary_to_char(chunk);
    }
    decoded_output[decoded_idx] = '\0';

    // ==========================================
    // 6. FINAL VERDICT WITH TEXT PRINT
    // ==========================================
    printf("\n=================================================================================\n");
    printf("                                 FINAL VERDICT                                   \n");
    printf("=================================================================================\n");
    if (corrupted) {
        printf("Result       : ERROR DETECTED! Frame DISCARDED.\n");
        printf("Received Text: [DATA CORRUPTED - CANNOT DISPLAY]\n");
    } else {
        printf("Result       : SUCCESS! Frame accepted cleanly.\n");
        printf("Received Text: %s\n", decoded_output); // PRINTS THE RECEIVED TEXT VALUES
    }
    printf("=================================================================================\n");

    return 0;
}
