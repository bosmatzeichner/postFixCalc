//
// Created by USER on 29/03/2018.
//
#include <stdbool.h>

#ifndef POSTFIXCALC_MAIN_H
#define POSTFIXCALC_MAIN_H
#define MAX_SIZE 20

typedef struct bignum {
    int sign;
    long capacity;
    long numberOfDigits;
    char* digit;
}bignum;
struct stack {
    int size;
    struct bignum* firstBignum[1024];
}stack;void addDigit(char c, struct bignum* number);
long getLongValue(const char* digits, long end, long begin);
extern long getCarry(long number);
extern long getResult(long carry);
long arrangeCarry(long *cellToChange);

long *convertToArray(struct bignum* number);
struct bignum* convertTObignum(long array[],long size);
void recCalcMult(long **twoDimArray, long counter, long *multiplied, long *multiplier, long multipliedSize, long multiplierSize);
void addingTwoArrays(const long first[],const long second[], long firstLength, long secondLength, long result[]);
struct stack;
void push(struct bignum *number, struct stack* s);
struct bignum *peek (struct stack *s);
struct bignum * pop(struct stack *s) ;
extern struct bignum* calcSum(struct bignum* first,struct bignum* second);
extern struct bignum* calcSub(struct bignum* first,struct bignum* second);
extern struct bignum* calcMult(struct bignum* first,struct bignum* second);
extern struct bignum* calcDiv(struct bignum* first,struct bignum* second);

extern void execute_p(struct stack *s);
extern void execute_c(struct stack *s);
long *sumTwoDimArray(long **twoDimArray, long max);
void subTwoArrays(long *bigger,const long *smaller, long max, long min, long *result);

int isEqualZeroOrOne(const long *a, long aSize);
bool isGE(const long* first,const long* second, long firstSize, long secondSize);
long *returnZeroOrOneArray(long eqZeroOrOne, long sign) ;
long returnSignOfCalc(const long *multiplied, const long *multiplier);
long *getFinalMult(long *multiplied, long *multiplier, long multipliedSize, long multiplierSize, long max);

int compare(struct bignum *number1, struct bignum *number2);
void negateNumber(struct bignum *number);
void printNumber(struct bignum *number);
void freeBignum(struct bignum *number);
void freeStack(struct stack* s);
void minimizeBignumDigits(struct bignum *number);

#endif //POSTFIXCALC_MAIN_H
