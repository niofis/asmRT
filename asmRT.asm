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

;Optimizaciones
;Evitar: shuffle, pack, unpack

TARGET equ exe ;dll ;
match =exe,TARGET {
format PE gui  4.0
entry start
}

match =dll,TARGET {
format PE GUI 4.0 DLL
entry dll_start
}


include 'c:\fasm\include\win32a.inc'

THREADING equ Multi ;Single ;

PLATFORM equ Win32

TIMERS equ Windows ;SDL ;

HUE equ no ;yes ;


include 'c:\fasm\INCLUDE\MACRO\if.inc'
include 'c:\fasm\include\macro\struct.inc'
include 'sdld.inc'
include 'freeimaged.inc'

section '.data' data readable writeable

include 'asmRT.inc'



sdlevnt SDL_Event 0


section '.text' code readable executable
include 'asmRT_funciones.inc'
include 'asmRT_utils.inc'
include 'asmRT_debug.inc'
include 'asmRT_macros.inc'
include 'asmRT_camara.inc'
include 'asmRT_demo.inc'
include 'asmRT_parts.inc'
include 'asmRT_raytrace_parts.inc'
include 'asmRT_raytrace.inc'
include 'asmRT_raytrace_s.inc'
match =dll,TARGET {
    include 'asmRT_dll.inc'
}

align 16

dll_start:
    mov eax,1
ret

start:
    call Initialize
    call MainInit
    call LimpiarHeap
    call Deinitialize
xor eax,eax
ret

