#include <stdio.h>
#include <malloc.h>

#define MAX_SIZE 20
typedef struct bignum {
    long capacity;
    long numberOfDigits;
    char* digit;
};

void addDigit(char c, struct bignum* number);

struct stack {
    int size;
    struct bignum* firstBignum[1024];
};
void push(struct bignum * number, struct stack* s);


extern void calcMult(struct stack* s);

void calcMult(struct stack *s) {//TODO remove
    printf("caculating mult on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}

extern void calcDiv(struct stack* s);//TODO remove
void calcDiv(struct stack *s) {
    printf("caculating div on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
}



extern void calcSum(struct stack* s);//TODO remove
void calcSum(struct stack *s) {
    printf("caculating addition on %s and %s\n", s->firstBignum[s->size-1]->digit,s->firstBignum[s->size-2]->digit);
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

