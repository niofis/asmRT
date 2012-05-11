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

Initialize:
    ;match =exe,TARGET {
    invoke HeapCreate,0,0,0
    mov [proc_heap],eax
    lea eax,[HeapInfo]
    invoke HeapSetInformation,[proc_heap],0,eax,4
    ;}
    match =0,TARGET {
    invoke GetProcessHeap
    mov [proc_heap],eax
    }

    ;Inicializar freeimage
    push 0
    call [FreeImage_Initialise]

    ;MASK all exceptions
    LDMXCSR [exceptions_mask]
    ;invoke CreateMutex,0,0,0
    ;mov [mutex],eax
    xor eax,eax
ret

Deinitialize:
    ;Deinicializar freeimage
    pushad
    call LimpiarHeap
    call [FreeImage_DeInitialise]
    invoke HeapDestroy,[proc_heap]
    ;invoke CloseHandle,[mutex]
    popad
ret


EstableceResolucion:
    push ebp
    mov ebp,esp
    pushad
    label .width  dword at ebp+8
    label .height dword at ebp +12
    xor eax,eax
    mov edx,[.width]
    mov ecx,[.height]
    cmp edx,eax
    jle .salir
    cmp ecx,eax
    jle .salir

    mov [sector_y],0
    mov [sector_height],ecx
    mov [_width],edx
    mov [_height],ecx
    .salir:
    popad
    mov esp,ebp
    pop ebp
retn 8

EstableceSector:
    push ebp
    mov ebp,esp
    label .y  dword at ebp+8
    label .height dword at ebp +12
    pushad
    mov ecx,[_height]
    mov edi,[.y]
    cmp edi,ecx
    jae .salir
       
    add edi,[.height]
    
    mov ecx,[_height]

    cmp edi,ecx
    ja .salir
    
    mov edi,[.y]
    mov ecx,[.height]
    mov [sector_y],edi    
    mov [sector_height],ecx


    .salir:
    popad
    mov esp,ebp
    pop ebp
retn 8

CreaMateriales: ;esp+4 ->num_materiales
    mov eax,[esp+4]
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
retn 4

CreaGrupos: ;esp+4 ->num_grupos
    mov eax,[esp+4]
    push ebx
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    pop ebx
retn 4

CreaObjetos3D: ;esp+4 ->num_objetos
    mov eax,[esp+4]
    push ebx
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx
    pop ebx
retn 4

CreaLuces: ;esp+4 ->num_luces
    mov eax,[esp+4]
    push ebx
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx
    pop ebx
retn 4

TraeMaterial: ;esp+4-> id
    mov eax,[esp+4]
    imul eax,MaterialSize
    add eax,[p_materiales]
retn 4

TraeGrupo: ;esp+4 -> id
    mov eax,[esp+4]
    imul eax,GrupoSize
    add eax,[p_grupos]
retn 4

TraeObjeto3D: ;esp+4 -> id
    mov eax,[esp+4]
    imul eax,Objeto3DSize
    add eax,[p_objetos]
retn 4

TraeLuz: ;esp+4 -> id
    mov edx,[esp+4]
    imul edx,LuzSize
    add edx,[p_luces]
    ;lea eax,[luz]
retn 4

TraeCamara: ;esp+4 -> id
    ;mov edx,[ebp+8]
    ;imul edx,CamaraSize
    ;add edx,[p_camaras
    lea eax,[camara]
retn 4

EstableceNumThreads: ;esp+4 -> id
    mov edx,[esp+4]
    xor eax,eax
    cmp edx,eax
    jle .salir
    mov [num_threads],edx
    .salir:
retn 4