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

align 16
RecorrePixeles4: ;parametro ebp+8
    push ebp
    mov ebp,esp
    
    VARS_ALIGN 14*4 + 28*VectorSize
    label .tvc       at esp 
    label .pcy       at esp+VectorSize
    label .pcx       at esp+VectorSize*2
    label .vx        at esp+VectorSize*3
    label .vy        at esp+VectorSize*4
    label .tvy       at esp+VectorSize*5
    label .vf        at esp+VectorSize*6
    label .tvx       at esp+VectorSize*7
    label .uvw       at esp+VectorSize*8
    label .xs        at esp+VectorSize*9
    label .ys        at esp+VectorSize*10
    label .zs        at esp+VectorSize*11
    label .vxus      at esp+VectorSize*12
    label .vxvs      at esp+VectorSize*13
    label .vxws      at esp+VectorSize*14
    label .vyus      at esp+VectorSize*15
    label .vyvs      at esp+VectorSize*16
    label .vyws      at esp+VectorSize*17
    label .tvxus      at esp+VectorSize*18
    label .tvxvs      at esp+VectorSize*19
    label .tvxws      at esp+VectorSize*20
    label .tvyus      at esp+VectorSize*21
    label .tvyvs      at esp+VectorSize*22
    label .tvyws      at esp+VectorSize*23
    label .grupos_ant at esp+VectorSize*24
    label .crs        at esp+VectorSize*25
    label .cgs        at esp+VectorSize*26
    label .cbs        at esp+VectorSize*27
    
    

    dwordsize = 4
    offset=VectorSize*28

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

    ; 	//Utilizando la fórmula presentada en el artículo 
    ;	//State of the Art in Interactive Ray Tracing
    ;	//Para convertir coordenadas de pantalla a coordenadas u,v,w
    ;	//float us=p_l + (p_r - p_l)*(x + 0.5)/d_width;
    ;	//float vs=p_b + (p_t - p_b)*(y + 0.5)/d_height;



    DELTAS_CAMARA
    ;xor eax,eax
    mov [.cont],0
    ;mov eax,[screen]
    ;mov eax,[eax + SDL_Surface.pixels]
    mov eax,[pixeles]
    mov [.pix],eax

    mov eax,[.parametro]
    mov [.y],eax

    mov eax,[sector_height]
    add eax,[sector_y]
    mov [.height],eax
    
    ;mov eax,[.y]       
    ;mov ebx,[_width]
    ;mul ebx
    ;mov ebx,4
    ;mul ebx
    ;add [.pix],eax
    
    
    ;debo convertir vx y vy al formato vertical
    
    ;vxus, tiene 4 veces la u de vx
    ;vxvs, tiene 4 veces la v de vx
    ;vxws, tiene 4 veces la w de vx
    ;vyus, tiene 4 veces la u de vy
    ;vyvs, tiene 4 veces la v de vy
    ;vyws, tiene 4 veces la w de vy
    
    movss xmm0,[.vx + Vector.u]
    movss xmm1,[.vx + Vector.v]
    movss xmm2,[.vx + Vector.w]
    movss xmm3,[.vy + Vector.u]
    movss xmm4,[.vy + Vector.v]
    movss xmm5,[.vy + Vector.w]    
    
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0
    shufps xmm3,xmm3,0
    shufps xmm4,xmm4,0
    shufps xmm5,xmm5,0 
    
    movaps [.vxus],xmm0
    movaps [.vxvs],xmm1
    movaps [.vxws],xmm2
    movaps [.vyus],xmm3
    movaps [.vyvs],xmm4
    movaps [.vyws],xmm5
    
    ;*************************************************************
    ;xmm0 -> xs
    ;xmm1 -> ys
    ;xmm2 -> zs
    ;*************************************************************
   
    ;grupos anteriores en cero
    xorps xmm0,xmm0
    movaps [.grupos_ant],xmm0
    align 16
    .LoopY:
        
        mov eax,[.y]            ;calcular el nuevo desplazamiento para pix, pues 'y' no siempre es cero
        mov ebx,[_width]
        mov ecx,[pixeles]
        imul eax,ebx
        lea eax,[eax*4 + ecx]
        mov [.pix],eax

        
        CALCULA_Ys
        CALCULA_Xs

        ;mov eax,[num_threads]
        ;add [.y],eax
        ;CALCULA_Ys
        ;CALCULA_Xs
        
        ;add [.y],1
        ;CALCULA_Ys
        ;CALCULA_Xs


        ;add [.y],1
        ;CALCULA_Ys
        ;CALCULA_Xs
        

        mov eax,[num_threads]
        add [.y],eax
        mov eax,[.y]
        mov edx,[.height]
        cmp eax,edx
        jl .LoopY
align 16
salir_RP:
;mov ebx,[mutex]
;invoke WaitForSingleObject,ebx,0xFFFFFFFF
;mov eax,[var_sincro]
;add eax,1
;mov [var_sincro],eax
;invoke ReleaseMutex,ebx
lock inc [var_sincro]

