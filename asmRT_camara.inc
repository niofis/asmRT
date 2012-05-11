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

;RotarCamara PROTO :real4,:dword
;MoverCamara PROTO :real4,:real4,:real4

RotarCamara: ;rads->edi,axis->esi ;axis-> 1=x 2=y 4=z
   push ebp
   mov ebp,esp
   VARS_ALIGN 11*4 + VectorSize*1
    
    label .vtc           at esp 

    dwordsize = 4
    offset=VectorSize*1

    label .x                       dword at esp+offset 
    label .y                      dword at esp+offset + dwordsize
    label .z                      dword at esp+offset + dwordsize*2
    label .co                       dword at esp+offset + dwordsize*3
    label .se                      dword at esp+offset + dwordsize*4
    label .t                       dword at esp+offset + dwordsize*5
    label .sx                      dword at esp+offset + dwordsize*6
    label .sy                 dword at esp+offset + dwordsize*7
    label .sz                 dword at esp+offset + dwordsize*8
    label .rads                 dword at esp+offset + dwordsize*9
    label .axis                 dword at esp+offset + dwordsize*10
    

    
    ;locals
    ; x  dd 0.0
    ; y  dd 0.0
    ; z  dd 0.0
    ; co dd 0.0
    ; se dd 0.0
    ; t  dd 0.0
    ; sx dd 0.0
    ; sy dd 0.0
    ; sz dd 0.0
    ;vtc Vector 0.0,0.0,0.0,0.0
    ;endl

    ;int Camera::Rotate(float rads, int axis) 
    ;{;
    ;		//http://www.gamedev.net/reference/articles/article1095.asp;
    ;	//Adaptado eje Y, Eje de rotacion (quaternion)
    ;
    		
    ;	//Rotacion: http://www.gamasutra.com/features/19980703/quaternions_01.htm
    ;	//v´ = q v q-1 (where v = [0, v]) 
    	
    ;	//Multiplicacion de quaterniones: http://www.gamedev.net/reference/articles/article1095.asp;
    ;	//w = w1w2 - x1x2 - y1y2 - z1z2
    ;	//x = w1x2 + x1w2 + y1z2 - z1y2
    ;	//y = w1y2 + y1w2 + z1x2 - x1z2
    ;	//z = w1z2 + z1w2 + x1y2 - y1x2 
    ;	
    ;	//http://www.gamedev.net/reference/articles/article1199.asp
    ;	//http://mathworld.wolfram.com/RodriguesRotationFormula.html
    ;	//float rads=0.017452 * -2.5;

    mov [.rads],edi
    mov [.axis],esi
    
    .if ([.axis]=1)   ;uso estas macros solo por flojo
        V_SUB .vtc,camara.righttop,camara.lefttop
        V_NORMALIZE .vtc
    .elseif ([.axis]=2)
        V_SUB .vtc,camara.lefttop,camara.leftbottom
        V_NORMALIZE .vtc
    .endif


    movss xmm0,[.vtc + Vector.u]
    movss [.x],xmm0

    movss xmm0,[.vtc + Vector.v]
    movss [.y],xmm0

    movss xmm0,[.vtc + Vector.w]
    movss [.z],xmm0

    ;	float c = cos(rads);
    fld [.rads]
    fcos
    fstp [.co]
    ;	float s = sin(rads);
    fld [.rads]
    fsin
    fstp [.se]

    ;	float t = 1-c;
    movss xmm0,[uno]
    subss xmm0,[.co]
    movss [.t],xmm0    

    ;	sx=eye->u;
    ;	sy=eye->v;
    ;	sz=eye->w;
    movss xmm0,[camara.eye.u]
    movss [.sx],xmm0
    movss xmm0,[camara.eye.v]
    movss [.sy],xmm0
    movss xmm0,[camara.eye.w]
    movss [.sz],xmm0

    ECU_ROTACION_U .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.eye.u],xmm0
    ;invoke DumpReal4,[camara.eye.u]

    ECU_ROTACION_V .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.eye.v],xmm0
    ;invoke DumpReal4,[camara.eye.v]

    ECU_ROTACION_W .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.eye.w],xmm0 
    ;invoke DumpReal4,[camara.eye.w]

    ;	sx=lt->u;
    ;	sy=lt->v;
    ;	sz=lt->w;
    movss xmm0,[camara.lefttop.u]
    movss [.sx],xmm0
    movss xmm0,[camara.lefttop.v]
    movss [.sy],xmm0
    movss xmm0,[camara.lefttop.w]
    movss [.sz],xmm0

    ECU_ROTACION_U .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.lefttop.u],xmm0

    ECU_ROTACION_V .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.lefttop.v],xmm0

    ECU_ROTACION_W .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.lefttop.w],xmm0

    ;	sx=rt->u;
    ;	sy=rt->v;
    ;	sz=rt->w;
    movss xmm0,[camara.righttop.u]
    movss [.sx],xmm0
    movss xmm0,[camara.righttop.v]
    movss [.sy],xmm0
    movss xmm0,[camara.righttop.w]
    movss [.sz],xmm0

    ECU_ROTACION_U .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.righttop.u],xmm0

    ECU_ROTACION_V .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.righttop.v],xmm0

    ECU_ROTACION_W .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.righttop.w],xmm0

    ;	sx=lb->u;
    ;	sy=lb->v;
    ;	sz=lb->w;
    movss xmm0,[camara.leftbottom.u]
    movss [.sx],xmm0
    movss xmm0,[camara.leftbottom.v]
    movss [.sy],xmm0
    movss xmm0,[camara.leftbottom.w]
    movss [.sz],xmm0

    ECU_ROTACION_U .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.leftbottom.u],xmm0

    ECU_ROTACION_V .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.leftbottom.v],xmm0
 
    ECU_ROTACION_W .t,.x,.y,.z,.se,.co,.sx,.sy,.sz
    movss [camara.leftbottom.w],xmm0
