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

;Macros
; -------------------------------------------------------------------------
macro VARS_ALIGN size
{
    sub esp,size

    mov eax,esp
    and eax,0Fh
    sub esp,eax       ;Alineación =)
}
macro V_CROSS v_sal,v_1,v_2
{
    movss xmm0,[v_1#.v#]
    mulss xmm0,[v_2#.w#]
    movss xmm1,[v_1#.w#]
    mulss xmm1,[v_2#.v#]
    subps xmm0,xmm1
    movss [v_sal#.u#],xmm0

    movss xmm0,[v_1#.w#]
    mulss xmm0,[v_2#.u#]
    movss xmm1,[v_1#.u#]
    mulss xmm1,[v_2#.w#]
    subps xmm0,xmm1
    movss [v_sal#.v#],xmm0

    movss xmm0,[v_1#.u#]
    mulss xmm0,[v_2#.v#]
    movss xmm1,[v_1#.v#]
    mulss xmm1,[v_2#.u#]
    subps xmm0,xmm1
    movss [v_sal#.w#],xmm0    

}

macro V_DOT v_1,v_2     ;regresa el producto punto en xmm0
{
    movss xmm0,[v_1#.u#]
    mulss xmm0,[v_2#.u#]
    movss xmm1,[v_1#.v#]
    mulss xmm1,[v_2#.v#]
    movss xmm2,[v_1#.w#]
    mulss xmm2,[v_2#.w#]
    addps xmm0,xmm1
    addps xmm0,xmm2
}

macro V_SUB v_sal,v_1,v_2
{
    movaps xmm0,[v_1]
    movaps xmm1,[v_2]
    subps xmm0,xmm1
    movaps [v_sal],xmm0
}

macro V_NORM v_1        ;regresa la norma en xmm4
{
    movaps xmm0,[v_1]
    mulps xmm0,xmm0
    movaps xmm2,xmm0
    shufps xmm2,xmm2,1
    movaps xmm3,xmm0
    shufps xmm3,xmm3,2
    movaps xmm4,xmm0
    shufps xmm4,xmm4,3
    
    ;movss xmm0,[v_1#.u#]
    ;mulss xmm0,xmm0
    ;movss xmm1,[v_1#.v#]
    ;mulss xmm1,xmm1
    ;movss xmm2,[v_1#.w#]
    ;mulss xmm2,xmm2

    movaps xmm5,xmm2
    addps xmm5,xmm3
    addps xmm5,xmm4

    sqrtps xmm4,xmm5
}

macro V_NORMALIZE v_1
{
    V_NORM v_1  ;regresa 1 / normal
    shufps xmm4,xmm4,0
    movaps xmm0,[v_1]
    divps xmm0,xmm4
    movaps [v_1],xmm0
}

macro V_NORMALIZE_intel v_1
{
local .zero,.store
lea eax,[v_1]
xorps xmm2, xmm2
movss xmm4,[uno]
shufps xmm4,xmm4,0
movaps xmm1, [eax] ;// loads the (x, y, z) of input vector plus x of next vector
movaps xmm7, xmm1 ;// save a copy of data from memory (to restore the unnormalized value)
movaps xmm5, [uvw_mask] ;// mask to select (x, y, z) values from an xmm register to normalize
andps xmm1, xmm5 ;// mask 1st 3 elements
movaps xmm6, xmm1 ;// save a copy of (x, y, z) to compute normalized vector later
mulps xmm1,xmm1 ;// 0, z*z, y*y, x*x
pshufd xmm3, xmm1, 00011011b ;// x*x, y*y, z*z, 0
addps xmm1, xmm3 ;// x*x, z*z+y*y, z*z+y*y, x*x
pshufd xmm3, xmm1, 01000001b ;// z*z+y*y, x*x, x*x, z*z+y*y
addps xmm1, xmm3 ;// x*x+y*y+z*z, x*x+y*y+z*z, x*x+y*y+z*z, x*x+y*y+z*z
comisd xmm1, xmm2 ;// compare size to 0
jz .zero
movaps xmm3, xmm4;// preloaded unitary vector (1.0, 1.0, 1.0, 1.0)
sqrtps xmm1, xmm1
divps xmm3, xmm1
jmp .store
.zero:
movaps xmm3, xmm2
.store:
mulps xmm3, xmm6 ;//normalize the vector in the lower 3 elements
andnps xmm5, xmm7 ;// mask off the lower 3 elements to keep the un-normalized value
orps xmm3, xmm5 ;// order the un-normalized component after the normalized vector
movaps [eax], xmm3 ;// writes normalized x, y, z; followed by unmodified value
}

macro GEN_NULO obj3d_1
{
    xorps xmm0,xmm0
    movaps [obj3d_1#.v1#],xmm0
    movaps [obj3d_1#.v2#],xmm0
    movaps [obj3d_1#.v3#],xmm0
    xor edx,edx
    mov [obj3d_1#.tipo#],edx
    mov [obj3d_1#.grupo_id#],edx
    mov [obj3d_1#.radio#],edx
    mov [obj3d_1#.padding#],edx
    add eax,Objeto3DSize

}

macro GEN_TRIANGULO obj3d_1, v1_u,v1_v,v1_w, v2_u,v2_v,v2_w, v3_u,v3_v,v3_w,  grupo_id ;obj_col_r,obj_col_g,obj_col_b
{
    mov [obj3d_1#.tipo#],OBJ_TRIANGULO
    ;mov edx,grupo_id
    ;imul edx,GrupoSize
    ;add edx,[p_grupos]
    mov [obj3d_1#.grupo_id#], grupo_id
    
    movss xmm0,[v1_u]
    movss [obj3d_1#.v1.u#],xmm0
    movss xmm0,[v1_v]
    movss [obj3d_1#.v1.v#],xmm0
    movss xmm0,[v1_w]
    movss [obj3d_1#.v1.w#],xmm0

    movss xmm0,[v2_u]
    movss [obj3d_1#.v2.u#],xmm0
    movss xmm0,[v2_v]
    movss [obj3d_1#.v2.v#],xmm0
    movss xmm0,[v2_w]
    movss [obj3d_1#.v2.w#],xmm0
      
    V_SUB obj3d_1#.v2#,obj3d_1#.v2#,obj3d_1#.v1# ;edge1 (v2-v1)
    
    movss xmm0,[v3_u]
    movss [obj3d_1#.v3.u#],xmm0
    movss xmm0,[v3_v]
    movss [obj3d_1#.v3.v#],xmm0
    movss xmm0,[v3_w]
    movss [obj3d_1#.v3.w]#,xmm0

    V_SUB obj3d_1#.v3#,obj3d_1#.v3#,obj3d_1#.v1#    ;edge2 (v3-v1)
    
    add eax,Objeto3DSize

}

macro GEN_ESFERA obj3d_1, v1_u,v1_v,v1_w, obj3d_radio, grupo_id ;obj_col_r,obj_col_g,obj_col_b 
{
    movss xmm0,[v1_u]
    movss [obj3d_1#.v1.u#],xmm0
    movss xmm0,[v1_v]
    movss [obj3d_1#.v1.v#],xmm0
    movss xmm0,[v1_w]
    movss [obj3d_1#.v1.w#],xmm0
    movss xmm0,[obj3d_radio]
    movss [obj3d_1#.radio#],xmm0
    ;mov edx,grupo_id
    ;imul edx,GrupoSize
    ;add edx,[p_grupos]
    mov [obj3d_1#.grupo_id#],grupo_id
    movss xmm0,[zero]
    movss [obj3d_1#.v2.u#],xmm0
    movss [obj3d_1#.v2.w#],xmm0
    movss [obj3d_1#.v3.u#],xmm0
    movss [obj3d_1#.v3.v#],xmm0
    movss xmm0,[uno]
    movss [obj3d_1#.v2.v#],xmm0
    movss xmm0,[muno]
    movss [obj3d_1#.v3.u#],xmm0

    mov [obj3d_1#.tipo#],OBJ_ESFERA
    add eax,Objeto3DSize
}

macro GEN_SKYDOME obj3d_1, v1_u,v1_v,v1_w, obj3d_radio, grupo_ptr ;obj_col_r,obj_col_g,obj_col_b 
{
    GEN_ESFERA obj3d_1, v1_u,v1_v,v1_w, obj3d_radio, grupo_ptr 
    sub eax,Objeto3DSize
    mov [obj3d_1#.tipo#],OBJ_SKYDOME
    add eax,Objeto3DSize
}

macro GEN_MATERIAL material_1,id, reflexion, refraccion,specular, col_a, col_r, col_g, col_b 
{
    mov edx,id
    mov [material_1#.id#],edx
    movss xmm0,[refraccion]
    movss [material_1#.refraccion#],xmm0
    movss xmm0,[reflexion]
    movss [material_1#.reflexion#],xmm0
    
    movss xmm0,[specular]
    movss [material_1#.specular#],xmm0
    movss xmm0,[col_a]
    movss [material_1#.color.a#],xmm0
    movss xmm0,[col_r]
    movss [material_1#.color.r#],xmm0
    movss xmm0,[col_g]
    movss [material_1#.color.g#],xmm0
    movss xmm0,[col_b]
    movss [material_1#.color.b#],xmm0
    
    xor edx,edx
    mov [material_1#.textura#],edx
    mov [material_1#.txt_width#],edx
    mov [material_1#.txt_height#],edx
    
    add eax,MaterialSize
}

macro GEN_GRUPO grupo_1,id,material_id
{
    mov edx,id
    mov [grupo_1#.id#],edx
    ;mov edx,material_id
    ;imul edx,MaterialSize
    ;add edx,[p_materiales]
    mov [grupo_1#.material_id#],material_id
    
    add eax,GrupoSize
}


macro ECU_ROTACION_U t,x,y,z,se,co,sx,sy,sz
{
    ;u = (((t*x*x)+c)*sx) + (((t*x*y) + (s*z))*sy) + (((t*x*z)-(s*y))*sz); 
    movss xmm0,[t]
    mulss xmm0,[x]
    mulss xmm0,[x]
    addss xmm0,[co]
    mulss xmm0,[sx]
    movaps xmm5,xmm0 ;(((t*x*x)+c)*sx)
    movss xmm0,[t]
    mulss xmm0,[x]
    mulss xmm0,[y]    ;(t*x*y)
    movss xmm1,[se]
    mulss xmm1,[z]    ;(s*z)
    addps xmm0,xmm1
    mulss xmm0,[sy]
    movaps xmm6,xmm0 ;(((t*x*y) + (s*z))*sy)
    movss xmm0,[t]
    mulss xmm0,[x]
    mulss xmm0,[z]    ;(t*x*z)
    movss xmm1,[se]
    mulss xmm1,[y]    ;(s*y)
    subps xmm0,xmm1
    mulss xmm0,[sz]
    movaps xmm7,xmm0 ;(((t*x*z)-(s*y))*sz)
    addps xmm5,xmm6
    addps xmm5,xmm7
    movaps xmm0,xmm5
}

macro ECU_ROTACION_V t,x,y,z,se,co,sx,sy,sz
{
    ;v = (((t*x*y)-(s*z))*sx) + (((t*y*y)+c)*sy) + (((t*y*z)+(x*s))*sz);
    movss xmm0,[t]
    mulss xmm0,[x]
    mulss xmm0,[y]    ;(t*x*y)
    movss xmm1,[se]
    mulss xmm1,[z]    ;(s*z)
    subps xmm0,xmm1 ;(t*x*y)-(s*z)
    mulss xmm0,[sx]
    movaps xmm5,xmm0 ;(((t*x*y)-(s*z))*sx)
    movss xmm0,[t]
    mulss xmm0,[y]
    mulss xmm0,[y]
    addss xmm0,[co]
    mulss xmm0,[sy]
    movaps xmm6,xmm0 ;(((t*y*y)+c)*sy)
    movss xmm0,[t]
    mulss xmm0,[y]
    mulss xmm0,[z]    ;(t*y*z)
    movss xmm1,[se]
    mulss xmm1,[x]    ;(x*s)
    addps xmm0,xmm1 ;(t*y*z)+(x*s)
    mulss xmm0,[sz]
    movaps xmm7,xmm0 ;(((t*y*z)+(x*s))*sz)
    addps xmm5,xmm6
    addps xmm5,xmm7
    movaps xmm0,xmm5
}

macro ECU_ROTACION_W t,x,y,z,se,co,sx,sy,sz
{
    ;w = (((t*x*z)+(s*y))*sx) + (((t*y*z)-(s*x))*sy) + (((t*z*z)+c)*sz);
    movss xmm0,[t]
    mulss xmm0,[x]
    mulss xmm0,[z]    ;(t*x*z)
    movss xmm1,[se]
    mulss xmm1,[y]    ;(s*y)
    addps xmm0,xmm1 ;(t*x*y)+(s*y)
    mulss xmm0,[sx]
    movaps xmm5,xmm0 ;(((t*x*z)+(s*y))*sx)
    movss xmm0,[t]
    mulss xmm0,[y]
    mulss xmm0,[z]    ;(t*y*z)
    movss xmm1,[se]
    mulss xmm1,[x]    ;(x*s)
    subps xmm0,xmm1 ;(t*y*z)-(x*s)
    mulss xmm0,[sy]
    movaps xmm6,xmm0 ;(((t*z*z)-(s*x))*sy) 
    movss xmm0,[t]
    mulss xmm0,[z]
    mulss xmm0,[z]
    addss xmm0,[co]
    mulss xmm0,[sz]
    movaps xmm7,xmm0 ;(((t*z*z)+c)*sz)
    addps xmm5,xmm6
    addps xmm5,xmm7
    movaps xmm0,xmm5
}

macro GRUPO_PTR id
{
    ;mov eax,id
    imul id,GrupoSize
    add id,[p_grupos]
}

macro MATERIAL_PTR id
{
    ;mov eax,id
    imul id,MaterialSize
    add id,[p_materiales]
}

macro LUZ_PTR id
{
    ;mov eax,id
    imul id,LuzSize
    add id,[p_luces]
}