mov esp,ebp
pop ebp
retn 4 ;macro que pone leave, retn (num de bytes de parametros)


;Entradas
;esi -> depth_treshold
;xmm0,1,2 direcciones
;xmm3,4,5 origenes
;xmm6 -> grupos_ant
;xmm7 -> refracciones_ant
;Salidas
;xmm0 -> Colores rojos para los 4 vectores
;xmm1 -> Colores verdes para los 4 vectores
;xmm2 -> Colores azules para los 4 vectores
;xmm3 -> objetos_detectados
;xmm4 -> distancias (t) para cada uno de los vectores, con el objeto que choco
;xmm5 -> refracciones anteriores
align 16
TraceRay4: ;vdir->edi vorg->esi depth_treshold-> edx,ptr_material_ant-> ecx ptr_grupo_ant->ebx ,     ;u,v,w,ou,ov,ow,depth_treshold ;u:eax,v:ebx,w:ecx,ou:edx,ov:esi,ow:edi
    push ebp
    mov ebp,esp
        
    dwordsize = 4    
    VARS_ALIGN 4*dwordsize + VectorSize*40

    label .ptr_grupos               at esp
    label .ptr_grupos_ant           at esp+VectorSize
    label .orgsu                at esp+VectorSize*2
    label .orgsv                at esp+VectorSize*3
    label .orgsw                at esp+VectorSize*4
    label .dirsu                at esp+VectorSize*5
    label .dirsv                at esp+VectorSize*6
    label .dirsw                at esp+VectorSize*7
    label .fus                  at esp+VectorSize*8
    label .fvs                  at esp+VectorSize*9
    label .fws                  at esp+VectorSize*10
    label .temp1                at esp+VectorSize*11
    label .temp2                at esp+VectorSize*12
    label .mask                 at esp+VectorSize*13
    label .ts                   at esp+VectorSize *14
    label .objetos_detectados   at esp+VectorSize*15
    label .cas                  at esp+VectorSize*16
    label .crs                  at esp+VectorSize*17
    label .cgs                  at esp+VectorSize*18
    label .cbs                  at esp+VectorSize*19
    label .tipos                at esp+VectorSize*20
    label .distancias           at esp+VectorSize*28
    label .normalesu            at esp+VectorSize*29
    label .normalesv            at esp+VectorSize*30
    label .normalesw            at esp+VectorSize*31
    label .bs                   at esp+VectorSize*32
    label .b2s                  at esp+VectorSize*33
    label .ptr_materiales       at esp+VectorSize*34
    label .refracciones         at esp+VectorSize*35
    label .refracciones_ant     at esp+VectorSize*36
    label .us                    at esp+VectorSize*37
    label .vs                    at esp+VectorSize*38
    label .specular             at esp+VectorSize*39
    
    offset=VectorSize*40

    label .depth_treshold				dword at esp+offset
    label .objeto                       dword at esp+offset+dwordsize
    label .idx                          dword at esp+offset+dwordsize*2

    label .luz                          dword at esp+offset+dwordsize*3
    
    ;label .ptr_grupos_ant   at ebp+8

    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Inicializar vectores
    mov [.depth_treshold],esi
    movaps [.ptr_grupos_ant],xmm6
    movaps [.refracciones_ant],xmm7
    
    ;mov eax,[.ptr_grupos_ant]
    ;movaps xmm0,[eax]
    ;movaps [.grupos_ant],xmm0

    movaps [.dirsu],xmm0
    movaps [.dirsv],xmm1
    movaps [.dirsw],xmm2
    movaps [.orgsu],xmm3
    movaps [.orgsv],xmm4
    movaps [.orgsw],xmm5

    ; ¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤*¤
    
    xorps xmm0,xmm0
    movaps [.ptr_grupos],xmm0
    movaps [.tipos],xmm0
    movaps [.objetos_detectados],xmm0
    movaps [.ptr_materiales],xmm0
    movaps [.refracciones],xmm0
    movaps [.specular],xmm0
    
    ;mov [.idx],0
    ;xor eax,eax
    mov [.idx],0
    ;movss xmm0,[max_dist]
    ;shufps xmm0,xmm0,0
    movaps [.ts],xmm0

    ; ¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤
    mov eax, [p_objetos]
    mov [.objeto],eax
    align 16
    .recorrer_objetos:
    ;calcula la posicion en memoria del siguiente objeto y pone el puntero en la variable objeto

    ;mov ecx, [p_objetos]
    ;mov eax, Objeto3DSize
    ;mul [.idx]
    ;add eax,ecx
    ;mov [.objeto],eax

    ;aki hay que ver si es esfera o triangulo
    ;por el momento sera, si no es triangulo, es esfera
    
    ;movaps xmm0,[.dirsu]
    ;movaps xmm1,[.dirsv]
    ;movaps xmm2,[.dirsw]
    ;movaps xmm3,[.orgsu]
    ;movaps xmm4,[.orgsv]
    ;movaps xmm5,[.orgsw]

    mov ebx,[eax + Objeto3D.tipo]
    test ebx,OBJ_TRIANGULO
    jnz  .es_triangulo
    ;cmp ebx,OBJ_TRIANGULO
    ;je  .es_triangulo



    ; ¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤÷¤
    ;Parte de deteccion de si es esfera
    ;tambien sirve para detectar colision
    ;vert0=(float*)(&(object->v1));//Vector *c=sphere->center;
    ;V_SUB(edge1,orig,vert0);//Vector *f= (*e)-c; (origen - centro)
