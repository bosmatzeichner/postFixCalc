//
// Created by hod on 30/03/18.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"



struct bignum* convertTObignum(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=malloc(sizeof(*num));
    num->digit = malloc(sizeof(char)*size*9+1);
    num->capacity = size*9+1;
    char* temp= num->digit;
    num->digit[size*9]='\0';
    for (long i=0;i<size-1;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}
    num->numberOfDigits= (long) strlen(num->digit+i);
    if(num->numberOfDigits==0)
        i--;
    char*newdigit =malloc(sizeof(char)*num->numberOfDigits+1);
    for(long j = 0;num->digit[i+j]!='\0';j++){
        newdigit[j]=num->digit[i+j];
        newdigit[j+1]='\0';
    }
    free(num->digit);
    num->digit=newdigit;
    num->sign=1;
    if (isNegative){
        num->sign=-1;
    }
    free(array);

    return num;

}
bool isGE(const long* first,const long* second, long firstSize, long secondSize){
    long min=firstSize;
    if(firstSize>secondSize){
        for(long i=firstSize;i>secondSize;i--){
            if(first[i]>0)
                return true;
        }
        min=secondSize;
    }
    else if (secondSize>firstSize){
        for(long i=secondSize;i>firstSize;i--){
            if(second[i]>0)
                return false;
        }
    }

    for(;min>0;min--){
        if(first[min-1]>second[min-1])
            return true;
        else if(second[min-1]>first[min-1])
            return false;
    }
    return false;
}
long getLongValue(const char* digits, long end, long begin){
    long ans=0;
    for(;begin<=end;begin++){
        ans = ans*10 + (digits[begin]-'0');
    }
    return ans;
}
long *convertToArray(struct bignum* number){
    bool isNegative = number->sign==-1;
    long size = number->numberOfDigits/9+1;
    long* answer=calloc((size_t)size+1,sizeof(long));
    long beginningOfLong=number->numberOfDigits-1;
    long endOfLong=beginningOfLong-8;
    if(isNegative)
        answer[0]=-1;
    else
        answer[0]=1;
    for(long j=1;j<size+1;j++,beginningOfLong-=9,endOfLong-=9){
        if(endOfLong<0)
            answer[j]=getLongValue(number->digit, beginningOfLong,0);
        else
            answer[j]=getLongValue(number->digit, beginningOfLong,endOfLong);
    }
    return answer;

}
void addDigit(char c, struct bignum* number) {
    number->digit[number->numberOfDigits]= c;
    number->numberOfDigits++;
    if(number->numberOfDigits==number->capacity-1){
        number->capacity= number->capacity+MAX_SIZE;
        char* newNumber;
        newNumber=(char*) malloc(sizeof(char)*(number->capacity));
        char*j= newNumber;
        for(char* i=number->digit; i < number->digit + number->numberOfDigits; i++,j++){
            *j=*i;
        }
        free(number->digit);
        number->digit=newNumber;
    }
    if(c=='\0')
        number->numberOfDigits--;
}


