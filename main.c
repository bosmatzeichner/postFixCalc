#include <stdio.h>
#include <stdbool.h>
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


extern void calcMult(stack);
extern void calcDiv(stack);
extern void calcSum(stack);
extern void calcSub(stack* s);
extern void execute_p();
extern void execute_c();

enum state{number,notNumber};
int main() {
    struct bignum* currbignum;
    enum state currState=number;
    long capacity=MAX_SIZE;
    struct stack stack;
    stack.size=0;
    while((c=fgetc(stdin))!='q'){
        switch (currState){
            case number:
                switch (c) {
                    case '0'...'9':
                        addDigit('c',currbignum);
                        break;
                    default:
                        push(currbignum,stack);
                        currState = notNumber;
                        break;
                }
            case notNumber:
                switch (c) {
                    case '*':
                        calcMult(stack);
                        break;
                    case '/':
                        calcDiv(stack);
                        break;
                    case '+':
                        calcSum(stack);
                        break;
                    case '-':
                        calcSub(stack);
                        break;
                    case 'p':
                        execute_p();
                        break;
                    case 'c':
                        execute_c();
                        break;
                    case '0'...'9':
                    case '_':
                        struct bignum* newNum;
                        currbignum=newNum;
                        addDigit(c,currbignum);
                        currState=number;
                        break;
                    default:
                        break;
                }
    }

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