align 16
    movaps xmm3,[.orgsu]
    movaps xmm4,[.orgsv]
    movaps xmm5,[.orgsw]
    
    movss xmm0,[eax + Objeto3D.v1.u]
    movss xmm1,[eax + Objeto3D.v1.v]
    movss xmm2,[eax + Objeto3D.v1.w]
    
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0
    
    subps xmm3,xmm0 ;edge1's u aka edge1
    subps xmm4,xmm1 ;edge1's v
    subps xmm5,xmm2 ;edge1's w
    
    ;B=-2*V_DOT(edge1,dir);//B=((*f) * d)*2;
    movaps xmm0,[.dirsu]
    movaps xmm1,[.dirsv]
    movaps xmm2,[.dirsw]
    
    mulps xmm0,xmm3
    mulps xmm1,xmm4
    mulps xmm2,xmm5
    
    addps xmm0,xmm1
    addps xmm0,xmm2 ;V_DOT(edge1,dir)
    
    
    movss xmm7,[mdos]
    shufps xmm7,xmm7,0
    mulps xmm0,xmm7         ;b -> xmm0
    
    ;B2=B*B;
    movaps xmm1,xmm0
    mulps xmm1,xmm1        ;b2 -> xmm1
            
    
    ;C=(V_DOT(edge1,edge1)) - (object->n[0]*object->n[0]);// C=((*f) * f) - r2;
    movss xmm6,[eax + Objeto3D.radio]
    shufps xmm6,xmm6,0
    mulps xmm6,xmm6 ;r2
    
    mulps xmm3,xmm3
    mulps xmm4,xmm4
    mulps xmm5,xmm5
    
    addps xmm3,xmm4
    addps xmm3,xmm5
    
    ;movaps xmm2,xmm3
    subps xmm3,xmm6 ;C -> xmm2


   
    ;I=B2 - 4*C;
    movss xmm7,[cuatro]
    shufps xmm7,xmm7,0
    mulps xmm3,xmm7 ;4*C
    subps xmm1,xmm3 ;I -> xmm1


    ;if(I<0)					//No hay intersección
        
    xorps xmm3,xmm3
    cmpps xmm3,xmm1,1       ;xmm1 mayor que xmm3
    movmskps ebx,xmm3
    mov edx,ebx
    test ebx,ebx
    jz .continuar_recorrer_objetos
        ;	break;
