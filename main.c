#include <stdio.h>
#include <malloc.h>
#include <limits.h>
#include <stdbool.h>
#define MAX_SIZE 20
typedef struct bignum {
    long capacity;
    long numberOfDigits;
    char* digit;
};
long getLongValue(const char* digits, long end, long begin){
    long ans=0;
    for(;begin<=end;begin++){
        ans = ans*10 + (digits[begin]-'0');
    }
    return ans;
}
long getCarry(long number){
    if (number>999999999||number<-999999999){
        for(int i=0;i<9;i++){
            number=number/10;
        }
        return number;
    }
    return 0;
}
long getResult(long carry) {
    long j = carry;
    for (int i=0; i < 9; i++) {
        j = j * 10;
    }
//        else if(j<0)
    return j;
}

long *addingTwoArrays(const long first[], const long second[], long firstLength, long secondLength) {
    long max = firstLength;
    long min=secondLength;
    if (firstLength<secondLength) {
        max = secondLength;
        min = firstLength;
    }
    long carry=0;
    long *result = malloc(sizeof(long)*(max+1));
    for(long i=0;i<min;i++){
        result[i]=first[i]+second[i]+carry;
        carry=getCarry(result[i]);
        if(carry>0)
            result[i] =result[i]  - getResult(carry);//setting result[i] to be the actual result it should have

    }
    if(firstLength==min){
        for(;min<=max;min++){
            result[min]=second[min]+carry;
            carry = getCarry(result[min]);
            if(carry>0) {
                result[min] = result[min]-getResult(carry)  ;//setting result[i] to be the actual result it should have
            }
        }
        result[max]=carry;
    }
    else{
        for(;min<max;min++){
            result[min]=first[min]+carry;
            carry = getCarry(result[min]);
            if(carry>0) {
                result[min] = result[min] - getResult(carry);//setting result[i] to be the actual result it should have
            }
        }
        result[max]=carry;
    }
    return result;

}


long *convertToArray(struct bignum* number){
    bool isNegative = number->digit[0]=='_';
    if (isNegative)
        number->digit=number->digit+1;
    long size = number->numberOfDigits/9;
    long* answer=malloc(sizeof(long)*(size+1));
    long beginningOfLong=number->numberOfDigits-1;
    long endOfLong=beginningOfLong-8;
    for(long j=0;j<size+1;j++,beginningOfLong-=9,endOfLong-=9){
        if(endOfLong<0)
            answer[j]=getLongValue(number->digit, beginningOfLong,0);
        else
            answer[j]=getLongValue(number->digit, beginningOfLong,endOfLong);
        if (isNegative)
            answer[j]=answer[j]*(-1);
    }
    return answer;

}
void addDigit(char c, struct bignum* number);

struct stack {
    int size;
    struct bignum* firstBignum[1024];
};
void push(struct bignum * number, struct stack* s);
struct bignum *peek (struct stack *s);
void pop (struct stack *s);


extern void calcMult(struct stack* s);

long *recursiveMult(long *multiplied, long *multiplyer, long multipliedSize, long multiplyerSize);

void calcMult(struct stack *s) {//TODO remove
    struct bignum* first= s->firstBignum[s->size-1];
    struct bignum* second= s->firstBignum[s->size-2];
    long* multiplyer = convertToArray(first);
    long* multiplied = convertToArray(second);
    long multiplyerSize= first->numberOfDigits/9 + 1;
    long multipliedSize= second->numberOfDigits/9 + 1;
    long max = multiplyerSize + multipliedSize;

    if (isGE(multiplyer + 1, multiplyer + 1, multiplyerSize, multipliedSize)) {
        long tmp = multipliedSize;
        multipliedSize= multiplyerSize;
        multiplyerSize = tmp;
        long* tmpArray = multiplied;
        multiplied = multiplyer;
        multiplyer = tmpArray;
    }
    long* answer = recursiveMult(multiplied , multiplyer, multipliedSize, multiplyerSize);

    if ( (multiplied[0] == -1 && multiplyer[0] == 1) || (multiplied[0] == 1 && multiplyer[0] == -1)  )
        answer[0] = -1;
    else
        answer[0] = 1;

    push(convertTObignum(answer,max),s);
    free(multiplied);
    free(multiplyer);
}