MainInit:
    push ebp
    mov ebp,esp
    sub esp,4*4
    
    dwordsize = 4
    label .fps             dword at esp
    label .keys            dword at esp + dwordsize
    label .start		   dword at esp + dwordsize *2
    label .stop			   dword at esp + dwordsize *3

    match =Windows,TIMERS {
    lea eax,[clock_freq]
    push eax
    call [QueryPerformanceFrequency]
    }

    push SDL_INIT_VIDEO
    call [SDL_Init]
    add esp,4
    .if (eax & 80000000h)
        jmp exit
    .endif
 
    push 1
    call [SDL_ShowCursor]
    add esp,4

    ;mov video_options,SDL_HWSURFACE or SDL_ASYNCBLIT or SDL_HWACCEL
    
    ;push [video_options]
    ;push _bpp
    ;push [_height]
    ;push [_width]
    ;call [SDL_SetVideoMode]
    ;add esp,16
    
    ;push 720
    ;push 1280
    ;call EstableceResolucion
    
    ;push 100
    ;push 200
    ;call EstableceSector
    
    call SetVideo
    .if (eax=0)
        jmp exit
    .endif

    mov [screen], eax

    call CreaDemo1


    
    .while (1)
        lea eax,[sdlevnt]
        push eax
        call [SDL_PollEvent]
        add esp,4
        .if (eax<>0)
            movzx eax, [sdlevnt.type]
            .if (eax = SDL_KEYDOWN)
                .if ([sdlevnt.key.keysym.sym] = SDLK_ESCAPE)
                    jmp exit2
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_1)
                    call LimpiarHeap
                    call CreaDemo1
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_2)
                    call LimpiarHeap
                    call CreaDemo2
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_3)
                    call LimpiarHeap
                    call CreaDemo3
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_4)
                    call LimpiarHeap
                    call CreaDemo4
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_5)
                    call LimpiarHeap
                    call CreaDemo5
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_6)
                    call LimpiarHeap
                    call CreaDemo6
				.elseif ([sdlevnt.key.keysym.sym] = SDLK_7)
                    call LimpiarHeap
                    call CreaDemo7
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_RETURN)
                    mov eax,[video_options]
                    xor eax,SDL_FULLSCREEN
                    mov [video_options],eax
                    call SetVideo
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_PAGEUP)
                    mov edi,1
                    call CambiaResolucion
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_PAGEDOWN)
                    mov edi,-1
                    call CambiaResolucion
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_HOME)
                    mov edi,1
                    call CambiaNumThreads
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_END)
                    mov edi,-1
                    call CambiaNumThreads
                .elseif ([sdlevnt.key.keysym.sym] = SDLK_F1)
                    xor [info_extendida],1
                .endif
                
            .elseif (eax = SDL_MOUSEBUTTONDOWN)
                .if ([sdlevnt.button.button]=SDL_BUTTON_LEFT)
                    mov [dragging],1
                    mov [grupo_drag],0
                .endif
            .elseif (eax = SDL_MOUSEBUTTONUP)
                .if ([sdlevnt.button.button]=SDL_BUTTON_LEFT)
                    mov [dragging],0
                    mov [grupo_drag],0
                .endif
            .elseif (eax = SDL_QUIT)
                jmp exit2
            .endif
        .endif
            push 0
            call [SDL_GetKeyState]
            add esp,4
            
            mov [.keys],eax
            add eax,SDLK_UP
            mov bl,[eax]
            .if(bl=1)
                mov esi,1
                mov edi,[rotacion_neg]
                call RotarCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_DOWN
            mov bl,[eax]
            .if(bl=1)
                mov esi,1
                mov edi,[rotacion_pos]
                call RotarCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_LEFT
            mov bl,[eax]
            .if(bl=1)
                mov esi,2
                mov edi,[rotacion_neg]
                call RotarCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_RIGHT
            mov bl,[eax]
            .if(bl=1)
                mov esi,2
                mov edi,[rotacion_pos]
                call RotarCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_w
            mov bl,[eax]
            .if(bl=1)
                mov edx,[uno]
                mov esi,[zero]
                mov edi,[zero]
                call MoverCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_s
            mov bl,[eax]
            .if(bl=1)
                mov edx,[muno]
                mov esi,[zero]
                mov edi,[zero]
                call MoverCamara
            .endif	
            mov eax,[.keys]
            add eax,SDLK_a
            mov bl,[eax]
            .if(bl=1)
                mov edx,[zero]
                mov esi,[zero]
                mov edi,[muno]
                call MoverCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_d
            mov bl,[eax]
            .if(bl=1)
                mov edx,[zero]
                mov esi,[zero]
                mov edi,[uno]
                call MoverCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_q
            mov bl,[eax]
            .if(bl=1)
                mov edx,[zero]
                mov esi,[uno]
                mov edi,[zero]
                call MoverCamara
            .endif
            mov eax,[.keys]
            add eax,SDLK_e
            mov bl,[eax]
            .if(bl=1)
                mov edx,[zero]
                mov esi,[muno]
                mov edi,[zero]
                call MoverCamara
            .endif

        push [screen]
        call [SDL_LockSurface]
        add esp,4
        test eax,eax
        jnz continua_while

        match =Windows,TIMERS {
        lea eax,[start_clock]
        push eax
        call [QueryPerformanceCounter]
        }
        match =SDL,TIMERS {
        call [SDL_GetTicks]
        mov [.start],eax
        }
        
        mov eax,[screen]
        mov eax,[eax + SDL_Surface.pixels]
        mov [pixeles],eax
        mov edi,[num_threads]
        call RecorrerPixelesMT

        match =Windows,TIMERS {
        lea eax,[stop_clock]
        push eax
        call [QueryPerformanceCounter]
        }
        match =SDL,TIMERS {
        call [SDL_GetTicks]
        mov [.stop],eax
        }
        push [screen]
        call [SDL_UnlockSurface]
        add esp,4

        

        ;Calculo de FPS, no creo que este bien, pero cuando menos servira como medida para ver el rendimiento
        match =Windows,TIMERS {
        mov eax, dword[stop_clock]
        sub eax, dword[start_clock]
        mov edx, dword[stop_clock+4]
        sbb edx, dword[start_clock+4]  ;resta larga de 64 bits, ¬¬

        ;la resta total esta en edx:eax

        div dword[clock_freq]

        mov [.fps],eax   
        CVTSI2SS xmm0,[.fps] ;enteros

        shr edx,1   ;movido a la derecha en 1 bit, para permitir que se pueda dividir bien por clock_freq
        mov [.fps],edx
        CVTSI2SS xmm1,[.fps] ;residuo

        mov ebx, dword[clock_freq]
        shr ebx,1   ;movido a la derehca en 1 bit, porque se intepreta como si fuese negativo, k no es!
        mov [.fps],ebx
        CVTSI2SS xmm2,[.fps] ;divisor

        divps xmm1,xmm2 ;cacula la parte fraccional
        addps xmm0,xmm1 ;le suma la parte entera
        }
        match =SDL,TIMERS {
        cvtsi2ss xmm1,[.start]
        cvtsi2ss xmm0,[.stop]
        subps xmm0,xmm1
        }
        
        ;en xmm0 esta el tiempo transcurrido en milisegundos
        
        movaps xmm1,xmm0
        match =SDL,TIMERS {
            divss xmm1,[mil]
        }
        movss [.stop],xmm1	;milisegundos segundos que tardo en ejecutarse
        
        rcpps xmm0,xmm0;cacula la inversa, que corresponde a los FPS
        match =SDL,TIMERS {
            mulss xmm0,[mil]
        }
        movss [.fps],xmm0	;cuadros pro segundo
        

        ;lea eax,[texto]
        ;push [.fps]
        ;push eax
        ;lea edi,[texto]
        ;mov esi,[.fps]
        ;call SPrintReal
        
        ;lea eax,[texto]
        ;push 0FFFFFFFFh
        ;push eax
        ;push 0
        ;push 0
        ;push [screen]
        ;call [SDL_WM_SetCaption]
        ;call [stringColor]
        ;add esp,20
        
        mov edi,[info_extendida]
        mov esi,[.fps]
        mov edx,[.stop]
        call MostrarInformacion

        push [screen]
        call [SDL_Flip]
        add esp,4
        ;termina el calculo de FPS
        continua_while:
    .endw
