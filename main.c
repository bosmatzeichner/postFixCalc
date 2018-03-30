#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"

#define MAX_SIZE 20

void push(struct bignum *number, struct stack *s) {
    s->firstBignum[s->size] = number;
    s->size++;
}
struct bignum *peek(struct stack * s) {
    struct bignum *peeked = s->firstBignum[s->size-1];
    return peeked;
}
struct bignum * pop(struct stack *s) {
    struct bignum *poped = peek(s);
    s->size --;
    return poped;

}
// meir advice - multiply with binary counter
struct bignum* calcMult(struct bignum* first,struct bignum* second) {
    long *multiplier = convertToArray(first);
    long *multiplied = convertToArray(second);
    long multiplierSize = first->numberOfDigits / 9 + 1;
    long multipliedSize = second->numberOfDigits / 9 + 1;
    //check greater or equal without sign bit
    long * finalAnswer;
    if (isGE(multiplier + 1, multiplier + 1, multiplierSize, multipliedSize)) {//TODO change isGE to compareTO
        long tmp = multipliedSize;
        multipliedSize = multiplierSize;
        multiplierSize = tmp;
        long *tmpArray = multiplied;
        multiplied = multiplier;
        multiplier = tmpArray;
    }
    long max = multiplierSize + multipliedSize;
    finalAnswer = getFinalMult (multiplied, multiplier, multipliedSize, multiplierSize, max);
    free(multiplied);
    free(multiplier);
    return convertTObignum(finalAnswer,max);
}

struct bignum* calcDiv(struct bignum* first,struct bignum* second) {
    // printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
    return first;
}
struct bignum* calcSum(struct bignum* first,struct bignum* second) {
    long firstNewSize = first->numberOfDigits/9+1;
    long secondNewSize = second->numberOfDigits/9+1;;
    long max = secondNewSize;
    long min = firstNewSize;
    long* smaller = convertToArray(first);
    long* bigger = convertToArray(second);
    bool firstGreaterOrEqualtoSecond =compare(first,second)>0;
    if(firstGreaterOrEqualtoSecond){//if first number size is smaller then the second
        max=firstNewSize;
        min = secondNewSize;
        smaller = convertToArray(second);
        bigger = convertToArray(first);
    }
    long *result= calloc((size_t)max+1, sizeof(long));

    if(bigger[0]==smaller[0]) {
        addingTwoArrays(bigger+1, smaller+1, max, min,result);
        result[0]=bigger[0];
    }
    else{
        subTwoArrays(bigger+1,smaller+1,max,min,result);
        if(bigger[0]==-1){//if the bigger is negative
            result[0]=-1;
        }
        else{
            result[0]=1;
        }
    }
    free(first);
    free(second);
    free(bigger);
    free(smaller);
    return convertTObignum(result,max+1);

}
struct bignum* calcSub(struct bignum* first,struct bignum* second) {
    negateNumber(first);
    return calcSum(first,second);
}



void execute_p(struct stack *s) {//TODO remove
    if(peek(s)->sign==-1)
        putchar('-');
    printf("%s\n",peek(s)->digit);
}
void execute_c(struct stack *s) {
    freeStack(s);

}

enum state{number,notNumber};

int main() {
    struct bignum* currbignum;
    struct bignum *first;
    struct bignum *second;
    enum state currState = notNumber;
    struct stack *stack= malloc(sizeof(stack));
    char c;
    while ((c = (char) fgetc(stdin)) != 'q') {
        switch (currState) {
            case number:

                switch (c) {
                    case '0'...'9':
                        addDigit(c, currbignum);
                        break;
                    default:
                        addDigit('\0',currbignum);
                        minimizeBignumDigits(currbignum);
                        push(currbignum, stack);
                        currState = notNumber;

                }
                break;
            case notNumber:
                switch (c) {
                    case '*':
                        first= pop(stack);
                        second= pop(stack);
                        push(calcMult(first,second),stack);
                        break;
                    case '/':
                        first= pop(stack);
                        second= pop(stack);
                        push(calcDiv(first,second),stack);
                        break;
                    case '+':
                        first= pop(stack);
                        second= pop(stack);
                        push(calcSum(first,second),stack);
                        break;
                    case '-':
                        first= pop(stack);
                        second= pop(stack);
                        push(calcSub(first,second),stack);
                        break;
                    case 'p':
                        execute_p(stack);
                        break;
                    case 'c':
                        execute_c(stack);
                        break;
                    case '0'...'9':
                        currbignum=malloc(sizeof(*currbignum));
                        currbignum->digit = malloc(sizeof(char)*MAX_SIZE);
                        currbignum->sign=1;
                        currbignum->capacity = MAX_SIZE;
                        currbignum->numberOfDigits=0;
                        addDigit(c, currbignum);
                        currState = number;
                        break;
                    case '_':
                        currbignum=malloc(sizeof(*currbignum));
                        currbignum->digit = malloc(sizeof(char)*MAX_SIZE);
                        currbignum->sign=-1;
                        currbignum->capacity = MAX_SIZE;
                        currbignum->numberOfDigits=0;
                        currState = number;
                        break;
                    default:
                        break;
                }
        }
    }
    printf("exited\n");
//    free(stack);
}







