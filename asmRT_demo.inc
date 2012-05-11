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

archivo db "world.jpg",0
world_tr db "world_tr.png",0
transparente db "transparente.png",0
stars  db "stars_map.png",0
fractal db "fractal.png",0
board db "board.gif",0

CreaDemo1:

    ;Tres esferas concentricas en el origen
    ;las tres con 1.0 de radio
    ;y separados por 3.0 del centro
    ;esfera 1: azul, centro: 0.0, 0.0, -3.0
    ;esfera 2: rojo, centro: 2.12132, 0.0, -2.12132
    ;esfera 3: verde, centro: -2.12132, 0.0, -2.12132


    


    ;Solicita la memoria para las luces
    
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx

    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0
    

    
    ;Inicializa los materiales
    mov eax,3
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, uno, uno,zero,zero,uno
    GEN_MATERIAL eax + Material,1, zero, zero, uno, uno,uno,zero,zero
    GEN_MATERIAL eax + Material,2, zero, zero, uno, uno,zero,uno,zero
    
    
    ;Inicializa los grupos
    mov eax,3
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
        
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    GEN_GRUPO eax + Grupo,2,2
    
    ;Solicita la memoria para los objetos

    mov eax,3
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx
    ;inizializa los valores para las esferas
    ;centro  Vector
    ;radio   real4
    ;color   Color
    ;esfera1: azul, centro: 0.0, 0.0, -3.0, radio: 1.0
    GEN_ESFERA eax + Objeto3D,zero,zero,mtres, uno, 0 ;zero,zero,uno
    ;esfera 2: rojo, centro: 2.12132, 0.0, -2.12132
    GEN_ESFERA eax + Objeto3D,dosp,zero,mdosp, uno, 1 ;uno,zero,zero  
    ;esfera 3: verde, centro: -2.12132, 0.0, -2.12132
    GEN_ESFERA eax + Objeto3D,mdosp,zero,mdosp,uno, 2 ;zero,uno,zero

ret

CreaDemo2:
    ;Sol un triangulo por el momento
    ;triangulo azul
    ;v1: azul, centro: 0.0, 0.0, -3.0
    ;v2: rojo, centro: 2.12132, 0.0, -2.12132
    ;v3: verde, centro: -2.12132, 0.0, -2.12132

    
    ;Solicita la memoria para las luces
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx
    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0
    


    ;Inicializa los materiales
    mov eax,2
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, uno, uno,zero,zero,uno
    GEN_MATERIAL eax + Material,1, zero, zero, uno, uno,uno,zero,zero


    
    ;Inicializa los grupos
    mov eax,2
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    
    ;calcular espacio para los objetos
    
    mov eax,2
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx
    ;inizializa los valores para el triangulo
    ;v1  Vector     ;vert0
    ;v2  Vector     ;edge1 (v2-v1)
    ;v3  Vector     ;edge2 (v3-v1)
    ;color   Color
    ;normal Vector
    ;triangulo1: azul
    ;v1: -2.12132,0,0
    ;v2: 0,3,0
    ;v3: 2.12132,0,0
    ;color: 0.0,0.0,1.0
    ;normal: calcular

    GEN_TRIANGULO eax + Objeto3D,mdos,zero,zero, dos,zero,zero, zero,tres,zero, 0
    
    ;esfera 2: rojo, centro: 2.12132, 0.0, -2.12132
    GEN_ESFERA eax + Objeto3D,dos,zero,mdos, uno, 1


      ret

