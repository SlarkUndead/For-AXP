
#include "stdafx.h"
#include "MFCCompressHelper.h"
#include "../MFCColor.h"

//=============================================================================
int CMFCCompressHelper::RLE_PCX_Encode(const BYTE* pInBuffer,int nInSize,BYTE* pOutBuffer)
{
	const BYTE   * pOutStart = pOutBuffer ;
	while(nInSize-- > 0)
	{
		const BYTE byData = *pInBuffer++ ;
		BYTE cCount = 1 ;
		while( (cCount < 0x3F) && (nInSize != 0) )
			if(*pInBuffer != byData) // Stat. the repeat BYTE
				break ;
			else
			{
				cCount++ ; pInBuffer++ ; nInSize-- ;
			}

			if(cCount == 1) // unique
			{
				if( byData >= 0xC0 ) // Data >= 0xC0
				{
					*pOutBuffer++ = 0xC1 ;
					*pOutBuffer++ = byData ;
				}
				else
					*pOutBuffer++ = byData ; // Data < 0xC0, write directly
			}
			else // repeat
			{
				*pOutBuffer++ = 0xC0 | cCount ;
				*pOutBuffer++ = byData ;
			}
	}
	return (int)(pOutBuffer - pOutStart) ;
}
//-----------------------------------------------------------------------------
int CMFCCompressHelper::RLE_PCX_Decode(const BYTE* pInBuffer,int nInSize,BYTE* pOutBuffer)
{
	const BYTE * pOutStart = pOutBuffer ;
	while(nInSize-- > 0)
	{
		const BYTE byData = *pInBuffer++ ; // read byte and move ptr to next
		if( byData >= 0xC0 )
		{
			// error : the inbuffer has been exhausted.
			if(nInSize <= 0)
				goto rleOver;

			BYTE cNum = byData & 0x3F ; // repeat current byte Num
			::memset (pOutBuffer, *pInBuffer++, cNum) ; // memset func will check "Num" =? 0
			pOutBuffer += cNum ;
			nInSize-- ;
		}
		else
			*pOutBuffer++ = byData ;
	}
rleOver:
	return (int)(pOutBuffer - pOutStart) ;
}
//-----------------------------------------------------------------------------
BYTE* CMFCCompressHelper::RLE_TGA_EncodeLine(const BYTE* InBuffer,int iColorBit,int iNumPixel,BYTE* OutBuffer)
{
	iColorBit /= 8 ; // convert to bytes : 1,2,3,4
	while(iNumPixel > 0)
	{
		DWORD Data = 0, Next = 0, Count = 1 ;
		CMFCColor::CopyPixel (&Data, InBuffer, iColorBit) ; // first pixel
		InBuffer += iColorBit ; iNumPixel-- ;
		while((Count < 0x80) && (iNumPixel > 0)) // Stat. the repeat pixel
		{
			CMFCColor::CopyPixel (&Next, InBuffer, iColorBit) ; // next pixel
			if (Next != Data)
				break ;
			InBuffer += iColorBit ; iNumPixel-- ; Count++ ;
		}

		*OutBuffer++ = 0x80 | (BYTE)--Count ;
		CMFCColor::CopyPixel (OutBuffer, &Data, iColorBit) ;
		OutBuffer += iColorBit ;
	}
	return OutBuffer ;
}
//-----------------------------------------------------------------------------
BYTE* CMFCCompressHelper::RLE_TGA_DecodeLine(const BYTE* InBuffer,int iColorBit,int iNumPixel,BYTE* OutBuffer)
{
	iColorBit /= 8 ; // convert to bytes : 1,2,3,4
	while(iNumPixel > 0)
	{
		const BYTE byData = *InBuffer++ ; // Next Byte
		if(byData & 0x80) // Data >= 0x80
		{
			const int nNum = (byData & 0x7F) + 1 ; // number of repeat pixel
			iNumPixel -= nNum ;
			for(int i=0 ; i < nNum ; i++, OutBuffer += iColorBit)
			{
				CMFCColor::CopyPixel(OutBuffer,InBuffer,iColorBit) ;
			}
			InBuffer += iColorBit ;
		}
		else
		{
			// copy directly
			const int n = byData + 1, // non-repeat pixel
				nByte = n * iColorBit ; // calculate copy bytes
			iNumPixel -= n ;
			::memcpy(OutBuffer, InBuffer, nByte) ;
			OutBuffer += nByte ;
			InBuffer += nByte ;
		}
	}
	return const_cast<BYTE*>(InBuffer) ;
}