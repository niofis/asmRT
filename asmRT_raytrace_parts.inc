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

macro OB_DIRECCION de_1,de_2,de_3, or_1,or_2,or_3
{
    subps de_1,or_1
    subps de_2,or_2
    subps de_3,or_3
    
    ;ahora hay que normalizarlos
    
    movaps or_1,de_1
    movaps or_2,de_2
    movaps or_3,de_3
    
    mulps or_1,or_1
    mulps or_2,or_2
    mulps or_3,or_3
    
    addps or_1,or_2
    addps or_1,or_3
    sqrtps or_1,or_1
    
    divps de_1,or_1
    divps de_2,or_1
    divps de_3,or_1
}

macro NORMAL_OBJETO c
{
    local .es_triangulo,.salir
    
    xorps xmm0,xmm0
    movaps [.temp1],xmm0
    
    mov eax,[.objetos_detectados + Vector.#c#]
    mov ecx,[eax + Objeto3D.tipo]
    ;cmp ecx,OBJ_TRIANGULO
    test ecx,OBJ_TRIANGULO
    jnz .es_triangulo
    ;pero como no es triangulo, entonces es esfera
    movss xmm0,[.orgsu + Vector.#c#]
    movss xmm1,[.orgsv + Vector.#c#]
    movss xmm2,[.orgsw + Vector.#c#]
    movss [.temp1 + Vector.u],xmm0
    movss [.temp1 + Vector.v],xmm1
    movss [.temp1 + Vector.w],xmm2
    movaps xmm0,[.temp1]
    movaps xmm1,[eax + Objeto3D.v1]
    subps xmm0,xmm1
    
    ;ya esta el vector en xmm0 ahora hay que normalizarlo ¬¬ que show
    movaps xmm1,xmm0
    mulps xmm1,xmm1 ;cuadrados de cada uno de los elementos del vector
    movaps [.temp1],xmm1 

    xorps xmm3,xmm3
    addss xmm3,[.temp1 + Vector.u]
    addss xmm3,[.temp1 + Vector.v]
    addss xmm3,[.temp1 + Vector.w]

    sqrtps xmm4,xmm3    ;la norma del vector 

    shufps xmm4,xmm4,0
    
    divps xmm0,xmm4 ;esta es la normal
    movaps [.temp1],xmm0
    
    movss xmm0,[.temp1 + Vector.u]
    movss xmm1,[.temp1 + Vector.v]
    movss xmm2,[.temp1 + Vector.w]
    
    mov ecx,[eax + Objeto3D.tipo]
    .if(ecx=OBJ_SKYDOME)
        movss xmm4,[muno]
        mulps xmm0,xmm4
        mulps xmm1,xmm4
        mulps xmm2,xmm4
    .endif
    
    movss [.normalesu + Vector.#c#],xmm0
    movss [.normalesv + Vector.#c#],xmm1
    movss [.normalesw + Vector.#c#],xmm2
    jmp .salir
    align 16
    .es_triangulo:
    
    V_CROSS .temp1 + Vector,eax + Objeto3D.v3,eax + Objeto3D.v2
    V_NORMALIZE .temp1
    movss xmm0,[.temp1 + Vector.u]
    movss xmm1,[.temp1 + Vector.v]
    movss xmm2,[.temp1 + Vector.w]
    movss [.normalesu + Vector.#c#],xmm0
    movss [.normalesv + Vector.#c#],xmm1
    movss [.normalesw + Vector.#c#],xmm2
    ;xmm0 queda con el vector normal
    align 16
    .salir:
}

macro MASKOUT mask,r1,r2,r3
{
    andps r1,mask
    andps r2,mask
    andps r3,mask
}

macro TRAE_COLOR c
{

    local .no_hay_textura,.continua_color
    mov ecx,[.objetos_detectados + Vector.#c#]
    
    mov eax,[ecx + Objeto3D.grupo_id]
    GRUPO_PTR eax
    mov [.ptr_grupos + Vector.#c#],eax
    
    mov eax,[eax + Grupo.material_id]
    MATERIAL_PTR eax
    mov [.ptr_materiales + Vector.#c#],eax
    
    
    mov ecx,[eax + Material.textura]
    test ecx,ecx
    jz .no_hay_textura
    mov edi,[.objetos_detectados + Vector.#c#]
    mov esi,eax
    movss xmm0,[.us + Vector.#c#]
    movss xmm1,[.vs + Vector.#c#]
    movss xmm2,[.normalesu + Vector.#c#]
    movss xmm3,[.normalesv + Vector.#c#]
    movss xmm4,[.normalesw + Vector.#c#]
    call TraeTextura
    mov [.cas + Vector.#c#],eax
    mov [.crs + Vector.#c#],edi
    mov [.cgs + Vector.#c#],ecx
    mov [.cbs + Vector.#c#],edx
    

    jmp .continua_color
    align 16
    .no_hay_textura:
    
    mov edi,[eax + Material.color.a]
    mov [.cas + Vector.#c#],edi
    mov edi,[eax + Material.color.r]
    mov [.crs + Vector.#c#],edi
    mov edi,[eax + Material.color.g]
    mov [.cgs + Vector.#c#],edi
    mov edi,[eax + Material.color.b]
    mov [.cbs + Vector.#c#],edi
    align 16
    .continua_color:
    
    mov eax,[.ptr_materiales + Vector.#c#]
    mov edi, [eax + Material.reflexion]
    ;.if(edi<>0)
        mov [.bs + Vector.#c#],edi
    ;.endif
    mov edi, [eax + Material.refraccion]
    ;.if (edi<>0)
        mov [.refracciones + Vector.#c#],edi
    ;.endif
    mov edi,[eax + Material.specular]
    mov [.specular + Vector.#c#],edi
}
    
;Entradas:
;edi -> ptr_objeto
;esi -> .ptr_material
;xmm0-> u
;xmm1 -> v   ; u,v son de triangulos
;xmm2-> x
;xmm3->y
;xmm4 -> z normal del objeto
;Salidas:
;eax -> A
;edi -> R
;ecx -> g
;edx -> b
TraeTextura: 
    
    push ebp
    mov ebp,esp
    
    dwordsize = 4    
    VARS_ALIGN 7*dwordsize + VectorSize*3

    label .color_obj               at esp
    label .normal                  at esp + VectorSize
    label .vtemp                  at esp + VectorSize*2
    
    offset=VectorSize*3

    label .i			  dword at esp+offset
    label .z              dword at esp+offset+dwordsize
    label .x              dword at esp+offset+dwordsize*2
    label .y              dword at esp+offset+dwordsize*3
    label .u              dword at esp+offset+dwordsize*4
    label .v              dword at esp+offset+dwordsize*5
    label .ptr_material   dword at esp+offset+dwordsize*6
    
    mov eax,edi
    movss [.u],xmm0
    movss [.v],xmm1
    
    movss [.normal + Vector.u],xmm2
    movss [.normal + Vector.v],xmm3
    movss [.normal + Vector.w],xmm4

    mov [.ptr_material],esi

    mov edi,[eax + Objeto3D.tipo]
    ;cmp edi,OBJ_TRIANGULO
    ;je .textura_triangulo
    test edi,OBJ_TRIANGULO
    jnz .textura_triangulo

    ;esfera
    ;tomado de:
    ;Ray Tracing: Graphics for the Masses
    ;by Paul Rademacher
    ;phi = arccos( -dot_product( Vn, Vp ))
    V_DOT eax + Objeto3D.v2,.normal + Vector

    mulss xmm0,[muno]
    movss [.i],xmm0
    ;arccos(N)=ATN ( SQR ( 1 – N ^ 2 ) / N ) 

    ;FINIT
    ARCCOS .i

    ;v = phi / PI
    ;fldpi
    ;fstp [.z]
    movss xmm0,[.i]
    divss xmm0,[pi]
    movss [.v],xmm0

    ;theta = ( arccos( dot_product( Vp, Ve ) / sin( phi )) ) / ( 2 * PI) 
    V_DOT .normal + Vector,eax + Objeto3D.v3

    movss [.x],xmm0
    fld [.i]
    fsin
    fstp [.y]
    movss xmm0,[.x]
    divss xmm0,[.y]
    movss [.x],xmm0

    ARCCOS .x

    movss xmm1,[dos]
    mulss xmm1,[pi]
    movss xmm0,[.x]
    divps xmm0,xmm1
    movss [.x],xmm0

    ;if ( dot_product( cross_product( Vn, Ve ), Vp ) > 0 )
    V_CROSS .vtemp + Vector,eax + Objeto3D.v2,eax + Objeto3D.v3
    V_DOT .vtemp + Vector,.normal + Vector
    comiss xmm0,[zero]
    jbe .else_textura_esfera
    ;u = theta
    movss xmm0,[.x]
    movss [.u],xmm0
    jmp .termina_textura_esfera
     
    ;else
    ;u = 1 - theta
    align 16
    .else_textura_esfera:
    movss xmm0,[uno]
    subss xmm0,[.x]
    movss [.u],xmm0

    align 16
    .termina_textura_esfera:

    jmp .trae_color_textura

    align 16
    .textura_triangulo:
    ;triangulo


    align 16
    .trae_color_textura:

    ;convierto u y v en coordenadas de textura;
    ;tomo u, v y width
    ;pixel = (v * width) + u
    mov eax,[.ptr_material]
    mov edi,[eax + Material.txt_width]
    mov edx,edi ;width

    mov ecx,[eax + Material.txt_height]

    cvtsi2ss xmm0,edi
    cvtsi2ss xmm1,ecx 

    mulss xmm0,[.u] ;u y v convertidos a corrdenadas de la textura
    mulss xmm1,[.v]

    cvtss2si edi,xmm0
    cvtss2si ecx,xmm1


    match =0,1 {
    mov esi,[eax + Material.ptr_textura]
    mov eax,ecx ;[.y]
    mul edx; [.z] ;y * width
    add eax,edi; [.x] ; (y*width) + x
    mov edi,4
    mul edi
    add eax,esi
    }
    match=1,1 {
    mov esi,[eax + Material.ptr_textura]
    ;ecx-> y (abcisa del punto a traer)
    ;edx -> ancho de la imagen
    ;edi -> x (ordenada del punto a traer) 
    ; (y*width) + x   
    imul ecx,edx
    add ecx,edi
    lea eax,[ecx * 4 + esi]
    }

    mov eax,[eax]
    mov ecx,255
    CVTSI2SS xmm1,ecx

    movzx ecx,al
    ;and ecx,0ffh
    CVTSI2SS xmm0,ecx
    divps xmm0,xmm1
    movss [.color_obj + Color.b],xmm0

    shr eax,8
    movzx ecx,al
    ;and ecx,0ffh
    CVTSI2SS xmm0,ecx
    divps xmm0,xmm1
    movss [.color_obj + Color.g],xmm0

    shr eax,8
    movzx ecx,al
    ;and ecx,0ffh
    CVTSI2SS xmm0,ecx
    divps xmm0,xmm1
    movss [.color_obj + Color.r],xmm0

    shr eax,8
    movzx ecx,al
    ;and ecx,0ffh
    CVTSI2SS xmm0,ecx
    divps xmm0,xmm1
    movss [.color_obj + Color.a],xmm0

    mov eax,[.color_obj + Color.a]
    mov edi,[.color_obj + Color.r]
    mov ecx,[.color_obj + Color.g]
    mov edx,[.color_obj + Color.b]
;leave
mov esp,ebp
pop ebp
retn


macro COMPARAR_DISTANCIAS_OBJ reg
{
    movaps xmm5,[.ts]
    movaps xmm6,reg    ;reg son las distancias
    cmpps xmm6,xmm5,1
    xorps xmm4,xmm4
    cmpps xmm4,xmm5,0	;son iguales a cero
    orps xmm6,xmm4
    andps xmm6,xmm7
    
    ;and ebx,edx
    match =0,1 \{
    xor ecx,ecx
    not ecx
    movd xmm7,ecx
    pshufd xmm7,xmm7,0
    pxor xmm7,xmm6
    
    movd xmm1,eax
    pshufd xmm1,xmm1,0
    movdqa xmm2, [.objetos_detectados]
    psubd xmm2,xmm1
    pand xmm2,xmm7
    paddd xmm2,xmm1
    movdqa [.objetos_detectados],xmm2
    
    
    movd xmm1,[eax + Objeto3D.tipo]
    pshufd xmm1,xmm1,0
    movdqa xmm2, [.tipos]
    psubd xmm2,xmm1
    pand xmm2,xmm7
    paddd xmm2,xmm1
    movdqa [.tipos],xmm2
    
    movaps xmm1,reg
    movaps xmm2, [.ts]
    subps xmm2,xmm1
    andps xmm2,xmm7
    addps xmm2,xmm1
    movaps [.ts],xmm2

    movaps xmm1,[.bs]
    movaps xmm2, [.us]
    subps xmm2,xmm1
    andps xmm2,xmm7
    addps xmm2,xmm1
    movaps [.us],xmm2
    
    movaps xmm1,[.b2s]
    movaps xmm2, [.vs]
    subps xmm2,xmm1
    andps xmm2,xmm7
    addps xmm2,xmm1
    movaps [.vs],xmm2
    \}

    match =1,1 \{
    movmskps ebx,xmm6
    movaps [.temp1],reg
    mov esi,[eax + Objeto3D.tipo]
    mov ecx,ebx
    and ecx,01000b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.w]
        mov [.ts + Vector.w],edi
        mov [.objetos_detectados + Vector.w],eax
        mov [.tipos + Vector.w],esi
        mov edi,[.bs + Vector.w]
        mov [.us + Vector.w],edi
        mov edi,[.b2s + Vector.w]
        mov [.vs + Vector.w] , edi
    .endif
        
    mov ecx,ebx
    and ecx,100b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.v]
        mov [.ts + Vector.v],edi
        mov [.objetos_detectados + Vector.v],eax
        mov [.tipos + Vector.v],esi
        mov edi,[.bs + Vector.v]
        mov [.us + Vector.v],edi
        mov edi,[.b2s + Vector.v]
        mov [.vs + Vector.v] , edi

    .endif
        
    mov ecx,ebx
    and ecx,10b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.u]
        mov [.ts + Vector.u],edi
        mov [.objetos_detectados + Vector.u],eax
        mov [.tipos + Vector.u],esi
        mov edi,[.bs + Vector.u]
        mov [.us + Vector.u],edi
        mov edi,[.b2s + Vector.u]
        mov [.vs + Vector.u] , edi

    .endif
        
    mov ecx,ebx
    and ecx,01b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.t]
        mov [.ts + Vector.t],edi
        mov [.objetos_detectados + Vector.t],eax
        mov [.tipos + Vector.t],esi
        mov edi,[.bs + Vector.t]
        mov [.us + Vector.t],edi
        mov edi,[.b2s + Vector.t]
        mov [.vs + Vector.t] , edi

    .endif
    \}
}
    
macro V4_CROSS v1u,v1v,v1w,v2u,v2v,v2w
{
    
    ;V_CROSS(dest,v1,v2)dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
    movaps xmm0,[v1v] ;[.dirsv]    ;v1[1]
    movaps xmm1,[v1w] ;[.dirsw]    ;v1[2]
    movaps xmm2,[v2w] ;[eax + Objeto3D.v3.w]  
    ;shufps xmm2,xmm2,0      ;v2[2]
    movaps xmm3,[v2v] ;[eax + Objeto3D.v3.v]
    ;shufps xmm3,xmm3,0      ;v2[1]
    
    movaps xmm4,xmm0
    mulps xmm4,xmm2
    movaps xmm7,xmm1
    mulps xmm7,xmm3
    
    subps xmm4,xmm7     ;pvec[0]
    
    ;-----------------------------------------------------------------
    movaps xmm0,[v1u] ;.dirsu    ;v1[0]
    movaps xmm3,[v2u];[eax + Objeto3D.v3.u]
    ;shufps xmm3,xmm3,0      ;v2[0]

    movaps xmm5,xmm1
    mulps xmm5,xmm3
    movaps xmm7,xmm0
    mulps xmm7,xmm2
    
    subps xmm5,xmm7     ;pvec[1]
    
    ;-----------------------------------------------------------------
    movaps xmm1,[v1v] ;[.dirsv]    ;v1[1]
    movaps xmm2,[v2v] ;[eax + Objeto3D.v3.v]  
    shufps xmm2,xmm2,0      ;v2[1]

    movaps xmm6,xmm0
    mulps xmm6,xmm2
    movaps xmm7,xmm1
    mulps xmm7,xmm3
    
    subps xmm6,xmm7     ;pvec[2]
}

macro REFLEXION4     ;tv->normal(vert1), vdir->V(vert0)
{
;if(material->reflection_index>0.0f)
;{
;    vert0=(float*)(&(ray->direction->u)); //V
;    vert1=(float*)(&(pt->normal->u));	//N
;	t=(V_DOT(vert0,vert1)) * 2 ;
;	//R = V – 2 * ( V . N ) * N;
;	
;   ray->direction->u=vert0[0] - t * vert1[0];
;	ray->direction->v=vert0[1] - t * vert1[1];
;	ray->direction->w=vert0[2] - t * vert1[2];
;	
;   ray->origin->u=pt->u;
;	ray->origin->v=pt->v;
;	ray->origin->w=pt->w;
;	
;   old_color[0]=pt->color->r;
;	old_color[1]=pt->color->g;
;	old_color[2]=pt->color->b;
;
;   old_color[0]*=1-material->reflection_index;
;	old_color[1]*=1-material->reflection_index;
;	old_color[2]*=1-material->reflection_index;
;	
;	Trace(pt,ray,depth-1);
;	pt->color->r=old_color[0] + pt->color->r*material->reflection_index;
;	pt->color->g=old_color[1] + pt->color->g*material->reflection_index;
;	pt->color->b=old_color[2] + pt->color->b*material->reflection_index;
;}
    ;xmm7 tiene reflection_index
    ;V_DOT .vdir + Vector,.tv + Vector
    local .salir_reflexion
    
    movaps xmm0,[.dirsu]
    movaps xmm1,[.dirsv]
    movaps xmm2,[.dirsw]
    movaps xmm3,[.normalesu]
    movaps xmm4,[.normalesv]
    movaps xmm5,[.normalesw]
    
    mulps xmm0,xmm3
    mulps xmm1,xmm4
    mulps xmm2,xmm5
    
    addps xmm0,xmm1
    addps xmm0,xmm2 ;V_DOT
    
    movss xmm6,[dos]
    shufps xmm6,xmm6,0
    
    mulps xmm0,xmm6 
    movaps xmm7,xmm0    ;t
    
    
    movaps xmm0,[.dirsu]
    movaps xmm1,[.dirsv]
    movaps xmm2,[.dirsw]
    movaps xmm3,[.normalesu]
    movaps xmm4,[.normalesv]
    movaps xmm5,[.normalesw]
    
    mulps xmm3,xmm7
    mulps xmm4,xmm7
    mulps xmm5,xmm7
    
    subps xmm0,xmm3
    subps xmm1,xmm4
    subps xmm2,xmm5 ;direcciones nuevas
    
    
    movaps xmm3,[.orgsu]
    movaps xmm4,[.orgsv]
    movaps xmm5,[.orgsw]
    
    movaps xmm6,[.mask]
    andps xmm0,xmm6
    andps xmm1,xmm6
    andps xmm2,xmm6
    andps xmm3,xmm6
    andps xmm4,xmm6
    andps xmm5,xmm6
    
    mov esi,[.depth_treshold]
    test esi,esi
    jz .salir_reflexion
    dec esi
    movaps xmm6,[.ptr_grupos]
    movaps xmm7,[.refracciones]
    call TraceRay4
    
    movaps xmm6,[.bs]
    ;movss xmm7,[uno]
    ;shufps xmm7,xmm7,0
    movaps xmm7,[unos]
    subps xmm7,xmm6
    
    movaps [.temp1],xmm7
    
    movaps xmm3,[.crs]
    movaps xmm4,[.cgs]
    movaps xmm5,[.cbs]
    
    mulps xmm0,xmm6
    mulps xmm1,xmm6
    mulps xmm2,xmm6
    
    mulps xmm3,xmm7
    mulps xmm4,xmm7
    mulps xmm5,xmm7
    
    addps xmm0,xmm3
    addps xmm1,xmm4
    addps xmm2,xmm5
    
    movaps [.crs],xmm0
    movaps [.cgs],xmm1
    movaps [.cbs],xmm2
    
             
    ;movss xmm7,[.t]
    ;shufps xmm7,xmm7,0
    ;mulps xmm1,xmm7
    ;movaps xmm2,[.tcolor1]
    ;addps xmm1,xmm2
    ;movaps [.tcolor1],xmm1
    align 16
    .salir_reflexion:

}



macro TRAE_IND_REFRAC vct
{
    xorps xmm0,xmm0
    movaps xmm1,[.objetos_detectados]
    cmpps xmm0,xmm1,4       ;los que no son cero
    movmskps ebx,xmm0
    
    mov ecx,ebx
    and ecx,01000b
    .if(ecx<>0)
        mov eax,[.objetos_detectados + Vector.w]
        mov [.ts + Vector.w],edi
        mov [.objetos_detectados + Vector.w],eax
        mov [.tipos + Vector.w],esi
    .endif
        
    mov ecx,ebx
    and ecx,100b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.v]
        mov [.ts + Vector.v],edi
        mov [.objetos_detectados + Vector.v],eax
        mov [.tipos + Vector.v],esi
    .endif
        
    mov ecx,ebx
    and ecx,10b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.u]
        mov [.ts + Vector.u],edi
        mov [.objetos_detectados + Vector.u],eax
        mov [.tipos + Vector.u],esi
    .endif
        
    mov ecx,ebx
    and ecx,01b
    .if(ecx<>0)
        mov edi,[.temp1 + Vector.t]
        mov [.ts + Vector.t],edi
        mov [.objetos_detectados + Vector.t],eax
        mov [.tipos + Vector.t],esi
    .endif
}


macro REFRACCION4
{
;Reflections and Refractions in Ray Tracing
;Bram de Greve (bram.degreve@gmail.com)
;10th October 2004
;const double n = n1 / n2;
;const double cosI = dot(normal, incident);
;const double sinT2 = n * n * (1.0 - cosI * cosI);
;if (sinT2 > 1.0)
;{
;return Vector::invalid;
;}
;return n * incident - (n + sqrt(1.0 - sinT2)) * normal;

local .invertir_normales,.continua_refraccion,.salir_refraccion

;hay que invertir las normales si se trata de una refraccion dentro del mismo objeto
;tambien, si se trata del mismo objeto, las refracciones nuevas se convierten en 1.0

;xorps xmm0,xmm0
;movaps xmm1,[.ptr_grupos]
;movaps xmm2,[.ptr_grupos_ant]

;cmpps xmm1,xmm2,0   ;pone en 1 los que son iguales
;movmskps ebx,xmm1
;IMPORTANTE: la comparacion anterior no es confiable,
;cmpps no funciona con numeros no enterosk


;call Counter
;mov esi,[.depth_treshold]
;.if(esi<8)
;	push esi
;	call DumpDW
;	call EndLine
;.endif


;DumpVectorH .ptr_grupos_ant
;DumpVectorH .ptr_grupos
;DumpVectorR .refracciones

;movss xmm6,[uno]
;shufps xmm6,xmm6,0
movaps xmm6,[unos]
movaps [.temp1],xmm6
mov esi,[muno]
mov edi,[uno]

;mov ecx,ebx
;and ecx,01000b
mov ebx,[.ptr_grupos_ant + Vector.w]
mov ecx,[.ptr_grupos + Vector.w]
.if(ecx=ebx)
    mov [.temp1 + Vector.w],esi
    mov [.refracciones + Vector.w],edi
.endif

mov ebx,[.ptr_grupos_ant + Vector.v]
mov ecx,[.ptr_grupos + Vector.v]
.if(ecx=ebx)
    mov [.temp1 + Vector.v],esi
    mov [.refracciones + Vector.v],edi
.endif

mov ebx,[.ptr_grupos_ant + Vector.u]
mov ecx,[.ptr_grupos + Vector.u]
.if(ecx=ebx)
    mov [.temp1 + Vector.u],esi
    mov [.refracciones + Vector.u],edi
.endif

mov ebx,[.ptr_grupos_ant + Vector.t]
mov ecx,[.ptr_grupos + Vector.t]
.if(ecx=ebx)
    mov [.temp1 + Vector.t],esi
    mov [.refracciones + Vector.t],edi
.endif

match =1,1 \{
xorps xmm0,xmm0
movaps xmm1,[.refracciones]
cmpps xmm0,xmm1,0   ;pone en 1 los que son iguales (iguales a cero)
movmskps ebx,xmm0
mov edi,[uno]
    
mov ecx,ebx
and ecx,01b
.if(ecx<>0)
    mov [.refracciones + Vector.t],edi
.endif
mov ecx,ebx
and ecx,10b
.if(ecx<>0)
    mov [.refracciones + Vector.u],edi
.endif
mov ecx,ebx
and ecx,100b
.if(ecx<>0)
    mov [.refracciones + Vector.v],edi
.endif
       
mov ecx,ebx
and ecx,01000b
.if(ecx<>0)
    mov [.refracciones + Vector.w],edi
.endif
\}

match =0,1 \{
xor eax,eax
cmpxchg [.refracciones + Vector.t],edi
xor eax,eax
cmpxchg [.refracciones + Vector.u],edi
xor eax,eax
cmpxchg [.refracciones + Vector.v],edi
xor eax,eax
cmpxchg [.refracciones + Vector.w],edi
\}
;DumpVectorR .refracciones_ant
;DumpVectorR .refracciones
;call Pausa
;call EndLine

movaps xmm6,[.temp1]

movaps xmm3,[.normalesu]
movaps xmm4,[.normalesv]
movaps xmm5,[.normalesw]
mulps xmm3,xmm6
mulps xmm4,xmm6
mulps xmm5,xmm6 ;tiene  -1 en los vectores a invertir
movaps [.fus],xmm3
movaps [.fvs],xmm4
movaps [.fws],xmm5

;const double n = n1 / n2;
;.bs -> n
movaps xmm6,[.refracciones_ant]
movaps xmm7,[.refracciones]
divps xmm6,xmm7
movaps [.bs],xmm6   ;n


;const double cosI = dot(normal, incident);
movaps xmm0,[.dirsu]
movaps xmm1,[.dirsv]
movaps xmm2,[.dirsw]
;movaps xmm3,[.fus]
;movaps xmm4,[.fvs]
;movaps xmm5,[.fws]       
mulps xmm0,xmm3
mulps xmm1,xmm4
mulps xmm2,xmm5
addps xmm0,xmm1
addps xmm0,xmm2
;cosI solo se utilisa para calcular sinT2 por lo tanto no se guardara en memoria
;const double sinT2 = n * n * (1.0 - cosI * cosI);
mulps xmm0,xmm0
;movss xmm1,[uno]
;shufps xmm1,xmm1,0
movaps xmm1,[unos]
movaps xmm7,xmm1    ;en xmm2 guardo 1's, para la comparacion y otras operaciones mas adelante
subps xmm1,xmm0
mulps xmm1,xmm6
mulps xmm1,xmm6
movaps [.b2s],xmm1  ;sinT2

;DumpVectorR .bs
;DumpVectorR .b2s

;if (sinT2 > 1.0)
;{
;return Vector::invalid;
;}
movaps xmm2,xmm1
cmpps xmm2,xmm7,2   ;pone en zeros todos los que son mayores a 1, y en 1 a los que son menores o iguales a 1 xD
andps xmm2,[.mask]
movmskps ebx,xmm2
test ebx,ebx
jz .salir_refraccion    ;si todos son mayores a 1 se sale


movaps [.mask],xmm2    ;guardo la mascara para eliminar los rayos que no sirven


;return n * incident - (n + sqrt(1.0 - sinT2)) * normal;
subps xmm7,xmm1
sqrtps xmm7,xmm7
addps xmm7,xmm6


movaps xmm0,[.dirsu]
movaps xmm1,[.dirsv]
movaps xmm2,[.dirsw]
mulps xmm0,xmm6
mulps xmm1,xmm6
mulps xmm2,xmm6

movaps xmm3,[.fus]
movaps xmm4,[.fvs]
movaps xmm5,[.fws]    
mulps xmm3,xmm7
mulps xmm4,xmm7
mulps xmm5,xmm7

subps xmm0,xmm3
subps xmm1,xmm4
subps xmm2,xmm5 ;direcciones listas, aunque tal vez deberia aplicarles la mascara, pero hmm

movaps xmm3,xmm0
movaps xmm4,xmm1
movaps xmm5,xmm2
mulps xmm3,xmm3
mulps xmm4,xmm4
mulps xmm5,xmm5
addps xmm3,xmm4
addps xmm3,xmm5
sqrtps xmm3,xmm3
divps xmm0,xmm3
divps xmm1,xmm3
divps xmm2,xmm3 ;direccion normalizada

movaps xmm3,[.orgsu]
movaps xmm4,[.orgsv]
movaps xmm5,[.orgsw]


movaps [.fus],xmm0
movaps [.fvs],xmm1
movaps [.fws],xmm2



mov esi,[.depth_treshold]
test esi,esi
jz .salir_refraccion
dec esi
movaps xmm6,[.ptr_grupos]
movaps xmm7,[.refracciones]
call TraceRay4


;movss xmm7,[uno]
;shufps xmm7,xmm7,0
movaps xmm7,[unos]
movaps xmm6,[.cas]
subps xmm7,xmm6

movaps xmm3,[.crs]
movaps xmm4,[.cgs]
movaps xmm5,[.cbs]
mulps xmm3,xmm6
mulps xmm4,xmm6
mulps xmm5,xmm6
mulps xmm0,xmm7
mulps xmm1,xmm7
mulps xmm2,xmm7

addps xmm0,xmm3
addps xmm1,xmm4
addps xmm2,xmm5

movaps [.crs],xmm0
movaps [.cgs],xmm1
movaps [.cbs],xmm2

align 16
.salir_refraccion:

}


macro CALCULA_Ys
{
;	float cy=(float)((y + 0.5)/d_height);
        CVTSI2SS xmm1,[.y]
        shufps xmm1,xmm1,0
        movss xmm3,[.delta_y]
        shufps xmm3,xmm3,0
        mulps xmm1,xmm3
        movss xmm3,[.delta_h]
        shufps xmm3,xmm3,0
        addps xmm1,xmm3
        ;movaps [.ys],xmm1
        
        ;multiplicar vyus,vyvs,vyws por xmm1
        movaps xmm3,[.vyus]
        movaps xmm4,[.vyvs]
        movaps xmm5,[.vyws]
        
        mulps xmm3,xmm1
        mulps xmm4,xmm1
        mulps xmm5,xmm1
        
        movaps [.tvyus],xmm3
        movaps [.tvyvs],xmm4
        movaps [.tvyws],xmm5
        
        ;	(*vy)*=cy;
        ;pcy debe ser multiplicado por vy,
        ;pero vy siendo u,v,z, debo esperar antes de hacer la multiplicacion
        ;hasta que ya tenga las x's y las z's
}

macro CALCULA_Xs
{
        local .LoopX
        ;xor esi,esi
        mov [.x],0
        align 16
        .LoopX:
    
            ;calcular u,v,w del nuevo rayo
            ;restando el ojo
            ;	float cx=(float)((x + 0.5)/d_width);
            ;voy a calcular cx para cuatro puntos consecutivos en el eje x
            
            CVTSI2SS xmm4,[.x]
            shufps xmm4,xmm4,0
            addps xmm4,[v0123]  ;calculo los nuevos puntos x,x+1,x+2,x+3

            ;x debe ser menor que width
            CVTSI2SS xmm3,[_width]
            subss xmm3,[uno]
            shufps xmm3,xmm3,0
            minps xmm4,xmm3 ;aki ya stan los menores o iguales a _width-1
            
            
            movss xmm3,[.delta_x] ;multiplicar .delta_x a cada una de las x's
            shufps xmm3,xmm3,0
            mulps xmm4,xmm3
            
            
            ;sumar a cada x, delta_w
            movss xmm3,[.delta_w]
            shufps xmm3,xmm3,0
            addps xmm4,xmm3
            
            
            
            ;	(*vx)*=cx
            ;lo mismo que con vy
            ;multiplicar vxus,vxvs,vxws por xmm0
            movaps xmm0,[.vxus]
            movaps xmm1,[.vxvs]
            movaps xmm2,[.vxws]
            
            mulps xmm0,xmm4     ;u
            mulps xmm1,xmm4     ;v
            mulps xmm2,xmm4     ;w
            
            ;movaps [.tvxus],xmm3
            ;movaps [.tvxvs],xmm4
            ;movaps [.tvxws],xmm5
            
    
           ;	Vector *sal= (*vx)+vy;
           ;
           ;	Vector *sal2=*lt + sal;
           
           ;calculo tvx + tvy + camara.lefttop
            addps xmm0,[.tvyus]
            addps xmm1,[.tvyvs]
            addps xmm2,[.tvyws]
            
            
            movss xmm3,[camara.lefttop.u]
            movss xmm4,[camara.lefttop.v]
            movss xmm5,[camara.lefttop.w]    
            shufps xmm3,xmm3,0
            shufps xmm4,xmm4,0
            shufps xmm5,xmm5,0
            
            addps xmm0,xmm3 ;sal2u
            addps xmm1,xmm4 ;sal2v
            addps xmm2,xmm5 ;sal2w

            ;los 4 nuevos puntos ya estan en xmm1,2,3
            ;ahora hay que convertirlos en rayos, restandoles el ojo
            movss xmm3,[camara.eye.u]
            movss xmm4,[camara.eye.v]
            movss xmm5,[camara.eye.w]    
            shufps xmm3,xmm3,0
            shufps xmm4,xmm4,0
            shufps xmm5,xmm5,0
            
            subps xmm0,xmm3 ;rayo u
            subps xmm1,xmm4 ;rayo v
            subps xmm2,xmm5 ;rayo w
            
            ;normalizar los rayos
            movaps xmm3,xmm0
            movaps xmm4,xmm1
            movaps xmm5,xmm2
            
            mulps xmm3,xmm3 ;los cuadrados para cada eje
            mulps xmm4,xmm4
            mulps xmm5,xmm5
            
            ;sumatorias de los cuadrados para cada eje
            addps xmm3,xmm4
            addps xmm3,xmm5
            
            sqrtps xmm3,xmm3    ;raices cuadradas para cada vector
            
            divps xmm0,xmm3 ;vectores normalizados
            divps xmm1,xmm3 ;direcciones
            divps xmm2,xmm3
            
            
            
            movss xmm3,[camara.eye.u]
            movss xmm4,[camara.eye.v]
            movss xmm5,[camara.eye.w]    
            shufps xmm3,xmm3,0
            shufps xmm4,xmm4,0 ;origenes
            shufps xmm5,xmm5,0
            

            ;esi -> depth_treshold
            ;edi -> puntero a grupos_ant t,u,v,w corresponden a vector 1,2,3,4
            ;xmm0,1,2 direcciones
            ;xmm3,4,5 origenes
            

            
            mov esi,10
            xorps xmm6,xmm6 ;grupos anteriores en cero
            movaps xmm7,[unos]

            call TraceRay4
            ;stdcall TraceRay,[eax + Vector.u],[eax + Vector.v],[eax + Vector.w],[camara.eye.u],[camara.eye.v],[camara.eye.w],1

            ;el color esta en xmm1
            movaps xmm7,[unos]
            mov ebx,255
            CVTSI2SS xmm6,ebx
            shufps xmm6,xmm6,0  ;255's
            
            ;aki hay que limitar que el color en real4 no se pase de 1.0
            minps xmm0,xmm7
            mulps xmm0,xmm6
            
            minps xmm1,xmm7
            mulps xmm1,xmm6
            
            ;;;;Pintar un poco de azul para determinar que este es el que lo ejecuta;;;;;;;;;
            match =yes,HUE \{
            movss xmm5,[p25]
            shufps xmm5,xmm5,0
            addps xmm2,xmm5
            \}
            ;;;;Termina pintada;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
            
            minps xmm2,xmm7
            mulps xmm2,xmm6 ;los colores en punto flotante escala 256
            
            CVTPS2DQ xmm0,xmm0
            CVTPS2DQ xmm1,xmm1
            CVTPS2DQ xmm2,xmm2

           
            movd xmm3,ebx
            ;pxor xmm3,xmm3
            shufps xmm3,xmm3,0
            pslld xmm3,8
            por xmm3,xmm0
            pslld xmm3,8
            por xmm3,xmm1
            pslld xmm3,8
            por xmm3,xmm2
            

            mov ebx,[.pix]
            match =exe,TARGET \{
            movaps [ebx],xmm3
            \}
            match =dll,TARGET \{
            movaps [ebx],xmm3 ;solo por esta vez, se espera que el buffer de salida este alineado a 16 bytes
            ;movdqu [ebx],xmm3
            \}
            add ebx,16
            mov [.pix],ebx
            
            add [.x],4
            mov eax,[.x]
            mov edx,[_width]
            cmp eax,edx
            jb .LoopX
}