align 16
    movaps xmm7,xmm3  ;mask
    ;//Cálculos para obtener el(los) punto(s) de intersección
    sqrtps xmm1,xmm1


    ;t=(B - t0)/2;
    movaps xmm3,xmm0
    subps xmm3,xmm1
    movss xmm4,[dos]
    shufps xmm4,xmm4,0
    divps xmm3,xmm4
    
    ;movaps [.temp1],xmm3
    
    ;if(t<0.01)
    movss xmm5,[delta]
    shufps xmm5,xmm5,0
    movaps xmm6,xmm5
    cmpps xmm6,xmm3,1   ;xmm6 menor que xmm3 ; t<0.01 volteado
    movmskps ebx,xmm6
    

    ;	t=(B + t0)/2;	    
    movaps xmm5,xmm0
    addps xmm5,xmm1
    divps xmm5,xmm4

    ;los que son menores que 0.01 se pasan a xmm3, los que no, no cambian
    match =0,1 {
    movaps [.temp1],xmm3
    movaps [.temp2],xmm5
    xor edx,edx
    mov ecx,ebx
    and ecx,01000b
    setnz dl
    sub edx,1
    mov ecx,[.temp1 + Vector.w]
    sub ecx,[.temp2 + Vector.w]
    and ecx,edx
    add ecx,[.temp2 + Vector.w]
    mov [.temp1 + Vector.w],ecx
    
    xor edx,edx
    mov ecx,ebx
    and ecx,0100b
    setnz dl
    sub edx,1
    mov ecx,[.temp1 + Vector.v]
    sub ecx,[.temp2 + Vector.v]
    and ecx,edx
    add ecx,[.temp2 + Vector.v]
    mov [.temp1 + Vector.v],ecx
    
    xor edx,edx
    mov ecx,ebx
    and ecx,010b
    setnz dl
    sub edx,1
    mov ecx,[.temp1 + Vector.u]
    sub ecx,[.temp2 + Vector.u]
    and ecx,edx
    add ecx,[.temp2 + Vector.u]
    mov [.temp1 + Vector.u],ecx
    
    xor edx,edx
    mov ecx,ebx
    and ecx,01b
    setnz dl
    sub edx,1
    mov ecx,[.temp1 + Vector.t]
    sub ecx,[.temp2 + Vector.t]
    and ecx,edx
    add ecx,[.temp2 + Vector.t]
    mov [.temp1 + Vector.t],ecx
    
    movaps xmm3,[.temp1]
    }
    
    match =0,1 {
    movaps [.temp1],xmm3
    movaps [.temp2],xmm5

    mov ecx,ebx
    and ecx,01000b
    mov edi,[.temp1 + Vector.w]
    cmovnz edi,[.temp2 + Vector.w]
    mov [.temp1 + Vector.w],edi
    
    mov ecx,ebx
    and ecx,0100b
    mov edi,[.temp1 + Vector.v]
    cmovnz edi,[.temp2 + Vector.v]
    mov [.temp1 + Vector.v],edi
    
    mov ecx,ebx
    and ecx,010b
    mov edi,[.temp1 + Vector.u]
    cmovnz edi,[.temp2 + Vector.u]
    mov [.temp1 + Vector.u],edi
    
    mov ecx,ebx
    and ecx,01b
    mov edi,[.temp1 + Vector.t]
    cmovnz edi,[.temp2 + Vector.t]
    mov [.temp1 + Vector.t],edi
    movaps xmm3,[.temp1]
    

    }
    
    match =1,1 {
    ;este método es un poquito mejor que todos los demás
    subps xmm3,xmm5
    andps xmm3,xmm6
    addps xmm3,xmm5
    
    ;movaps [.temp1],xmm3
    }
    match =0,1 {
    movaps [.temp1],xmm3
    movaps [.temp2],xmm5
    mov ecx,ebx
    and ecx,01000b
    .if(ecx<>0)
        mov edi,[.temp2 + Vector.w]
        mov [.temp1 + Vector.w],edi
    .endif
        
    mov ecx,ebx
    and ecx,100b
    .if(ecx<>0)
        mov edi,[.temp2 + Vector.v]
        mov [.temp1 + Vector.v],edi
    .endif
        
    mov ecx,ebx
    and ecx,10b
    .if(ecx<>0)
        mov edi,[.temp2 + Vector.u]
        mov [.temp1 + Vector.u],edi
    .endif
        
    mov ecx,ebx
    and ecx,1b
    .if(ecx<>0)
        mov edi,[.temp2 + Vector.t]
        mov [.temp1 + Vector.t],edi
    .endif
    movaps xmm3,[.temp1]
    }

    ;if(t<0.01)
    movss xmm5,[delta]
    shufps xmm5,xmm5,0
    ;ovaps xmm6,xmm3
    cmpps xmm5,xmm3,1   ;xmm6 menor que xmm5
    movmskps edx,xmm5
    test edx,edx
    jz .continuar_recorrer_objetos
    
    andps xmm7,xmm5
    ;movaps [.mask],xmm7
    andps xmm3,xmm7
    ;las distancias nuevas son menores a las anteriores o las anteriores, iguales a cero
    COMPARAR_DISTANCIAS_OBJ xmm3


    ;movaps [.temp1],xmm1

    
    ;if(t<ta || ta==0)
    ;{
    ;	ta=t;
    ;	closest_object=object;
    ;}
    align 16
    .continuar_recorrer_objetos:
    add eax,Objeto3DSize
    mov [.objeto],eax
    mov ebx, [.idx]
    add ebx,1
    mov [.idx],ebx
    mov edx,[num_objetos]
    cmp ebx,edx
    jl  .recorrer_objetos

    jmp .termina_recorrer_objetos

