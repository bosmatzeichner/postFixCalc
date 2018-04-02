//
// Created by hod on 30/03/18.
//
#include "main.h"
#include <malloc.h>

int compare(struct bignum *number1, struct bignum *number2) {//return |number1|-|number2| (sort-of)
    if(number1->numberOfDigits>number2->numberOfDigits){
        return 1;
    }
    else if(number1->numberOfDigits<number2->numberOfDigits)
        return-1;
    for (long i = 0;i<number1->numberOfDigits;i++){
        if (number1->digit[i]>number2->digit[i])
            return 1;
        else if(number1->digit[i]<number2->digit[i])
            return -1;
    }
    return 0;
}
void subTwoArrays(long *bigger, const long *smaller, long max, long min,long *result) {
    long borrow=0;
    borrow=bigger[0]-smaller[0];
    for(long i=0;i<max;i++){
        if(i<min)
            borrow=bigger[i]-smaller[i];
        else
            borrow=bigger[i];
        if(borrow<0){
            bigger[i+1]--;
            borrow+=1000000000;
            }
        result[i+1]=borrow;
    }
    if(min!=max)
        result[max]=bigger[max-1];
}
long getCarry(long number){
    if (number>999999999){
        number=number/1000000000;
        return number;
    }
    return 0;
}
long arrangeCarry(long *cellToChange) {
    long carry = getCarry(*cellToChange);
    if(carry>0)
        *cellToChange = *cellToChange-getResult(carry);//setting result[i] to be the actual result it should have
    return carry;
}
long getResult(long carry) {
    long j = carry;
    j=j*1000000000;
    return j;
}
void addingTwoArrays(const long bigger[], const long smaller[], long max, long min, long result[]) {
    long carry=0;
    for(long i=0;i<min;i++){
        result[i+1]=bigger[i]+smaller[i]+carry;
        carry=arrangeCarry(result+i+1);
    }
    for(;min<max;min++){
        result[min+1]=bigger[min]+carry;
        carry = arrangeCarry(result+min+1);
    }
    result[max]=result[max]+carry;
}
