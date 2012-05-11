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

macro  DELTAS_CAMARA
{
    ; 	//Utilizando la fórmula presentada en el artículo 
    ;	//State of the Art in Interactive Ray Tracing
    ;	//Para convertir coordenadas de pantalla a coordenadas u,v,w
    ;	//float us=p_l + (p_r - p_l)*(x + 0.5)/d_width;
    ;	//float vs=p_b + (p_t - p_b)*(y + 0.5)/d_height;
    ;	//return new Vector(us,vs,p_w);
    ;	
    ;	float cx=(float)((x + 0.5)/d_width);
    ;	float cy=(float)((y + 0.5)/d_height);
    ;

    ;	(*vx)*=cx;
    ;	(*vy)*=cy;
    ;
    ;	Vector *sal= (*vx)+vy;
    ;	
    ;	Vector *sal2=*lt + sal;
    ;	
    ;	Vector *sal3=*sal2-this->eye;
    ;	//Test(x);
    ;	//Print(sal2);
    ;	delete sal;
    ;	delete vx;
    ;	delete vy;
    ;	delete sal2;
    ;	//Print(sal3);
    ;	sal3->Normalize();
    ;	return sal3;
    
    ;	Vector *vx=*rt - lt;
    ;oword ptr
    movaps xmm0,[camara.righttop]
    subps xmm0,[camara.lefttop]
    movaps [.vx],xmm0
   
    
        
    ;	Vector *vy=*lb - lt;
    movaps xmm0,[camara.leftbottom]
    subps xmm0,[camara.lefttop]
    movaps [.vy],xmm0
    
    
    ;(x /d_width)->delta_x   (0.5/d_width)->delta_w
    mov eax,[_width]
    mov [.x],eax
    CVTSI2SS xmm0,[.x]
    rcpps xmm1,xmm0
    movss [.delta_x],xmm1
    mulss xmm1,[mitad]
    movss [.delta_w],xmm1
    
    
    ;(y /d_height)->delta_y   (0.5/d_height)->delta_h
    mov eax,[_height]
    mov [.y],eax
    CVTSI2SS xmm0,[.y]
    rcpps xmm1,xmm0
    movss [.delta_y],xmm1
    mulss xmm1,[mitad]
    movss [.delta_h],xmm1
}

macro PUNTO_CAMARA punto_x,punto_y
{
CVTSI2SS xmm0,[punto_y]
    mulss xmm0,[.delta_y]
    addss xmm0,[.delta_h]
    shufps xmm0,xmm0,0
    movaps [.pcy],xmm0
    ;movss [pcy.u],xmm0
    ;movss [pcy.v],xmm0
    ;movss [pcy.w],xmm0

   
    
    ;	(*vy)*=cy;
    movaps xmm0,[.vy]
    movaps xmm1,[.pcy]
    mulps xmm0,xmm1
    movaps [.tvy],xmm0
    
    ;calcular u,v,w del nuevo rayo
    ;restando el ojo
    ;	float cx=(float)((x + 0.5)/d_width);
    CVTSI2SS xmm0,[punto_x]
    mulss xmm0,[.delta_x]
    addss xmm0,[.delta_w]
    shufps xmm0,xmm0,0
    movaps [.pcx],xmm0
    ;movss [pcx.u],xmm0
    ;movss [pcx.v],xmm0
    ;movss [pcx.w],xmm0

    ;	(*vx)*=cx
    movaps xmm0,[.vx]
    movaps xmm1,[.pcx]
    mulps xmm0,xmm1
    movaps [.tvx],xmm0
    
    ;	Vector *sal= (*vx)+vy;
    ;
    ;	Vector *sal2=*lt + sal;
    movaps xmm0, [.tvx]
    movaps xmm1, [.tvy]
    movaps xmm3, [camara.lefttop]
    addps xmm0,xmm1
    addps xmm0,xmm3
}
macro REFLEXION     ;tv->normal(vert1), vdir->V(vert0)
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
    V_DOT .vdir + Vector,.tv + Vector
    mulss xmm0,[dos]
    ;movss [t],xmm0
    shufps xmm0,xmm0,0

    movaps xmm1,[.vdir]
    movaps xmm2,[.tv]
    mulps xmm2,xmm0
    subps xmm1,xmm2
    movaps [.dlv],xmm1
    
    ;origen del rayo nuevo: tov
    ;lea eax,[dlv]
    ;lea ebx,[tov]
   
    mov ebx,[.ptr_grupo]               ;grupo_ant
    mov ecx,[.ptr_material]               ;material_ant
    mov edx,[.depth_treshold]
    dec edx               ;depth_treshold
    lea esi,[.tov]    ;origen
    lea edi,[.dlv]          ;direccion
    call TraceRay
    
    ;stdcall TraceRay,[eax + Vector.u],[eax + Vector.v],[eax + Vector.w],[ebx + Vector.u],[ebx + Vector.v],[ebx + Vector.w],1
    
    
            
    movss xmm7,[.t]
    shufps xmm7,xmm7,0
    mulps xmm1,xmm7
    movaps xmm2,[.tcolor1]
    addps xmm1,xmm2
    movaps [.tcolor1],xmm1

}

