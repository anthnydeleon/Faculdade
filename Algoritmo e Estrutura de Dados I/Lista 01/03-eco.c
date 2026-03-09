#include <stdio.h>

void readNumber(long long *num) {
    scanf("%lld", num);
}

void printNumber(long long num) {
    printf("%lld\n", num);
}

int main() {
    long long number;
    readNumber(&number);
    printNumber(number);
    
    return 0;
}