CreaDemo3:
    ;Sol un triangulo por el momento
    ;triangulo azul
    ;v1: azul, centro: 0.0, 0.0, -3.0
    ;v2: rojo, centro: 2.12132, 0.0, -2.12132
    ;v3: verde, centro: -2.12132, 0.0, -2.12132


    
    
    
    ;Solicita la memoria para las luces
    
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx
    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0

    ;Inicializa los materiales
    mov eax,6
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, zero, uno,uno,uno,uno
    GEN_MATERIAL eax + Material,1, zero, zero, zero, uno,zero,uno,zero
    GEN_MATERIAL eax + Material,2, zero, zero, zero, uno,uno,zero,zero
    GEN_MATERIAL eax + Material,3, p75, zero, uno, uno,uno,mitad,zero
    ;GEN_MATERIAL eax + Material,3, uno, zero, zero,zero,zero,zero
    GEN_MATERIAL eax + Material,4, zero, refrac_vidrio, uno, p15,uno,uno,zero
    GEN_MATERIAL eax + Material,5, zero, zero, zero, uno,uno,uno,uno
    
    
    
    mov edi,0
    lea esi,[board]
    call AplicaTextura
    
    mov edi,4
    lea esi,[world_tr]
    call AplicaTextura
    
    ;Inicializa los grupos
    mov eax,7
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    GEN_GRUPO eax + Grupo,2,2
    GEN_GRUPO eax + Grupo,3,5
    GEN_GRUPO eax + Grupo,4,5
    GEN_GRUPO eax + Grupo,5,3
    GEN_GRUPO eax + Grupo,6,4

    ;calcular espacio para los objetos
    
    mov eax,12
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx

    ;GEN_TRIANGULO eax +  Objeto3D,muno,zero,zero, uno,zero,zero, zero,uno,zero, zero,zero,uno


    ;//Parte de Atraz
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,mcuatro,cuatro, cuatro,mcuatro,cuatro,mcuatro,cuatro,cuatro,  0
    GEN_TRIANGULO eax +  Objeto3D,cuatro,cuatro,cuatro, mcuatro,cuatro,cuatro, cuatro,mcuatro,cuatro, 0

    ;//Parte Izquierda
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,cuatro,mcuatro, mcuatro,mcuatro,mcuatro, mcuatro,cuatro,cuatro, 1
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,mcuatro,mcuatro, mcuatro,mcuatro,cuatro, mcuatro,cuatro,cuatro, 1
    
    ;//Parte Derecha
    GEN_TRIANGULO eax +  Objeto3D,cuatro,mcuatro,mcuatro, cuatro,cuatro,mcuatro, cuatro,cuatro,cuatro, 2
    GEN_TRIANGULO eax +  Objeto3D,cuatro,mcuatro,cuatro, cuatro,mcuatro,mcuatro, cuatro,cuatro,cuatro, 2

    ;//Parte Arriba	
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,cuatro,mcuatro, cuatro,cuatro,cuatro, cuatro,cuatro,mcuatro, 3
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,cuatro,cuatro, cuatro,cuatro,cuatro, mcuatro,cuatro,mcuatro, 3
    
    ;//Parte Abajo	
    GEN_TRIANGULO eax +  Objeto3D,cuatro,mcuatro,cuatro, mcuatro,mcuatro,mcuatro, cuatro,mcuatro,mcuatro, 4
    GEN_TRIANGULO eax +  Objeto3D,cuatro,mcuatro,cuatro, mcuatro,mcuatro,cuatro, mcuatro,mcuatro,mcuatro, 4
    
    ;esfera 2: rojo, centro: 2.12132, 0.0, -2.12132
    GEN_ESFERA eax +  Objeto3D,mtres,mtres,uno, uno, 5
    GEN_ESFERA eax +  Objeto3D,zero,zero,zero, uno, 6




    ret


