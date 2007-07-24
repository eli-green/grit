//
//! \file cldib_core.h
//!  Some DIB utilities (thank you FreeImage)
//! \date 20050823 - 20060816
//! \author cearn
// === NOTES ===
// * 20060816, CLD: added swap_rgb16/24/32
// * 20060725, CLD: moved cross-platform stuff to "winglue.h"
//   Include that instead of <windows.h>
// * 20060725,CLD: INLINE vs inline


#ifndef __CLDIB_CORE_H__
#define __CLDIB_CORE_H__

#ifndef STRICT
#define STRICT
#endif


#include "winglue.h"


// --------------------------------------------------------------------
// MISCELLANEOUS
// --------------------------------------------------------------------


/*! \addtogroup grpMisc
*	\{
*/

#define INLINE static inline	//<! For portable inline functions

// Like all good scientists, I too am a lazy bastard :P
#define BMIH_SIZE		sizeof(BITMAPINFOHEADER)
#define RGB_SIZE		sizeof(RGBQUAD)

#ifndef PAL_MAX
#define PAL_MAX 256
#endif

#ifndef MAX

//! Get the maximum of two values
#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN

//! Get the minimum of two values
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))
#endif

//! In-place swap
#define SWAP2(a, b)			( (b) ^= ((a) ^= ((b) ^= (a))))

//! Swap using a temporary placeholder
#define SWAP3(a, b, tmp)	\
	do { (tmp)=(b); (b)=(a); (a)=(tmp);	} while(0)


// --- utility macros ---

//! Massage \a x for use in bitfield \a name.
#define BF_PREP(x, name)	( ((x)<<name##_SHIFT) & name##_MASK )

//! Get the value of bitfield \a name from \a y. Equivalent to (var=) y.name
#define BF_GET(y, name)		( ((y) & name##_MASK)>>name##_SHIFT )

//! Set bitfield \a name from \a y to \a x: y.name= x.
#define BF_SET(y, x, name)	(y = ((y)&~name##_MASK) | BF_PREP(x,name) )

//! Massage \a x for use in bitfield \a name with pre-shifted \a x
#define BF_PREP2(x, name)	( (x) & name##_MASK )

//! Get the value of bitfield \a name from \a y, but don't down-shift
#define BF_GET2(y, name)	( (y) & name##_MASK )

//! Set bitfield \a name from \a y to \a x with pre-shifted \a x
#define BF_SET2(y, x, name)	(y = ((y)&~name##_MASK) | BF_PREP2(x,name) )


INLINE int clamp(int x, int min, int max);
INLINE int reflect(int x, int min, int max);
INLINE int wrap(int x, int min, int max);


/*!	\}	*/


// --------------------------------------------------------------------
// CONSTANTS
// --------------------------------------------------------------------



/*! \addtogroup grpColor
*	\{
*/

// PONDER: not sure I should do this, but we'll see
#if (BYTE_ORDER == LITTLE_ENDIAN)
	#define CCID_RED   2
	#define CCID_GREEN 1
	#define CCID_BLUE  0
	#define CCID_ALPHA 3
#else	
	#define CCID_RED   0
	#define CCID_GREEN 1
	#define CCID_BLUE  2
	#define CCID_ALPHA 3
#endif

//! Channel identifiers for adjustment functions.
enum eClrChannel
{
	CCE_BLUE = 0x01,	//<! Act on blue.
	CCE_GREEN= 0x02,	//<! Act on green.
	CCE_RED  = 0x04,	//<! Act on red.
	CCE_RGB  = 0x07,	//<! Act on red,Gree,Blue.
	CCE_ALPHA= 0x08,	//<! Act on alpha field
	CCE_RGBA = 0x0F		//<! Act on all fields
};

/*!	\}	/addtogroup	*/


// --------------------------------------------------------------------
// CLASSES
// --------------------------------------------------------------------


/*! \addtogroup grpDibMain
*	\{
*/
//! Main dib struct
typedef struct CLDIB
{	
	BYTE *data;
} CLDIB;

