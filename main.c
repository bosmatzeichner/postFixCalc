#include <stdio.h>
#include <stdbool.h>
typedef struct bignum {
    long capacity;
    long numberOfDigits;
    char* digit;
    struct bignum* addDigit(char c);
};

struct stack {
    int size;
    struct bignum* firstBignum[1024];
    void clear();
    struct bignum* pop();
    struct bignum* peak();
    void push(struct bignum * number);
    bool isEmpty();
};

extern struct bignum* calcMult(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern struct bignum* calcDiv(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern struct bignum* calcSum(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern struct bignum* calcSub(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
enum state{number,notNumber};
int main() {
    struct bignum* currbignum;
    enum state currState=notNumber;

    printf("Hello, World!\n");
    return 0;
}