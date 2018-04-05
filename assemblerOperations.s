;;; 64bit-frame.x86-64
;;; Code for easy access to arguments & components of
;;; the frame (from within a function)
;;;
;;; The assumption is that the frame is accessed through the RBP!
;;;
;;; Programmer: Mayer Goldberg, 2018
%define A0 rdi
%define A1 rsi
%define A2 rdx
%define A3 rcx
%define A4 r8
%define A5 r9

%define param(offset) qword [rbp + offset]

struc x86frame64bit
.old_rbp: resq 1
.ret_addr: resq 1
.A6: resq 1
.A7: resq 1
.A8: resq 1
.A9: resq 1
.A10: resq 1
.A11: resq 1
endstruc

%define old_rbp param(x86frame64bit.old_rbp)
%define ret_addr param(x86frame64bit.ret_addr)
%define A6 param(x86frame64bit.A6)
%define A7 param(x86frame64bit.A7)
%define A8 param(x86frame64bit.A8)
%define A9 param(x86frame64bit.A9)
%define A10 param(x86frame64bit.A10)
%define A11 param(x86frame64bit.A11)

struc x86frame64bitLocals
.old_rbp: resq 1
.Loc0: resq 1
.Loc1: resq 1
.Loc2: resq 1
.Loc3: resq 1
.Loc4: resq 1
.Loc5: resq 1
.Loc6: resq 1
.Loc7: resq 1
.Loc8: resq 1
.Loc9: resq 1
.Loc10: resq 1
.Loc11: resq 1
endstruc

struc bignum
.sign: resd 1
.capacity: resq 1
.numberOfDigits: resq 1
.digit: resq 1
endstruc

%define local_var(offset) qword [rbp - offset]
%define Loc0 local_var(x86frame64bitLocals.Loc0)
%define Loc1 local_var(x86frame64bitLocals.Loc1)
%define Loc2 local_var(x86frame64bitLocals.Loc2)
%define Loc3 local_var(x86frame64bitLocals.Loc3)
%define Loc4 local_var(x86frame64bitLocals.Loc4)
%define Loc5 local_var(x86frame64bitLocals.Loc5)
%define Loc6 local_var(x86frame64bitLocals.Loc6)
%define Loc7 local_var(x86frame64bitLocals.Loc7)
%define Loc8 local_var(x86frame64bitLocals.Loc8)
%define Loc9 local_var(x86frame64bitLocals.Loc9)
%define Loc10 local_var(x86frame64bitLocals.Loc10)
%define Loc11 local_var(x86frame64bitLocals.Loc11)
section .text
    extern free
    extern calcSubWithoutFree
    extern calcSumWithoutFree
    extern freeBignum
    global getCarry
    global getResult
    global arrangeCarry
    global addingTwoArrays
    global subTwoArrays
    global compare
    global recCalcMult
	global recCalcDiv
    
getCarry:
    push rbp ; backup RBP
    mov rbp, rsp
    mov rax,0 ; setting return value to 0 as default
    cmp A0, 999999999 ; comparing the argument given to 999999999
    jle .done
    mov rax,A0; moving argument to rax as dividend
    cqo         ;converting
    mov rcx, 1000000000; moving the divisor to rcx
    DIV rcx;dividing rax with 1000000000, and setting quotient rax
    .done:
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret             ;returning the result in rax
getResult:
    push rbp ; backup RBP
    mov rbp, rsp
    mov rax, rdi       ; moving carry to rbp-8 as local variable- long j = carry
    imul rax,rax, 1000000000 ; multiplying carry by 1000000000-> j=j*1000000000
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret             ;returning the result in rax

