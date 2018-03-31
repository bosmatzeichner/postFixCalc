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
    global getCarry
    global getResult
    global arrangeCarry
getCarry:
    push rbp ; backup RBP
    mov rbp, rsp
    mov rax,0 ; setting return value to 0 as default
    cmp A0, 999999999 ; comparing the argument given to 999999999
    jle .done
    mov rax,A0; moving argument to rax as dividend
    mov rcx, 1000000000; moving the divisor to rcx
    DIV rcx;dividing rax with 1000000000, and setting quotient rax
    .done:
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret             ;returning the result in rax
getResult:
    push rbp ; backup RBP
    mov rbp, rsp
    mov rax,1000000000  ;setting rax with 1000000000 for mul
    mov rbx, A0         ;setting rxb with parameter
    mul rbx             ;multing rbx and rax and saving result to rax
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret
arrangeCarry:
    push rbp ; backup RBP
    mov rbp, rsp
    mov Loc0, A0        ; bucking up the address of the number given for future use
    mov rbx,[A0]        ; saving number value in A0
    mov Loc1, rbx        ;in Loc1
    mov A0, [A0]        ; setting parameter for getCarry function
    call getCarry       ; calling getCarry
    mov rbx,rax         ;saving the carry in rbx for future use
    cmp rax, 0          ; checking if rax (result from getCarry) is 0
    jle .done           ; if rax is 0 we can return 0
    ;mov Loc1, [Loc0]    ; saving number value in loc1
    
    mov A0, rax         ; saving rax (carry) as parameter for getResult
    call getResult      ; now the result of the carry should be in rax
    sub Loc1,rax        ; subtracting the number from the result of the carry
    mov rax, Loc1
    mov [A0], rax    ; saving the new value in the original adress
    mov rax, rbx        ; returning the carry back to rax in order to return it
    .done:
    mov rsp, rbp ; move RBP to RSP
    pop rbp ; restore old RBP
    ret
