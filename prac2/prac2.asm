section .data
    w:      DW 0b0000111100001110
    res:    DB 0
    
section .text
global main
main:
    mov     ebp, esp    ; for correct debugging
    mov     ax, [w]     ; move word
    xor     ecx, ecx    ; clear counter
    call    cycle
    xor     al, al      ; clear res
    cmp     cx, 8      ; compare if counter = 8 (8 ones and 8 zeros)
    je      equal

save:
    mov     [res], al
    ret    
    
equal:
    mov     al, 1      ; set result to 1 if equal
    jmp     save
    
cycle:
    mov     dx, ax      ; copy word to dx
    and     dx, 1      ; apply mask to get last bit
    add     cx, dx      ; add result to counter
    shr     ax, 1      ; ax = ax >> 1
    cmp     ax, 0      ; if ax != 0:
    jne     cycle       ;   continue
    ret