arrangeCarry:
    push rbp ; backup RBP
    mov rbp, rsp
    mov r8, A0        ; bucking up the address of the number given for future use
    mov rbx,[A0]        ; saving number value in A0
    mov r9, rbx        ;in r9
    mov A0, [A0]        ; setting parameter for getCarry function
    call getCarry       ; calling getCarry
    mov r10,rax         ;saving the carry in Loc2 for future use
    cmp rax, 0          ; checking if rax (result from getCarry) is 0
    jle .done           ; if rax is 0 we can return 0
    mov A0, rax         ; saving rax (carry) as parameter for getResult
    call getResult      ; now the result of the carry should be in rax
    mov rdx, r9       ; moving argument value to rdx
    sub rdx,rax        ; subtracting the number from the result of the carry
    mov rbx, r8       ; moving the adrress from r8 to rbx in order to store the new value in it
    mov [rbx], rdx    ; saving the new value in the original adress
    mov rax,qword r10        ; returning the carry back to rax in order to return it
    .done:
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret
addingTwoArrays:
    push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 72             ; giving rsp the space it needs for the parameters
    mov qword [rbp-16], A0  ; rbp-16 = pointer to bigger array
    mov qword [rbp-24], A1  ; rbp-24 = pointer to smaller array
    mov qword [rbp-32], A2  ; rbp-32 = length of bigger array
    mov qword [rbp-40], A3  ; rbp-40 = length of smaller array
    mov qword [rbp-48], A4  ; rbp-48 = pointer to result array
    mov qword [rbp-56], 0   ; rbp-56 = 0 (current carry)
    mov qword [rbp-64], 0          ; passing 0 to rbp-64 as counter for the memory
    mov rbx, qword 0
    .firstLoop:
        mov rdi,qword [rbp-48]   ; moving pointer to result array to rdi
        mov rsi,qword [rbp-16]   ; moving pointer to bigger array to rsi
        mov rdx,qword [rbp-24]   ; moving pointer to smaller array to rdx
        mov rax,qword 0         ; zeroing rax for summing
        add rax,qword [rbp-56]   ; rax will hold the summing, at first we add the carry to it
        add rax, [rdx+rbx*8]; adding smaller[i] to rax
        add rax, [rsi+rbx*8]; adding bigger[i] to rax
        mov [rdi+rbx*8 +8], rax  ; moving the sum value to result[i+1]
        lea A0, [rdi+rbx*8+8]      ; giving rdi the pointer to result[i+1]
        call arrangeCarry   ; calling arrangeCarry
        mov [rbp-56], rax       ; passing the returned value of arrangeCarry to Loc5
        inc qword [rbp-64]             ; incrementing rbx (the counter)
        mov rax, qword [rbp-40]     ; moving rax the lngeth of smaller array
        mov rbx, qword [rbp-64]     ; moving to rbx the current loop count
        sub rax,rbx             ; subtracting length of the loop from current count
        cmp rax,0               ; comparing rax to see if loop should end
        ja .firstLoop      ; commiting the firstloop
    mov rax, qword [rbp-32]     ; moving rax the lngeth of bigger array
    mov rbx, qword [rbp-40]     ; moving to rbx the current loop count (smaller array length)
    sub rax,rbx             ; subtracting length of the loop from current count
    cmp rax,0               ; comparing rax to see if loop should end
    je .done                ; if sizes of big and small arrays equal- finish
    .secondLoop:
        mov rdi, [rbp-48]   ; moving pointer to result array to rdi
        mov rsi, [rbp-16]   ; moving pointer to bigger array to rsi
        mov rax,qword 0         ; zeroing rax for summing
        add rax, [rbp-56]   ; rax will hold the summing, at first we add the carry
        add rax, [rsi+rbx*8]; adding bigger[i] to rax
        mov [rdi+rbx*8+8], rax   ; moving the sum value to result[i+1]
        lea A0, [rdi+rbx*8+8]      ; giving rdi the pointer to result[i+1]
        call arrangeCarry   ; calling arrangeCarry
        mov [rbp-56], rax       ; passing the returned value of arrangeCarry to Loc5
        inc qword [rbp-40]             ; incrementing the counter
        mov rax, qword [rbp-32]     ; moving rax the lngeth of bigger array
        mov rbx, qword [rbp-40]     ; moving to rbx the current loop count
        sub rax,rbx             ; subtracting length of the loop from current count
        cmp rax,0               ; comparing rax to see if loop should end
        ja .secondLoop      ; commiting the secondloop
    mov rbx, qword [rbp-48]     ; moving pointer to result to rbx
    mov rcx, qword [rbp-32]     ; moving length of bigger array to rcx
    mov rax,qword [rbx+rcx*8]    ; moving result[max] to rax
    add rax, [rbp-56]           ; adding carry to rax
    mov [rbx+rcx*8], rax    ; result[max]=result[max]+carry
    .done:
        mov rsp, rbp ; move RBP to RSP
        pop rbp ; restore old RBP
        ret