CreaDemo4:
    ;Sol un triangulo por el momento
    ;triangulo azul
    ;v1: azul, centro: 0.0, 0.0, -3.0
    ;v2: rojo, centro: 2.12132, 0.0, -2.12132
    ;v3: verde, centro: -2.12132, 0.0, -2.12132


    ;Solicita la memoria para las luces
    
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx
    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0

    ;Inicializa los materiales
    mov eax,6
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, mitad, uno,uno,uno,uno
    GEN_MATERIAL eax + Material,1, zero, zero, mitad, uno,zero,uno,zero
    GEN_MATERIAL eax + Material,2, zero, zero, mitad, uno,uno,zero,zero
    GEN_MATERIAL eax + Material,3, zero, zero, mitad, uno,uno,uno,zero
    GEN_MATERIAL eax + Material,4, zero, zero, mitad, uno,uno,mitad,zero
    GEN_MATERIAL eax + Material,5, zero, zero, mitad, uno,zero,zero,uno
    
    
    ;Inicializa los grupos
    mov eax,6
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    GEN_GRUPO eax + Grupo,2,2
    GEN_GRUPO eax + Grupo,3,3
    GEN_GRUPO eax + Grupo,4,4
    GEN_GRUPO eax + Grupo,5,5
    
    ;calcular espacio para los objetos
    
    mov eax,12
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx

    
    ;PLano
    ;movss xmm0,[dos]
    ;mulss xmm0,[cinco]
    ;mulss xmm0,xmm0
    ;mulss xmm0,xmm0
    ;movss [x],xmm0
    ;mulss xmm0,muno
    ;movss [y],xmm0
    ;GEN_TRIANGULO eax +  Objeto3D,y,mtres,x, y,mtres,y, x,mtres,x, uno,uno,uno
    ;GEN_TRIANGULO eax +  Objeto3D,x,mtres,x, y,mtres,y, x,mtres,y, uno,uno,uno


    ;//Parte de Atraz
    GEN_TRIANGULO eax +  Objeto3D,mdos,mdos,dos, mdos,dos,dos, dos,mdos,dos, 0
    GEN_TRIANGULO eax +  Objeto3D,dos,dos,dos, dos,mdos,dos, mdos,dos,dos, 0

    ;//Parte Izquierda
    GEN_TRIANGULO eax +  Objeto3D,mdos,mdos,mdos, mdos,dos,mdos, mdos,dos,dos, 1
    GEN_TRIANGULO eax +  Objeto3D,mdos,mdos,dos, mdos,mdos,mdos, mdos,dos,dos, 1
    
    ;//Parte Derecha
    GEN_TRIANGULO eax +  Objeto3D,dos,dos,mdos, dos,mdos,mdos, dos,dos,dos, 2
    GEN_TRIANGULO eax +  Objeto3D,dos,mdos,mdos, dos,mdos,dos, dos,dos,dos, 2

    ;//Parte Arriba	
    GEN_TRIANGULO eax +  Objeto3D,dos,dos,dos, mdos,dos,mdos, dos,dos,mdos, 3
    GEN_TRIANGULO eax +  Objeto3D,dos,dos,dos, mdos,dos,dos, mdos,dos,mdos, 3
    
    ;//Parte Abajo	
    GEN_TRIANGULO eax +  Objeto3D,mdos,mdos,mdos, dos,mdos,dos, dos,mdos,mdos, 4
    GEN_TRIANGULO eax +  Objeto3D,mdos,mdos,dos, dos,mdos,dos, mdos,mdos,mdos, 4

    ;//Parte de Enfrente
    GEN_TRIANGULO eax +  Objeto3D,mdos,dos,mdos, mdos,mdos,mdos, dos,mdos,mdos, 5
    GEN_TRIANGULO eax +  Objeto3D,dos,mdos,mdos, dos,dos,mdos, mdos,dos,mdos, 5

   ret
   
    
    
