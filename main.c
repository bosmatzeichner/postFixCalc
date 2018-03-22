#include <stdio.h>
#include <stdbool.h>
#define MAX_SIZE 20
typedef struct bignum {
    long numberOfDigits;
    char* digit;
};
struct bignum* addDigit(char c, struct stack);

struct stack {
    int size;
    struct bignum* firstBignum[1024];
    void push(struct bignum * number);
    bool isEmpty();
};

extern void calcMult(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern void calcDiv(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern void calcSum(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern void calcSub(long firstNumSize,
                               char* firstNumDigits,
                               long secondNumSize,
                               char* secondNumDigits);
extern void execute_p();
extern void execute_c();

enum state{number,notNumber};
int main() {
    struct bignum* currbignum;
    enum state currState=notNumber;
    long capacity=MAX_SIZE;

    printf("Hello, World!\n");
    return 0;
}