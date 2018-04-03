#include <malloc.h>
#include "main.h"

////
//// Created by hod on 30/03/18.
////


//int compare(struct bignum *number1, struct bignum *number2) {//return |number1|-|number2| (sort-of)
//    if(number1->numberOfDigits>number2->numberOfDigits){
//        return 1;
//    }
//    else if(number1->numberOfDigits<number2->numberOfDigits)
//        return-1;
//    for (long i = 0;i<number1->numberOfDigits;i++){
//        if (number1->digit[i]>number2->digit[i])
//            return 1;
//        else if(number1->digit[i]<number2->digit[i])
//            return -1;
//    }
//    return 0;
//}
//void subTwoArrays(long *bigger, const long *smaller, long max, long min,long *result) {
//    long borrow=0;
//    borrow=bigger[0]-smaller[0];
//    for(long i=0;i<max;i++){
//        if(i<min)
//            borrow=bigger[i]-smaller[i];
//        else
//            borrow=bigger[i];
//        if(borrow<0){
//            bigger[i+1]--;
//            borrow+=1000000000;
//            }
//        result[i+1]=borrow;
//    }
//    if(min!=max)
//        result[max]=bigger[max-1];
//}
//long getCarry(long number){
//    if (number>999999999){
//        number=number/1000000000;
//        return number;
//    }
//    return 0;
//}
//long arrangeCarry(long *cellToChange) {
//    long carry = getCarry(*cellToChange);
//    if(carry>0)
//        *cellToChange = *cellToChange-getResult(carry);//setting result[i] to be the actual result it should have
//    return carry;
//}
//long getResult(long carry) {
//    long j = carry;
//    j=j*1000000000;
//    return j;
//}
//void addingTwoArrays(const long bigger[], const long smaller[], long max, long min, long result[]) {
//    long carry=0;
//    for(long i=0;i<min;i++){
//        result[i+1]=bigger[i]+smaller[i]+carry;
//        carry=arrangeCarry(result+i+1);
//    }
//    for(;min<max;min++){
//        result[min+1]=bigger[min]+carry;
//        carry = arrangeCarry(result+min+1);
//    }
//    result[max]=result[max]+carry;
//}
void recCalcDiv(struct bignum** toDivide, struct bignum* divisor, struct bignum* factor, struct bignum** result) //toDo: moveToAssembly
{
   if (compare(*toDivide,divisor) >= 0) {
        struct bignum *newFactor = calcSumWithoutFree(factor, factor);
        struct bignum *newResult = calcSumWithoutFree(divisor, divisor);
        recCalcDiv(toDivide, newResult, newFactor, result);
        freeBignum(newFactor);
        freeBignum(newResult);
        if (compare(*toDivide,divisor) >= 0){
            struct bignum *newToDivide = calcSubWithoutFree(divisor,*toDivide);
            newResult = calcSumWithoutFree(factor,*result);
            free(*result);
            free(*toDivide);
            *result = newResult;
            *toDivide = newToDivide;
        }
    }
}
void recCalcMult(struct bignum** multiplier, struct bignum* multiplied, struct bignum* factor, struct bignum** result) {

  /*  if (compare(*multiplier,factor) < 0){
        return;
    }
    */if(compare(*multiplier,factor) >= 0) {
        struct bignum *newFactor = calcSumWithoutFree(factor, factor);
        struct bignum *newResult = calcSumWithoutFree(multiplied, multiplied);
        recCalcMult(multiplier, newResult, newFactor, result);
        freeBignum(newFactor);
        freeBignum(newResult);
        if (compare(*multiplier,factor) >= 0){
            struct bignum *newMultiplier = calcSubWithoutFree(factor,*multiplier);
            newResult = calcSumWithoutFree(multiplied,*result);
            free(*result);
            free(*multiplier);
            *result = newResult;
            *multiplier = newMultiplier;
        }
    }
}