CreaDemo5:

    ;Solicita la memoria para las luces
   

    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks   
    mov [p_luces],eax
    mov [p_luces_na],edx

    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0
    
    ;Inicializa los materiales
    mov eax,2
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, uno, uno,zero,zero,uno
    GEN_MATERIAL eax + Material,1,zero,zero, zero,uno,mitad,mitad,uno
    
    mov edi,0
    lea esi,[archivo]
    call AplicaTextura
    
    mov edi,1
    lea esi,[stars]
    call AplicaTextura
        
    ;Inicializa los grupos
    mov eax,2
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    
    ;Solicita la memoria para los objetos

    mov eax,2
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx
    ;inizializa los valores para las esferas
    ;centro  Vector
    ;radio   real4
    ;color   Color
    ;esfera1: azul, centro: 0.0, 0.0, -3.0, radio: 1.0
    GEN_ESFERA eax + Objeto3D,zero,zero,zero,tres, 0 ;zero,zero,uno
    GEN_SKYDOME eax + Objeto3D,zero,zero,zero,cien, 1

ret

CreaDemo6:
    ;Sol un triangulo por el momento
    ;triangulo azul
    ;v1: azul, centro: 0.0, 0.0, -3.0
    ;v2: rojo, centro: 2.12132, 0.0, -2.12132
    ;v3: verde, centro: -2.12132, 0.0, -2.12132


    
    
    
    ;Solicita la memoria para las luces
    
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx
    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    
    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0

    ;Inicializa los materiales
    mov eax,3
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, zero, uno,uno,uno,uno
    GEN_MATERIAL eax + Material,1, uno, zero, zero, uno,zero,zero,zero
    GEN_MATERIAL eax + Material,2, zero, refrac_vidrio, zero, p15,zero,zero,zero

    
    
    mov edi,0
    lea esi,[board]
    call AplicaTextura

    
    ;Inicializa los grupos
    mov eax,3
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
    GEN_GRUPO eax + Grupo,0,0
    GEN_GRUPO eax + Grupo,1,1
    GEN_GRUPO eax + Grupo,2,2


    ;calcular espacio para los objetos
    
    mov eax,5
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx

    ;GEN_TRIANGULO eax +  Objeto3D,muno,zero,zero, uno,zero,zero, zero,uno,zero, zero,zero,uno


    ;//Parte de Atraz
    ;GEN_TRIANGULO eax +  Objeto3D,mcien,mcien,cuatro, cien,mcien,cuatro,mcien,cien,cuatro,  0
    ;GEN_TRIANGULO eax +  Objeto3D,cien,cien,cuatro, mcien,cien,cuatro, cien,mcien,cuatro, 0
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,mcuatro,cuatro, cuatro,mcuatro,cuatro,mcuatro,cuatro,cuatro,  0
    GEN_TRIANGULO eax +  Objeto3D,cuatro,cuatro,cuatro, mcuatro,cuatro,cuatro, cuatro,mcuatro,cuatro, 0

    ;//Parte Izquierda
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,cuatro,mcuatro, mcuatro,mcuatro,mcuatro, mcuatro,cuatro,cuatro, 1
    GEN_TRIANGULO eax +  Objeto3D,mcuatro,mcuatro,mcuatro, mcuatro,mcuatro,cuatro, mcuatro,cuatro,cuatro, 1
    
    ;esfera 2: rojo, centro: 2.12132, 0.0, -2.12132
    GEN_ESFERA eax +  Objeto3D,zero,zero,zero, uno, 2
    ret

