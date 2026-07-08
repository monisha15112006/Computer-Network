int main() {
    int total_cases;
    
    // Store the input value and validate it inside the if condition
    if (scanf("%d", &total_cases) == 1) {
        while (total_cases--) {
            char s[10001];
            scanf("%s", s);
            
            int sum = 0;
            for (int i = 0; s[i] != '\0'; i++) {
                if (s[i] >= '0' && s[i] <= '9') {
                    sum += (s[i] - '0');
                }
            }
            printf("%d\n", sum);
        }
    }
    return 0;
}
