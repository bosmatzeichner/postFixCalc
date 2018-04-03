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
extern long arrangeCarry(long *cellToChange);

long *convertToArray(struct bignum* number);
struct bignum* convertTObignum(long array[],long size);
struct bignum* convertTObignumWithoutFree(long array[],long size);
void recCalcMult(long **twoDimArray, long counter, long *multiplied, long *multiplier, long multipliedSize, long multiplierSize);
extern void addingTwoArrays(const long first[],const long second[], long firstLength, long secondLength, long result[]);
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
extern void subTwoArrays(long *bigger,const long *smaller, long max, long min, long *result);
void recCalcMult1(struct bignum** multiplier, struct bignum* multiplied, struct bignum* factor, struct bignum** result);
int isEqualZeroOrSign(struct bignum** multiplier,struct bignum** multiplied );
struct bignum* returnZeroArray();
bool isGE(const long* first,const long* second, long firstSize, long secondSize);
long returnSignOfCalc(struct bignum *multiplied, struct bignum  *multiplier);
//long *getFinalMult(long *multiplied, long *multiplier, long multipliedSize, long multiplierSize, long max);

int compare(struct bignum *number1, struct bignum *number2);
void negateNumber(struct bignum *number);
void printNumber(struct bignum *number);
void freeBignum(struct bignum *number);
void freeStack(struct stack* s);
void minimizeBignumDigits(struct bignum *number);



struct bignum* calcSumWithoutFree(struct bignum* first,struct bignum* second);
struct bignum* calcSubWithoutFree(struct bignum* first,struct bignum* second);

#endif //POSTFIXCALC_MAIN_H
