#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

void calculate_the_maximum(int n, int k) {
    int max_and = 0;
    int max_or = 0;
    int max_xor = 0;
    
    // Loop through all unique pairs (i, j)
    for (int i = 1; i <= n; i++) {
        for (int j = i + 1; j <= n; j++) {
            
            // Check Bitwise AND
            if ((i & j) > max_and && (i & j) < k) {
                max_and = i & j;
            }
            
            // Check Bitwise OR
            if ((i | j) > max_or && (i | j) < k) {
                max_or = i | j;
            }
            
            // Check Bitwise XOR
            if ((i ^ j) > max_xor && (i ^ j) < k) {
                max_xor = i ^ j;
            }
        }
    }
    
    // Print results on separate lines
    printf("%d\n%d\n%d\n", max_and, max_or, max_xor);
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    calculate_the_maximum(n, k);
    return 0;
}
