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

match Win32,PLATFORM {
}

match PLATFORM,Linux {
;para compilar se usa:
;./JWasm -elf -Fo=asmRT.o asmRT.asm
;para link con las librerias hay que poner:
;gcc -lSDL -lSDLmain -lpthreads -s -nostartfiles -o asmRT asmRT.o

;malloc 	proto syscall :dword
;free	proto syscall :dword
;usleep	proto syscall :dword
;exit 	proto syscall :dword
;puts proto syscall :dword
;pthread_create proto syscall :dword,:dword,:dword,:dword
;include 'SDL.inc'
}

;Contenido
;Estructuras
;Constantes
;Variables Globales
;Prototipos
;Macros

;Estructuras
; «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««
struct Vector
    t     dd   0
    u     dd   0
    v     dd   0
    w     dd   0
ends
;Metadatos de Vector
VectorSize equ  16 ;bytes

struct Color
    a   dd 0
    r   dd 0
    g   dd 0
    b   dd 0
ends
;Metadatos de Color
ColorSize   equ 16; bytes

struct Camara
      lefttop   Vector 0.0,-3.2,2.4,-5.0
      leftbottom   Vector 0.0,-3.2,-2.4,-5.0
      righttop   Vector 0.0,3.2,2.4,-5.0
      eye  Vector 0.0,0.0,0.0,-10.0
      id dd 0
      padding dd 0,0,0
ends
;Metadatos de Camara
CamaraSize  equ VectorSize*4  + 16  ;bytes

struct Luz
    posicion    Vector 0.0,0.0,-1.0,-5.0
    color       Color  0.0,1.0,1.0,1.0
    intensidad  dd   100.0
    id dd 0
    stub_for_aligment dd 0,0
ends
;Metadatos de Luz
LuzSize equ VectorSize  + ColorSize  + 16

;Esfera STRUCT
;    centro  Vector  <0.0,0.0,0.0,0.0>
;    color   Color   <0.0,0.0,0.0,1.0>
;    radio   real4   1.0
;    stub_for_aligment dword 3+8 dup(0) ;El anterior y este son: 16+16+16 = 48 bytes
;Esfera ENDS
;Metadatos de esfera
;EsferaSize  equ VectorSize + 48 + ColorSize

;Triangulo STRUCT
;    v1  Vector <0.0,0.0,0.0,0.0>
;    v2  Vector <0.0,0.0,0.0,0.0>
;    v3  Vector <0.0,0.0,0.0,0.0>
;    normal Vector <0.0,0.0,0.0,0.0>
;    color Color <0.0,0.0,0.0,0.0>
;Triangulo ENDS
;Metadatos Triangulo
;TrianguloSize equ VectorSize * 4 + ColorSize

align 16

struct Grupo
    id dd 0
    material_id dd 0
    padding dd 0,0
ends
GrupoSize equ 16

struct Material
    id dd 0
    refraccion dd 0.0
    reflexion   dd 0.0
    specular   dd 0.0
    textura dd 0
    ptr_textura dd 0
    txt_width dd 0
    txt_height dd 0
    color Color 0.0,0.0,0.0,0.0
ends
MaterialSize equ 32 + ColorSize
    
struct Objeto3D
    tipo dd 0            ;tipo de figura
    grupo_id dd 0
    radio dd 0
    id dd 0
    v1  Vector 0.0,0.0,0.0,0.0
    v2  Vector 0.0,0.0,0.0,0.0
    v3  Vector 0.0,0.0,0.0,0.0
    ;color Color 0.0,0.0,0.0,0.0
    ;normal Vector 0.0,0.0,0.0,0.0
ends
Objeto3DSize equ 16 + VectorSize*3 
; «««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««««

;Constantes
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤
OBJ_NULO equ 0
OBJ_ESFERA equ 1
OBJ_TRIANGULO equ 2
OBJ_SKYDOME equ 3


sector_y dd 0
sector_height dd 480

_width  dd 640
_height  dd 480

