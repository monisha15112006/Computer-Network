Muthu Lekshmi B
	
3:16 PM (50 minutes ago)
	
	
to me


---------- Forwarded message ---------
From: Jeyasrilakshmi M <muthurajkanisha_bcs28@mepcoeng.ac.in>
Date: Wed, Sep 9, 2026 at 3:15 PM
Subject:
To: <cbalacbala_bcs28@mepcoeng.ac.in>


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

void stopAndWait(int total_frames) {
    printf("\n--- Stop-and-Wait ARQ Simulation ---\n");
    int i = 1;
    while (i <= total_frames) {
        printf("Sender: Sent Frame %d\n", i);

        // Simulating receiver choice
        int choice;
        printf("Receiver: Enter 1 for Success (ACK), 0 for Loss/Timeout: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Sender: ACK %d received.\n\n", i);
            i++;
        } else {
            printf("Sender: Timeout! Retransmitting Frame %d...\n\n", i);
        }
    }
    printf("Stop-and-Wait: All %d frames sent successfully!\n", total_frames);
}

void goBackN(int total_frames, int window_size) {
    printf("\n--- Go-Back-N ARQ Simulation ---\n");
    int ack_ptr = 1;
    int sender_ptr = 1;

    while (ack_ptr <= total_frames) {
        while (sender_ptr < ack_ptr + window_size && sender_ptr <= total_frames) {
            printf("Sender: Sent Frame %d\n", sender_ptr);
            sender_ptr++;
        }

        int next_expected;
        printf("\nReceiver: Next Expected Frame is %d. Enter ACK received by Sender: ", ack_ptr);
        scanf("%d", &next_expected);

        if (next_expected == ack_ptr) {
            printf("Sender: ACK %d verified. Sliding window forward...\n\n", ack_ptr);
            ack_ptr++;
        } else {
            printf("Sender: Wrong ACK / Timeout! Go-Back-N triggered. Retransmitting window from Frame %d...\n\n", ack_ptr);
            sender_ptr = ack_ptr; // Go back to last unacknowledged frame
        }
    }
    printf("Go-Back-N: All %d frames sent successfully!\n", total_frames);
}

void selectiveRepeat(int total_frames, int window_size) {
    printf("\n--- Selective Repeat ARQ Simulation ---\n");
    bool acknowledged[total_frames + 1];
    for (int i = 1; i <= total_frames; i++) acknowledged[i] = false;

    int window_start = 1;

    while (window_start <= total_frames) {
        // Send all unacknowledged frames currently inside the window
        for (int i = window_start; i < window_start + window_size && i <= total_frames; i++) {
            if (!acknowledged[i]) {
                printf("Sender: Sent/Resent Frame %d\n", i);
            }
        }

        // Receive status updates for frames in the current window
        for (int i = window_start; i < window_start + window_size && i <= total_frames; i++) {
            if (!acknowledged[i]) {
                int choice;
                printf("Receiver: Did you receive Frame %d properly? (1 for Yes, 0 for No/NAK): ", i);
                scanf("%d", &choice);
                if (choice == 1) {
                    acknowledged[i] = true;
                    printf("Sender: ACK received for Frame %d\n", i);
                } else {
                    printf("Sender: NAK/Loss recorded for Frame %d\n", i);
                }
            }
        }

        // Slide the window as far forward as possible
        while (window_start <= total_frames && acknowledged[window_start]) {
            window_start++;
        }
        printf(">>> Window starting edge slid to Frame %d <<<\n\n", window_start);
    }
    printf("Selective Repeat: All %d frames sent successfully!\n", total_frames);
}

int main() {
    int choice, total_frames, window_size;

    printf("Enter total number of frames to transmit: ");
    scanf("%d", &total_frames);

    while (1) {
        printf("\n========================================\n");
        printf("SLIDING WINDOW PROTOCOLS\n");
        printf("========================================\n");
        printf("1. Stop-and-Wait ARQ\n");
        printf("2. Go-Back-N ARQ\n");
        printf("3. Selective Repeat ARQ\n");
        printf("4. Exit\n");
        printf("Enter your choice (1-4): ");
        scanf("%d", &choice);

        if (choice == 4) {
            printf("Exiting simulation.\n");
            break;
        }

        switch (choice) {
            case 1:
                stopAndWait(total_frames);
                break;
            case 2:
                printf("Enter window size for Go-Back-N: ");
                scanf("%d", &window_size);
                goBackN(total_frames, window_size);
                break;
            case 3:
                printf("Enter window size for Selective Repeat: ");
                scanf("%d", &window_size);
                selectiveRepeat(total_frames, window_size);
                break;
            default:
                printf("Invalid choice! Please select between 1 and 4.\n");
        }
    }
    return 0;
}