align 16
    .es_triangulo:
    
    ;T Moller, B. Trumbore, Fast, Minimum Storage Ray/Triangle Intersection
    ;mov eax,[.objeto]
    
    ;V_CROSS(pvec,dir,triangle->edge2);
    ;V_CROSS .temp1 + Vector,.vdir + Vector,eax + Objeto3D.v3
    
    movss xmm0,[eax + Objeto3D.v3.u]
    movss xmm1,[eax + Objeto3D.v3.v]
    movss xmm2,[eax + Objeto3D.v3.w]
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0
    
    movaps [.fus],xmm0
    movaps [.fvs],xmm1
    movaps [.fws],xmm2
    
    V4_CROSS .dirsu,.dirsv,.dirsw,.fus,.fvs,.fws
    
    movaps [.fus],xmm4
    movaps [.fvs],xmm5
    movaps [.fws],xmm6  ;pvec

    
    
    ;det=V_DOT(triangle->edge1,pvec);
    ;V_DOT eax + Objeto3D.v2,.pvec + Vector
    movss xmm0,[eax + Objeto3D.v2.u]
    shufps xmm0,xmm0,0
    movss xmm1,[eax + Objeto3D.v2.v]
    shufps xmm1,xmm1,0
    movss xmm2,[eax + Objeto3D.v2.w]
    shufps xmm2,xmm2,0
    
    mulps xmm0,xmm4
    mulps xmm1,xmm5
    mulps xmm2,xmm6
    addps xmm0,xmm1
    addps xmm0,xmm2
    
    movaps [.temp1],xmm0    ;dets


    ;//NO CULLING
    ;if(det > -EPSILON && det <EPSILON)
    ;	break;
    ;mepsilon<det<epsilon break
    movss xmm1,[epsilon]
    shufps xmm1,xmm1,0
    cmpps xmm1,xmm0,1       ;xmm1 tiene los mayores a epsilon
    movss xmm2,[mepsilon]
    shufps xmm2,xmm2,0
    movaps xmm3,xmm0        
    cmpps xmm3,xmm2,1       ;xmm3 tiene los menores a mepsilon
    
    orps xmm1,xmm3 ;aki stan lo sque si pasaron la prueba del epsilon
        
    movmskps ebx,xmm1   ;si son todos cero, entonces saltar a la siguiente figura
    test ebx,ebx
    jz .continuar_recorrer_objetos_triangulo ;si es menor que epsilon continua con la siguiente figura
    
align 16
    movaps [.mask],xmm1 ;aki van guardados los ke han pasado las pruebas

    
    movaps xmm1,[unos]
    divps xmm1,xmm0
    movaps [.temp2],xmm1
    movaps xmm0,xmm1
    
    
    ;V_SUB(tvec,orig,triangle->vert0);
    ;V_SUB tvec,.vorg,eax + Objeto3D.v1
    movaps xmm1,[.orgsu]
    movaps xmm2,[.orgsv]
    movaps xmm3,[.orgsw]
    movss xmm4,[eax + Objeto3D.v1.u]
    movss xmm5,[eax + Objeto3D.v1.v]
    movss xmm6,[eax + Objeto3D.v1.w]
    shufps xmm4,xmm4,0
    shufps xmm5,xmm5,0
    shufps xmm6,xmm6,0
    
    subps xmm1,xmm4
    subps xmm2,xmm5
    subps xmm3,xmm6 ;tvec
    
    
    movaps [.normalesu],xmm1
    movaps [.normalesv],xmm2
    movaps [.normalesw],xmm3 ;tvec
    
    ;b=V_DOT(tvec,pvec) * inv_det;
    ;V_DOT .tvec + Vector,.pvec + Vector
    
    mulps xmm1,[.fus]
    mulps xmm2,[.fvs]
    mulps xmm3,[.fws]
    
    addps xmm1,xmm2
    addps xmm1,xmm3
    
    mulps xmm1,xmm0
    movaps [.bs],xmm1
    
    
    ;if(b < 0.0f || b > 1.0f)
    ;	break;
    ; ó 0<b<1
    xorps xmm2,xmm2
    cmpps xmm2,xmm1,2 ;b mayor que cero, xmm2 son los bs que son mayores a cero
    ;movss xmm3,[uno]
    ;shufps xmm3,xmm3,0
    movaps xmm3,[unos]
    movaps xmm4,xmm1
    cmpps xmm4,xmm3,2 ;bs que son menores que uno
    
    andps xmm4,xmm2
    
    movmskps ebx,xmm4
    test ebx,ebx
    jz .continuar_recorrer_objetos_triangulo
    
    align 16
    movaps xmm2,[.mask]
    andps xmm2,xmm4
    movaps [.mask],xmm2 ;los que siguen funcionando


    ;V_CROSS(qvec,tvec,triangle->edge1);
    ;V_CROSS .qvec + Vector,.tvec + Vector,eax + Objeto3D.v2
    
    movss xmm0,[eax + Objeto3D.v2.u]
    movss xmm1,[eax + Objeto3D.v2.v]
    movss xmm2,[eax + Objeto3D.v2.w]
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0
    
    movaps [.crs],xmm0
    movaps [.cgs],xmm1
    movaps [.cbs],xmm2
    
    V4_CROSS .normalesu,.normalesv,.normalesw,.crs,.cgs,.cbs
    
    movaps [.crs],xmm4
    movaps [.cgs],xmm5
    movaps [.cbs],xmm6  ;qvec
    

    ;b2=V_DOT(dir,qvec) * inv_det;
    ;V_DOT .vdir + Vector,.qvec + Vector
    
    mulps xmm4,[.dirsu]
    mulps xmm5,[.dirsv]
    mulps xmm6,[.dirsw]
    
    addps xmm4,xmm5
    addps xmm4,xmm6
    
    mulps xmm4,[.temp2]
    movaps [.b2s],xmm4
        
    ;if(b2 < 0.0f || b + b2 > 1.0f)
    ;	break;
    
    xorps xmm2,xmm2
    cmpps xmm2,xmm4,2 ;b2 mayor que cero, xmm2 son los bs que son mayores a cero
    ;movss xmm3,[uno]
    ;shufps xmm3,xmm3,0
    movaps xmm3,[unos]
    addps xmm4,[.bs]
    cmpps xmm4,xmm3,1 ;b + b2 que son menores que uno
    
    andps xmm4,xmm2
    
    movmskps ebx,xmm4
    test ebx,ebx
    jz .continuar_recorrer_objetos_triangulo
    
    align 16
    movaps xmm3,[.mask]
    andps xmm3,xmm4
    ;movaps [.mask],xmm3
    
    
    
    ;t=V_DOT(triangle->edge2,qvec)*inv_det;
    ;V_DOT eax + Objeto3D.v3,.qvec + Vector
    
    movss xmm0,[eax + Objeto3D.v3.u]
    movss xmm1,[eax + Objeto3D.v3.v]
    movss xmm2,[eax + Objeto3D.v3.w]
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0

    mulps xmm0,[.crs]
    mulps xmm1,[.cgs]
    mulps xmm2,[.cbs]
    
    addps xmm0,xmm1
    addps xmm0,xmm2
    
    mulps xmm0,[.temp2]
    
    movaps [.temp1],xmm0    ;ts
    

    ;if(t<0.01)
    ;	break;
    
    movss xmm5,[delta]
    shufps xmm5,xmm5,0
    cmpps xmm5,xmm0,1   ;xmm6 menor que xmm5
    movmskps ebx,xmm5
    test ebx,ebx
    jz .continuar_recorrer_objetos_triangulo

    
    ;if(t<ta || ta==0)
    ;{
    ;	ta=t;
    ;	closest_object=object;
    ;}
    ;movaps xmm1,[.mask]
    ;movmskps edx,xmm1
    ;and edx,ebx
    andps xmm3,xmm5
    andps xmm0,xmm3
    movaps xmm7,xmm3
    ;movmskps edx,xmm3
    COMPARAR_DISTANCIAS_OBJ xmm0
    align 16
    .continuar_recorrer_objetos_triangulo:
    add eax,Objeto3DSize
    mov [.objeto],eax
    mov ebx, [.idx]
    add ebx,1
    mov [.idx],ebx
    mov edx,[num_objetos]
    cmp ebx,edx
    jl  .recorrer_objetos
    align 16
    .termina_recorrer_objetos:
    ;probar si los closes objects son distintos de cero
    ;en cuyo caso poner un 1.0 en el xmm correspondiente de los colores de salida
    ;xmm0 -> Colores rojos para los 4 vectores
    ;xmm1 -> Colores verdes para los 4 vectores
    ;xmm2 -> Colores azules para los 4 vectores
    ;DumpVectorH .objetos_detectados

    ;termina la deteccion de objetos
    ;ahora obtener los colores
    
