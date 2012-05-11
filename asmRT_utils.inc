;Copyright (c) 2012, Enrique CR
;All rights reserved.
;
;Redistribution and use in source and binary forms, with or without modification, are permitted
;provided that the following conditions are met:
;
;- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
;- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer 
;   in the documentation and/or other materials provided with the distribution.
;
;THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED 
;WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
;A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE 
;FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
;LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
;INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
;TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
;ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

;section '.text' code readable executable
;MemAlloc	PROTO :DWORD
;MemFree		PROTO :DWORD

MemAlloc: ;num_bytes->edi
    match PLATFORM,Win32{
    ;invoke GetProcessHeap
    ;mov [proc_heap],eax
    invoke HeapAlloc,[proc_heap],HEAP_ZERO_MEMORY or HEAP_GENERATE_EXCEPTIONS,eax
    ;invoke malloc,[num_bytes]
    }
    match PLATFORM,Linux{
    ;invoke malloc,[num_bytes]
    }
ret

;regresa en eax, el alineado, y en ebx el no alineado
MemAllocAlign: ;num_bytes->eax

    add eax,15
    call MemAlloc
    mov edx,eax

    push edx
    ;alineacion de la memoria a 16 bytes
    mov ecx,eax
    and ecx,0Fh ;obtengo el residuo de ecx/16
    mov edx,10h
    sub edx,ecx
    and edx,0Fh
    add eax,edx ;Alineación =)
    pop edx
ret


MemAllocAlignBlocks: ;num_blocks-> eax,block_size->eax
    ;mov eax,edi
    ;mov ebx,esi
    ;mul ebx
    
    ;mov edi,eax
    imul eax,edx
    call MemAllocAlign

ret

MemFree: ;puntero->eax
    invoke HeapFree,[proc_heap],0,eax
    ;push edi
    ;call [_aligned_free]
ret



;proc SPrintReal sas,val
;    locals
;    frmt  db "%f ",0
;    endl

;    FLD [val]
;    SUB ESP,8
;    FSTP qword[esp]
;    lea ebx,[frmt]
    
;    push ebx    
;    mov eax,[sas]
;    push eax

;    call [sprintf]
;    add esp,10h
;    ret
;endp

SPrintReal.frmt  db "%f %f ",0
SPrintReal: ;ptr_string -> edi,valor->esi
    push ebp
    mov ebp,esp
    sub esp,16
    
    label .val             qword at esp
    label .val2             qword at esp+8
    
    mov dword[.val],esi
    movss xmm0,dword[.val]
    cvtss2sd xmm0,xmm0
    movsd [.val],xmm0
    
    movss xmm0,[uno]
    cvtss2sd xmm0,xmm0
    movsd [.val2],xmm0
    
    lea ebx,[SPrintReal.frmt]
    push ebx
    push edi
    call [sprintf]
    ;add esp,10h
    add esp,8
    leave
ret

GetTime:
    call [clock]
ret

CLOCKS_PER_SEC = 1000