subTwoArrays:
    push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 72             ; giving rsp the space it needs for the parameters
    mov qword [rbp-16], A0  ; rbp-16 = pointer to bigger array
    mov qword [rbp-24], A1  ; rbp-24 = pointer to smaller array
    mov qword [rbp-32], A2  ; rbp-32 = length of bigger array
    mov qword [rbp-40], A3  ; rbp-40 = length of smaller array
    mov qword [rbp-48], A4  ; rbp-48 = pointer to result array
    mov qword [rbp-56], 0   ; rbp-56 = 0 (current borrow)
    mov qword [rbp-64], 0   ; rbp-64 = 0 (long i = 0->for the loop)
    mov rcx, qword [rbp-32] ; setting rcx with the value of the counter
    .loop:
        mov rdi, qword [rbp-64] ; copying current i
        mov rax, qword [rbp-16] ; copying pointer to bigger array
        mov rax, qword [rax+rdi*8]  ; copying value of bigger[i]
        mov rbx, qword [rbp-24] ; copying pointer to smaller array
        mov rbx, qword [rbx+rdi*8]  ;copying value of smaller[i]
        sub rax, rbx    ; subtracting the firsts cells in both arrys to get the first borrow
        mov [rbp-56], rax       ; borrow = bigger[i]-smaller[i]
        mov rdx, qword [rbp-64]  ; saving i in rdx
        mov rsi, qword [rbp-40]   ; saving length of smaller array
        sub rsi,rdx               ; rsi=rsi-rdx=min-i
        cmp rsi,0              ; if(i<min)
        jg .continue           ; dont change nothing
        mov rax, qword [rbp-16] ; copying pointer to bigger array
        mov rax, qword [rax+rdi*8]  ; copying value of bigger[i]
        mov qword [rbp-56], rax       ; borrow = bigger[i]
    .continue:
        cmp rax,0                 ; if borrow<0
        JGE .finishRound         ; it means that there is no borrow
        nop
        mov rbx, qword [rbp-16]       ; copying pointer to bigger array
        lea rsi,[rbx+rdi*8+8]           ;bigger[i+1]--
        mov rdx, [rsi]
        dec rdx                       
        mov [rsi], rdx
        add rax, qword 1000000000         ; borrow+=1000000000
        mov qword [rbp-56], rax     ; copying current borrow in the stack
    .finishRound:
        mov rbx,qword [rbp-48]           ; copying result array pointer
        lea rbx, [rbx+rdi*8+8]      ; copying address of result[i+1]
        mov rax, qword [rbp-56]     ; saving borrow in rax
        mov [rbx], rax              ; result[i+1]=borrow
        inc rdi                     ; i++
        mov [rbp-64], rdi
        loop .loop
    .finalAssignment:
        mov rdi, qword [rbp-32]         ; moving bigger.length
        mov rsi, qword [rbp-40]         ; moving smaller.length
        sub rdi, rsi
        cmp rdi, 0                      ; if(min!=max)
        je .done
        nop
        mov rbx,qword [rbp-16]           ; rbx = &bigger
        mov rax,qword [rbp-32]           ; rax = bigger.length
        lea rcx, [rbx +rax*8-8]     ; rcx = &(bigger+max-1)
        mov rdi,qword [rcx]              ; rdi=bigger[max-1]
        mov rbx, qword [rbp-48]             ; rbx=&result
        lea rcx, [rbx+rax*8]                ; rcx = result[max]
        mov rcx, rdi                    ; result[max]=bigger[max-1]
    .done:
        mov rsp, rbp ; move RBP to RSP
        pop rbp ; restore old RBP
        ret
