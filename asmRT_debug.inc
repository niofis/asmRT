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

debug_cnt dd 0

macro PrintReals [val]
{
    forward stdcall DumpReal,val
    common stdcall EndLine
}



proc DumpReal val
    locals
    frmt  db "%f ",0
    endl
    push eax
    push ebx
    FINIT
    FLD [val]
    SUB ESP,8
    FSTP qword[esp]
    lea eax,[frmt]
    push eax
    call [printf]
    add esp,12
    pop ebx
    pop eax
    ret
endp

proc EndLine
    locals
    end_line db 10,13,0
    endl
    push eax
    lea eax,[end_line]
    push eax
    call [printf]
    pop eax
    ret
endp

proc Pausa
    push eax
    call [getch]
    pop eax
    ret
endp

proc DumpDW val
    locals
    frmt  db "%d ",0
    endl
    push eax
    push [val]
    lea eax,[frmt]
    push eax
    call [printf]
    pop eax
    ret
endp

proc DumpDWH val
    push eax
    locals
    frmt  db "%x ",0
    endl
    push [val]
    lea eax,[frmt]
    push eax
    call [printf]
    pop eax
    ret
endp

macro DumpVectorR vct
{
    push dword[vct + Vector.t]
    call DumpReal
    push dword[vct + Vector.u]
    call DumpReal
    push dword[vct + Vector.v]
    call DumpReal
    push dword[vct + Vector.w]
    call DumpReal
    call EndLine
}

macro DumpVectorH vct
{
    push dword[vct + Vector.t]
    call DumpDWH
    push dword[vct + Vector.u]
    call DumpDWH
    push dword[vct + Vector.v]
    call DumpDWH
    push dword[vct + Vector.w]
    call DumpDWH
    call EndLine
}

proc DumpW val ;:WORD
    PUSHAD 
    xor eax,eax
    mov ax,val
    ;invoke ltoa,eax,ADDR texto
    invoke MessageBox, 0, ADDR texto, 0, MB_OK
    POPAD
    ret
endp

proc ShowMsg  msg
    PUSHAD
    invoke MessageBox,0,msg,0,MB_OK
    POPAD
    ret
endp

proc Aki
    locals
    aki db "Aki",10,13,0
    endl
    push eax
    lea eax,[aki]
    push eax
    call [printf]
    pop eax
    ret
endp

proc True
    locals
    aki db "Si",10,13,0
    endl
    push eax
    lea eax,[aki]
    push eax
    call [printf]
    pop eax
    ret
endp

proc False
    locals
    aki db "No",10,13,0
    endl
    push eax
    lea eax,[aki]
    push eax
    call [printf]
    pop eax
    ret
endp

proc Counter
    inc [debug_cnt]
    push [debug_cnt]
    call DumpDW
    call EndLine
    ret
endp
