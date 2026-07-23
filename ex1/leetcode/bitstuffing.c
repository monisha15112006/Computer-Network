[24bcs015@mepcolinux newex1]$cat newbit.c
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_BITS 2000

int main()
{
    char str[100];
    int binary_stream[MAX_BITS];
    int stuffed_stream[MAX_BITS];
    int transmitted_stream[MAX_BITS];
    int received_stream[MAX_BITS];
    int destuffed_stream[MAX_BITS];
    int flag[8] = {0,1,1,1,1,1,1,0};

    int bit_idx = 0;
    int stuffed_idx = 0;
    int destuffed_idx = 0;
    int count = 0;
    int stuffing_performed = 0;
    int len, i, b;
    char ch;

    int choice;
    int error_detected = 0;
    int error_pos;    
    int num_errors;  

    srand(time(NULL));

    // 1. Get string input from user
    printf("Enter word: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';

    len = strlen(str);

    // Convert character text string into its raw binary bit array form
    for(i = 0; i < len; i++)
    {
        ch = str[i];
        for(b = 7; b >= 0; b--)
        {
            binary_stream[bit_idx++] = (ch >> b) & 1;
        }
    }

    // 2. Sender Side: Bit Stuffing
    for(i = 0; i < bit_idx; i++)
    {
        stuffed_stream[stuffed_idx++] = binary_stream[i];

        if(binary_stream[i] == 1)
            count++;
        else
            count = 0;

        if(count == 5)
        {
            stuffed_stream[stuffed_idx++] = 0;
            stuffing_performed = 1;
            count = 0;
        }
    }

    printf("\n=========== SENDER SIDE ===========\n");

    printf("\nOriginal Binary Data : ");
    for(i = 0; i < bit_idx; i++)
        printf("%d", binary_stream[i]);

    printf("\n\nStuffed Frame        : ");
    for(i = 0; i < 8; i++) printf("%d", flag[i]);
    for(i = 0; i < stuffed_idx; i++) printf("%d", stuffed_stream[i]);
    for(i = 0; i < 8; i++) printf("%d", flag[i]);

    if(stuffing_performed)
        printf("\nStatus               : Bit Stuffing Performed\n");
    else
        printf("\nStatus               : No Bit Stuffing Required\n");

    printf("Total bits inside frame (excluding flags): %d\n", stuffed_idx);

    // Copy clean stuffed stream into transmission channel buffer
    for(i = 0; i < stuffed_idx; i++)
        transmitted_stream[i] = stuffed_stream[i];

    // Transmitted Frame is printed here—guaranteed to be 100% correct and error-free
    printf("\nTransmitted Frame    : ");
    for(i = 0; i < 8; i++) printf("%d", flag[i]);
    for(i = 0; i < stuffed_idx; i++) printf("%d", transmitted_stream[i]);
    for(i = 0; i < 8; i++) printf("%d", flag[i]);


    // Simulate wire transmission: Copy the clean transmitted data over to the receiver first
    for(i = 0; i < stuffed_idx; i++)
        received_stream[i] = transmitted_stream[i];


    // 3. Channel Error Injection (Modifies RECEIVED stream only)
    printf("\n\nChoose error type option to apply during transit:\n");
    printf("1. No Error (Clean channel)\n");
    printf("2. Single Bit Error (User Input Position)\n");
    printf("3. Multiple Bit Error (User Input Positions)\n");
    printf("Enter Choice (1-3): ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            printf("No Error Introduced.\n");
            break;

        case 2:
            if (stuffed_idx > 0) {
                printf("Enter bit position to flip (1 to %d): ", stuffed_idx);
                scanf("%d", &error_pos);
                if (error_pos >= 1 && error_pos <= stuffed_idx) {
                    received_stream[error_pos - 1] ^= 1; // Flips bit inside received_stream ONLY
                    printf("Single bit error injected into channel at position: %d\n", error_pos);
                } else {
                    printf("Invalid position! No error introduced.\n");
                }
            }
            break;

        case 3:
            if (stuffed_idx > 1) {
                printf("Enter how many bit errors you want to inject (max %d): ", stuffed_idx);
                scanf("%d", &num_errors);
               
                if (num_errors >= 1 && num_errors <= stuffed_idx) {
                    printf("Enter %d distinct bit positions separated by spaces (1 to %d): ", num_errors, stuffed_idx);
                    for (int e = 0; e < num_errors; e++) {
                        scanf("%d", &error_pos);
                        if (error_pos >= 1 && error_pos <= stuffed_idx) {
                            received_stream[error_pos - 1] ^= 1; // Flips bit inside received_stream ONLY
                        } else {
                            printf("\nInvalid position %d ignored.", error_pos);
                        }
                    }
                    printf("\nMultiple bit errors successfully injected into incoming stream.\n");
                } else {
                    printf("Invalid quantity! No errors introduced.\n");
                }
            }
            break;

        default:
            printf("Invalid choice received. Defaulting to \"No Error\" mode.\n");
            break;
    }


    // 4. Receiver Side: Processing Corrupted Stream
    printf("\n=========== RECEIVER SIDE ===========\n");

    printf("\nReceived Frame       : ");
    for(i = 0; i < 8; i++) printf("%d", flag[i]);
    for(i = 0; i < stuffed_idx; i++) printf("%d", received_stream[i]); // Displays the change visibly
    for(i = 0; i < 8; i++) printf("%d", flag[i]);

    // De-stuffing algorithm implementation logic
    count = 0;
    for(i = 0; i < stuffed_idx; i++)
    {
        destuffed_stream[destuffed_idx++] = received_stream[i];

        if(received_stream[i] == 1)
            count++;
        else
            count = 0;

        if(count == 5)
        {
            // Verify the stuffed zero safely
            if(i + 1 < stuffed_idx)
            {
                if(received_stream[i+1] == 0) {
                    i++; // Drop the valid stuffed bit safely
                } else {
                    error_detected = 1; // Bad structural frame format (6 consecutive ones)

                }
            }
            count = 0;
        }
    }

    printf("\n\nDestuffed Frame      : ");
    for(i = 0; i < destuffed_idx; i++)
        printf("%d", destuffed_stream[i]);

    // 5. Verification Check Engine
    if (destuffed_idx % 8 != 0)
    {
        error_detected = 1;
    }

    if (bit_idx != destuffed_idx)
    {
        error_detected = 1;
    }
    else
    {
        for(i = 0; i < bit_idx; i++)
        {
            if(binary_stream[i] != destuffed_stream[i])
            {
                error_detected = 1;
                break;
            }
        }
    }

    // 6. Output Error Detection Status Summary Report
    if (error_detected)
    {
        printf("\n\nError Detection Status: ERROR DETECTED! Frame discarded or invalid.\n");
    }
    else
    {
        printf("\n\nError Detection Status: SUCCESS! Frame matches original text.\n");
        printf("Recovered Data       : ");
        for(i = 0; i < destuffed_idx; i += 8)
        {
            ch = 0;
            for(b = 0; b < 8; b++)
                ch = (ch << 1) | destuffed_stream[i+b];

            printf("%c", ch);
        }
        printf("\n");
    }

    return 0;
}

