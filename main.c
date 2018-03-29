#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#define MAX_SIZE 20
typedef struct bignum {
    long capacity;
    long numberOfDigits;
    char* digit;
};
struct bignum* convertTObignum(long array[],long size){
    bool isNegative=array[0]==-1;
    struct bignum* num=malloc(sizeof(*num));
    num->digit = malloc(sizeof(char)*size*9+2);
    num->capacity = size*9+2;
    char* temp= num->digit;
    num->digit[size*9]='\0';
    num->digit[0]='0';//saving space for '-' if necessary
    temp++;
    for (long i=0;i<size-1;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}
    num->digit= (char*) num->digit+i;
    if (isNegative){
        num->digit[-1]='_';
        num->digit--;
    }
    num->numberOfDigits= (long) strlen(num->digit);
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
    for(;min>=0;min--){
        if(first[min]>second[min])
            return true;
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
void addingTwoArrays(const long first[], const long second[], long firstLength, long secondLength) {
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
    long *result = malloc(sizeof(long)*(max+1));
    for(long i=0;i<min;i++){
        result[i+1]=bigger[i]+smaller[i]+carry;
        carry=getCarry(result[i+1]);
        if(carry>0)
            result[i+1] =result[i+1]-getResult(carry);//setting result[i] to be the actual result it should have

    }
    for(;min<max;min++){
        result[min+1]=bigger[min]+carry;
        carry = getCarry(result[min+1]);
        if(carry>0) {
            result[min+1] = result[min+1]-getResult(carry)  ;//setting result[i] to be the actual result it should have
        }
    }
    result[max]=result[max]+carry;
    return result;

}
long *convertToArray(struct bignum* number){
    bool isNegative = number->digit[0]=='_';
    if (isNegative) {
        number->digit = number->digit + 1;
        number->numberOfDigits=number->numberOfDigits-1;
    }
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
void addDigit(char c, struct bignum *number) {
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
        answer = malloc(sizeof(long) * (max) * sizeof(long) * (multiplierSize));
        long counter = 0;
        recCalcMult(answer, counter, multiplied, multiplier, multipliedSize, multiplierSize);
        finalAnswer =  sumTwoDimArray(answer, max);
        finalAnswer[0] = sign;
        free(answer);
    }
    return finalAnswer;
}
long *sumTwoDimArray(long **twoDimArray, long max) {
    long i;
    long *finalArray;
    finalArray = calloc((size_t) max, sizeof(long));
    for ( i = 0; i < sizeof(twoDimArray); i++ ){
       addingTwoArrays(finalArray, twoDimArray[i], max, max);
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
    for(long i=0;i<max-1;i++){
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
    result[max]=bigger[max-1];
    return result;
}

void calcDiv(struct stack *s) {
    printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}
void calcSum(struct stack *s) {
    struct bignum *first= pop(s);
    struct bignum *second= pop(s);
    long firstNewSize = first->numberOfDigits/9+1;
    if(first->digit[0]=='_')
        firstNewSize = (first->numberOfDigits-1)/9+1;
    long secondNewSize = second->numberOfDigits/9+1;
    if(second->digit[0]=='_')
        secondNewSize=(second->numberOfDigits-1)/9+1;
    long max=firstNewSize;
    long min=secondNewSize;
    if(firstNewSize<secondNewSize){//if first number size is smaller then the second
        max = secondNewSize;
        min = firstNewSize;
    }
    long *result;
    long* firstArray = convertToArray(first);
    long* secondArray = convertToArray(second);
    if(firstArray[0]==secondArray[0]) {
        result = addingTwoArrays(firstArray+1, secondArray+1, firstNewSize, secondNewSize);
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
    push(convertTObignum(result,max+1),s);

}
void calcSub(struct stack *s) {
    printf("caculating sub on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);

}

void execute_p(struct stack *s) {
    if(peek(s)->digit[0]=='_'){
        putchar('-');
        printf("%s\n",peek(s)->digit+1);
    }
    else
        printf("%s\n",peek(s)->digit);
}
void execute_c() {
    printf("executing c\n");

}

enum state{number,notNumber};

int main() {
    struct bignum* currbignum;
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
                        calcSum(stack);
                        break;
                    case '-':
                        calcSub(stack);
                        break;
                    case 'p':
                        execute_p(stack);
                        break;
                    case 'c':
                        execute_c();
                        break;
                    case '0'...'9':
                    case '_':
                        currbignum=malloc(sizeof(*currbignum));
                        currbignum->digit = malloc(sizeof(char)*MAX_SIZE);
                        currbignum->capacity = MAX_SIZE;
                        currbignum->numberOfDigits=0;
                        addDigit(c, currbignum);
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