/*!	\}	*/


/*! \addtogroup grpRecord
*	\{
*/

//! Utility struct for passing sized data.
/*!	\note PONDER: use <vector> or class instead?
*/
typedef struct RECORD
{
	int width;		//!< Width of \a data / datatype
	int height;		//!< Height of data / Length of data
	BYTE *data;		//!< Binary data.
} RECORD;

//! Return full size of \a rec in bytes.
INLINE int rec_size(const RECORD *rec)
{ return rec->width*rec->height;	}

//! Shallow (semisafe) copy from \a src to \a dst
/*! \note \a dst's data is freed, but the data still points to 
	  src's data. Yes, this can be a good thing.
*/
INLINE RECORD *rec_alias(RECORD *dst, const RECORD *src)
{	free(dst->data); *dst= *src; return dst;	}


/*!	\}	*/


// --------------------------------------------------------------------
// GLOBALS
// --------------------------------------------------------------------


// --------------------------------------------------------------------
// MACROS
// --------------------------------------------------------------------

/*! \addtogroup grpMisc

*	\{
*/
//! Create standard declaration for loading \a _type images.
#define LOAD_DECL(_type) \
CLDIB *dib_load_##_type##(const char *fpath)

//! Create standard declaration for saving \a _type images.
#define SAVE_DECL(_type) \
BOOL dib_save_##_type##(const char *fpath, CLDIB *dib)

//! Safety routine: free() and NULLify \a _mem pointer.
#define SAFE_FREE(ptr)			do { free(ptr); ptr= NULL; } while(0)

//! Safety routine: delete and NULLify \a _mem pointer.
#define SAFE_DELETE(ptr)		do { delete (ptr); ptr= NULL; } while(0)

//! Safety routine: delete[] and NULLify \a _mem pointer.
#define SAFE_DELETE_ARRAY(ptr)	do { delete[] (ptr); ptr= NULL; } while(0)

//! Round up to next multiple of four.
#define ALIGN4(n) ( ((n)+3)&~3 )


/*!	\}	*/


// --------------------------------------------------------------------
// PROTOTYPES
// --------------------------------------------------------------------

/*! \addtogroup grpColor
*	\{
*/
// --- several useful INLINEs ---
INLINE COLORREF rgb2clr(RGBQUAD rgb);			// RGBQUAD -> COLORREF
INLINE RGBQUAD clr2rgb(COLORREF clr);			// COLORREF -> RGBQUAD
INLINE WORD RGB16(BYTE rr, BYTE gg, BYTE bb);	// x rrrrr ggggg bbbbb

INLINE WORD swap_rgb16(WORD clr);
INLINE RGBTRIPLE swap_rgb24(RGBTRIPLE clr);
INLINE RGBQUAD swap_rgb32(RGBQUAD clr);

/*!	\}	*/


/*! \addtogroup grpDibMain
*	\{
*/

INLINE int dib_align(int width, int bpp);	// align to 32bit boundary


//! \name DIB information
// \{

INLINE int dib_padding(CLDIB *dib);			// # padding bytes
INLINE int dib_get_size(CLDIB *dib);
INLINE BOOL dib_is_topdown(CLDIB *dib);

INLINE int dib_get_width(CLDIB *dib);
INLINE int dib_get_height(CLDIB *dib);		// always >0
INLINE int dib_get_height2(CLDIB *dib);		// signed height
INLINE int dib_get_bpp(CLDIB *dib);
INLINE int dib_get_pitch(CLDIB *dib);
INLINE int dib_get_nclrs(CLDIB *dib);
INLINE int dib_get_size_img(CLDIB *dib);

BOOL dib_get_attr(CLDIB *dib, int *width, int *height, int *bpp, int *pitch);
// \}

//! \name DIB sections
// \{