compare:
    push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 32             ; giving rsp the space it needs for the parameters
    mov qword [rbp-16], A0  ; rbp-16 = pointer to number1
    mov qword [rbp-24], A1  ; rbp-24 = pointer to number2
    mov rax,qword [A1+16]        ; rax = number2->numberOfDigits
    mov rdx, qword [A0+16]      ; rdx = number1->numberOfDigits
    cmp rdx,rax           ; if (number1->numberOfDigits>number2->numberOfDigits)
    jg .retTrue                ; return 1
    jl .retFalse               ; return -1
    mov rcx, rax                ; moving number of digits to rcx as counter
    mov rdi,0                  ; giving rdi the inc size for the loop
    .loop:
        mov rax, qword [rbp-16] ; rax = &number1
        mov rax, qword [rax+24] ; rax = number->digits
        mov rax, qword [rax +rdi*8] ; rax = number1->digits[i]
        mov rdx, qword [rbp-24] ; rdx = &number2
        mov rdx, qword [rdx+24] ; rdx = number2->digits
        mov rdx, qword [rdx+rdi*8]  ; rdx = number2->digit[i]
        cmp rdx,rax           ; if (number1->digits[i]>number2->digit[i])
        jg .retTrue                ; return 1
        jl .retFalse               ; return -1
        loop .loop
    mov rax,0
    jmp .return
    .retTrue:
        mov rax, 1      ; return 1
        jmp .return
    .retFalse:
        mov rax, -1     ; return -1
    .return:
        mov rsp, rbp ; move RBP to RSP
        pop rbp ; restore old RBP
        ret
       
