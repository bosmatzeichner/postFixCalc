#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
void addDigit(char c, struct bignum* number);
long *convertToArray(struct bignum* number);
void addingTwoArrays(long first[],long second[], long firstLength, long secondLength, long result[]);

#define MAX_SIZE 20
typedef struct bignum {
    int sign;
    long capacity;
    long numberOfDigits;
    char* digit;
};
struct bignum* convertTObignum(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=malloc(sizeof(*num));
    num->digit = malloc(sizeof(char)*size*9+1);
    num->capacity = size*9+1;
    char* temp= num->digit;
    char* buckup = num->digit;
    num->digit[size*9]='\0';
    for (long i=0;i<size-1;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}
    num->digit= num->digit+i;
    num->sign=1;
    if (isNegative){
        num->sign=-1;
    }
    num->numberOfDigits= (long) strlen(num->digit);
    free(array);
//    realloc(num->digit,(size_t)num->numberOfDigits);
//    free(buckup);
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
long getCarry(long number){
    if (number>999999999){
        for(int i=0;i<9;i++){
            number=number/10;
        }
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
    for (int i=0; i < 9; i++) {
        j = j * 10;
    }
    return j;
}

void addingTwoArrays(long first[],long second[], long firstLength, long secondLength, long result[]) {
    long max = firstLength;
    long min=secondLength;
    long* bigger = first;
    long *smaller = second;

    if (firstLength<secondLength) {
        max = secondLength;
        min = firstLength;
        bigger = second;
        smaller = first;
    }
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

struct stack {
    int size;
    struct bignum* firstBignum[1024];
};
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
}
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

void recCalcMult(long **twoDimArray, long counter, long *multiplied, long *multiplier, long multipliedSize, long multiplierSize){
    //finished the calculating
    if (counter == multiplierSize)
        return;

    long carry = 0;
    long index;

    for (index = 1; index < multipliedSize ; index++){
        //couter start by 1 couse the first long is the sign
        twoDimArray[counter - 1][counter + index - 1] =  multiplier [counter] * multiplied [index] + carry;
        carry = arrangeCarry(& twoDimArray[counter - 1][index-1]);
    }
    if (carry > 0){
        twoDimArray[counter - 1][counter + index] = carry ;
    }
    recCalcMult(twoDimArray, counter+1 , multiplied, multiplier, multipliedSize , multiplierSize);
}

// meir advice - multiply with binary counter
void calcMult(struct stack *s) {
    struct bignum *first = s->firstBignum[s->size - 1];
    struct bignum *second = s->firstBignum[s->size - 2];
    long *multiplier = convertToArray(first);
    long *multiplied = convertToArray(second);
    long * finalAnswer;
    long multiplierSize = first->numberOfDigits / 9 + 1;
    long multipliedSize = second->numberOfDigits / 9 + 1;
    //check greater or equal without sign bit
    if (isGE(multiplier + 1, multiplier + 1, multiplierSize, multipliedSize)) {
        long tmp = multipliedSize;
        multipliedSize = multiplierSize;
        multiplierSize = tmp;
        long *tmpArray = multiplied;
        multiplied = multiplier;
        multiplier = tmpArray;
    }
    long max = multiplierSize + multipliedSize;
    finalAnswer = getFinalMult (multiplied, multiplier, multipliedSize, multiplierSize, max);
    push(convertTObignum(finalAnswer,max),s);
    free(multiplied);
    free(multiplier);
}
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
long *returnZeroOrOneArray(long eqZeroOrOne, long sign) {
    //check for cases of 0\1
    long *finalAnswer;
    if (eqZeroOrOne == 0) {
        finalAnswer = calloc(2, sizeof(long));
        finalAnswer[0] = sign;
    } else if (eqZeroOrOne == 1) {
        finalAnswer = calloc(2, sizeof(long));
        finalAnswer[0] = sign;
        finalAnswer[1] = 1;
    }}
long returnSignOfCalc(const long *multiplied, const long *multiplier) {
    if ( (multiplied[0] == -1 && multiplier[0] == 1) || (multiplied[0] == 1 && multiplier[0] == -1)  )
        return -1;
    else
        return 1;
}
long *getFinalMult(long *multiplied, long *multiplier, long multipliedSize, long multiplierSize, long max) {
    long **answer;
    long *finalAnswer;
    long sign = returnSignOfCalc (multiplied, multiplier);
    long secEqZeroOrOne;
    secEqZeroOrOne = isEqualZeroOrOne(multiplier, multiplierSize);
    if (secEqZeroOrOne == 0 || secEqZeroOrOne == 1) {
        finalAnswer = returnZeroOrOneArray (secEqZeroOrOne, sign);
    }
    else {
        answer = calloc(multiplierSize, sizeof(long));
        for(long i=0; i<multiplierSize; i++){
            answer[i] = calloc(max, sizeof(long));
        }
        long counter = 0;
        recCalcMult(answer, counter, multiplied, multiplier, multipliedSize, multiplierSize);
        finalAnswer =  sumTwoDimArray(answer, max);
        finalAnswer[0] = sign;
        //free any array !
        for(long i=0; i<multiplierSize; i++){
            free (answer[i]);
        }
        free(answer);
    }
    return finalAnswer;
}
long *sumTwoDimArray(long **twoDimArray, long max) {
    long i;
    long *finalArray;
    finalArray = calloc((size_t) max, sizeof(long));
    for ( i = 0; i < sizeof(twoDimArray); i++ ){
       addingTwoArrays(finalArray, twoDimArray[i], max, max, finalArray);
    }
    return finalArray;
}

long *subTwoArrays(long *toSubFrom, long *substructor, long toSubFromSize, long substructorSize) {
    long borrow=0;
    long max=toSubFromSize;
    long min = substructorSize;
    long* bigger = toSubFrom;
    long* smaller = substructor;
    if(!isGE(toSubFrom,substructor,toSubFromSize,substructorSize)){
        max=substructorSize;
        min = toSubFromSize;
        bigger = substructor;
        smaller = toSubFrom;
    }
    long* result = malloc(sizeof(long)*max+1);
    borrow=bigger[0]-smaller[0];
    for(long i=0;i<max;i++){
        if(i<min){
            borrow=bigger[i]-smaller[i];
            if(borrow<0){
                bigger[i+1]--;
                borrow+=1000000000;
            }
            result[i+1]=borrow;
        }
        else{
            borrow=bigger[i];
            if(borrow<0){
                bigger[i+1]--;
                borrow+=1000000000;
            }
            result[i+1]=borrow;
        }
    }
    if(min!=max)
        result[max]=bigger[max-1];
    return result;
}

void calcDiv(struct stack *s) {
    printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}
struct bignum* calcSum(struct bignum* first,struct bignum* second) {
    long firstNewSize = first->numberOfDigits/9+1;
    long secondNewSize = second->numberOfDigits/9+1;;
    long max=firstNewSize;
    if(firstNewSize<secondNewSize){//if first number size is smaller then the second
        max = secondNewSize;
    }
    long *result= calloc((size_t)max+1, sizeof(long));
    long* firstArray = convertToArray(first);
    long* secondArray = convertToArray(second);
    if(firstArray[0]==secondArray[0]) {
        addingTwoArrays(firstArray+1, secondArray+1, firstNewSize, secondNewSize,result);
        result[0]=firstArray[0];
    }
    else{
        result = subTwoArrays(firstArray+1,secondArray+1,firstNewSize,secondNewSize);
        bool firstGreaterOrEqualtoSecond = isGE(firstArray+1,secondArray+1,firstNewSize,secondNewSize);
        if(firstArray[0]==-1){//if the first is negative
            if(firstGreaterOrEqualtoSecond)
                result[0]=-1;
            else
                result[0]=1;
        }
        else{
            if(firstGreaterOrEqualtoSecond)
                result[0]=1;
            else
                result[0]=-1;
        }
    }
    free(first);
    free(second);
    free(firstArray);
    free(secondArray);
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
void execute_c() {
    printf("executing c\n");

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
                        push(currbignum, stack);
                        currState = notNumber;

                }
                break;
            case notNumber:
                switch (c) {
                    case '*':
                        calcMult(stack);
                        break;
                    case '/':
                        calcDiv(stack);
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
                        execute_c();
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

int compare(struct bignum *number1, struct bignum *number2) {//return |number1|-|number2| (sort-of)
    for (long i = 0;i<number1->numberOfDigits&&i<number2->numberOfDigits;i++){
        if (number1->digit[i]>number2->digit[i])
            return 1;
        else if(number1->digit[i]<number2->digit[i])
            return -1;
    }
    if(number1->numberOfDigits>number2->numberOfDigits)
        return 1;
    else if(number1->numberOfDigits>number2->numberOfDigits){
        return -1;
    }
    return 0;
}

void negateNumber(struct bignum *number) {
    number->sign=number->sign*(-1);
}

void printNumber(struct bignum *number) {
    printf("{\nsign: %d\ncapacity: %ld\nnumberOfDigits: %ld\ndigits: %s\n}",number->sign
    ,number->capacity,number->numberOfDigits, number->digit);
}


