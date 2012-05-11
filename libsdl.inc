

;-------------------------------------------------------------------------------

;subsystems to initialize
SDL_INIT_TIMER          = 0x00000001
SDL_INIT_AUDIO          = 0x00000010
SDL_INIT_VIDEO          = 0x00000020
SDL_INIT_CDROM          = 0x00000100
SDL_INIT_JOYSTICK       = 0x00000200
SDL_INIT_NOPARACHUTE    = 0x00100000
SDL_INIT_EVENTTHREAD    = 0x01000000
SDL_INIT_EVERYTHING     = 0x0000FFFF

;surface flags
SDL_SWSURFACE   = 0x00000000
SDL_HWSURFACE   = 0x00000001
SDL_ASYNCBLIT   = 0x00000004
SDL_ANYFORMAT   = 0x10000000
SDL_HWPALETTE   = 0x20000000
SDL_DOUBLEBUF   = 0x40000000
SDL_FULLSCREEN  = 0x80000000
SDL_OPENGL      = 0x00000002
SDL_OPENGLBLIT  = 0x0000000A
SDL_RESIZABLE   = 0x00000010
SDL_NOFRAME     = 0x00000020
SDL_HWACCEL     = 0x00000100
SDL_SRCCOLORKEY = 0x00001000
SDL_RLEACCELOK  = 0x00002000
SDL_RLEACCEL    = 0x00004000
SDL_SRCALPHA    = 0x00010000
SDL_PREALLOC    = 0x01000000

;overlay formats
SDL_YV12_OVERLAY    = 0x32315659
SDL_IYUV_OVERLAY    = 0x56555949
SDL_YUY2_OVERLAY    = 0x32595559
SDL_UYVY_OVERLAY    = 0x59565955
SDL_YVYU_OVERLAY    = 0x55595659

;audio flags
SDL_MAX_TRACKS  = 99
SDL_AUDIO_TRACK = 0x00
SDL_DATA_TRACK  = 0x04

;event actions
SDL_ADDEVENT    = 0
SDL_PEEKEVENT   = 1
SDL_GETEVENT    = 2

;event types
SDL_NOEVENT         =  0
SDL_ACTIVEEVENT     =  1
SDL_KEYDOWN         =  2
SDL_KEYUP           =  3
SDL_MOUSEMOTION     =  4
SDL_MOUSEBUTTONDOWN =  5
SDL_MOUSEBUTTONUP   =  6
SDL_JOYAXISMOTION   =  7
SDL_JOYBALLMOTION   =  8
SDL_JOYHATMOTION    =  9
SDL_JOYBUTTONDOWN   = 10
SDL_JOYBUTTONUP     = 11
SDL_QUIT            = 12
SDL_SYSWMEVENT      = 13
SDL_VIDEORESIZE     = 16
SDL_VIDEOEXPOSE     = 17
SDL_USEREVENT       = 24
SDL_NUMEVENTS       = 32

;eventmasks
SDL_ALLEVENTS           = 0xFFFFFFFF
SDL_ACTIVEEVENTMASK	    = 0x00000001 shl SDL_ACTIVEEVENT
SDL_KEYDOWNMASK		    = 0x00000001 shl SDL_KEYDOWN
SDL_KEYUPMASK		    = 0x00000001 shl SDL_KEYUP
SDL_KEYEVENTMASK	    = SDL_KEYDOWNMASK or SDL_KEYUPMASK
SDL_MOUSEMOTIONMASK	    = 0x00000001 shl SDL_MOUSEMOTION
SDL_MOUSEBUTTONDOWNMASK = 0x00000001 shl SDL_MOUSEBUTTONDOWN
SDL_MOUSEBUTTONUPMASK	= 0x00000001 shl SDL_MOUSEBUTTONUP
SDL_MOUSEEVENTMASK	    = SDL_MOUSEMOTIONMASK or SDL_MOUSEBUTTONDOWNMASK or SDL_MOUSEBUTTONUPMASK
SDL_JOYAXISMOTIONMASK	= 0x00000001 shl SDL_JOYAXISMOTION
SDL_JOYBALLMOTIONMASK	= 0x00000001 shl SDL_JOYBALLMOTION
SDL_JOYHATMOTIONMASK	= 0x00000001 shl SDL_JOYHATMOTION
SDL_JOYBUTTONDOWNMASK	= 0x00000001 shl SDL_JOYBUTTONDOWN
SDL_JOYBUTTONUPMASK	    = 0x00000001 shl SDL_JOYBUTTONUP
SDL_JOYEVENTMASK	    = SDL_JOYAXISMOTIONMASK or SDL_JOYBALLMOTIONMASK or SDL_JOYHATMOTIONMASK or SDL_JOYBUTTONDOWNMASK or SDL_JOYBUTTONUPMASK
SDL_VIDEORESIZEMASK	    = 0x00000001 shl SDL_VIDEORESIZE
SDL_VIDEOEXPOSEMASK	    = 0x00000001 shl SDL_VIDEOEXPOSE
SDL_QUITMASK		    = 0x00000001 shl SDL_QUIT
SDL_SYSWMEVENTMASK	    = 0x00000001 shl SDL_SYSWMEVENT

