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
struct bignum* calcMult(struct bignum* first,struct bignum* second) {
    struct bignum **result =calloc(1, sizeof(long));
    struct bignum* multiplier = first;
    struct bignum* multiplied = second;
    if (compare(multiplier,multiplied) > 0 ) {
        struct bignum* tmp = multiplied;
        multiplied = multiplier;
        multiplier = tmp;
    }
    int sign = isEqualZeroOrSign(&multiplier,&multiplied, 0);
    if (sign==0 || sign == -2) {
        *result = returnZeroArray();
        freeBignum(multiplier);
        (*result) ->sign =sign;
    }
    else{

        struct bignum **multiplierPTR = calloc(1, sizeof(long)); /////
        *multiplierPTR = multiplier;

        long *factor = calloc(2,sizeof(long));
        factor[0] = 1;
        factor[1] = 1;

        struct bignum* factorPTR = convertTObignumWithoutFree(factor, 2);
        long *resultArr = calloc(2,sizeof(long));
        resultArr[0] = 1;
        *result = convertTObignumWithoutFree(resultArr, 2);
        recCalcMult(multiplierPTR, multiplied, factorPTR, result);
        ((*result)->sign) = 1;
        if (sign == -1){
            resultArr = convertToArray(*result);
            resultArr[0] = -1;
            long resultSize = (*result)->numberOfDigits/9+1;
            *result = convertTObignumWithoutFree(resultArr, resultSize+1);
            ((*result)->sign) = -1;
        }
        free(factor);
        freeBignum(*multiplierPTR);
        free(multiplierPTR); //free multiplier
        free(resultArr);

    }

    freeBignum(multiplied);


    return *result;
}
struct bignum* calcDiv(struct bignum* first,struct bignum* second) {
    struct bignum **result = calloc(1,sizeof(long));
    struct bignum* toDivide = second;
    struct bignum* divisor = first;

    int sign = isEqualZeroOrSign(&toDivide,&divisor, 1);
    if (sign==0 || sign == -2) {
        *result = returnZeroArray();
        freeBignum(toDivide);
        (*result) ->sign =sign;
    }
    else{
        struct bignum **toDividePTR = calloc(1, sizeof(long));
        *toDividePTR = toDivide;
        long *factor = calloc(2,sizeof(long));
        factor[0] = 1;
        factor[1] = 1;

        struct bignum* factorPTR = convertTObignumWithoutFree(factor, 2);
        long *resultArr = calloc(2,sizeof(long));
        resultArr[0] = 1;
        *result = convertTObignumWithoutFree(resultArr, 2);
        recCalcDiv(toDividePTR, divisor, factorPTR, result);
        if (isEqualZeroOrSign(result,result,0) == 0)
            sign = 0;
        ((*result)->sign) = sign;
        if (sign == -1){
            resultArr = convertToArray(*result);
            resultArr[0] = -1;
            long resultSize = (*result)->numberOfDigits/9+1;
            *result = convertTObignumWithoutFree(resultArr, resultSize+1);
            ((*result)->sign) = -1;
        }
        free(factor);
        freeBignum(*toDividePTR);
        free(toDividePTR);
        free(resultArr);
//
    }

    freeBignum(divisor);


    return *result;
}
struct bignum* calcSum(struct bignum* first,struct bignum* second) {
    struct bignum* answer = calcSumWithoutFree(first,second);
    freeBignum(first);
    freeBignum(second);
    return answer;
}
struct bignum* calcSub(struct bignum* first,struct bignum* second) {
    struct bignum* answer= calcSubWithoutFree(first,second);
    freeBignum(first);
    freeBignum(second);
    return answer;
}

void execute_p(struct stack *s) {//TODO remove
//    printNumber(peek(s));
    if (peek(s)->sign ==-2) {
        printf("Error: division by zero!\n");
        struct bignum *result = pop(s);
        freeBignum(result);
    } else{
        if(peek(s)->sign==-1)
            putchar('-');
        printf("%s\n",peek(s)->digit);
    }
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
    struct stack *stack= malloc(sizeof(struct stack));
    if (stack==NULL)
        return -1;
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
                        struct bignum *result = calcDiv(first,second);
                        push(result,stack);
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
                        if (currbignum==NULL)
                            return -1;
                        currbignum->digit = malloc(sizeof(char)*MAX_SIZE);
                        if (currbignum->digit==NULL){
                            free(currbignum);
                            return -1;
                        }
                        currbignum->sign=1;
                        currbignum->capacity = MAX_SIZE;
                        currbignum->numberOfDigits=0;
                        addDigit(c, currbignum);
                        currState = number;
                        break;
                    case '_':
                        currbignum=malloc(sizeof(*currbignum));
                        if (currbignum==NULL)
                            return -1;
                        currbignum->digit = malloc(sizeof(char)*MAX_SIZE);
                        if (currbignum->digit==NULL){
                            free(currbignum);
                            return -1;
                        }                        currbignum->sign=-1;
                        currbignum->capacity = MAX_SIZE;
                        currbignum->numberOfDigits=0;
                        currState = number;
                        break;
                    default:
                        break;
                }
        }
    }
}