leave
retn


MoverCamara: ;dex->edi,dey->esi,dez->edx
dwordsize = 4 
    label .vtc_x           at esp 
    label .vtc_y          at esp+VectorSize
    label .vtc_z         at esp+VectorSize*2


    offset=VectorSize*3

    label .dex                      dword at esp+offset 
    label .dey                      dword at esp+offset + dwordsize
    label .dez                      dword at esp+offset + dwordsize*2
    ;locals
    ;vtc_x Vector 0.0,0.0,0.0,0.0
    ;vtc_y Vector 0.0,0.0,0.0,0.0
    ;vtc_z Vector 0.0,0.0,0.0,0.0
    ;endl

    push ebp
    mov ebp,esp
    VARS_ALIGN dwordsize*3 + VectorSize*3

  mov [.dex],edi
  mov [.dey],esi
  mov [.dez],edx
  

    V_SUB .vtc_x,camara.righttop,camara.lefttop
    V_NORMALIZE .vtc_x

    V_SUB .vtc_y,camara.lefttop,camara.leftbottom
    V_NORMALIZE .vtc_y

    V_CROSS .vtc_z + Vector,.vtc_x + Vector,.vtc_y + Vector
    V_NORMALIZE .vtc_z

    movss xmm0,[.dex]
    movss xmm1,[.vtc_x + Vector.u]
    mulss xmm1,xmm0
    movss [.vtc_x + Vector.u],xmm1
    movss xmm1,[.vtc_x + Vector.v]
    mulss xmm1,xmm0
    movss [.vtc_x + Vector.v],xmm1
    movss xmm1,[.vtc_x + Vector.w]
    mulss xmm1,xmm0
    movss [.vtc_x + Vector.w],xmm1

    movss xmm0,[.dey]
    movss xmm1,[.vtc_y + Vector.u]
    mulss xmm1,xmm0
    movss [.vtc_y + Vector.u],xmm1
    movss xmm1,[.vtc_y + Vector.v]
    mulss xmm1,xmm0
    movss [.vtc_y + Vector.v],xmm1
    movss xmm1,[.vtc_y + Vector.w]
    mulss xmm1,xmm0
    movss [.vtc_y + Vector.w],xmm1

    movss xmm0,[.dez]
    movss xmm1,[.vtc_z + Vector.u]
    mulss xmm1,xmm0
    movss [.vtc_z + Vector.u],xmm1
    movss xmm1,[.vtc_z + Vector.v]
    mulss xmm1,xmm0
    movss [.vtc_z + Vector.v],xmm1
    movss xmm1,[.vtc_z + Vector.w]
    mulss xmm1,xmm0
    movss [.vtc_z + Vector.w],xmm1

    movaps xmm1,[.vtc_x]
    movaps xmm2,[.vtc_y]
    movaps xmm3,[.vtc_z]
    
    movaps xmm0,[camara.eye]
    addps xmm0,xmm1
    addps xmm0,xmm2
    addps xmm0,xmm3
    movaps [camara.eye],xmm0

    movaps xmm0,[camara.lefttop]
    addps xmm0,xmm1
    addps xmm0,xmm2
    addps xmm0,xmm3
    movaps [camara.lefttop],xmm0

    movaps xmm0,[camara.righttop]
    addps xmm0,xmm1
    addps xmm0,xmm2
    addps xmm0,xmm3
    movaps [camara.righttop],xmm0

    movaps xmm0,[camara.leftbottom]
    addps xmm0,xmm1
    addps xmm0,xmm2
    addps xmm0,xmm3
    movaps [camara.leftbottom],xmm0
leave
retn