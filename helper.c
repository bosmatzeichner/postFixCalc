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
    if (num==NULL)
        exit(-1);
    num->digit = malloc(sizeof(char)*size*9+1);
    if(num->digit==NULL){
        free(num);
        exit(-1);
    }
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
    if(newdigit==NULL){
        exit(-1);
    }
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
struct bignum* convertTObignumWithoutFree(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=malloc(sizeof(*num));
    if (num==NULL)
        exit(-1);
    num->digit = malloc(sizeof(char)*size*9+1);
    if(num->digit==NULL){
        free(num);
        exit(-1);
    }
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
    if(newdigit==NULL){
        exit(-1);
    }
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
    //free(array);

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
    if(answer==NULL)
        exit(-1);
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
        if(newNumber==NULL)
            exit(-1);
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


struct bignum* calcSumWithoutFree(struct bignum* first,struct bignum* second) {
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
    if(result==NULL)
        exit(-1);
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
    free(bigger);
    free(smaller);
    struct bignum* answer = convertTObignum(result,max+1);
    return answer;
}
struct bignum* calcSubWithoutFree(struct bignum* first,struct bignum* second) {
    negateNumber(first);
    return calcSumWithoutFree(first,second);
}
int isEqualZeroOrSign(struct bignum** multiplier,struct bignum** multiplied, int multOrDiv ) {
    long multiplierNewSize = (*multiplier)->numberOfDigits / 9 + 1;
    long multipliedNewSize = (*multiplied)->numberOfDigits / 9 + 1;
    long *multiplierArr = convertToArray(*multiplier);
    long *multipliedArr = convertToArray(*multiplied);
    int con = 0;
    int con2 = 0;

    for (long i = 1; con != -1 && i <= multiplierNewSize; i++) {
        if (multiplierArr[i] != 0)
            con = -1;
    }

    if (con != 0) {
        con = 0;
        con2 = 0;
        for (long i = 1; con != -1 && i <= multipliedNewSize; i++) {
            if (multipliedArr[i] != 0) {
                con = -1;
                con2 = -1;
            }
        }
    }
    if (multOrDiv == 1 && con2 == 0) {
        con = -2;
    }
    if (((*multiplier)->sign == -2) ||((*multiplied)->sign == -2)){
        con = -2;
    }

    if (con ==-1){
        con = ((*multiplied)->sign) * ((*multiplier)->sign);
            multipliedArr[0]=1;
            *multiplied = convertTObignum(multipliedArr,multipliedNewSize+1);
            (*multiplied)->sign = 1;
            multiplierArr[0]=1;
            *multiplier = convertTObignum(multiplierArr,multiplierNewSize+1);
            (*multiplier)->sign = 1;

    }
    //free(multiplierArr);
    //free(multipliedArr);
    return con;
}
struct bignum* returnZeroArray(){
    long *resultArr =  calloc(2, sizeof(long));
   // if(resultArr==NULL)
   //     exit(-1);
    resultArr[0] = 1;
    resultArr[1] = 0;
    return convertTObignum(resultArr,2);
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
    if(newdigit==NULL)
        exit(-1);
    for(long j = 0;j<number->numberOfDigits+i;j++){
        newdigit[j]=number->digit[i+j];
    }
    free(number->digit);
    number->digit=newdigit;
}