bool isEqualZero(const long *a,long aSize ,const long *b, long bSize);
bool isEqualOne(long *a,long aSize);

long *subTwoArrays(long *toSub, long *substructor, long toSubSize, long substructorSize);

long *recursiveMult(long *multiplied, long *multiplyer, long multipliedSize, long multiplyerSize) {
    long* decrement;
    long * newMultiplyer;
    if (isEqualZero (multiplied, multipliedSize, multiplyer, multiplyerSize))
        return (long*)malloc(sizeof(long)*(2)) ;
    else if (isEqualOne(multiplied, multipliedSize) )
        return multiplyer;
    else if (isEqualOne (multiplyer, multiplyerSize))
        return multiplied;

    decrement = malloc(sizeof(long) * (2));
    decrement[0]= 1;
    decrement[1]= 1;
    newMultiplyer = subTwoArrays(multiplyer, decrement, multiplyerSize, 2 );
    //thinking how to save memory space by freeing the newMultiplyer pointer
    return addingTwoArrays(multiplied, recursiveMult(multiplied, newMultiplyer, multipliedSize,multiplyerSize ), multipliedSize, multiplyerSize);
}

long *subTwoArrays(long *toSub, long *substructor, long toSubSize, long substructorSize) {
    return NULL;
}

bool isEqualZero(const long *a, long aSize, const long *b, long bSize) {
    int con = 1;
    for (long i = 1; con && i < aSize-1; i++ ){
        if (a[i] != 0)
            con = 0;
    }
    if (con)
        return true;
    con=1;
    for (long j = 1; con && j < bSize - 1; j++ ){
        if (b[j]!=0)
            con = 0;
    }
    return con==1;
}
bool isEqualOne(long *a, long aSize) {
    int con = 1;
    if (a[1] != 1)
        con = 0;
    for (long i = 2; con && i < aSize-2; i++ ){
        if (a[i] != 0)
            con = 0;
    }
    return con==1;
}
extern void calcDiv(struct stack* s);//TODO remove
void calcDiv(struct stack *s) {
    printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}
struct bignum* convertTObignum(long array[],long size){
    struct bignum* num=malloc(sizeof(*num));
    num->digit = malloc(sizeof(char)*size*9+1);
    num->capacity = size*9+1;
    num->numberOfDigits=size*9+1;
    char* temp= num->digit;
    for (long i=0;i<size;i++,temp+=9) {
        sprintf(temp, "%09ld", array[size-1-i]);
    }
    num->digit[size*9]='\0';
    long i=0;
    for(;i<size*9&&num->digit[i]=='0';i++){}
    num->digit= (char *) num->digit+i;
    return num;

}

extern void calcSum(struct stack* s);//TODO remove
void calcSum(struct stack *s) {
    struct bignum *first= s->firstBignum[s->size-2];
    struct bignum *second= s->firstBignum[s->size-1];
    long firstNewSize = first->numberOfDigits/9+1;
    long secondNewSize = second->numberOfDigits/9+1;
    long max=firstNewSize;
    long min=secondNewSize;
    if(firstNewSize<secondNewSize){//if first number size is smaller then the second
        max = secondNewSize;
        min = firstNewSize;
    }
    long *result;
    result = addingTwoArrays(convertToArray(first), convertToArray(second),firstNewSize,secondNewSize);
    push(convertTObignum(result,max+1),s);

}

extern void calcSub(struct stack* s);//TODO remove
void calcSub(struct stack *s) {
    printf("caculating sub on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);

}

extern void execute_p(struct stack *s);
void execute_p(struct stack *s) {//TODO remove
    printf("%s\n",s->firstBignum[s->size-1]->digit);
}

extern void execute_c();
void execute_c() {//TODO remove
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

void pop(struct stack *s) {
      struct bignum *poped = peek(s);
     s->size --;
     free(poped);
}



