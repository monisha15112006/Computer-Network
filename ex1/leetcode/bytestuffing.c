#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char word[100];
    // Sized safely to handle max theoretical payloads under error conditions
    int data[100][8], stuffed_output[250][8], destuffed_output[250][8];
    char recovered_word[250];

    int flag[8] = {0,1,1,1,1,1,1,0}; // '~' (0x7E)
    int esc[8]  = {0,1,1,1,1,1,0,1}; // '}' (0x7D)

    int i, k;
    int stuffed_idx = 0;
    int destuffed_idx = 0;
    int stuffing_counter = 0;

    int choice;
    int byteNo, bitNo;

    printf("Enter a word (~ is FLAG, } is ESC): ");
    if (scanf("%99s", word) != 1) return 1;

    int n = strlen(word);
    for(i = 0; i < n; i++) {
        unsigned char ch = word[i];
        for(k = 7; k >= 0; k--) {
            data[i][k] = ch % 2;
            ch /= 2;
        }
    }

    // Add Starting Flag
    for(k = 0; k < 8; k++)
        stuffed_output[stuffed_idx][k] = flag[k];
    stuffed_idx++;

    // Perform Byte Stuffing
    for(i = 0; i < n; i++) {
        int isFlag = 1;
        int isEsc = 1;

        for(k = 0; k < 8; k++) {
            if(data[i][k] != flag[k]) isFlag = 0;
            if(data[i][k] != esc[k])  isEsc = 0;
        }

        if(isFlag || isEsc) {
            for(k = 0; k < 8; k++)
                stuffed_output[stuffed_idx][k] = esc[k];
            stuffed_idx++;
            stuffing_counter++;
        }

        for(k = 0; k < 8; k++)
            stuffed_output[stuffed_idx][k] = data[i][k];
        stuffed_idx++;
    }

    // Add Ending Flag
    for(k = 0; k < 8; k++)
        stuffed_output[stuffed_idx][k] = flag[k];
    stuffed_idx++;

    printf("\nOriginal Word: %s\n", word);
    printf("\nOriginal Binary:\n");
    for(i = 0; i < n; i++) {
        for(k = 0; k < 8; k++) printf("%d", data[i][k]);
        printf(" ");
    }

    printf("\n\nAfter Stuffing (Total Bytes = %d):\n", stuffed_idx);
    for(i = 0; i < stuffed_idx; i++) {
        for(k = 0; k < 8; k++) printf("%d", stuffed_output[i][k]);
        printf(" ");
    }

    printf("\n\nStatus : %s", stuffing_counter ? "Performed" : "Not Required");

    // NEW MENU: No Error vs Bit Level vs Byte Level
    printf("\n\nChoose Error Injection Type:\n");
    printf("1. No Error\n");
    printf("2. Bit-Level Error (Flips a single specific bit)\n");
    printf("3. Byte-Level Error (Flips all 8 bits of a chosen byte)\n");
    printf("Enter Choice (1-3): ");
    if (scanf("%d", &choice) != 1) choice = 1;

    switch(choice) {
        case 1:
            printf("\nNo Error Introduced. Proceeding safely.\n");
            break;

        case 2: // Bit-Level Error Injection
            printf("Enter byte index to corrupt (0-%d): ", stuffed_idx - 1);
            scanf("%d", &byteNo);
            if(byteNo >= 0 && byteNo < stuffed_idx) {
                printf("Enter specific bit index to flip (0-7, where 0 is MSB): ");
                scanf("%d", &bitNo);

                if(bitNo >= 0 && bitNo < 8) {
                    stuffed_output[byteNo][bitNo] ^= 1; // Flip exactly one bit
                    printf("\nBit %d inside byte %d successfully flipped!\n", bitNo, byteNo);
                } else {
                    printf("\nInvalid Bit Position! No corruption injected.\n");
                }
            } else {
                printf("\nInvalid Byte Position! No corruption injected.\n");
            }
            break;

        case 3: // Byte-Level Error Injection
            printf("Enter byte index to entirely flip (0-%d): ", stuffed_idx - 1);
            scanf("%d", &byteNo);
            if(byteNo >= 0 && byteNo < stuffed_idx) {
                for(k = 0; k < 8; k++) {
                    stuffed_output[byteNo][k] ^= 1; // Flip all bits in the row
                }
                printf("\nByte at index %d successfully flipped!\n", byteNo);
            } else {
                printf("\nInvalid Position! No corruption injected.\n");
            }
            break;

        default:
            printf("\nInvalid Choice! Proceeding with no error introduction.\n");
            break;
    }

    if (choice == 2 || choice == 3) {
        printf("\nTransmitted Corrupted Frame:\n");
        for(i = 0; i < stuffed_idx; i++) {
            for(k = 0; k < 8; k++) printf("%d", stuffed_output[i][k]);
            printf(" ");
        }
        printf("\n");
    }

    // Secure De-stuffing algorithm implementation logic
    for(i = 1; i < stuffed_idx - 1; i++) {
        int isEsc = 1;
        for(k = 0; k < 8; k++) {
            if(stuffed_output[i][k] != esc[k]) {
                isEsc = 0;
                break;
            }
        }

        // If Escape character found and there is an available byte next
        if(isEsc && (i + 1) < (stuffed_idx - 1)) {
            i++;
        }

        // Safety guard against array limit overruns
        if (destuffed_idx < 250) {
            for(k = 0; k < 8; k++)
                destuffed_output[destuffed_idx][k] = stuffed_output[i][k];
            destuffed_idx++;
        }
    }

    // Convert bits back into chars
    for(i = 0; i < destuffed_idx && i < 249; i++) {
        int ascii_val = 0;
        for(k = 0; k < 8; k++) {
            ascii_val = (ascii_val << 1) | destuffed_output[i][k];
        }
        recovered_word[i] = (char)ascii_val;
    }
    recovered_word[i] = '\0';

    printf("\nDestuffed Data:\n");
    for(i = 0; i < destuffed_idx; i++) {
        for(k = 0; k < 8; k++) printf("%d", destuffed_output[i][k]);
        printf(" ");
    }

    printf("\n\nRecovered Word : %s\n", recovered_word);

    // Verify Integrity
    if(strcmp(word, recovered_word) == 0) {
        printf("\nTransmission Successful. No Error Detected.\n");
    } else {
        printf("\nTransmission Error Detected!\n");
    }

    return 0;
}
