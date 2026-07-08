#include <stdlib.h>

int* countBits(int n, int* returnSize) {
    *returnSize = n + 1;
    int* ans = (int*)malloc((*returnSize) * sizeof(int));
    ans[0] = 0;
    for (int i = 1; i <= n; i++) {
       
        if (i % 2 == 0) {
            ans[i] = ans[i / 2];
        } else {
            ans[i] = ans[i / 2] + 1;
        }
    }
    return ans;
}