exit2:
        cinvoke SDL_Quit
exit:
;leave
mov esp,ebp
pop ebp
ret


MostrarInformacion: ;edi: 0->minimo, 1->completo  esi->fps   edx->spf
    
    push ebp
    mov ebp,esp
    sub esp,24
    
    label .val             qword at esp
    label .val2             qword at esp+8
    label .val3             qword at esp+16
    
    mov dword[.val],esi
    mov dword[.val2],edx
	mov eax,[num_frames]
	add eax,1
	mov [num_frames],eax
    
    
    ;obtener maximos y minimos
    movss xmm0,dword[.val]
    movss xmm1,[min_fps]
    minss xmm1,xmm0
    movss [min_fps],xmm1
    movss xmm1,[max_fps]
    maxss xmm1,xmm0
    movss [max_fps],xmm1
    
    movss xmm1,[avg_fps]
    addss xmm1,xmm0
    movss [avg_fps],xmm1
    
    movss xmm0,dword[.val2]
    movss xmm1,[min_spf]
    minss xmm1,xmm0
    movss [min_spf],xmm1
    comiss xmm0,xmm1
    jne .continua_max_spf
    mov eax,[num_threads]
    mov [mejor_num_threads],eax
	
    .continua_max_spf:
    movss xmm1,[max_spf]
    maxss xmm1,xmm0
    movss [max_spf],xmm1
    
    movss xmm1,[avg_spf]
    addss xmm1,xmm0
    movss [avg_spf],xmm1
    
    
    ;muestra framerates    
    movss xmm0,dword[.val]
    cvtss2sd xmm0,xmm0
    movsd [.val],xmm0
    movss xmm0,dword[.val2]
    cvtss2sd xmm0,xmm0
    movsd [.val2],xmm0


    lea ebx,[MostrarInformacion.frame_rate]
    push ebx
    lea eax,[texto]
    push eax
    call [sprintf]
    add esp,8h

    lea eax,[texto]
    push 0FFFFFFFFh
    push eax
    push 0
    push 0
    push [screen]
    call [stringColor]
    add esp,20
    
    test edi,edi
    jz  .salir
    
    
    ;muestra max y min de fps
    movss xmm0,[avg_fps]
    cvtsi2ss xmm1,[num_frames]
    divss xmm0,xmm1
    cvtss2sd xmm0,xmm0
    movsd [.val],xmm0
    movss xmm0,[max_fps]
    cvtss2sd xmm0,xmm0
    movsd [.val2],xmm0
    movss xmm0,[min_fps]
    cvtss2sd xmm0,xmm0
    movsd [.val3],xmm0

    lea ebx,[MostrarInformacion.mm_fps]
    push ebx
    lea eax,[texto]
    push eax
    call [sprintf]
    add esp,8h

    lea eax,[texto]
    push 0FFFFFFFFh
    push eax
    push 8
    push 0
    push [screen]
    call [stringColor]
    add esp,20
    
    ;muestra max y min de spf
    movss xmm0,[max_spf]
    cvtss2sd xmm0,xmm0
    movsd [.val],xmm0
    movss xmm0,[min_spf]
    cvtss2sd xmm0,xmm0
    movsd [.val2],xmm0
    movss xmm0,[avg_spf]
    cvtsi2ss xmm1,[num_frames]
    divss xmm0,xmm1
    cvtss2sd xmm0,xmm0
    movsd [.val3],xmm0
    

    lea ebx,[MostrarInformacion.mm_spf]
    push ebx
    lea eax,[texto]
    push eax
    call [sprintf]
    add esp,8h

    lea eax,[texto]
    push 0FFFFFFFFh
    push eax
    push 16
    push 0
    push [screen]
    call [stringColor]
    add esp,20
    
    ;muestra resolucion
    lea ebx,[MostrarInformacion.resolucion]
    push _bpp
    push [_height]
    push [_width]
    push ebx
    lea eax,[texto]
    push eax
    call [sprintf]
    add esp,20

    lea eax,[texto]
    push 0FFFFFFFFh
    push eax
    push 24
    push 0
    push [screen]
    call [stringColor]
    add esp,20
    
    ;muestra threads
    lea ebx,[MostrarInformacion.threads]
    push [mejor_num_threads]
    push [num_threads]
    push ebx
    lea eax,[texto]
    push eax
    call [sprintf]
    add esp,16

    lea eax,[texto]
    push 0FFFFFFFFh
    push eax
    push 32
    push 0
    push [screen]
    call [stringColor]
    add esp,20
    
    .salir:
    ;leave
    mov esp,ebp
    pop ebp