macro REFRACCION
{
;if(material->transparency_index>0.0f)
;{
;	
;	vert0=(float*)(&(ray->direction->u)); //V
;	vert1=(float*)(&(pt->normal->u));	//N
	
;	if(previous_group!=-1)
;		ta=_scene->materials[_scene->groups[previous_group]->material_id]->refraction_index;//n1
;	else
;		ta=1.0f;
;	
;	if(pt->group_id!=NULL)
;		t=_scene->materials[closest_object->material_id]->refraction_index;
;	else
;		t=1.0f;
	
;	if(previous_group==pt->group_id)
;	{
;		vert1[0]*=-1;
;		vert1[1]*=-1;
;		vert1[2]*=-1;
;	}
;	else
;	{
;		ta=1.0f;
;	}
;			
;	//t=material->refraction_index;//n2
;
;    v = ta / t;//n
;	u = V_DOT(vert0,vert1);//cosI

;	t0 = (float)( v * v * (1.0 - u * u)); //sinT2
;
;   if (t0 <= 1.0)
;	{				
;		//n * incident - (n + sqrt(1.0 - t0)) * normal;
;		vert0[0]*=v;
;		vert0[1]*=v;
;		vert0[2]*=v;
;				
;		t=-(v + sqrt(1.0f - t0));
;		vert0[0]+=t * vert1[0];
;		vert0[1]+=t * vert1[1];			
;		vert0[2]+=t * vert1[2];
		
;		ray->origin->u=pt->u;
;		ray->origin->v=pt->v;
;		ray->origin->w=pt->w;
;		
;		ray->direction->Normalize();
;		old_color[0]=pt->color->r;
;		old_color[1]=pt->color->g;
;		old_color[2]=pt->color->b;
;		Trace(pt,ray,depth-1);
;		pt->color->r=(pt->color->r * (material->transparency_index)) + (old_color[0] * (1-material->transparency_index));
;		pt->color->g=(pt->color->g * (material->transparency_index)) + (old_color[1] * (1-material->transparency_index));
;		pt->color->b=(pt->color->b * (material->transparency_index)) + (old_color[2] * (1-material->transparency_index));
;		
;	}
;}

;tv->normal(vert1), vdir->V(vert0)
;v -> t
;u->b
;u = V_DOT(vert0,vert1);



;	else
;		ta=1.0f;


;const double n = n1 / n2;
;const double cosI = dot(normal, incident);
;const double sinT2 = n * n * (1.0 - cosI * cosI);
;if (sinT2 > 1.0)
;{
;return Vector::invalid;
;}
;return n * incident - (n + sqrt(1.0 - sinT2)) * normal;

;if(previous_group!=-1)
;		ta=_scene->materials[_scene->groups[previous_group]->material_id]->refraction_index;//n1
jmp .siguiente_refraccion_1

mov eax,[.ptr_material_ant]
test eax,eax
jz .siguiente_refraccion_1

mov eax,[.ptr_material_ant]
mov ebx,[eax + Material.refraccion]
mov [.t0],ebx

.siguiente_refraccion_1:

;if(previous_group==pt->group_id)
;	{
;		vert1[0]*=-1;
;		vert1[1]*=-1;
;		vert1[2]*=-1;
;	}

movaps xmm0,[.tv]
mov eax,[.ptr_grupo_ant]
mov ebx,[.ptr_grupo]
sub eax,ebx
jne .siguiente_refraccion_2
mov eax,[uno]
mov [.t0],eax

jmp .siguiente_refraccion_3

.siguiente_refraccion_2:

movss xmm1,[muno]
shufps xmm1,xmm1,0
mulps xmm0,xmm1


.siguiente_refraccion_3:

movaps [.vtemp],xmm0
;	if(pt->group_id!=NULL)
;		t=_scene->materials[closest_object->material_id]->refraction_index;

mov eax,[.ptr_material]

;b = ta / t;//n
movss xmm0,[.t0]
divss xmm0,[eax + Material.refraccion]
movss [.b],xmm0

V_DOT .vdir + Vector,.vtemp + Vector

mulps xmm0,xmm0
;t0 = (float)( v * v * (1.0 - u * u));
movss xmm2,[uno]
subps xmm2,xmm0

movss xmm1,[.b]
mulps xmm1,xmm1

mulps xmm1,xmm2
movss [.t0],xmm1

comiss xmm1,[uno]
ja .salir_refraccion

;t=-(v + sqrt(1.0f - t0));
;vert0[0]*=v;
;vert0[1]*=v;
;vert0[2]*=v;
movaps xmm7,[.vdir]
movss xmm1,[.b]
shufps xmm1,xmm1,0
mulps xmm7,xmm1
;movaps [vdir],xmm7

;t=-(v + sqrt(1.0f - t0));
movss xmm0,[uno]
subss xmm0,[.t0]
sqrtps xmm0,xmm0
addss xmm0,[.b]
mulss xmm0,[muno]
;movss [t],xmm0

shufps xmm0,xmm0,0
movaps xmm6,[.vtemp]
mulps xmm0,xmm6
addps xmm7,xmm6
movaps [.dlv],xmm7

V_NORMALIZE .dlv


mov ebx,[.ptr_grupo]               ;grupo_ant
mov ecx,[.ptr_material]               ;material_ant
mov edx,[.depth_treshold]
dec edx               ;depth_treshold
lea esi,[.tov]    ;origen
lea edi,[.dlv]          ;direccion
call TraceRay

;		pt->color->r=(pt->color->r * (material->transparency_index)) + (old_color[0] * (1-material->transparency_index));
;		pt->color->g=(pt->color->g * (material->transparency_index)) + (old_color[1] * (1-material->transparency_index));
;		pt->color->b=(pt->color->b * (material->transparency_index)) + (old_color[2] * (1-material->transparency_index));

mov eax, [.ptr_material]
;movss xmm2,[eax + Material.color.a]
movss xmm3,[.color_obj + Color.a]
movss xmm2,[uno]
subps xmm2,xmm3
shufps xmm2,xmm2,0
mulps xmm1,xmm2
movaps xmm3,[.tcolor1]
movss xmm4,[uno]
shufps xmm4,xmm4,0
subps xmm4,xmm2
mulps xmm3,xmm4
addps xmm1,xmm3

movaps [.tcolor1],xmm1

.salir_refraccion:

}

