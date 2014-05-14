FIT_UNKNOWN = 0	;// unknown type
FIT_BITMAP  = 1	;// standard image			: 1-, 4-, 8-, 16-, 24-, 32-bit
FIT_UINT16	= 2	;// array of unsigned short	: unsigned 16-bit
FIT_INT16	= 3	;// array of short			: signed 16-bit
FIT_UINT32	= 4	;// array of unsigned long	: unsigned 32-bit
FIT_INT32	= 5	;// array of long			: signed 32-bit
FIT_FLOAT	= 6	;// array of float			: 32-bit IEEE floating point
FIT_DOUBLE	= 7	;// array of double			: 64-bit IEEE floating point
FIT_COMPLEX	= 8	;// array of FICOMPLEX		: 2 x 64-bit IEEE floating point
FIT_RGB16	= 9	;// 48-bit RGB image			: 3 x 16-bit
FIT_RGBA16	= 10	;// 64-bit RGBA image		: 4 x 16-bit
FIT_RGBF	= 11	;// 96-bit RGB float image	: 3 x 32-bit IEEE floating point
FIT_RGBAF	= 12	;// 128-bit RGBA float image	: 4 x 32-bit IEEE floating point