;gl attributes
SDL_GL_RED_SIZE             =  0
SDL_GL_GREEN_SIZE           =  1
SDL_GL_BLUE_SIZE            =  2
SDL_GL_ALPHA_SIZE           =  3
SDL_GL_BUFFER_SIZE          =  4
SDL_GL_DOUBLEBUFFER         =  5
SDL_GL_DEPTH_SIZE           =  6
SDL_GL_STENCIL_SIZE         =  7
SDL_GL_ACCUM_RED_SIZE       =  8
SDL_GL_ACCUM_GREEN_SIZE     =  9
SDL_GL_ACCUM_BLUE_SIZE      = 10
SDL_GL_ACCUM_ALPHA_SIZE     = 11
SDL_GL_STEREO               = 12
SDL_GL_MULTISAMPLEBUFFERS   = 13
SDL_GL_MULTISAMPLESAMPLES   = 14
SDL_GL_ACCELERATED_VISUAL   = 15
SDL_GL_SWAP_CONTROL         = 16

;grab modes
SDL_GRAB_QUERY      = -1
SDL_GRAB_OFF        =  0
SDL_GRAB_ON         =  1
SDL_GRAB_FULLSCREEN =  2

;cdrom
CD_TRAYEMPTY    =  0
CD_STOPPED      =  1
CD_PLAYING      =  2
CD_PAUSED       =  3
CD_ERROR        = -1