align 16
    xorps xmm0,xmm0
    movaps [.cas],xmm0
    movaps [.crs],xmm0
    movaps [.cgs],xmm0
    movaps [.cbs],xmm0
    movaps [.normalesu],xmm0
    movaps [.normalesv],xmm0
    movaps [.normalesw],xmm0

    ;movss xmm7,[uno]
    ;shufps xmm7,xmm7,0
    movaps xmm7,[unos]
    movaps [.temp2],xmm7
    movaps [.temp1],xmm0
    
    movaps xmm3,[.objetos_detectados]
    cmpps xmm3,xmm0,4   ;distintos
    movmskps ebx,xmm3
    test ebx,ebx
    jz .salir_TraceRay_definitivo
    
    ;prueba del depth
    mov ecx,[.depth_treshold]
    test ecx,ecx
    jz .salir_TraceRay_definitivo
    
    
align 16
    movaps [.mask],xmm3
    
    ;hasta aki .mask tiene 1's en los elementos que deben ser analizados
    
    ;primero calcular los puntos de interseccion
    ;para eso, tomar .dirs, multiplicar por ts  y sumar a orgs.
    movaps xmm0,[.dirsu]
    movaps xmm1,[.dirsv]
    movaps xmm2,[.dirsw]
    movaps xmm3,[.orgsu]
    movaps xmm4,[.orgsv]
    movaps xmm5,[.orgsw]

    mulps xmm0,[.ts]
    mulps xmm1,[.ts]
    mulps xmm2,[.ts]
    
    addps xmm3,xmm0
    addps xmm4,xmm1
    addps xmm5,xmm2 ;nuevos puntos

    ;no creo que ocupe los origenes anteriores, por lo que los voy a reemplazar aki
    movaps [.orgsu],xmm3
    movaps [.orgsv],xmm4
    movaps [.orgsw],xmm5


    ;debo guardar ebx, como referencia de los objetos que se encontraron
    ;y los que no
    ;aki reviso cada objeto, si fue encontrado alguno,
    ;se tae el id del material
    ;.tipos tiene 0 si es un material normal, 1 si tiene reflexion, 2 si tiene refraccion, y combinaciones con OR
    ;MAT_NORMAL equ 0
    ;MAT_REFLEXION equ 1
    ;MAT_REFRACCION equ 2

    xorps xmm0,xmm0
    movaps [.bs],xmm0
    movaps [.b2s],xmm0
    mov ecx,ebx
    and ecx,01000b
    .if(ecx<>0)
        ;mov edx,[.ts + Vector.w]
        ;mov [.temp1 + Vector.w],edx
        NORMAL_OBJETO w
        TRAE_COLOR w
    .endif
        
    mov ecx,ebx
    and ecx,100b
    .if(ecx<>0)
        ;mov edx,[.ts + Vector.v]
        ;mov [.temp1 + Vector.v],edx
        NORMAL_OBJETO v
        TRAE_COLOR v
    .endif
        
    mov ecx,ebx
    and ecx,10b
    .if(ecx<>0)
        ;mov edx,[.ts + Vector.u]
        ;mov [.temp1 + Vector.u],edx
        NORMAL_OBJETO u
        TRAE_COLOR u   
    .endif
        
    mov ecx,ebx
    and ecx,01b
    .if(ecx<>0)
        ;mov edx,[.ts + Vector.t]
        ;mov [.temp1 + Vector.t],edx
        NORMAL_OBJETO t
        TRAE_COLOR t
    .endif
    

    ;probar iluminación
    ;ya tengo
    ;ebx -> son los t's que sirven
    ;.ts distancias
    ;entonces hay que calcular los nuevos puntos y las normales de los puntos
    ;segun los objetos encontrados
    ;en .tipos esta el tipo de objeto de cada cosa
    ;shale :S

    ;las direcciones nuevas son ahcia la luz
    ;por lo tanto
    align 16
    xor eax,eax
    LUZ_PTR eax
    mov [.luz],eax
    
    movss xmm0,[eax + Luz.posicion.u]
    movss xmm1,[eax + Luz.posicion.v]
    movss xmm2,[eax + Luz.posicion.w]
    shufps xmm0,xmm0,0
    shufps xmm1,xmm1,0
    shufps xmm2,xmm2,0
    movaps xmm3,[.orgsu]
    movaps xmm4,[.orgsv]
    movaps xmm5,[.orgsw]
    OB_DIRECCION xmm0,xmm1,xmm2,xmm3,xmm4,xmm5
    movaps [.fus],xmm0
    movaps [.fvs],xmm1
    movaps [.fws],xmm2 ;direcciones normalizadas
    movaps [.distancias],xmm3
    
    
    ;detectar sombras
    ;1- ver que vectores contienen objeto, los que no tienen, hay que ponerlos en ceros
    ;hasta este momento sta sin modificar ebx
    ;las direcciones ya estaán
    movaps xmm3,[.orgsu]
    movaps xmm4,[.orgsv]
    movaps xmm5,[.orgsw]
    
    
    ;MASKOUT [.mask],xmm0,xmm1,xmm2
    ;MASKOUT [.mask],xmm3,xmm4,xmm5

    ;mov esi,0
    xor esi,esi
    call TraceRay4
    
    