INLINE BITMAPINFOHEADER *dib_get_hdr(CLDIB *dib);
INLINE BITMAPINFO *dib_get_info(CLDIB *dib);
INLINE RGBQUAD *dib_get_pal(CLDIB *dib);
INLINE BYTE *dib_get_img(CLDIB *dib);

BYTE *dib_get_img_at(CLDIB *dib, int x, int y);

// \}


//! \name General operations
// \{

CLDIB *dib_alloc(int width, int height, int bpp, const BYTE *data, 
	BOOL bTopDown=TRUE);
void dib_free(CLDIB *dib);
CLDIB *dib_clone(CLDIB *src);
INLINE RGBQUAD *dib_pal_cpy(CLDIB *dst, CLDIB *src);
CLDIB *dib_copy(CLDIB *src, int ll, int tt, int rr, int bb, BOOL bClip);

BOOL dib_hflip(CLDIB *dib);
BOOL dib_vflip(CLDIB *dib);		// turns lines, but h >0
BOOL dib_vflip2(CLDIB *dib);	// top-down <-> bottom-up

// \}


#ifdef _MSC_VER

//! \name Windows Blitters
// \{

HBITMAP dib_to_hbm(CLDIB *dib);
CLDIB *dib_from_hbm(HBITMAP hbm);
int dib_to_hdc(HDC hdc, const RECT *rectDest, 
	CLDIB *dib, const RECT *rectSrc, DWORD flags);
int dib_blit(HDC hdc, int dX, int dY, int dW, int dH, 
	CLDIB *dib, int sX, int sY, int sW, int sH, DWORD flags);
int hbm_blit(HDC hdc, int dX, int dY, int dW, int dH, 
	HBITMAP hbm, int sX, int sY, int sW, int sH, DWORD flags);
// \}

#endif	// _MSC_VER


/*!	\}	*/


// --------------------------------------------------------------------
// INLINES
// --------------------------------------------------------------------

// === MISC ===

//! Truncates \a x to stay in range [\a min, \a max>
/*!	\return Truncated value of \a x.
*	\note	\a max is exclusive!
*/
INLINE int clamp(int x, int min, int max)
{	return (x>=max) ? (max-1)		: ( (x<min) ? min		: x );		}

//! Reflects \a x at boundaries \a min and \a max
/*! If \a x is outside the range [\a min, \a max>, 
*	  it'll be placed inside again with the same distance 
*	  to the 'wall', but on the other side. Example for lower 
*	  border: y = \a min - (\a x- \a min)  = 2*\a min + \a x.
*	\return	Reflected value of \a x.
*	\note	\a max is exclusive!
*/
INLINE int reflect(int x, int min, int max)
{	return (x>=max) ? (2*(max-1)-x)	: ( (x<min) ? (2*min-x) : x );		}

//! Wraps \a x to stay in range [\a min, \a max>
INLINE int wrap(int x, int min, int max)
{	return (x>=max) ? (x+min-max)	: ( (x<min) ? (x+max-min) : x );	}


// === 


//! Convert from RGBQUAD to COLORREF
INLINE COLORREF rgb2clr(RGBQUAD rgb)
{	return RGB(rgb.rgbRed, rgb.rgbGreen, rgb.rgbBlue);		}

//! Convert from COLORREF to RGBQUAD
INLINE RGBQUAD clr2rgb(COLORREF clr)
{
	RGBQUAD rgb= { GetBValue(clr), GetGValue(clr), GetRValue(clr), 0};
	return rgb;
}

//! Create 16bit RGB (5.5.5 format)
/*! \note	The ranges of \a rr, \a gg, \a gg are [0, 255] and 
*	  cut down inside the routine.
*/
INLINE WORD RGB16(BYTE rr, BYTE gg, BYTE bb)
{	return (rr/8)*1024 | (gg/8)*32 | (bb/8)*1; }

//! Swap red and blue in a 16bit color
INLINE WORD swap_rgb16(WORD clr)
{
	DWORD tmp= clr&0x7C1F;	// mask R-B
	return (WORD)( (clr&0x83E0)| (tmp<<10) | (tmp>>10) );
}