recCalcMult:
    push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 64             ; giving rsp the space for the parameters
    mov qword [rbp-0x28], A0  ; rbp-40 = ** multiplier
    mov qword [rbp-0x30], A1  ; rbp-48 = *multiplied
    mov qword [rbp-0x38], A2 ; rbp-56 = *factor A2-rdx
    mov qword [rbp-0x40], A3 ; rbp-64 = **result 
    ;;;;;;;;End OF PARAMETERS
    mov rax, qword [rbp-0x28] ; move to rax **multipier
    mov rax, [rax] ; move to rax *multiplier
    mov rdx, qword [rbp-0x38]; A2 <- *factor 
    mov rsi, rdx ; *factor
    mov rdi,rax ; *multiplier 
    call compare ; compare factorA1 and multiplierA0
    test eax, eax ;;;;
    jl .return    
    ;;;;;;;if condition (multipier>=factor)
    ;mov rdx,[rbp-0x38]; A2 <-*factor
    ;mov rax, [rbp-0x38] ; rax <-*factor
    mov rsi, qword [rbp-0x38] ; A1 <-factor
    mov rdi, qword [rbp-0x38]; A0 <-factor
    call calcSumWithoutFree ; with A0, A1
    ;
    mov qword [rbp-0x18], rax ; rax <-*newfactor   
    ;;;;;;;;;;;;;;;;
   ; mov rdx,qword [rbp-0x30]; A2 <-*multiplied
    ;mov rax, qword [rbp-0x30] ; rax <-*multiplied
    mov rsi, qword [rbp-0x30] ; A1 <-multiplied
    mov rdi, qword [rbp-0x30]; A0 <-multiplied
    call calcSumWithoutFree ; with A0, A1
    ;
    mov qword [rbp-0x10], rax ; rbp-16 = newResult<-rax= SumOf2Multiplied  
    ;;;;;;;;;;;;;;
    mov rcx,qword [rbp-0x40] ; **result
    mov rdx,qword [rbp-0x18] ; *newFactor
    mov rsi,qword [rbp-0x10] ; *new multiplied
    mov rax,qword [rbp-0x28] ; **multiplier
    mov  rdi, rax ; A0<-double pointer to multiplier
    call recCalcMult ; to the next recursive call   
    ;;;;;;;;;;;;;;;
    mov rax, qword [rbp-0x18] ; newFactor
    mov rdi, rax ;  
    call freeBignum ;free newFactor
    mov rax, qword [rbp-0x10] ; newResult (twiceMultiplied)
    mov rdi, rax
    call freeBignum ; free newResult  
    ;;;;;;;;;;;;
    mov rax, qword [rbp-0x28] ;  rax<- ** multiplier 
    mov rax, qword[rax] ; rax <- *multiplier
    mov rdx, qword[rbp-0x38] ; A2(rdx) <- *factor
    mov rsi, rdx ; A1(rsi)<- A1<-*factor
    mov rdi, rax ; A0<- *multipier
    call compare ; compare factorA1 and multiplierA0
    test eax, eax
    jl .return    
    ;;;;;;;;;;;;;if condition (multipier>=factor)
		mov rax, qword[rbp-0x28] ; rax<-**multiplier
		mov rdx, qword[rax] ; A2=rdx
		mov rax, qword[rbp-0x38] ; rax <- *factor
		mov rsi, rdx; multipier 
		mov rdi, rax ; factor as prameter
		call calcSubWithoutFree ; sub multipier-factor
		mov qword[rbp-0x08], rax  ; save sub to rbp-8
		;;;;;rest of multiplier
		mov rax, qword[rbp-0x40] ; rax<-**result
		mov rdx, qword[rax] ; A2 <-*result
		mov rax, qword[rbp-0x30] ; rax<-multiplied
		mov rsi, rdx ; A1<-multiplied
		mov rdi, rax ; A0<-*result
		call calcSumWithoutFree 
		; sum (multiplied, result)
		mov qword[rbp-0x10], rax ; save sum to rbp-16    
		;;;;;;;;;;;;;;the new result    ;free prev result and multiplier
		mov rax, qword[rbp-0x40] ; rax<-**result
		mov rax, qword[rax] ; rax<-*result
		mov rdi, rax ; ready to be deleted
		call freeBignum    ;;;;;;;;;;;;;;;
		mov rax, qword[rbp-0x28] ; rax<-**multiplier
		mov rax, qword[rax] ; rax<-*multipier
		mov rdi, rax ; ready to be deleted
		call freeBignum     
		;;;;;;;;;;;    ; save the new parameters so that in returning to the previous recursive call they will be updated
		mov rax, qword [rbp-0x40] ; **result
		mov rdx, qword [rbp-0x10] ; the new *result
		mov qword[rax], rdx   
		;;;;;;;;;;
		mov rax, qword [rbp-0x28] ; **result
		mov rdx, qword [rbp-8] ; the new result
		mov [rax], rdx ; the new *multiplier   
		;;;;;;;;;;
		jmp .return
        .return:
            nop
            mov rsp, rbp ; move RBP to RSP
            pop rbp ; restore old RBP
            ret