macro TEXTURAS
{


;mov eax,[.objeto_detectado]
mov ebx,[eax + Objeto3D.tipo]
sub ebx,OBJ_TRIANGULO
jz .textura_triangulo

;esfera
;tomado de:
;Ray Tracing: Graphics for the Masses
;by Paul Rademacher

;phi = arccos( -dot_product( Vn, Vp ))
V_DOT eax + Objeto3D.v2,.tv + Vector

mulss xmm0,[muno]
movss [.i],xmm0
;arccos(N)=ATN ( SQR ( 1 – N ^ 2 ) / N ) 

;FINIT
ARCCOS .i

;v = phi / PI
fldpi
fstp [.z]
movss xmm0,[.i]
divss xmm0,[.z]
movss [.v],xmm0

;theta = ( arccos( dot_product( Vp, Ve ) / sin( phi )) ) / ( 2 * PI) 
V_DOT .tv + Vector,eax + Objeto3D.v3

movss [.x],xmm0
fld [.i]
fsin
fstp [.y]
movss xmm0,[.x]
divss xmm0,[.y]
movss [.x],xmm0

ARCCOS .x

movss xmm1,[dos]
mulss xmm1,[.z]
movss xmm0,[.x]
divps xmm0,xmm1
movss [.x],xmm0

;if ( dot_product( cross_product( Vn, Ve ), Vp ) > 0 )
V_CROSS .vtemp + Vector,eax + Objeto3D.v2,eax + Objeto3D.v3
V_DOT .vtemp + Vector,.tv + Vector
comiss xmm0,[zero]
jbe .else_textura_esfera
;u = theta
movss xmm0,[.x]
movss [.u],xmm0
jmp .termina_textura_esfera
 
;else
;u = 1 - theta
.else_textura_esfera:
movss xmm0,[uno]
subss xmm0,[.x]
movss [.u],xmm0

.termina_textura_esfera:

jmp .trae_color_textura

.textura_triangulo:
;triangulo
;real U1 = m_Vertex[0]->GetU(), V1 = m_Vertex[0]->GetV();		
;real U2 = m_Vertex[1]->GetU(), V2 = m_Vertex[1]->GetV();		
;real U3 = m_Vertex[2]->GetU(), V3 = m_Vertex[2]->GetV();		
;real u = U1 + m_U * (U2 - U1) + m_V * (U3 - U1);
;real v = V1 + m_U * (V2 - V1) + m_V * (V3 - V1);
;retval = m_Material->GetTexture()->GetTexel( u, v ) * m_Material->GetColor();


.trae_color_textura:

;convierto u y v en coordenadas de textura;
;tomo u, v y width
;pixel = (v * width) + u
mov eax,[.ptr_material]
mov ebx,[eax + Material.txt_width]
mov edx,ebx ;width

mov ecx,[eax + Material.txt_height]

cvtsi2ss xmm0,ebx
cvtsi2ss xmm1,ecx 

mulss xmm0,[.u] ;u y v convertidos a corrdenadas de la textura
mulss xmm1,[.v]

cvtss2si ebx,xmm0
cvtss2si ecx,xmm1


mov esi,[eax + Material.ptr_textura]
mov eax,ecx ;[.y]
mul edx; [.z] ;y * width
add eax,ebx; [.x] ; (y*width) + x
mov ebx,4
mul ebx
add eax,esi

mov ebx,[eax]
mov edx,255
CVTSI2SS xmm1,edx

mov ecx,ebx
and ecx,0ffh
CVTSI2SS xmm0,ecx
divps xmm0,xmm1
movss [.color_obj + Color.b],xmm0

shr ebx,8
mov ecx,ebx
and ecx,0ffh
CVTSI2SS xmm0,ecx
divps xmm0,xmm1
movss [.color_obj + Color.g],xmm0

shr ebx,8
mov ecx,ebx
and ecx,0ffh
CVTSI2SS xmm0,ecx
divps xmm0,xmm1
movss [.color_obj + Color.r],xmm0

shr ebx,8
mov ecx,ebx
and ecx,0ffh
CVTSI2SS xmm0,ecx
divps xmm0,xmm1
movss [.color_obj + Color.a],xmm0

;mov ebx,[eax]
;mov [.color_obj + Color.r],ebx

;add eax,4
;mov ebx,[eax]
;mov [.color_obj + Color.g],ebx

;add eax,4
;mov ebx,[eax]
;mov [.color_obj + Color.b],ebx

;add eax,4
;mov ebx,[eax]
;mov [.color_obj + Color.a],ebx
}

macro ARCCOS cos
{

fld   [cos]             ;ST(0)=cos(x), ST(1)=zzz
fld   st        ;ST(0)=cos(x), ST(1)=cos(x), ST(2)=zzz
fmul  st,st     ;ST(0)=cos2(x), ST(1)=cos(x), ST(2)=zzz
fld1            ;ST(0)=1.0, ST(1)=cos2(x), ST(2)=cos(x), ST(3)=zzz
fsubrp         ;ST(0)=1-cos2(x)=sin2(x), ST(1)=cos(x), ST(2)=zzz
fsqrt           ;ST(0)=sin(x), ST(1)=cos(x), ST(2)=zzz
fxch            ;ST(0)=cos(x), ST(1)=sin(x), ST(2)=zzz
fpatan          ;ST(0)=arccos[cos(x)]=x (in radians), ST(1)=zzz
FSTP  [cos]
ffree st
 
}