ret

CambiaNumThreads:	;edi->delta -1, +1
	xor eax,eax
    mov [avg_fps],eax
    mov [avg_spf],eax
    mov [num_frames],eax
    
    mov eax,[num_threads]
    add eax,edi
    cmp eax,0
    jg .restar_edi
    mov [num_threads],1
    jmp .salir
    .restar_edi:
    add [num_threads],edi
    .salir:
ret

BorraMarcadores:
	xor eax,eax
    mov [max_fps],eax
    mov [min_fps],1000.0
    mov [max_spf],eax
    mov [min_spf],10000000000000000000000000000.0
    mov [mejor_num_threads],1
    mov [avg_fps],eax
    mov [avg_spf],eax
    mov [num_frames],eax
ret

CambiaResolucion: ;edi->delta -1, +1
    call BorraMarcadores
    mov eax,[resolucion]
    add eax,edi
    mov ebx,eax 		;checar si es menor que cero
    cmp ebx,0
    jg .probar_mayor
    xor eax,eax
    jmp .todo_bien
    .probar_mayor:
    mov ebx,eax			;checar si es mayor que max_resolucion
    cmp ebx,max_resolucion
    jna .todo_bien
    mov eax,max_resolucion
    .todo_bien:
    
    mov [resolucion],eax
    mov ebx,4
    mul ebx
    
    lea ebx,[widths]
    add ebx,eax
    mov ebx,[ebx]
    
    lea ecx,[heights]
    add ecx,eax
    mov ecx,[ecx]
    
    mov [sector_y],0
    mov [sector_height],ecx
    
    mov [_width],ebx
    mov [_height],ecx
    call SetVideo