recCalcDiv:
 push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 64             ; giving rsp the space for the parameters
    mov qword [rbp-0x28], A0  ; rbp-40 = ** diviser
    mov qword [rbp-0x30], A1  ; rbp-48 = *divisor
    mov qword [rbp-0x38], A2 ; rbp-56 = *factor A2-rdx
    mov qword [rbp-0x40], A3 ; rbp-64 = **result     ;;;;;;;;End OF PARAMETERS
    mov rax, qword [rbp-0x28] ; move to rax **diviser
    mov rax, [rax] ; move to rax *diviser
    mov rdx, qword [rbp-0x30]; A2 <- *divisor 
    mov rsi, rdx ; *divisor
    mov rdi,rax ; *diviser 
    call compare ; compare diviser and divisor
    test eax, eax 
    jl .return        ;;;;;;;if condition (diviser>=divisor)
    ;mov rdx,[rbp-0x38]; A2 <-*factor
    ;mov rax, [rbp-0x38] ; rax <-*factor
    mov rsi, qword [rbp-0x38] ; A1 <-factor
    mov rdi, qword [rbp-0x38]; A0 <-factor
    call calcSumWithoutFree ; with A0, A1    ;
    mov qword [rbp-0x18], rax ; rax <-*newfactor       ;;;;;;;;;;;;;;;;
    ;mov rdx,qword [rbp-0x30]; A2 <-*divisor
    ;mov rax, qword [rbp-0x30] ; rax <-*divisor
    mov rsi, qword [rbp-0x30] ; A1 <-divisor
    mov rdi, qword [rbp-0x30]; A0 <-divisor
    call calcSumWithoutFree ; with A0, A1    ;
    mov qword [rbp-0x10], rax ; rbp-16 = newResult<-rax= SumOf2Multiplied      ;;;;;;;;;;;;;;
    mov rcx,qword [rbp-0x40] ; **result
    mov rdx,qword [rbp-0x18] ; *newFactor
    mov rsi,qword [rbp-0x10] ; *new multiplied
    mov rax,qword [rbp-0x28] ; **multiplier
    mov  rdi, rax ; A0<-double pointer to multiplier
    call recCalcDiv ; to the next recursive call       ;;;;;;;;;;;;;;;
    mov rax, qword [rbp-0x18] ; newFactor
    mov rdi, rax ;  
    call freeBignum ;free newFactor
    mov rax, qword [rbp-0x10] ; newResult (twiceMultiplied)
    mov rdi, rax
    call freeBignum ; free newResult      ;;;;;;;;;;;;
    mov rax, qword [rbp-0x28] ; move to rax **diviser
    mov rax, [rax] ; move to rax *diviser
    mov rdx, qword [rbp-0x30]; A2 <- *divisor 
    mov rsi, rdx ; *divisor
    mov rdi,rax ; *diviser 
    call compare ; compare diviser and divisor
    test eax, eax 
    jl .return          ;;;;;;;if condition (diviser>=divisor)
		mov rax, qword[rbp-0x28] ; rax<-**diviser
		mov rdx, qword[rax] ; A2=rdx
		mov rax, qword[rbp-0x30] ; rax <- *divisor
		mov rsi, rdx; multipier 
		mov rdi, rax ; divisor as prameter
		call calcSubWithoutFree ; sub diviser-divisor
		mov qword[rbp-0x08], rax  ; save sub to rbp-8		;;;;;rest to divide
		mov rax, qword[rbp-0x40] ; rax<-**result
		mov rdx, qword[rax] ; A2 <-*result
		mov rax, qword[rbp-0x38] ; rax<-*factor
		mov rsi, rdx ; A1<-*factor
		mov rdi, rax ; A0<-*result
		call calcSumWithoutFree 		; sum (factor, result)
		mov qword[rbp-0x10], rax ; save sum to rbp-16    		;;;;;;;;;;;;;;the new result    ;free prev result and multiplier
		mov rax, qword[rbp-0x40] ; rax<-**result
		mov rax, qword[rax] ; rax<-*result
		mov rdi, rax ; ready to be deleted
		call freeBignum    ;;;;;;;;;;;;;;;
		mov rax, qword[rbp-0x28] ; rax<-**toDivide
		mov rax, qword[rax] ; rax<-*toDivide
		mov rdi, rax ; ready to be deleted
		call freeBignum     ;;;;;;;;;;;    ; save the new parameters so that in returning to the previous recursive call they will be updated
		mov rax, qword [rbp-0x40] ; **result
		mov rdx, qword [rbp-0x10] ; the new *result
		mov qword[rax], rdx 		;;;;;;;;;;
		mov rax, qword [rbp-0x28] ; **todivide
		mov rdx, qword [rbp-8] ; the rest to divide
		mov [rax], rdx ; the new *diviser   		;;;;;;;;;;
		jmp .return
        .return:
            nop
            mov rsp, rbp ; move RBP to RSP
            pop rbp ; restore old RBP
            ret