align 16
    movaps xmm6,[.distancias]   ;determina que rayos fueron bloqueados antes de llegar a la luz
    cmpps xmm6,xmm4,1
    xorps xmm0,xmm0
    cmpps xmm0,xmm4,0
    orps xmm6,xmm0
    
    mov eax,[.luz]
    movss xmm7,[eax + Luz.intensidad] ;que rayos stan fura del alcance de la luz
    shufps xmm7,xmm7,0
    movaps xmm0,xmm4
    cmpps xmm0,xmm7,1
    andps xmm6,xmm0
    

    
    ;aplicar distancias a la intensidad de la luz
    movaps xmm0,xmm7
    subps xmm7,[.distancias]
    divps xmm7,xmm0
    xorps xmm0,xmm0
    maxps xmm7,xmm0
    andps xmm7,xmm6
    ;aplicar las sombras
    
    ;aplica la mascara para eliminar puntos que no sirven
    andps xmm7,[.mask]
    
    ;calcular productos punto para obtener el nivel de iluminación
    movaps xmm0,[.fus]
    movaps xmm1,[.fvs]
    movaps xmm2,[.fws]
    movaps xmm3,[.normalesu]
    movaps xmm4,[.normalesv]
    movaps xmm5,[.normalesw]
    
    mulps xmm3,xmm0
    mulps xmm4,xmm1
    mulps xmm5,xmm2
    
    addps xmm3,xmm4
    addps xmm3,xmm5

    
    xorps xmm6,xmm6
    cmpps xmm6,xmm3,1   ;los que son menores a cero, los borra


    andps xmm3,xmm6
    movaps xmm6,xmm3
    mulps xmm3,xmm7

    movaps xmm0,[.crs]
    movaps xmm1,[.cgs]
    movaps xmm2,[.cbs]
    


    movaps [.temp2],xmm3

    movaps xmm7,[.specular]
    xorps xmm5,xmm5
    cmpps xmm5,xmm7,4   ;si son cero los valores de specular
    movmskps ebx,xmm5
    test ebx,ebx
    jz .no_hay_specular
    
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

    ;2.0f * DOT( L, N ) * N
    movss xmm7,[mdos]
    shufps xmm7,xmm7,0
    mulps xmm6,xmm7
    movaps xmm3,[.normalesu]
    movaps xmm4,[.normalesv]
    movaps xmm5,[.normalesw]
    mulps xmm3,xmm6
    mulps xmm4,xmm6
    mulps xmm5,xmm6

    addps xmm3,[.fus]
    addps xmm4,[.fvs]
    addps xmm5,[.fws]   ;R
    
    mulps xmm3,[.dirsu]
    mulps xmm4,[.dirsv]
    mulps xmm5,[.dirsw]
    addps xmm3,xmm4
    addps xmm3,xmm5 ;dot
    
    xorps xmm5,xmm5
    cmpps xmm5,xmm3,1   
    movmskps ebx,xmm5
    test ebx,ebx
    jz .no_hay_specular
    
    ;movaps [.mask],xmm5
    
    mulps xmm3,xmm3
    mulps xmm3,xmm3
    
    movaps xmm6,xmm3
    
    mulps xmm3,xmm6
    mulps xmm3,xmm6
    mulps xmm3,xmm6
    mulps xmm3,xmm6 ;xmm3 potencia 20
    
    mulps xmm3,[.specular]
    andps xmm3,xmm5
    movaps xmm6,xmm3
    
    mov eax,[.luz]
    movss xmm3,[eax + Luz.color.r]
    shufps xmm3,xmm3,0
    movss xmm4,[eax + Luz.color.g]
    shufps xmm4,xmm4,0
    movss xmm5,[eax + Luz.color.b]
    shufps xmm5,xmm5,0
    
    mulps xmm3,xmm6
    mulps xmm4,xmm6
    mulps xmm5,xmm6
    
    addps xmm0,xmm3
    addps xmm1,xmm4
    addps xmm2,xmm5
    
    align 16
    .no_hay_specular:
    
    movaps xmm3,[.temp2]
    mulps xmm0,xmm3
    mulps xmm1,xmm3
    mulps xmm2,xmm3

    
    movaps [.crs],xmm0
    movaps [.cgs],xmm1
    movaps [.cbs],xmm2

    align 16
    .termina_iluminacion:
    
    ;generar rayos de reflexion
    ;y combinar con los colores anteriores
    ;primero probar hay cuando menos una reflexion, si no pasar a refraccion
    xorps xmm7,xmm7
    movaps xmm0,[.bs]
    cmpps xmm7,xmm0,4   ;xmm1, no son iguales a cero (xmm0)
    movmskps ebx,xmm7
    test ebx,ebx
    jz .refraccion       ;todos son ceros, entonces pasa a reflexion
    ;hay cuandomenos un rayo a calcular por reflexion, asi que a calcular y mandar llamar
    ;guardar en .mask, xmm7, que tendrá la máscara para los rayos a tomar en cuenta al volver de la funcion
    movaps [.mask] , xmm7
    
    ;calcular nuevos rayos
    REFLEXION4
    
        align 16
    .refraccion:
    xorps xmm7,xmm7
    movaps xmm0,[.refracciones]
    cmpps xmm7,xmm0,4   ;xmm1, no son iguales a cero (xmm0)
    movmskps ebx,xmm7
    test ebx,ebx
    jz .salir_TraceRay_definitivo
    ;movaps [.mask] , xmm7
    
    ;movss xmm6,[uno]
    ;shufps xmm6,xmm6,0
    movaps xmm6,[unos]
    movaps xmm0,[.cas]
    cmpps xmm0,xmm6,1   ;xmm0 son menores a uno
    andps xmm0,xmm7
    movmskps ebx,xmm0
    movaps [.temp1],xmm0
    test ebx,ebx
    jz .salir_TraceRay_definitivo
    
    movaps [.mask] , xmm0
    
    REFRACCION4

align 16        
.salir_TraceRay_definitivo:   
    movaps xmm0,[.crs]
    movaps xmm1,[.cgs]
    movaps xmm2,[.cbs]
    movaps xmm3,[.objetos_detectados]
    movaps xmm4,[.ts]
    movaps xmm5,[.refracciones]
;leave
mov esp,ebp
pop ebp
retn

;utiliza xmm0-5
