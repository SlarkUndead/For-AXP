
#ifndef _MFC_HISTOGRAM_060701_H_
#define _MFC_HISTOGRAM_060701_H_

#include "MFCArray.h"
#include "MFCBaseFunc.h"
//=============================================================================
// Calculate histogram(Öù×´Í¼) of image.
class CMCMFCHistogram
{
public:
	// Calculate img's histogram, img's bpp >= 24.
	CMCMFCHistogram(const CMFCImageObj& img) ;

	// Analyze image (only 24bpp or 32bpp image can be analyzed).
	void AnalyzeImage(const CMFCImageObj& img) ;

	// name Get/Set histogram Stat. range.
	int GetStart(void) const {return m_nStart;}					// Get start position.
	int GetEnd(void) const {return m_nEnd;}						// Get end position.
	void SetStart(int nStart){m_nStart = MFCClamp0255(nStart);}	// Set start position.
	void SetEnd(int nEnd) {m_nEnd = MFCClamp0255(nEnd);}		// Set end position.


	//  Query attributes.nChannel can be MFC_CHANNEL_GRAY, MFC_CHANNEL_RED, MFC_CHANNEL_GREEN, MFC_CHANNEL_BLUE
	// Get pixel count of Stat.
	int GetPixelNumber(void)const{return m_nPixelNum;}
	// Pixel count of selected region (nChannel).
	int GetCount(MFC_IMAGE_CHANNEL nChannel = MFC_CHANNEL_GRAY) const ;
	// Pixel count of nValue position (nChannel).
	int GetValueCount(int nValue, MFC_IMAGE_CHANNEL nChannel = MFC_CHANNEL_GRAY) const ;
	// Pixel average of selected region (nChannel).
	int GetAverage(MFC_IMAGE_CHANNEL nChannel = MFC_CHANNEL_GRAY) const ;
	// Get max count in selected region (nChannel).
	int GetMaxCount(MFC_IMAGE_CHANNEL nChannel = MFC_CHANNEL_GRAY) const ;
	// Received img is 24bpp, size is (256,nHeight)
	void GetHistogramImage(int nHeight,CMFCImageObj* img,MFC_IMAGE_CHANNEL nChannel = MFC_CHANNEL_GRAY) const ;


private:
	const CMFCArray<int>& GetChannelHistogram(MFC_IMAGE_CHANNEL nChannel) const
	{
		switch(nChannel)
		{
			case MFC_CHANNEL_RED  :  return m_HisRed ;
			case MFC_CHANNEL_GREEN:  return m_HisGreen ;
			case MFC_CHANNEL_BLUE :  return m_HisBlue ;
		}
		return m_HisGray ;
	}

private:
	CMFCArray<int>   m_HisGray, m_HisRed, m_HisGreen, m_HisBlue ;
	int     m_nStart ;
	int     m_nEnd ;
	int     m_nPixelNum ;
};

//=============================================================================
// inline Implement
//=============================================================================
inline CMFCHistogram::CMFCHistogram (const CMFCImageObj& img) : m_HisGray(256), m_HisRed(256), m_HisGreen(256), m_HisBlue(256)
{
	m_nStart = 0 ;
	m_nEnd = 255 ;
	m_nPixelNum = 0 ;
	AnalyzeImage(img) ;
}
//-----------------------------------------------------------------------------
inline void CMFCHistogram::AnalyzeImage(const CMFCImageObj& img)
{
	if (!img.IsValidImage() || (img.ColorBits() < 24))
		return;

	for (int i=0 ; i < 256 ; i++)
	{
		m_HisGray[i] = m_HisRed[i] = m_HisGreen[i] = m_HisBlue[i] = 0 ;
	}

	m_nPixelNum = img.Width()*img.Height() ;
	for(int y=0 ; y < img.Height() ; y++)
	{
		for(int x=0 ; x <img.Width() ; x++)
		{
			BYTE   * p = img.GetBits(x,y) ;
			m_HisBlue[MFC_B(p)]++ ;
			m_HisGreen[MFC_G(p)]++ ;
			m_HisRed[MFC_R(p)]++ ;
			m_HisGray[FCColor::GetGrayscale(p)]++ ;
		}
	}
}
//-----------------------------------------------------------------------------
inline int CMFCHistogram::GetCount(MFC_IMAGE_CHANNEL nChannel) const
{
	int nCount = 0 ;
	for(int i=m_nStart ; i <= m_nEnd ; i++)
	{
		nCount += GetChannelHistogram(nChannel)[i] ;
	}
	return nCount ;
}
//-----------------------------------------------------------------------------
inline int CMFCHistogram::GetValueCount(int nValue, MFC_IMAGE_CHANNEL nChannel) const
{
	return GetChannelHistogram(nChannel)[MFCClamp0255(nValue)] ;
}
//-----------------------------------------------------------------------------
inline int CMFCHistogram::GetAverage(MFC_IMAGE_CHANNEL nChannel) const
{
	double fAverage = 0 ;
	int nCount = 0 ;
	for(int i=m_nStart ; i <= m_nEnd ; i++)
	{
		int   n = GetChannelHistogram(nChannel)[i] ;
		nCount += n ;
		fAverage = fAverage + n * i ;
	}
	return nCount ? (int)(fAverage/nCount) : 0 ;
}
//-----------------------------------------------------------------------------
inline int CMFCHistogram::GetMaxCount(MFC_IMAGE_CHANNEL nChannel) const
{
	int nMaxCount = 0 ,n = 0;
	for (int i=m_nStart ; i <= m_nEnd ; i++)
	{
		n = GetChannelHistogram(nChannel)[i] ;
		if(n > nMaxCount)
			nMaxCount = n ;
	}
	return nMaxCount ;
}
//-----------------------------------------------------------------------------
inline void CMFCHistogram::GetHistogramImage (int nHeight, CMFCImageObj* img, MFC_IMAGE_CHANNEL nChannel) const
{
	if(!img || !img->Create (256, nHeight, 24))
		return ;

	// fill white back
	memset(img->GetMemStart(), 0xFF, img->GetImgPitch() * img->GetImgHeight()) ;

	int     nMaxCount = GetMaxCount(nChannel) ;
	if(nMaxCount == 0)
		return ;

	for(int x=0 ; x < img->Width() ; x++)
	{
		int nFill = FClamp (GetChannelHistogram(nChannel)[x]*img->GetImgHeight()/nMaxCount, 0, img->GetImgHeight()) ;
		for(int i=0 ; i < nFill ; i++)
		{
			BYTE   * p = img->GetBits(x, img->GetImgHeight() - 1 - i) ;
			MFC_R(p) = MFC_G(p) = MFC_B(p) = 0 ;
		}
	}
}
#endif