ret

SetVideo:
    push [video_options]
    push _bpp
    push [_height]
    push [_width]
    call [SDL_SetVideoMode]
    add esp,16
    test eax,eax
    jz .salir
    mov [screen], eax
    .salir:
ret

LimpiarHeap:

	call BorraMarcadores
    
    mov eax,[p_luces_na]
    test eax,eax
    jz cont_objs_LimpiarHeap
    ;invoke HeapFree,proc_heap,0,p_luces
    mov eax,[p_luces_na]
    call MemFree
    ;stdcall MemFree,[p_luces_na]
	xor eax,eax
    mov [p_luces_na],eax
    mov [num_luces],eax
    
    cont_objs_LimpiarHeap:
    mov eax,[p_objetos_na]
    test eax,eax
    jz  cont_mats_LimpiarHeap
    ;invoke HeapFree,proc_heap,0,p_objetos_noal
    mov eax,[p_objetos_na]
    call MemFree
	xor eax,eax
    mov [p_objetos_na],eax
    mov [num_objetos],eax
    
    cont_mats_LimpiarHeap:
    
    ;falta borrar las imagenes cargadas por DevIL
	align 16
    .borrar_texturas:
    mov edx,[num_materiales]
    test edx,edx
    jz .texturas_todas_eliminadas
    dec [num_materiales]
    dec edx
    
    mov eax,MaterialSize
    mul edx
    add eax, [p_materiales]
    mov ecx,[eax + Material.textura]
    test ecx,ecx
    jz .borrar_texturas
    
    mov edx,[eax + Material.textura]
    push edx
    call [FreeImage_Unload]
    jmp .borrar_texturas
    
	align 16
    .texturas_todas_eliminadas:
    mov eax,[p_materiales_na]
    test eax,eax
    jz  cont_grups_LimpiarHeap
    ;invoke HeapFree,proc_heap,0,p_objetos_noal
    mov eax,[p_materiales_na]
    call MemFree
	xor eax,eax
    mov [p_materiales_na],eax
    mov [num_materiales],eax
    
    cont_grups_LimpiarHeap:
    mov eax,[p_grupos_na]
    test eax,eax
    jz  salir_LimpiarHeap
    ;invoke HeapFree,proc_heap,0,p_objetos_noal
    mov eax,[p_grupos_na]
    call MemFree
	xor eax,eax
    mov [p_grupos_na],eax
    mov [num_grupos],eax

    salir_LimpiarHeap:

ret


RecorrerPixelesMT: ;num_threads->edi
    push ebp
    mov ebp,esp
    ;sub esp,0C4h ; 196
    VARS_ALIGN 16*4 + 8*VectorSize

    label .pcx       at esp 
    label .pcy       at esp+VectorSize
    label .vx        at esp+VectorSize*2
    label .vy        at esp+VectorSize*3
    label .tvy       at esp+VectorSize*4
    label .tvx       at esp+VectorSize*5
    label .punto_act at esp+VectorSize*6
    label .punto_ant at esp+VectorSize*7
    
    dwordsize = 4
    offset=VectorSize*8

    label .x         dword at esp+offset 
    label .y         dword at esp+offset + dwordsize
    label .delta_x   dword at esp+offset + dwordsize*2
    label .delta_y   dword at esp+offset + dwordsize*3
    label .delta_w   dword at esp+offset + dwordsize*4
    label .delta_h   dword at esp+offset + dwordsize*5
    label .mouse_x   dword at esp+offset + dwordsize*6
    label .mouse_y   dword at esp+offset + dwordsize*7
    label .inic_th   dword at esp+offset + dwordsize*8
    label .alto_th   dword at esp+offset + dwordsize*9
    label .ultimo_th dword at esp+offset + dwordsize*10
    label .cont      dword at esp+offset + dwordsize*11
    label .handle    dword at esp+offset + dwordsize*12
    label .idx       dword at esp+offset + dwordsize*13
    label .objeto       dword at esp+offset + dwordsize*14
    label .num_threads       dword at esp+offset + dwordsize*15
    
    mov [.num_threads],edi

