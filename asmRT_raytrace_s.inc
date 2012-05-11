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

RecorrePixeles: ;parametro ebp+8
    
    push ebp
    mov ebp,esp
    
    VARS_ALIGN 15*4 + 9*VectorSize
    
    label .tvc       at esp 
    label .pcy       at esp+VectorSize
    label .pcx       at esp+VectorSize*2
    label .vx        at esp+VectorSize*3
    label .vy        at esp+VectorSize*4
    label .tvy       at esp+VectorSize*5
    label .vf        at esp+VectorSize*6
    label .tvx       at esp+VectorSize*7
    label .uvw       at esp+VectorSize*8

    dwordsize = 4
    offset=VectorSize*9

    label .x         dword at esp+offset 
    label .y         dword at esp+offset + dwordsize
    label .delta_x   dword at esp+offset + dwordsize*2
    label .delta_y   dword at esp+offset + dwordsize*3
    label .ancho_c   dword at esp+offset + dwordsize*4
    label .alto_c    dword at esp+offset + dwordsize*5
    label .delta_w   dword at esp+offset + dwordsize*6
    label .delta_h   dword at esp+offset + dwordsize*7
    label .cont      dword at esp+offset + dwordsize*8
    label .pix       dword at esp+offset + dwordsize*9
    label .mouse_x   dword at esp+offset + dwordsize*10
    label .mouse_y   dword at esp+offset + dwordsize*11
    label .height    dword at esp+offset + dwordsize*12
    label .z         dword at esp+offset + dwordsize*13
    
    
    label .parametro         dword at ebp + 8



    DELTAS_CAMARA

    mov [.cont],0
    mov eax,[screen]
    mov eax,[eax + SDL_Surface.pixels]
    mov [.pix],eax

    mov eax,[.parametro]
    xor ebx,ebx
    mov bx,ax
    shr eax,16
    mov [.y],eax


    add eax,ebx
    mov [.height],eax

    mov eax,[.y]       ;calcular el nuevo desplazamiento para pix, pues 'y' no siempre es cero
    mov ebx,[_width]
    mul ebx
    mov ebx,4
    mul ebx
    add [.pix],eax

   
    .LoopY:
    
        ;	float cy=(float)((y + 0.5)/d_height);
        CVTSI2SS xmm0,[.y]
        mulss xmm0,[.delta_y]
        addss xmm0,[.delta_h]
        shufps xmm0,xmm0,0
        movdqa [.pcy],xmm0
        ;movss [pcy + Vector.u],xmm0
        ;movss [pcy+ Vector.v],xmm0
        ;movss [pcy+ Vector.w],xmm0
        
    ;	(*vy)*=cy;
        movdqa xmm0,[.vy]
        movdqa xmm1,[.pcy]
        mulps xmm0,xmm1
        movdqa [.tvy],xmm0
    
    
        mov [.x],0
        .LoopX:
    
            ;calcular u,v,w del nuevo rayo
            ;restando el ojo
            ;	float cx=(float)((x + 0.5)/d_width);
            CVTSI2SS xmm0,[.x]
            mulss xmm0,[.delta_x]
            addss xmm0,[.delta_w]
            shufps xmm0,xmm0,0
            movdqa [.pcx],xmm0
            ;movss [pcx+ Vector.u],xmm0
            ;movss [pcx+ Vector.v],xmm0
            ;movss [pcx+ Vector.w],xmm0

            ;	(*vx)*=cx
            movdqa xmm0,[.vx]
            movdqa xmm1,[.pcx]
            mulps xmm0,xmm1
            movdqa [.tvx],xmm0
    
           ;	Vector *sal= (*vx)+vy;
           ;
           ;	Vector *sal2=*lt + sal;

            ;movdqa xmm0,[tvx]
            movdqa xmm1,[.tvy]
            movdqa xmm2,[camara.lefttop]
            addps xmm0,xmm1
            addps xmm0,xmm2

            ;el nuevo punto ya esta en xmm0
            ;ahora hay que convertirlo en rayo, restandole el ojo
            movdqa xmm1,[camara.eye]
            subps xmm0,xmm1
            ;movdqa [uvw],xmm0
            movdqa xmm6,xmm0


            
            ;normalizar el rayo
            movdqa xmm1,xmm0
            mulps xmm1,xmm1

            ;con esto desempaco los cuadrados guardados en xmm0 y se ponen en xmm2,3,4
            movdqa xmm2,xmm1
            shufps xmm2,xmm2,1
            movdqa xmm3,xmm1
            shufps xmm3,xmm3,2
            movdqa xmm4,xmm1
            shufps xmm4,xmm4,3
                          

            ;suma horizontal
            
            xorps xmm5,xmm5
            addps xmm5,xmm2
            addps xmm5,xmm3
            addps xmm5,xmm4
    
            sqrtps xmm4,xmm5
            ;RSQRTPS xmm4,xmm5
            ;RCPPS xmm4,xmm4
            
            ;movss [tvc+ Vector.u],xmm4
            ;movss [tvc+ Vector.v],xmm4
            ;movss [tvc+ Vector.w],xmm4 
            ;UNPCKLPS xmm4,xmm4
            ;UNPCKLPS xmm4,xmm4
            SHUFPS xmm4,xmm4,0;vector con 4 veces la norma
        
            ;movdqa xmm0,[uvw]
            ;movdqa xmm1,[tvc]
            divps xmm6,xmm4
            ;mulps xmm6,xmm4
            movdqa [.uvw],xmm6   ;vector normalizado
            
            
            mov ebx,0               ;grupo_ant
            mov ecx,0               ;material_ant
            mov edx,10               ;depth_treshold
            lea esi,[camara.eye]    ;origen
            lea edi,[.uvw]          ;direccion
            call TraceRay
            ;stdcall TraceRay,[eax + Vector.u],[eax + Vector.v],[eax + Vector.w],[camara.eye.u],[camara.eye.v],[camara.eye.w],1

            ;aki hay que limitar que el color en real4 no se pase de 1.0
            ;el color esta en xmm1
            movss xmm2,[uno]
            shufps xmm2,xmm2,0
            
            minps xmm1,xmm2
            
            mov [.z],255
            CVTSI2SS xmm3,[.z]
            shufps xmm3,xmm3,0
            mulps xmm1,xmm3


            movdqa xmm2,xmm1
            shufps xmm2,xmm2,1
            movdqa xmm3,xmm1
            shufps xmm3,xmm3,2
            movdqa xmm4,xmm1
            shufps xmm4,xmm4,3

            xor eax,eax
            CVTSS2SI ebx,xmm2
            or eax,ebx
            shl eax,8

            CVTSS2SI ebx,xmm3
            or eax,ebx
            shl eax,8

            CVTSS2SI ebx,xmm4
            or eax,ebx 

            mov ebx,[.pix]
            mov [ebx],eax
    
            add [.pix],4
            
            inc [.x]
            mov eax,[.x]
            sub eax,[_width]
            jnz .LoopX
            
        inc [.y]
        mov eax,[.y]
        sub eax,[.height]
        jnz .LoopY    

