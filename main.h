//
// Created by USER on 29/03/2018.
//

#ifndef POSTFIXCALC_MAIN_H
#define POSTFIXCALC_MAIN_H


long getLongValue(const char* digits, long end, long begin);
long getCarry(long number);
long getResult(long carry);
long arrangeCarry(long *cellToChange);


struct bignum* convertTObignum(long array[],long size);
void recCalcMult(long **twoDimArray, long counter, long *multiplied, long *multiplier, long multipliedSize, long multiplierSize);

int main();
struct stack;
void push(struct bignum *number, struct stack* s);
struct bignum *peek (struct stack *s);
struct bignum * pop(struct stack *s) ;
extern struct bignum* calcSum(struct bignum* first,struct bignum* second);
extern struct bignum* calcSub(struct bignum* first,struct bignum* second);
extern struct bignum* calcMult(struct bignum* first,struct bignum* second);
extern struct bignum* calcDiv(struct bignum* first,struct bignum* second);

extern void execute_p(struct stack *s);
extern void execute_c();

long *subTwoArrays(long *toSubFrom, long *substructor, long toSubFromSize, long substructorSize);

int isEqualZeroOrOne(const long *a, long aSize);
bool isGE(const long* first,const long* second, long firstSize, long secondSize);
long *returnZeroOrOneArray(long eqZeroOrOne, long sign) ;
long returnSignOfCalc(const long *multiplied, const long *multiplier);
long *getFinalMult(long *multiplied, long *multiplier, long multipliedSize, long multiplierSize, long max);

int compare(struct bignum *number1, struct bignum *number2);
void negateNumber(struct bignum *number);
void printNumber(struct bignum *number);
#endif //POSTFIXCALC_MAIN_H