;keyboard
SDLK_UNKNOWN        =   0
SDLK_FIRST          =   0
SDLK_BACKSPACE      =   8
SDLK_TAB            =   9
SDLK_CLEAR          =  12
SDLK_RETURN         =  13
SDLK_PAUSE          =  19
SDLK_ESCAPE         =  27
SDLK_SPACE          =  32
SDLK_EXCLAIM        =  33
SDLK_QUOTEDBL       =  34
SDLK_HASH           =  35
SDLK_DOLLAR         =  36
SDLK_AMPERSAND      =  38
SDLK_QUOTE          =  39
SDLK_LEFTPAREN      =  40
SDLK_RIGHTPAREN     =  41
SDLK_ASTERISK       =  42
SDLK_PLUS           =  43
SDLK_COMMA          =  44
SDLK_MINUS          =  45
SDLK_PERIOD         =  46
SDLK_SLASH          =  47
SDLK_0              =  48
SDLK_1              =  49
SDLK_2              =  50
SDLK_3              =  51
SDLK_4              =  52
SDLK_5              =  53
SDLK_6              =  54
SDLK_7              =  55
SDLK_8              =  56
SDLK_9              =  57
SDLK_COLON          =  58
SDLK_SEMICOLON      =  59
SDLK_LESS           =  60
SDLK_EQUALS         =  61
SDLK_GREATER        =  62
SDLK_QUESTION       =  63
SDLK_AT             =  64
SDLK_LEFTBRACKET    =  91
SDLK_BACKSLASH      =  92
SDLK_RIGHTBRACKET   =  93
SDLK_CARET          =  94
SDLK_UNDERSCORE     =  95
SDLK_BACKQUOTE      =  96
SDLK_a              =  97
SDLK_b              =  98
SDLK_c              =  99
SDLK_d              = 100
SDLK_e              = 101
SDLK_f              = 102
SDLK_g              = 103
SDLK_h              = 104
SDLK_i              = 105
SDLK_j              = 106
SDLK_k              = 107
SDLK_l              = 108
SDLK_m              = 109
SDLK_n              = 110
SDLK_o              = 111
SDLK_p              = 112
SDLK_q              = 113
SDLK_r              = 114
SDLK_s              = 115
SDLK_t              = 116
SDLK_u              = 117
SDLK_v              = 118
SDLK_w              = 119
SDLK_x              = 120
SDLK_y              = 121
SDLK_z              = 122
SDLK_DELETE         = 127
SDLK_WORLD_0        = 160
SDLK_WORLD_1        = 161
SDLK_WORLD_2        = 162
SDLK_WORLD_3        = 163
SDLK_WORLD_4        = 164
SDLK_WORLD_5        = 165
SDLK_WORLD_6        = 166
SDLK_WORLD_7        = 167
SDLK_WORLD_8        = 168
SDLK_WORLD_9        = 169
SDLK_WORLD_10       = 170
SDLK_WORLD_11       = 171
SDLK_WORLD_12       = 172
SDLK_WORLD_13       = 173
SDLK_WORLD_14       = 174
SDLK_WORLD_15       = 175
SDLK_WORLD_16       = 176
SDLK_WORLD_17       = 177
SDLK_WORLD_18       = 178
SDLK_WORLD_19       = 179
SDLK_WORLD_20       = 180
SDLK_WORLD_21       = 181
SDLK_WORLD_22       = 182
SDLK_WORLD_23       = 183
SDLK_WORLD_24       = 184
SDLK_WORLD_25       = 185
SDLK_WORLD_26       = 186
SDLK_WORLD_27       = 187
SDLK_WORLD_28       = 188
SDLK_WORLD_29       = 189
SDLK_WORLD_30       = 190
SDLK_WORLD_31       = 191
SDLK_WORLD_32       = 192
SDLK_WORLD_33       = 193
SDLK_WORLD_34       = 194
SDLK_WORLD_35       = 195
SDLK_WORLD_36       = 196
SDLK_WORLD_37       = 197
SDLK_WORLD_38       = 198
SDLK_WORLD_39       = 199
SDLK_WORLD_40       = 200
SDLK_WORLD_41       = 201
SDLK_WORLD_42       = 202
SDLK_WORLD_43       = 203
SDLK_WORLD_44       = 204
SDLK_WORLD_45       = 205
SDLK_WORLD_46       = 206
SDLK_WORLD_47       = 207
SDLK_WORLD_48       = 208
SDLK_WORLD_49       = 209
SDLK_WORLD_50       = 210
SDLK_WORLD_51       = 211
SDLK_WORLD_52       = 212
SDLK_WORLD_53       = 213
SDLK_WORLD_54       = 214
SDLK_WORLD_55       = 215
SDLK_WORLD_56       = 216
SDLK_WORLD_57       = 217
SDLK_WORLD_58       = 218
SDLK_WORLD_59       = 219
SDLK_WORLD_60       = 220
SDLK_WORLD_61       = 221
SDLK_WORLD_62       = 222
SDLK_WORLD_63       = 223
SDLK_WORLD_64       = 224
SDLK_WORLD_65       = 225
SDLK_WORLD_66       = 226
SDLK_WORLD_67       = 227
SDLK_WORLD_68       = 228
SDLK_WORLD_69       = 229
SDLK_WORLD_70       = 230
SDLK_WORLD_71       = 231
SDLK_WORLD_72       = 232
SDLK_WORLD_73       = 233
SDLK_WORLD_74       = 234
SDLK_WORLD_75       = 235
SDLK_WORLD_76       = 236
SDLK_WORLD_77       = 237
SDLK_WORLD_78       = 238
SDLK_WORLD_79       = 239
SDLK_WORLD_80       = 240
SDLK_WORLD_81       = 241
SDLK_WORLD_82       = 242
SDLK_WORLD_83       = 243
SDLK_WORLD_84       = 244
SDLK_WORLD_85       = 245
SDLK_WORLD_86       = 246
SDLK_WORLD_87       = 247
SDLK_WORLD_88       = 248
SDLK_WORLD_89       = 249
SDLK_WORLD_90       = 250
SDLK_WORLD_91       = 251
SDLK_WORLD_92       = 252
SDLK_WORLD_93       = 253
SDLK_WORLD_94       = 254
SDLK_WORLD_95       = 255
SDLK_KP0            = 256
SDLK_KP1            = 257
SDLK_KP2            = 258
SDLK_KP3            = 259
SDLK_KP4            = 260
SDLK_KP5            = 261
SDLK_KP6            = 262
SDLK_KP7            = 263
SDLK_KP8            = 264
SDLK_KP9            = 265
SDLK_KP_PERIOD      = 266
SDLK_KP_DIVIDE      = 267
SDLK_KP_MULTIPLY    = 268
SDLK_KP_MINUS       = 269
SDLK_KP_PLUS        = 270
SDLK_KP_ENTER       = 271
SDLK_KP_EQUALS      = 272
SDLK_UP             = 273
SDLK_DOWN           = 274
SDLK_RIGHT          = 275
SDLK_LEFT           = 276
SDLK_INSERT         = 277
SDLK_HOME           = 278
SDLK_END            = 279
SDLK_PAGEUP         = 280
SDLK_PAGEDOWN       = 281
SDLK_F1             = 282
SDLK_F2             = 283
SDLK_F3             = 284
SDLK_F4             = 285
SDLK_F5             = 286
SDLK_F6             = 287
SDLK_F7             = 288
SDLK_F8             = 289
SDLK_F9             = 290
SDLK_F10            = 291
SDLK_F11            = 292
SDLK_F12            = 293
SDLK_F13            = 294
SDLK_F14            = 295
SDLK_F15            = 296
SDLK_NUMLOCK        = 300
SDLK_CAPSLOCK       = 301
SDLK_SCROLLOCK      = 302
SDLK_RSHIFT         = 303
SDLK_LSHIFT         = 304
SDLK_RCTRL          = 305
SDLK_LCTRL          = 306
SDLK_RALT           = 307
SDLK_LALT           = 308
SDLK_RMETA          = 309
SDLK_LMETA          = 310
SDLK_LSUPER         = 311
SDLK_RSUPER         = 312
SDLK_MODE           = 313
SDLK_COMPOSE        = 314
SDLK_HELP           = 315
SDLK_PRINT          = 316
SDLK_SYSREQ         = 317
SDLK_BREAK          = 318
SDLK_MENU           = 319
SDLK_POWER          = 320
SDLK_EURO           = 321
SDLK_UNDO           = 322
SDLK_LAST           = 323