match =exe,TARGET
{
    DELTAS_CAMARA

    lea eax,[.mouse_x]
    lea ebx,[.mouse_y]
    cinvoke SDL_GetMouseState,eax,ebx
    ;add esp, 8

    PUNTO_CAMARA .mouse_x,.mouse_y
    
    .if([dragging]=0)
		xor eax,eax
		LUZ_PTR eax
        movaps [eax + Luz.posicion],xmm0
    .else
        movaps [.punto_act],xmm0
    .endif
    
    ;calcular la direccion del rayo segun la posicion del mouse
    ;despues generar un rayo y detectar el objeto que se encuentra justo enfrente, para despues
    ;calcular el movimiento del objeto al ser arrastrado
    
    ;si no hay dragging, se brinca esta parte
    mov eax,[dragging]
    test eax,eax
    jz .sale_dragging
    
    ;si ya esta detectado el obj_drag, entonces tambien saltar esto
    mov eax,[grupo_drag]
    test eax,eax
    jnz .no_detectar_obj_drag
    
    ;dragging = 1 y obj_drag!=0 por lo tanto hay que detectar el objeto
    movaps xmm1,[camara.eye]
    subps xmm0,xmm1
    movaps [.tvx],xmm0
    
    V_NORMALIZE .tvx
    
    xor ebx,ebx               ;grupo_ant
    xor ecx,ecx               ;material_ant
    xor edx,edx               ;depth_treshold
    lea esi,[camara.eye]    ;origen
    lea edi,[.tvx]          ;direccion
    call TraceRay
    ;en ecx se encuentra el puntero al objeto detectado, ahora se pasa a obj_drag si dragging != 0
    mov [grupo_drag],edx
    test edx,edx
    jz .sale_dragging
    
	align 16
    .no_detectar_obj_drag:
    
    ;ahora, ver si mouse_x_ant o mouse_y_ant son distintos de zero, en cuyo caso, calcular el movimiento y mover el objeto
    .if([mouse_x_ant]<>0 | [mouse_y_ant]<>0)
        PUNTO_CAMARA mouse_x_ant,mouse_y_ant
        
        ;movaps [punto_ant],xmm0
        movaps xmm1,[.punto_act]
        subps xmm1,xmm0 ;punto actual menos anterior
        movss xmm2,[muno]
        shufps xmm2,xmm2,0
        mulps xmm1,xmm2

        
        mov eax ,[grupo_drag]
        mov eax,[eax + Grupo.id]
        mov [.x],eax ;//ptr del grupo a mover
        
		;xor ecx,ecx
        mov [.idx],0
        
       
    
        .siguiente_mover_objetos:
        
        mov ecx, [p_objetos]
        mov eax, Objeto3DSize
        mul [.idx]
        add eax,ecx
        ;mov [objeto],eax
        
        mov ebx,[.x]
        cmp ebx,[eax + Objeto3D.grupo_id]
        jne .continua_mover_objetos
        
        
        movaps xmm3,[eax + Objeto3D.v1]
        subps xmm3,xmm1
        movaps [eax + Objeto3D.v1],xmm3
        
        .continua_mover_objetos:
		mov eax, [.idx]
		add eax,1
        mov [.idx],eax
        cmp eax,[num_objetos]
        jae .sale_dragging

        jmp  .siguiente_mover_objetos
    
    .endif
    
	align 16
    .sale_dragging:
    mov eax,[.mouse_x]
    mov ebx,[.mouse_y]
    mov [mouse_x_ant],eax
    mov [mouse_y_ant],ebx
    
}

    ;Llamar a RecorrePixeles con CreateThread, como parametro enviar un DWORD
    ;el HighWord indica el renglon inicial y el low word indica la cantidad de lineas a procesar

	;xor ebx,ebx
    mov [.cont],0
    mov [var_sincro],0

    align 16
    .threads_loop:
        mov eax,[sector_y]
        add eax,[.cont]
        match =Single,THREADING {
            push eax
            call RecorrePixeles4
        }
        match =Multi,THREADING {
            .if([num_threads]=1)
                push eax
                call RecorrePixeles4
                jmp .salir
            .endif
            ;match PLATFORM,Win32\{
            invoke CreateThread,0,0,RecorrePixeles4,eax,0,0
            
            ;push eax
            ;push RecorrePixeles
            ;call [SDL_CreateThread]	;este tiene un extraño memory leak
            ;add esp,8
            ;\}
            ;match PLATFORM,Linux\{
            ;lea ebx,[.handle]
            ;stdcall pthread_create,ebx,0,RecorrePixeles,eax
            ;\}
        }       
		mov eax,[.cont]
		add eax,1
		mov [.cont],eax
		mov ebx,[num_threads]
        cmp eax,ebx
        je .termina_RPMT
        jmp .threads_loop

    
	align 16
    .termina_RPMT:
        ;pause
        invoke Sleep,1
		;mov esi,[mutex]
		;invoke WaitForSingleObject,esi,0xFFFFFFFF
        mov edi,[var_sincro]
		;invoke ReleaseMutex,esi
        cmp edi,ebx		;.num_threads, guardado de mas atras
        jb .termina_RPMT