//! Swap red and blue in an RGBTRIPLE
INLINE RGBTRIPLE swap_rgb24(RGBTRIPLE clr)
{
	RGBTRIPLE tmp= { clr.rgbtRed, clr.rgbtGreen, clr.rgbtBlue };
	return tmp;
}

//! Swap red and blue in an RGBQUAD
INLINE RGBQUAD swap_rgb32(RGBQUAD clr)
{
	RGBQUAD tmp= { clr.rgbRed, clr.rgbGreen, clr.rgbBlue, clr.rgbReserved };
	return tmp;
}


// --- dib misc ---


//! Align to 4-byte boundary.
INLINE int dib_align(int width, int bpp)
{	return ((width*bpp+31)>>5)*4;				}

//! Calculate the number of padding bytes.
INLINE int dib_padding(CLDIB *dib)
{	return ((dib_get_width(dib)*dib_get_bpp(dib))>>3)&3;	}

//! Full size of dib in bytes.
INLINE int dib_get_size(CLDIB *dib)
{	return BMIH_SIZE + dib_get_nclrs(dib)*RGB_SIZE + dib_get_size_img(dib);	}

//! Indicated whether the DIB is top-down or bottom-up (default).
INLINE BOOL dib_is_topdown(CLDIB *dib)
{	return dib_get_height2(dib) < 0;	}

//! Copy the palette of two dibs.
INLINE RGBQUAD *dib_pal_cpy(CLDIB *dst, CLDIB *src)
{
	return (RGBQUAD*)memcpy( dib_get_pal(dst), dib_get_pal(src), 
		MIN(dib_get_nclrs(dst), dib_get_nclrs(src))*RGB_SIZE );
}

// --- dib accessors ---

//! Returns pointer to the BITMAPINFOHEADER struct.
INLINE BITMAPINFOHEADER *dib_get_hdr(CLDIB *dib)
{	return (BITMAPINFOHEADER*)(dib->data);		}

//! Returns pointer to the BITMAPINFO struct.
INLINE BITMAPINFO *dib_get_info(CLDIB *dib)
{	return (BITMAPINFO*)(dib->data);			}

//! Returns pointer to the palette.
/*! \note Even returns non-NULL if there is no real palette. 
      may change in future.
 */
INLINE RGBQUAD *dib_get_pal(CLDIB *dib)
{	return (RGBQUAD*)(&dib->data[BMIH_SIZE]);	}

//! Returns pointer to the image bits
INLINE BYTE *dib_get_img(CLDIB *dib)
{	return &dib->data[BMIH_SIZE+dib_get_nclrs(dib)*RGB_SIZE];	}

// --- dib attributes ---

//! Returns DIB's width in pixels.
INLINE int dib_get_width(CLDIB *dib)
{	return dib_get_hdr(dib)->biWidth;			}

//! Returns DIB's number of scanlines.
INLINE int dib_get_height(CLDIB *dib)
{
	int hh= dib_get_hdr(dib)->biHeight;
	return hh>0 ? hh : -hh;
}

//! Returns the height field; may be negative.
INLINE int dib_get_height2(CLDIB *dib)
{	return dib_get_hdr(dib)->biHeight;			}

//! Returns the bitdepth.
INLINE int dib_get_bpp(CLDIB *dib)
{	return dib_get_hdr(dib)->biBitCount;		}

//! Returns the scanline size in bytes.
INLINE int dib_get_pitch(CLDIB *dib)
{	return dib_align(dib_get_width(dib), dib_get_bpp(dib));		}

//! Returns the number of used palette colors/palette size.
INLINE int dib_get_nclrs(CLDIB *dib)
{	return dib_get_hdr(dib)->biClrUsed;			}

//! Returns the number of bytes of the image itself.
INLINE int dib_get_size_img(CLDIB *dib)
{	return dib_get_hdr(dib)->biSizeImage;		}


#endif // __CLDIB_CORE_H__

// EOF