;key modifiers
KMOD_NONE       = 0x0000
KMOD_LSHIFT     = 0x0001
KMOD_RSHIFT     = 0x0002
KMOD_LCTRL      = 0x0040
KMOD_RCTRL      = 0x0080
KMOD_LALT       = 0x0100
KMOD_RALT       = 0x0200
KMOD_LMETA      = 0x0400
KMOD_RMETA      = 0x0800
KMOD_NUM        = 0x1000
KMOD_CAPS       = 0x2000
KMOD_MODE       = 0x4000
KMOD_RESERVED   = 0x8000
KMOD_CTRL       = KMOD_LCTRL  or KMOD_RCTRL
KMOD_SHIFT      = KMOD_LSHIFT or KMOD_RSHIFT
KMOD_ALT        = KMOD_LALT   or KMOD_RALT
KMOD_META       = KMOD_LMETA  or KMOD_RMETA

;-------------------------------------------------------------------------------

struct SDL_keysym
    stub    dw ?
    scancode    db	0
    sym	    dd	0
    mod_	    dd	0
    unicode	    dw	0
ends

struct SDL_ActiveEvent	
type_	db	?
gain	db	?
state	db	?
ends

struct SDL_KeyboardEvent
stub  dw ?
type_	db	?
which	db	?
state	db	?
keysym	SDL_keysym ?
ends

struct SDL_MouseMotionEvent	
stub  dw ?
type_	db	?
which	db	?
state	db	?
x	dw	?
y	dw	?
xrel	dw	?
yrel	dw	?
ends

struct SDL_MouseButtonEvent	 
type_	db	?
which	db	?
button	db	?
state	db	?
x	dw	?
y	dw	?
ends

struct SDL_JoyAxisEvent	 
type_	db	?
which	db	?
axis	db	?
value	dw	?
ends

struct SDL_JoyBallEvent	 
type_	db	?
which	db	?
ball	db	?
xrel	dw	?
yrel	dw	?
ends

struct SDL_JoyHatEvent	 
type_	db	?
which	db	?
hat	db	?
value	db	?
ends

struct SDL_JoyButtonEvent	 
type_	db	?
which	db	?
button	db	?
state	db	?
ends

struct SDL_ResizeEvent	 
type_	db	?
w	dd	?
h	dd	?
ends

struct SDL_ExposeEvent	 
type_	db	?
ends

struct SDL_QuitEvent	 
type_	db	?
ends

struct SDL_UserEvent	 
type_	db	?
code	dd	?
data1	dd	?
data2	dd	?
ends


struct SDL_SysWMEvent	 
type_	db	?
msg	dd	?
ends

	 
struct	 SDL_Event
	 union
		type	      rb 1
		active	      SDL_ActiveEvent
		key	      SDL_KeyboardEvent
		motion	      SDL_MouseMotionEvent
		button	      SDL_MouseButtonEvent
		jaxis	      SDL_JoyAxisEvent
		jball	      SDL_JoyBallEvent
		jhat	      SDL_JoyHatEvent
		jbutton       SDL_JoyButtonEvent
		resize	      SDL_ResizeEvent
		expose	      SDL_ExposeEvent
		quit	      SDL_QuitEvent
		user	      SDL_UserEvent
		syswm	      SDL_SysWMEvent
	  ends
ends

struct SDL_Rect	 
x	dw	?
y	dw	?
w	dw	?
h	dw	?
ends

struct SDL_Surface
stub  dw ?
flags	dd	?
format	dd	?
w	dd	?
h	dd	?
pitch	dw	?
pixels	dd	?
offset_	dd	?
hwdata dd ?
clip_rect	SDL_Rect	?
unused1	dd	?
locked	dd	?
map dd ?
format_version	dd	?
refcount	dd	?
ends