video_options dd SDL_HWSURFACE + SDL_ASYNCBLIT + SDL_HWACCEL
mode  dd 0  ;0->4:3  1 ->16:9
resolucion dd 3 ;indice de la resolucion
max_resolucion equ 6 ;min->cero
widths dd 160,320,400,640,800,1024,1280
heights dd 120,240,300,480,600,768,1024

_bpp    equ 32

num_threads dd 1
; ¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤

;Variables Globales
; ¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤
align 16
camara  Camara
luz     Luz
uvw_mask Vector 0FFFFFFFFh,0FFFFFFFFh,0FFFFFFFFh,0FFFFFFFFh
v0123 Vector 0,1.0,2.0,3.0
v_temp Vector 0,0,0,0
unos Vector 1.0,1.0,1.0,1.0
;esfera  Esfera  <>

align 16
start_clock dq 0,0
stop_clock  dq 0,0
clock_freq  dq 0,0

align 16
p_luces_na         dd 0
p_luces         dd 0
p_objetos_na  dd 0
p_objetos       dd 0
p_materiales_na dd 0
p_materiales    dd 0
p_grupos_na     dd 0
p_grupos        dd 0

num_grupos      dd 0
num_materiales dd 0
num_objetos dd   0
num_luces   dd   0
var_sincro  dd   0
HeapInfo    dd   2

;real4's
;align 16
uno     dd 1.0
zero   dd 0.0
muno    dd -1.0
dos     dd 2.0
mdos    dd -2.0
delta   dd 0.01
mitad   dd 0.5
p15 dd 0.15
p25 dd 0.25
p75 dd 0.75
p90 dd 0.9
p85 dd 0.85
epsilon dd 0.001
mepsilon dd -0.001
rotacion_neg dd -0.04363;-0.008726;-0.017452
rotacion_pos dd  0.04363; 0.008726; 0.017452
refrac_vidrio dd 1.51714
mil dd 1000.0
diez    dd 10.0
mdiez    dd -10.0
cien    dd 100.0
mcien dd -100.0
cinco   dd 5.0
mcinco  dd -5.0
tres    dd 3.0
mtres   dd -3.0
dosp    dd 2.12132
mdosp   dd -2.12132
cuatro  dd 4.0
mcuatro dd -4.0
max_dist dd 10000000000000000000000000000.0
pi dd 3.1415926535897932384626433832795

texto db 256 dup(0)
MostrarInformacion.frame_rate   db "Fps: %f Spf: %f ",0
MostrarInformacion.mm_fps     db    "Prom: %f Max fps: %f Min fps: %f",0
MostrarInformacion.mm_spf     db    "Max spf: %f Min spf: %f Prom: %f",0
MostrarInformacion.resolucion 	db 		"Res.: %dx%dx%d",0
MostrarInformacion.threads		db		"Hilos: %d Mejor: %d",0

align 16
info_extendida dd 1
max_fps dd 0.0
min_fps dd 1000.0
max_spf dd 0.0
min_spf dd 10000000000000000000000000000.0
avg_fps dd 0.0
avg_spf dd 0.0
num_frames dd 0
mejor_num_threads dd 1


align 16
;sdlevnt SDL_Event 0 ;tuvo que moverse a asmRT.asm pk no se podia accesar desde SDL_PollEvent
;mutex dd 0
screen dd 0
pixeles dd 0

proc_heap dd 0
dragging dd 0
grupo_drag dd 0
mouse_x_ant dd 0
mouse_y_ant dd 0
pixel dd 0
exceptions_mask dd 00FFFFh
max_dd dd 0FFFFFFFFh

; ¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤=÷=¤

;Prototipos
; ÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷
;section '.text' code readable executable
;MainInit		PROTO	
;RecorrerPixelesMT PROTO :DWORD
;RecorrePixeles  PROTO :DWORD
;TraceRay    PROTO :real4,:real4,:real4,:real4,:real4,:real4,:DWORD ;u,v,w de direccion, ou,ov,ow de origen, depth_treshold
;LimpiarHeap PROTO
; ÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷·÷