.salir_RP:
inc [var_sincro]
leave
retn 4 ;macro que pone leave, retn (num de bytes de parametros)

TraceRay: ;vdir->edi vorg->esi depth_treshold-> edx,ptr_material_ant-> ecx ptr_grupo_ant->ebx ,     ;u,v,w,ou,ov,ow,depth_treshold ;u:eax,v:ebx,w:ecx,ou:edx,ov:esi,ow:edi
    push ebp
    mov ebp,esp
    
    VARS_ALIGN 26*4 + VectorSize*13

    label .f           at esp 
    label .tv          at esp+VectorSize
    label .tov         at esp+VectorSize*2
    label .dlv         at esp+VectorSize*3
    label .vdir        at esp+VectorSize*4
    label .vtemp       at esp+VectorSize*5
    label .pvec        at esp+VectorSize*6
    label .tvec        at esp+VectorSize*7
    label .qvec        at esp+VectorSize*8
    label .tcolor1     at esp+VectorSize*9
    label .tcolor2     at esp+VectorSize*10
    label .vorg        at esp+VectorSize*11
    label .color_obj        at esp+VectorSize*12

    dwordsize = 4
    offset=VectorSize*13

    label .b                       dword at esp+offset 
    label .b2                      dword at esp+offset + dwordsize
    label .ce                      dword at esp+offset + dwordsize*2
    label .i                       dword at esp+offset + dwordsize*3
    label .t0                      dword at esp+offset + dwordsize*4
    label .t                       dword at esp+offset + dwordsize*5
    label .ta                      dword at esp+offset + dwordsize*6
    label .int_luz                 dword at esp+offset + dwordsize*7
    label .inv_det                 dword at esp+offset + dwordsize*8
    label .objeto                  dword at esp+offset + dwordsize*9
    label .interseccion            dword at esp+offset + dwordsize*10
    label .id_objeto_detectado     dword at esp+offset + dwordsize*11
    label .salida                  dword at esp+offset + dwordsize*12
    label .z                       dword at esp+offset + dwordsize*13
    label .idx                     dword at esp+offset + dwordsize*14
    label .objeto_detectado        dword at esp+offset + dwordsize*15
    label .ptr_material            dword at esp+offset + dwordsize*16
    label .ptr_grupo				dword at esp+offset + dwordsize*17
    label .grupo_id				dword at esp+offset + dwordsize*18
    label .ptr_grupo_ant				dword at esp+offset + dwordsize*19
    label .ptr_material_ant				dword at esp+offset + dwordsize*20
    label .depth_treshold				dword at esp+offset + dwordsize*21
    label .x			            dword at esp+offset + dwordsize*22
    label .y				        dword at esp+offset + dwordsize*23
    label .u			            dword at esp+offset + dwordsize*24
    label .v				        dword at esp+offset + dwordsize*25
    

    ;push ebp       ;proc ya hace esto automaticamente
    ;mov ebp,esp    ;y esto

    

    ;no los pude alinea, no c pork
    ;locals

    ;idx dd 0             
    ;objeto_detectado dd 0 ;movidos para la alineacion

    ;_1 dd 0
    ;_2 dw 0

    ;f           Vector 0.0,0.0,0.0,0.0
    ;tv          Vector 0.0,0.0,0.0,0.0
    ;tov         Vector 0.0,0.0,0.0,0.0
    ;dlv         Vector 0.0,0.0,0.0,0.0
    ;vdir        Vector 0.0,0.0,0.0,0.0
    ;vtemp       Vector 0.0,0.0,0.0,0.0
    ;pvec        Vector 0.0,0.0,0.0,0.0
    ;tvec        Vector 0.0,0.0,0.0,0.0
    ;qvec        Vector 0.0,0.0,0.0,0.0
    ;tcolor1     Color 0.0,0.0,0.0,0.0
    ;tcolor2     Color 0.0,0.0,0.0,0.0
    ;vorg        Vector 0.0,0.0,0.0,0.0
    
    ;b   dd 0.0
    ;b2  dd 0.0
    ;ce  dd 0.0
    ;i   dd 0.0

    ;t0      dd 0.0
    ;t       dd 0.0
    ;ta      dd 0.0
    ;int_luz dd 0.0

    ;det     dd 0.0
    ;inv_det dd 0.0


    ;objeto dd 0
       
    ;interseccion        dd 0
    ;id_objeto_detectado dd 0
    ;salida              dd 0
    ;z                   dd 0

    ;endl
    ;align 16



    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Inicializar vectores
    movdqa xmm0,[edi]
    movdqa [.vdir],xmm0
    
    movdqa xmm0,[esi]
    movdqa [.vorg],xmm0
    
    mov [.ptr_grupo_ant],ebx
    mov [.ptr_material_ant],ecx
    mov [.depth_treshold],edx

            
    
    ;mov [vdir + Vector.t],0
    ;mov eax,[u]
    ;mov [vdir + Vector.u],eax
    ;mov eax,[v]
    ;mov [vdir + Vector.v],eax
    ;mov eax,[w]
    ;mov [vdir + Vector.w],eax

    ;mov [vorg + Vector.t],0
    ;mov eax,[ou]
    ;mov [vorg + Vector.u],eax
    ;mov eax,[ov]
    ;mov [vorg + Vector.v],eax
    ;mov eax,[ow]
    ;mov [vorg + Vector.w],eax

    ; ¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤
    
    mov [.salida],0
    mov [.interseccion],0
    mov [.id_objeto_detectado],0

    ;poner en negro tcolor1
    xorps xmm0,xmm0
    movdqa [.tcolor1],xmm0
    
    mov [.ptr_grupo],0
    mov [.ptr_material],0

    movss xmm0,[zero]
    movss [.ta],xmm0

    mov [.idx],0

    ; ¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤

    .recorrer_objetos:
    ;calcula la posicion en memoria del siguiente objeto y pone el puntero en la variable objeto
    mov ecx, [p_objetos]
    mov eax, Objeto3DSize
    mul [.idx]
    add eax,ecx
    mov [.objeto],eax

    ;aki hay que ver si es esfera o triangulo
    ;por el momento sera, si no es triangulo, es esfera

    mov ebx,[eax + Objeto3D.tipo]
    sub ebx,OBJ_TRIANGULO
    jz  .es_triangulo_pt1



    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Parte de deteccion de si es esfera
    ;tambien sirve para detectar colicion
   ;Vector *f= (*e)-c; origen del rayo - centro esfera (edge1)
    movdqa xmm0,[.vorg]

    mov eax,[.objeto]
    movdqa xmm1,[eax + Objeto3D.v1]
    subps xmm0,xmm1

    movdqa [.f],xmm0

    ;B=-2*V_DOT(edge1,dir);//B=((*f) * d)*2;

    ;producto punto
    movdqa xmm1,[.vdir]
    mulps xmm0,xmm1
    
    ;con esto desempaco los cuadrados guardados en xmm0 y se ponen en xmm2,3,4
    movdqa xmm2,xmm0
    shufps xmm2,xmm2,1
    movdqa xmm3,xmm0
    shufps xmm3,xmm3,2
    movdqa xmm4,xmm0
    shufps xmm4,xmm4,3

    xorps xmm5,xmm5
    movdqa xmm5,xmm2
    addps xmm5,xmm3
    addps xmm5,xmm4 ;sumatoria de los productos x1*x2 + y1*y2 ... PRODUCTO PUNTO!!!
    
    mulps xmm0,xmm0
    mulss xmm5,[mdos]
    movss [.b],xmm5    ;-2*producto punto de f y direccion del rayo

    mulps xmm5,xmm5
    movss [.b2],xmm5   ;cuadrado de b
   
    ;C=(V_DOT(edge1,edge1)) - (object->n[0]*object->n[0]);// C=((*f) * f) - r2;
    movss xmm0,[.f + Vector.u]
    movss xmm1,[.f + Vector.v]
    movss xmm2,[.f + Vector.w]
    mulps xmm0,xmm0
    mulps xmm1,xmm1
    mulps xmm2,xmm2
    addps xmm0,xmm1
    addps xmm0,xmm2
    
    mov eax,[.objeto]
    movss xmm1,[eax + Objeto3D.radio] ;r*r = r^2
    mulps xmm1,xmm1

    subps xmm0,xmm1
    movss [.ce],xmm0

    ;I=B2 - 4*C;
    addps xmm0,xmm0
    addps xmm0,xmm0; 4*C


    movss xmm1,[.b2]
    subps xmm1,xmm0
    movss [.i],xmm1

    ;if(I<0)					//No hay intersección
    ;break;
    COMISS xmm1,[zero]
    jb .continuar_recorrer_objetos

    ;t0=sqrt(I);
    ;movss xmm1,[i]
    SQRTpS xmm0,xmm1

    movss [.t0],xmm0

    ;t=(B - t0)/2;
    movss xmm0,[.b]
    subss xmm0,[.t0]
    divss xmm0,[dos]
    movss [.t],xmm0
    
    ;if(t<0.01)
    ;t=(B + t0)/2;
    COMISS xmm0,[delta]
    jae .cont1
    movss xmm0,[.b]
    addss xmm0,[.t0]
    divss xmm0,[dos]
    movss [.t],xmm0

    ;if(t<0.01)
    COMISS xmm0,[delta]
    jb .continuar_recorrer_objetos

    .cont1:
    ;if(t<ta || ta==0)
    movss xmm0,[.ta]
    comiss xmm0,[zero]
    je .continua_ta
    
    comiss xmm0,[.t]
    jb .continuar_recorrer_objetos
    
    .continua_ta:

    mov [.interseccion],1
    movss xmm0,[.t]
    movss [.ta],xmm0
    
    mov [.objeto_detectado],eax

    mov eax,[.idx]
    mov [.id_objeto_detectado],eax


    .continuar_recorrer_objetos:
    inc [.idx]
    mov eax, [.idx]
    sub eax,[num_objetos]
    jae .salir_TraceRay

    jmp  .recorrer_objetos

    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Parte de deteccion de colision con triangulos
    .es_triangulo_pt1:

    ;T Moller, B. Trumbore, Fast, Minimum Storage Ray/Triangle Intersection
    mov eax,[.objeto]
    
    ;V_CROSS(pvec,dir,triangle->edge2);
    V_CROSS .pvec + Vector,.vdir + Vector,eax + Objeto3D.v3
    ;det=V_DOT(triangle->edge1,pvec);
    V_DOT eax + Objeto3D.v2,.pvec + Vector
    ;movss det,xmm0

    ;//NO CULLING
    ;if(det > -EPSILON && det <EPSILON)
    ;	break;
    comiss xmm0,[mepsilon]
    jbe .cont_triangulo_pt1   ;si es menor que -epsilon continua bien
    comiss xmm0,[epsilon]
    jb .continuar_recorrer_objetos2 ;si es menor que epsilon continua con la siguiente figura
    
    .cont_triangulo_pt1:
    ;inv_det=1.0f/det;
    ;movss xmm0,uno
    ;divss xmm0,det
    ;movss inv_det,xmm0
    movss xmm7,[uno]
    divps xmm7,xmm0
    
    
    ;V_SUB(tvec,orig,triangle->vert0);
    movdqa xmm0,[.vorg]
    movdqa xmm1,[ eax + Objeto3D.v1]
    subps xmm0,xmm1
    movdqa [.tvec],xmm0
    
    ;b=V_DOT(tvec,pvec) * inv_det;
    V_DOT .tvec + Vector,.pvec + Vector

    mulps xmm0,xmm7;inv_det
    movss [.b],xmm0
    
    ;if(b < 0.0f || b > 1.0f)
    ;	break;
    comiss xmm0,[zero]
    jb .continuar_recorrer_objetos2
    comiss xmm0,[uno]
    jae .continuar_recorrer_objetos2
    

    ;V_CROSS(qvec,tvec,triangle->edge1);
    V_CROSS .qvec + Vector,.tvec + Vector,eax + Objeto3D.v2
    

    ;b2=V_DOT(dir,qvec) * inv_det;
    V_DOT .vdir + Vector,.qvec + Vector

    mulps xmm0,xmm7;inv_det
    movss [.b2],xmm0
    
    ;if(b2 < 0.0f || b + b2 > 1.0f)
    ;	break;
    comiss xmm0,[zero]
    jb .continuar_recorrer_objetos2
    addss xmm0,[.b]
    comiss xmm0,[uno]
    ja .continuar_recorrer_objetos2
    
    ;t=V_DOT(triangle->edge2,qvec)*inv_det;
    V_DOT eax + Objeto3D.v3,.qvec + Vector

    
    mulps xmm0,xmm7;inv_det
    movss [.t],xmm0
    
    ;if(t<0.01)
    ;	break;
    comiss xmm0,[delta]
    jb .continuar_recorrer_objetos2
    
    ;if(t<ta || ta==0)
    ;{
    ;	ta=t;
    ;	closest_object=object;
    ;}
    movss xmm0,[.ta]
    comiss xmm0,[zero]
    je .continua_ta2

    comiss xmm0,[.t]
    jb .continuar_recorrer_objetos2
    
    .continua_ta2:

    mov [.interseccion],1
    movss xmm0,[.t]
    movss [.ta],xmm0

    mov [.objeto_detectado],eax    

    mov eax,[.idx]
    mov [.id_objeto_detectado],eax
    
    mov eax,[.b]
    mov [.u],eax
    mov eax,[.b2]
    mov [.v],eax

    .continuar_recorrer_objetos2:
    inc [.idx]
    mov eax, [.idx]
    sub eax,[num_objetos]
    jae .salir_TraceRay

    jmp  .recorrer_objetos
    ; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
    .salir_TraceRay:

    

    mov eax,[.interseccion]
    test eax,eax
    jz .salir_TraceRay_definitivo

    ;poner en ptr_grupo, el grupo detectado
    mov eax,[.objeto_detectado]
    mov ebx,[eax + Objeto3D.grupo_id]
    GRUPO_PTR ebx
    ;mov [.grupo_id],ebx
    ;mov eax, GrupoSize
    ;mul ebx
    ;add eax,[p_grupos]
    mov [.ptr_grupo],ebx
    
    ;poner en ptr_material, el material del objeto encontrado
    mov ebx,[eax + Grupo.material_id]
    MATERIAL_PTR ebx
    ;mov eax, MaterialSize
    ;mul ebx
    ;add eax,[p_materiales]
    mov [.ptr_material],ebx
    

    
    
    mov eax,[.depth_treshold]
    test eax,eax
    jz .salir_TraceRay_definitivo

    
    ; «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
    

    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Obtener Iluminación
    
    ;Calcular punto de origen nuevo

    movdqa xmm0,[.vdir] ;tomar el vector direccion
    movss xmm3,[.ta]
    shufps xmm3,xmm3,0
    movdqa [.vtemp],xmm3
    ;movss [vtemp + Vector.u],xmm3
    ;movss [vtemp + Vector.v],xmm3
    ;movss [vtemp + Vector.w],xmm3
    movdqa xmm1,[.vtemp] ;multiplicarlo por ta (la distancia)
    mulps xmm0,xmm1
    movdqa xmm1,[.vorg]
    addps xmm0,xmm1     ;sumarle el origen del rayo
    movdqa [.tov],xmm0   ;y aki sta el nuevo origen
    
    ;xmm0 es el nuevo punto de origen, hay que restarselo a la luz
    
    ;Calcular la direccion hacia la luz
    movdqa xmm1,[luz.posicion]
    subps xmm1,xmm0

    ;xmm1 es el nuevo vector, hay que normalizarlo
    ;Normalizar el vector de direccion
    movdqa xmm0,xmm1
    mulps xmm0,xmm0
    movdqa [.vtemp],xmm0

    movss xmm3,[zero]
    addss xmm3,[.vtemp + Vector.u]
    addss xmm3,[.vtemp + Vector.v]
    addss xmm3,[.vtemp + Vector.w]

    sqrtps xmm4,xmm3
    
    movss [.b],xmm4

    ;si es el skydome, siempre sta iluminado, por lo tanto .b debe valer 0
    mov eax,[.objeto_detectado]
    mov ebx,[eax + Objeto3D.tipo]
    sub ebx,OBJ_SKYDOME
    jnz  .no_es_skydome_intensidad_luz
    xorps xmm4,xmm4
    movss [.b],xmm4
    .no_es_skydome_intensidad_luz:
    
    ;si xmm4 -> distancia del origen a la luz es mayor que la intensidad de la luz, salir
    comiss xmm4,[luz.intensidad]
    ja .salir_TraceRay_definitivo

    shufps xmm4,xmm4,0
    ;movdqa [vtemp],xmm4
    ;movss xmm0,[uno]
    ;movss [vtemp+ Vector.t],xmm4
    ;movss [vtemp+ Vector.u],xmm4
    ;movss [vtemp+ Vector.v],xmm4
    ;movss [vtemp+ Vector.w],xmm4

    ;movdqa xmm2,[vtemp] ;vector con 3 veces repetida la norma del vector, para normalizar
    
    divps xmm1,xmm4
    
    movdqa [.dlv],xmm1   ;vector normalizado

    

    ;aki hay que ver si es esfera o triangulo y obtener la normal
    ;por el momento sera, si no es triangulo, es esfera

    ;mov ecx, p_objetos
    ;mov eax, Objeto3DSize
    ;mul id_objeto_detectado
    ;add eax,ecx
    ;eax ya esta igualado a .objeto_detectado desde arriba en deteccion de skydome
    ;mov eax,[.objeto_detectado]
    mov ebx,[eax + Objeto3D.tipo]

    sub ebx,OBJ_TRIANGULO
    jz  .es_triangulo_pt2

    ; ¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤
    ;Iluminacion de esfera
    ;como se trata de una esfera, hay que calcular la normal, y normalizarla jajaja
    ;aki tambien determina la normal del skydome

    movdqa xmm0,[.tov]
    ;mov ecx, p_objetos
    ;mov eax, Objeto3DSize
    ;mul id_objeto_detectado
    ;add eax,ecx
    movdqa xmm1,[eax + Objeto3D.v1]
    subps xmm0,xmm1
    
    ;ya esta el vector en xmm0 ahora hay que normalizarlo ¬¬ que show
    movdqa xmm1,xmm0
    mulps xmm1,xmm1 ;cuadrados de cada uno de los elementos del vector
    movdqa [.vtemp],xmm1 

    movss xmm3,[zero]
    addss xmm3,[.vtemp + Vector.u]
    addss xmm3,[.vtemp + Vector.v]
    addss xmm3,[.vtemp + Vector.w]

    sqrtps xmm4,xmm3    ;la 1/norma del vector 

    shufps xmm4,xmm4,0
    
    ;movss xmm5,[uno]
    ;movss [vtemp + Vector.t],xmm4
    ;movss [vtemp + Vector.u],xmm4
    ;movss [vtemp + Vector.v],xmm4
    ;movss [vtemp + Vector.w],xmm4

    ;movdqa xmm2,[vtemp]
    divps xmm0,xmm4

    ;vector normal de la esfera
    
    ;siendo un skydome, hay que invertir la normal
    mov ebx,[eax + Objeto3D.tipo]
    sub ebx,OBJ_SKYDOME
    jnz  .no_es_skydome_normal
    movss xmm1,[muno]
    shufps xmm1,xmm1,0
    mulps xmm0,xmm1
    .no_es_skydome_normal:
    
    movdqa [.tv],xmm0
    jmp .calculo_prod_punto_objeto

    .es_triangulo_pt2:
    ;mov ecx, [p_objetos]
    ;mov eax, Objeto3DSize
    ;mul [id_objeto_detectado]
    ;add eax,ecx
    ;movdqa xmm0,[eax + Objeto3D.normal]
    ;movdqa [tv],xmm0
    
    V_CROSS .tv + Vector,eax + Objeto3D.v3,eax + Objeto3D.v2
    V_NORMALIZE .tv


    .calculo_prod_punto_objeto:
    
    ;primero traer el color del objeto, ya sea del color establecido o de la textura


    
    mov ebx,[.ptr_material]
    movdqa xmm7,[ebx + Material.color]
    movdqa [.color_obj],xmm7
    
    mov ebx,[ebx + Material.textura]
    test ebx,ebx
    jz .no_hay_textura
    TEXTURAS

    .no_hay_textura:
    

    ;si es un skydome, .color_obj se pasa directamente a .tcolor1 y se brinca hasta la salida del programa,
    ;no hay que calcular la intensidad de la luz
    mov eax,[.objeto_detectado]
    mov ebx,[eax + Objeto3D.tipo]
    sub ebx,OBJ_SKYDOME
    jnz  .no_es_skydome_color
    movdqa xmm0,[.color_obj]
    movdqa [.tcolor1],xmm0
    jmp .salir_TraceRay_definitivo
    .no_es_skydome_color:

    ;ahora calcular el producto punto del vector normal de direccion hacia la luz, con la normal del objeto
    movdqa xmm0,[.tv]
    movdqa xmm1,[.dlv]
    mulps xmm0,xmm1


    movdqa [.vtemp],xmm0
    ;xorps xmm4,xmm4
    movss xmm4,[.vtemp + Vector.u]
    addss xmm4,[.vtemp + Vector.v]
    addss xmm4,[.vtemp + Vector.w]  ;en xmm4 se guarda la intensidad de la luz

    ;movss [int_luz],xmm4   ;en int_luz se guarda la intensidad de la luz


    
    ;si el producto punto es menos de cero significa que la luz no incide lo suficiente y es sombra lo que se ve
    ;if(dg<=0)
    ;			continue;
    comiss xmm4,[zero]
    jbe .calcular_otros_efectos

    
    ;incluir en la intensidad de la luz, la distancia con la misma
    ;d=(light->intensity-d)/light->intensity;
    ;   dg*=d;
    movss xmm0,[luz.intensidad]
    movdqa xmm1,xmm0
    subss xmm1,[.b]
    divps xmm1,xmm0
    mulps xmm4,xmm1
    movss [.int_luz],xmm4

    ;sombras
    ;yasta listo, ahora buscar alguna interseccion entre la luz y el punto de origen

  
    mov ebx,[.ptr_grupo]               ;grupo_ant
    mov ecx,[.ptr_material]               ;material_ant
    mov edx,0               ;depth_treshold
    lea esi,[.tov]    ;origen
    lea edi,[.dlv]          ;direccion
    call TraceRay
    ;stdcall TraceRay,[eax + Vector.u],[eax + Vector.v],[eax + Vector.w],[ebx + Vector.u],[ebx + Vector.v],[ebx + Vector.w],0


    test ebx,ebx    ;si no hubo interseccion continuar
    jz .continua_iluminacion_1

    comiss xmm0,[.b]   ;b es la distancia del punto a la luz, si la distancia del objeto intersectado es menor a esto, hay sombra, asi que salir
    jb .calcular_otros_efectos
    
    
    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    .continua_iluminacion_1:



    ;calcula nuevo color
   
    ;movss xmm0,[eax + Objeto3D.color.r]
    ;mulss xmm0,[int_luz]                           ;con int_luz se aplica la intensidad de la luz
    ;movss xmm1,[eax + Objeto3D.color.g]
    ;mulss xmm1,[int_luz]
    ;movss xmm2,[eax + Objeto3D.color.b]
    ;mulss xmm2,[int_luz]
    ;movss [tcolor1 + Color.r],xmm0
    ;movss [tcolor1 + Color.g],xmm1
    ;movss [tcolor1 + Color.b],xmm2
    
    
    movdqa xmm0,[.color_obj]
    movss xmm1,[.int_luz]
    shufps xmm1,xmm1,0
    mulps xmm0,xmm1
    movdqa [.tcolor1],xmm0
    
    
    mov eax,[.objeto_detectado]
    ;por el momento solo aplicare specular lighting en esferas, los triangulos se ven rarones
    mov ebx,[eax + Objeto3D.tipo]
    sub ebx,OBJ_TRIANGULO
    jz  .no_hay_specular



    ;iluminacion specular
    ;tomado de:
    ;http://www.flipcode.com/archives/Raytracing_Topics_Techniques-Part_2_Phong_Mirrors_and_Shadows.shtml
    ;por Jacco Bikker, a.k.a. "The Phantom"
    ;vector3 V = a_Ray.GetDirection();
    ;vector3 R = L - 2.0f * DOT( L, N ) * N;
    ;float dot = DOT( V, R );
    ;if (dot > 0)
    ;{
    ;	float spec = powf( dot, 20 ) * prim->GetMaterial()->GetSpecular() * shade;
    ;// add specular component to ray color
    ;a_Acc += spec * light->GetMaterial()->GetColor();
    ;}
    ;L = dlv
    ;V = vdir
    ;R = qvec
    ;N = tv
    ;vtemp =vtemp
    V_DOT .dlv + Vector,.tv + Vector    ;xmm0 tiene el producto punnto

    
    mulss xmm0,[dos]
    shufps xmm0,xmm0,0
    ;movss [vtemp + Vector.t],xmm0
    ;movss [vtemp + Vector.u],xmm0
    ;movss [vtemp + Vector.v],xmm0
    ;movss [vtemp + Vector.w],xmm0
    movdqa xmm1,[.tv]
    ;movdqa xmm2,[vtemp]
    mulps xmm1,xmm0
    movdqa [.pvec],xmm1
    V_SUB .qvec,.dlv,.pvec
    V_DOT .vdir + Vector,.qvec + Vector
    
    
    comiss xmm0,[zero]
    jbe .no_hay_specular

    mulps xmm0,xmm0
    movdqa xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    mulps xmm1,xmm0
    ;pow,xmm1,20 ¬¬ k feo hack
    ;* prim->GetMaterial()->GetSpecular() * shade; los ignoro, specular lo considero 1 y shade tambien
    ;pues este indica si hay luz o no
    shufps xmm1,xmm1,0
    movdqa [.tcolor2],xmm1
    ;movss [.tcolor2 + Color.r],xmm1
    ;movss [.tcolor2 + Color.g],xmm1
    ;movss [.tcolor2 + Color.b],xmm1
    movdqa xmm0,[luz.color]
    movdqa xmm1,[.tcolor2]
    mulps xmm0,xmm1
    movdqa xmm2,[.tcolor1]
    addps xmm2,xmm0
    movdqa [.tcolor1],xmm2

    
    .no_hay_specular:

    .calcular_otros_efectos:
    
    ;REFLEXION
    ;calcular REFLEXION, primero hay que ver si el material tiene reflexion
    ;if(material->reflection_index>0.0f)
    mov eax,[.ptr_material]
    movss xmm7,[eax + Material.reflexion]
    comiss xmm7,[zero]
    je .probar_Refraccion
    movss [.t],xmm7
    REFLEXION
    
    .probar_Refraccion:
    mov eax,[.ptr_material]
    movss xmm7,[eax + Material.refraccion]
    comiss xmm7,[zero]
    je .salir_TraceRay_definitivo
    movss xmm7,[.color_obj + Color.a]
    comiss xmm7,[uno]
    jae .salir_TraceRay_definitivo
    REFRACCION
    

    .salir_TraceRay_definitivo:
    
    mov eax,[.salida]
    mov ebx,[.interseccion]
    mov ecx,[.objeto_detectado]
    mov edx,[.ptr_grupo]
    mov esi,[.ptr_material]
    movss xmm0,[.ta]
    movdqa xmm1,[.tcolor1]
leave
retn
