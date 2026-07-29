#include <string.h>
#include <stdlib.h>

char* addBinary(char* a, char* b) {
    int len_a = strlen(a);
    int len_b = strlen(b);
    
    // Maximum possible result size is max(len_a, len_b) + 1 digit + 1 null terminator
    int max_len = (len_a > len_b ? len_a : len_b) + 1;
    char* result = (char*)malloc((max_len + 1) * sizeof(char));
    
    result[max_len] = '\0'; // Set null terminator
    
    int i = len_a - 1;
    int j = len_b - 1;
    int k = max_len - 1;
    int carry = 0;
    
    // Process strings from right to left
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        
        if (i >= 0) {
            sum += a[i] - '0';
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0';
            j--;
        }
        
        result[k] = (sum % 2) + '0'; // Compute current bit
        carry = sum / 2;             // Compute new carry
        k--;
    }
    
    // If the leading extra slot wasn't used, shift pointer forward to skip leading zero
    if (k >= 0) {
        return &result[k + 1];
    }
    
    return result;
}
