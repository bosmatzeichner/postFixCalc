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

long convertToArray[](struct bignum* number){
    bool isNegative = number->digit[0]=='_';
    if (isNegative)
        number->digit=number->digit+1;
    long size = number->numberOfDigits/9+1;
    long* answer=malloc(sizeof(long)*size);
    long count=0;
    long acc;
    long index=0;
    while (count<size){
        acc=0;
        for(int i=0;i<9;i++,index++){
            acc = acc * 10 + (number->digit[index] - '0');
        }
        answer[count]=acc;
        if (isNegative)
            answer[count]=answer[count]*(-1);
        count++;
    }
    return answer;

}
void addDigit(char c, struct bignum* number);

struct stack {
    int size;
    struct bignum* firstBignum[1024];
};
void push(struct bignum * number, struct stack* s);


extern void calcMult(struct stack* s);

void calcMult(struct stack *s) {//TODO remove
    printf("caculating mult on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
    int carry;
    struct bignum* resBignum;

    struct bignum* bignum1= s->firstBignum[s->size-1];
    struct bignum* bignum2= s->firstBignum[s->size-1];
    long* bignum1Array = convertToArray(bignum1);
    long* bignum2Array = convertToArray(bignum2);
    long maxSize = bignum1->numberOfDigits+bignum2->numberOfDigits;
    long* answer= malloc(sizeof(long)*(maxSize)) ;
    long minNumDigits= bignum1->numberOfDigits;
    long maxNumDigits= bignum2->numberOfDigits;
    long*  multiplyer, multiplied;
    multiplyer = bignum1Array;
    multiplied = bignum2Array;
    if (bignum1->numberOfDigits > bignum2->numberOfDigits) {
        maxNumDigits= bignum1->numberOfDigits;
        minNumDigits = bignum2->numberOfDigits;
        multiplied = bignum1Array;
        multiplyer = bignum2Array;
    }
    char* newNumber1=(char*) malloc(sizeof(char)*(resBignum->capacity));
    char* newNumber2=(char*) malloc(sizeof(char)*(resBignum->capacity));
    //
    for (long i =0; i<maxSize ; i++ ){

        //multiply every 2 single digits
              for(long j=0; j<minNumDigits; j++){
                  mylong = multiplied[i]
              }


    }
}

extern void calcDiv(struct stack* s);//TODO remove
void calcDiv(struct stack *s) {
    printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}



extern void calcSum(struct stack* s);//TODO remove
void calcSum(struct stack *s) {
    struct bignum *first= s->firstBignum[s->size-2];
    struct bignum *second= s->firstBignum[s->size-1];
    long firstOriginalSize=first->numberOfDigits;
    long secondOriginalSize = second->numberOfDigits;
    long firstNewSize = firstOriginalSize/8+1;
    long secondNewSize = secondOriginalSize/8+1;
    long max;
    long min;
    if(firstNewSize<secondNewSize){
        max = secondNewSize;
        min = firstOriginalSize;
    }
    else{
        max = firstNewSize;
        min= secondNewSize;
    }
    long result[max =1];
    long carry;

}



extern void calcSub(struct stack* s);//TODO remove
void calcSub(struct stack *s) {
    printf("caculating sub on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);

}



extern void execute_p();
void execute_p() {//TODO remove
    printf("executing p\n");
}



extern void execute_c();
void execute_c() {//TODO remove
    printf("executing c\n");

}
enum state{number,notNumber};
int main() {
    printf("max long size is: %ld\n min long size is: %ld\n",LONG_MAX,LONG_MIN);
    printf("max int size is: %d\n min int size is: %d\n",INT_MAX,INT_MIN);
    printf("size of long is %ld",(unsigned long) sizeof(int));

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
                        execute_p();
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
    free(stack);
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