;leave
.salir:
mov esp,ebp
pop ebp
ret

section '.idata' import data readable writeable

  library kernel32,'KERNEL32.DLL',\
      user32,'USER32.DLL',sdl,'sdl.dll',msvcrt,'msvcrt.dll',\
      freeimage,'freeimage.dll',sdl_gfx,'sdl_gfx.dll'

  include 'c:\fasm\include\api\kernel32.inc'
  include 'c:\fasm\include\api\user32.inc'
  include 'sdl.inc'
  include 'msvcrt.inc'
  include 'freeimage.inc'
  include 'sdl_gfx.inc'

match =dll,TARGET {
section '.edata' export data readable

export 'asmRT.dll',\
        Initialize,'?Initialize@@YGXXZ',\
        Deinitialize,'?Deinitialize@@YGXXZ',\
        RenderFrame,'?RenderFrame@@YGXXZ',\
        LoadDemo1,'?LoadDemo1@@YGXXZ',\
        EstableceResolucion,'?SetResolution@@YGXHH@Z',\
        EstableceSector,'?SetSector@@YGXHH@Z',\
        SetPixelBuffer,'?SetPixelBuffer@@YGXPAX@Z',\
		CreaMateriales,'?CreateMaterials@@YGPAXH@Z',\
		CreaGrupos,'?CreateGroups@@YGPAXH@Z',\
		CreaObjetos3D,'?CreateObjects@@YGPAXH@Z',\
		CreaLuces,'?CreateLights@@YGPAXH@Z',\
		TraeMaterial,'?GetMaterial@@YGPAXH@Z',\
		TraeGrupo,'?GetGroup@@YGPAXH@Z',\
		TraeObjeto3D,'?GetObject@@YGPAXH@Z',\
		TraeLuz,'?GetLight@@YGPAXH@Z',\
		TraeCamara,'?GetCamera@@YGPAXH@Z',\
		LimpiarHeap,'?CleanAll@@YGXXZ',\
		EstableceNumThreads,'?SetNumThreads@@YGXH@Z'
section '.reloc' fixups data discardable

}