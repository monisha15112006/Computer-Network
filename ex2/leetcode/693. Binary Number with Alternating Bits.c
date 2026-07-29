#include <stdbool.h>

bool hasAlternatingBits(int n) {
    // Shifting 'n' right by 1 and XORing with 'n' flips all bits 
    // to 1s if the original bits were strictly alternating.
    unsigned int x = n ^ (n >> 1);
    
    // Check if x is of the form 111...111 (all bits set).
    // An all-ones number plus 1 yields a power of 2 (e.g., 7 + 1 = 8, or 0111 + 0001 = 1000).
    // A bitwise AND between x and (x + 1) will evaluate to 0 if all bits were 1.
    return (x & (x + 1)) == 0;
}
