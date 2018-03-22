#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>

#define MAX_SIZE 20
typedef struct bignum {
    long numberOfDigits;
    char* digit;
};

void addDigit(char c, struct bignum* number);
long capacity=MAX_SIZE;

struct stack {
    int size;
    struct bignum* firstBignum[1024];
};
void push(struct bignum * number, struct stack s);


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

    printf("Hello, World!\n");
    return 0;
}

void addDigit(char c, struct bignum *number) {
    number->digit[number->numberOfDigits]=c;
    number->numberOfDigits++;
    if(number->numberOfDigits==capacity){
        capacity=capacity+MAX_SIZE;
        char* newNumber;
        newNumber=(char*) malloc(sizeof(char)*capacity);
        char*j= newNumber;
        for(char* i=number->digit; i < number->digit + number->numberOfDigits; i++,j++){
            *j=*i;
        }
        number->digit=newNumber;
    }
}

void push(struct bignum *number, struct stack s) {
    s.firstBignum[s.size]=number;
    s.size++;
}