void recCalcMult2(long** multiplier, long * multiplied, long* factor, long** result) {

    if (isGE(*multiplier,factor, sizeof(*multiplier), sizeof(factor)) < 0){
        *result = calloc(sizeof(long), 2 );
        *result[0] = 1;
    }
    else {
        long *newFactor = calloc(sizeof(long), sizeof(factor)+1 );
        addingTwoArrays(factor+1, factor+1, sizeof(factor), sizeof(factor), newFactor);
        long *newResult = calloc(sizeof(long), sizeof(multiplied)+1 );
        addingTwoArrays(multiplied+1, multiplied+1, sizeof(multiplied),sizeof(multiplied), newResult);
        recCalcMult2(multiplier, newResult, newFactor, result);
        free(newFactor);
        free(newResult);
        if (isGE(*multiplier,factor, sizeof(*multiplier), sizeof(factor)) >= 0){
            long *newMultiplier ;
            subTwoArrays(*multiplier, factor, sizeof(*multiplier), sizeof(factor), newMultiplier);
            addingTwoArrays(multiplied+1, *result+1 , sizeof(multiplied),sizeof(*result), newResult);
            free(*result);
            free (*multiplier);
            *result = newResult;
            *multiplier = newMultiplier;
        }


    }
}
/*void recCalcMult(long **twoDimArray, long counter, long *multiplied, long *multiplier, long multipliedSize, long multiplierSize){
    //finished the calculating
    if (counter == multiplierSize) {
        return;
    }
    long carry = 0;
    long index;

    for (index = 0; index < multipliedSize ; index++){
        //couter start by 1 couse the first long is the sign
        twoDimArray[counter][counter + index + 1] =  multiplier [counter] * multiplied [index+1] + carry;
        carry = arrangeCarry(& twoDimArray[counter][counter + index+1]);
    }
   // if (carry > 0){
   //     twoDimArray[counter - 1][counter + index] = carry ;
   // }
    recCalcMult(twoDimArray, counter+1 , multiplied, multiplier, multipliedSize , multiplierSize);
}
*/
int isEqualZeroOrOne(const long *a, long aSize) {
    int con;
    if (a[1] != 1) {
        con = -1;
    }
    else if (a[1] != 0 ){
        con = -1;
    }
    else {
        if (a[1] == 1){
            con = 1;
        }
        else {
            con = 0;
        }
        for (long i = 2; con!=-1 && i < aSize-1; i++ ){
            if (a[i] != 0)
                con = -1;
        }
    }
    return con;
}
long *returnZeroOrOneArray(long eqZeroOrOne1,long eqZeroOrOne2, long sign) {
    //check for cases of 0\1
    long *finalAnswer = calloc(2, sizeof(long));
    if (eqZeroOrOne1 == 0 || eqZeroOrOne2==0) {
        finalAnswer[0] = sign;
        finalAnswer[1] = 0;
    } else if (eqZeroOrOne1 == 1 || eqZeroOrOne2 == 1) {
        finalAnswer[0] = sign;
        finalAnswer[1] = 1;
    }
    return finalAnswer;
}
long returnSignOfCalc(const long *multiplied, const long *multiplier) {
    if ( (multiplied[0] == -1 && multiplier[0] == 1) || (multiplied[0] == 1 && multiplier[0] == -1)  )
        return -1;
    else
        return 1;
}
/*long *getFinalMult(long *multiplied, long *multiplier, long multipliedSize, long multiplierSize, long max) {
    long **answer;
    long *finalAnswer;
    long sign;
    sign = returnSignOfCalc(multiplied, multiplier);
    long secEqZeroOrOne;
    secEqZeroOrOne = isEqualZeroOrOne(multiplier, multiplierSize);
    if (secEqZeroOrOne == 0 || secEqZeroOrOne == 1) {
        finalAnswer = returnZeroOrOneArray (secEqZeroOrOne, sign);
    }
    else {
        answer = calloc((size_t)multiplierSize, sizeof(long));
        for(long i=0; i<multiplierSize; i++){
            answer[i] = calloc((size_t)max, sizeof(long));
        }
        long counter = 0;
       // recCalcMult(answer, counter, multiplied, multiplier, multipliedSize, multiplierSize);
        finalAnswer =  sumTwoDimArray(answer, multiplierSize, max);
        finalAnswer[0] = sign;
        //free any array !
        for(long i=0; i<multiplierSize; i++){
            free (answer[i]);
        }
        free(answer);
    }
    return finalAnswer;
}*/
long *sumTwoDimArray(long **twoDimArray, long multiplierSize, long max) {
    long i;
    long *finalArray;
    finalArray = calloc((size_t) max, sizeof(long));
    for ( i = 0; i < multiplierSize; i++ ){
        addingTwoArrays(finalArray+1, twoDimArray[i], max, max, finalArray+1);
    }
    return finalArray;
}
void negateNumber(struct bignum *number) {
    number->sign=number->sign*(-1);
}
void printNumber(struct bignum *number) {
    printf("{\n\tsign: %d\n\tcapacity: %ld\n\tnumberOfDigits: %ld\n\tdigits: %s\n\t}\n",number->sign
            ,number->capacity,number->numberOfDigits, number->digit);
}
void freeBignum(struct bignum *number) {
    free(number->digit);
    free(number);
}
void freeStack(struct stack *s) {
    for (struct bignum** number=s->firstBignum;number<s->firstBignum+s->size;number++){
        freeBignum(*number);
    }
    free(s);
}

void minimizeBignumDigits(struct bignum *number) {
    long i=0;
    for(;i<number->numberOfDigits*9&&number->digit[i]=='0';i++){}
    if (i==number->numberOfDigits)
        i--;
    number->numberOfDigits=number->numberOfDigits-i;
    char*newdigit =malloc(sizeof(char)*number->numberOfDigits);
    for(long j = 0;j<number->numberOfDigits+i;j++){
        newdigit[j]=number->digit[i+j];
    }
    free(number->digit);
    number->digit=newdigit;
}