CreaDemo7:
    ;Solicita la memoria para las luces
    
    mov eax,1
    mov [num_luces],eax
    mov edx,LuzSize
    call MemAllocAlignBlocks
    mov [p_luces],eax
    mov [p_luces_na],edx

    ;Inicializa los valores para la luz
    ;posicion    Vector <0.0,10.0,-5.0>
    ;intensidad  real4   100.0
    movss xmm0,[zero]
    movss [eax + Luz.posicion.u],xmm0
    movss xmm0,[diez]
    movss [eax + Luz.posicion.v],xmm0
    movss xmm0,[mcinco]
    movss [eax + Luz.posicion.w],xmm0
    movss xmm0,[cien]
    movss [eax + Luz.intensidad],xmm0
    

    movss xmm0,[uno]
    movss [eax + Luz.color.a],xmm0
    movss [eax + Luz.color.r],xmm0
    movss [eax + Luz.color.g],xmm0
    movss [eax + Luz.color.b],xmm0
    
    ;Inicializa los materiales
    mov eax,1
    mov [num_materiales],eax
    mov edx,MaterialSize
    call MemAllocAlignBlocks
    mov [p_materiales],eax
    mov [p_materiales_na],edx
    
    GEN_MATERIAL eax + Material,0, zero, zero, uno, uno,uno,zero,zero
    
    
    ;Inicializa los grupos
    mov eax,1
    mov [num_grupos],eax
    mov edx,GrupoSize
    call MemAllocAlignBlocks
    mov [p_grupos],eax
    mov [p_grupos_na],edx
    
        
    GEN_GRUPO eax + Grupo,0,0
    
    ;Solicita la memoria para los objetos

    mov eax,100
    mov [num_objetos],eax
    mov edx,Objeto3DSize
    call MemAllocAlignBlocks
    mov [p_objetos],eax
    mov [p_objetos_na],edx
    ;inizializa los valores para las esferas
    ;centro  Vector
    ;radio   real4
    ;color   Color
    ;esfera1: azul, centro: 0.0, 0.0, -3.0, radio: 1.0
    
    
    
    mov ebx,5.0
    movd xmm2,ebx
    
    mov edx,10
    .loopy:
    
    mov ebx,-5.0
    movd xmm1,ebx
    mov ecx,10

    .loopx:
    movss [eax + Objeto3D.v1.u],xmm1
    movss [eax + Objeto3D.v1.v],xmm2
    mov [eax + Objeto3D.v1.w],0.0
    mov [eax + Objeto3D.radio],0.5
    
    ;mov edi,0
    ;imul edi,GrupoSize
    ;add edi,[p_grupos]
    mov [eax + Objeto3D.grupo_id],0
    mov [eax + Objeto3D.tipo],OBJ_ESFERA
    
    addss xmm1,[uno]
    
    add eax,Objeto3DSize
    
    sub ecx,1
    test ecx,ecx
    jnz .loopx
    
    addss xmm2,[muno]
    sub edx,1
    test edx,edx
    jnz .loopy
    
    
    ;GEN_ESFERA eax + Objeto3D,zero,zero,mtres, uno, 0 ;zero,zero,uno
ret

AplicaTextura: ;edi->material_id, esi->ptr_nombre_archivo
    push ebp
    mov ebp,esp
        

    ;sub esp,0C4h ; 196
dwordsize = 4
    VARS_ALIGN  dwordsize*1

    label .ptr_material         dword at esp 
    
    mov eax,MaterialSize
    mul edi
    add eax,[p_materiales]
    mov [.ptr_material], eax
    
    
    push 0
    push esi
    call [FreeImage_GetFileType]
    
    push 0
    push esi
    push eax
    call [FreeImage_Load]
    mov esi,eax

    ;push 1
    ;push FIT_UINT32
    push eax
    call [FreeImage_ConvertTo32Bits]
    mov edi,eax
    
    ;aki tengo que asegurarme que convert elimine la memoria utilizada
    ;por el anterior load
    push esi
    call [FreeImage_Unload]    
    
    ;necesito espejear la imagen en el eje y
    push edi
    call [FreeImage_FlipHorizontal]


    
    mov ebx,[.ptr_material]
    mov [ebx+Material.textura],edi

    ;mov edi,esi
    push edi
    call [FreeImage_GetWidth]
    
    mov ebx,[.ptr_material]
    mov [ebx + Material.txt_width],eax
    
    push edi
    call [FreeImage_GetHeight]
    
    mov ebx,[.ptr_material]
    dec eax
    mov [ebx + Material.txt_height],eax

    
    push edi
    call [FreeImage_GetBits]
    mov ebx,[.ptr_material]
    mov [ebx + Material.ptr_textura],eax
    
    leave
ret
