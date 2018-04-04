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
    extern compare
    global recCalcMult
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
        
recCalcMult:
    push rbp ; backup RBP
    mov rbp, rsp
    sub rsp, 64             ; giving rsp the space it needs for the parameters
    mov qword [rbp-16], A0  ; rbp-16 = pointer to pointer to multiplier
    mov qword [rbp-24], A1  ; rbp-24 = pointer to multiplied
    mov qword [rbp-32], A2 ; rbp-32 = pointer to factor A2-rdx
    mov qword [rbp-40], A3 ; rbp-40 = pointer to pointer to result End OF PARAMETERS
    mov rax, qword [rbp-16] ; move to rax **multiplier
    mov rax, qword [rax] ; move to rax *multiplier
    mov A2, qword [rbp-32]; A2 <- *factor 
    mov A1, A2 ; *factor
    mov A0,rax ; *multiplier 
    call compare ; compare factorA1 and multiplierA0
    cmp rax,0
    jb .return    ;;;;;;;if condition (multipier<=factor)
    mov A2,[rbp-32]; A2 <-*factor
    mov rax, [rbp-32] ; rax <-*factor
    mov A1, A2 ; A1 <-factor
    mov A0, rax; A0 <-factor
    call calcSumWithoutFree ; with A0, A1
    mov qword [rbp-56], rax ; rax <-*newfactor    ;;;;;;;;;;;;;;;;
    mov A2,qword [rbp-32]; A2 <-*multiplied
    mov rax, qword [rbp-32] ; rax <-*multiplied
    mov A1, A2 ; A1 <-multiplied
    mov A0, rax; A0 <-multiplied
    call calcSumWithoutFree ; with A0, A1
    mov qword [rbp-48], rax ; rbp-48 = newResult<-rax= SumOf2Multiplied     ;;;;;;;;;;;;;;
    mov A3,qword [rbp-40] ; pointer to pointer to result
    mov A2,qword [rbp-56] ; rbp-52 = newFactor
    mov A1,qword [rbp-48]; newResult= new multiplied
    mov rax,qword [rbp-16] ; double pointer to multiplier
    mov  A0, rax ; A0<-double pointer to multiplier
    call recCalcMult ; to the next recursive call    ;;;;;;;;;;;;;;;
    mov rax, qword [rbp-56] ; newFactor
    mov A0, rax ;  
    call freeBignum ;free newFactor
    mov rax, qword [rbp-48] ; newResult (twiceMultiplied)
    mov A0, rax
    call freeBignum ; free newResult    ;;;;;;;;;;;;
    mov rax, qword [rbp-16] ;  rax<- ** multiplier 
    mov rax, qword[rax] ; rax <- *multiplier
    mov A2, qword[rbp-32] ; A2(rdx) <- *factor
    mov A1, A2 ; A1(rsi)<- A1<-*factor
    mov A0, rax ; A0<- *multipier
    call compare ; compare factorA1 and multiplierA0
    cmp rax,0
    jb .return    ;;;;;;;;;;;;;if condition (multipier<=factor)
    mov rax, qword[rbp-16] ; rax<-**multiplier
    mov A2, qword[rax] ; A2=rdx
    mov rax, qword[rbp-32] ; rax <- *factor
    mov A1, A2; multipier 
    mov A0, rax ; factor as prameter
    call calcSubWithoutFree ; sub A1-A0
    mov qword[rbp-64], rax  ; save sub to rbp-64
    ;;;;;rest of multiplier
    mov rax, qword[rbp-40] ; rax<-**result
    mov A2, qword[rax] ; A2 <-*result
    mov rax, qword[rbp-24] ; rax<-multiplied
    mov A1, A2 ; A1<-multiplied
    mov A0, rax ; A0<-*result
    call calcSumWithoutFree ; sum (multiplied, result)
    mov qword[rbp-8], rax ; save sum to rbp-8    ;;;;;;;;;;;;;;the new result    ;free prev result and multiplier
    mov rax, qword[rbp-40] ; rax<-**result
    mov rax, qword[rax] ; rax<-*result
    mov A0, rax ; ready to be deleted
    call freeBignum    ;;;;;;;;;;;;;;;
    mov rax, qword[rbp-16] ; rax<-**multiplier
    mov rax, qword[rax] ; rax<-*multipier
    mov A0, rax ; ready to be deleted
    call freeBignum     ;;;;;;;;;;;    ; save the new parameters so that in returning to the previous recursive call they will be updated
    mov rax, qword [rbp-40] ; **result
    mov A2, qword [rbp-8] ; the new *result
    mov qword[rax], A2    ;;;;;;;;;;
    mov rax, qword [rbp-16] ; **result
    mov A2, qword [rbp-64] ; the new result
    mov qword[rax], A2 ; the new *multiplier    ;;;;;;;;;;
        .return: 
            mov rsp, rbp ; move RBP to RSP
            pop rbp ; restore old RBP
            ret