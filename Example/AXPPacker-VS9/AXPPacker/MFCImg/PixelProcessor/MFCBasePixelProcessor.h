#ifndef _MFC_BASE_PIXEL_PROCESSOR_06_07_09_H_
#define _MFC_BASE_PIXEL_PROCESSOR_06_07_09_H_
#include "../MFCImageObj.h"
#include "../MFCHistogram.h"
#include "../MFCColor.h"

//class FCInterface_PixelProcess ;
class CMFCBasePixelProcess ;
    class CMFCPixelConvertTo16Bit ; // 1, 4, 8, 24, 32 ==> 16
    class CMFCPixelConvertTo8BitGray ; // 1, 4, 8, 16, 24, 32 ==> 8bit gray
    class CMFCPixelGrayscale ; // gray scale (>=24 bit)
    class CMFCPixelFillColor ; // Fill color (>=24 bit)
    class CMFCPixelHueSaturation ; // hue saturation (>=24 bit)]
    class CMFCPixelMirror ; // mirror (>=8 bit)
    class CMFCPixelFlip ; // flip (>=8 bit)
    class CMFCPixelShift ; // shift (>=24 bit)
    class CMFCPixelAutoContrast ; // auto contrast (>=24 bit)
    class CMFCPixelAutoColorEnhance ; // auto color enhance (>=24 bit)
    class CMFCPixelEmboss ; // emboss (>=24 bit)
    class CMFCPixelIllusion ; // illusion (>=24 bit)
    class CMFCPixelBlinds ; // blinds (>=24 bit)
    class CMFCPixelMosaic ; // mosaic (32 bit)
    class CMFCPixelFill3DSolidFrame ; // fill 3D solid frame (>=24 bit)
    class CMFCPixelAdjustRGB ; // adjust RGB (>=24 bit)
    class CMFCPixelColorLevel ; // color level (>=24 bit)
    class CMFCPixelThreshold ; // threshold (>=24 bit)
    class CMFCPixelRotate90 ; // clockwise rotate 90' (>=8 bit)
        class CMFCPixelRotate270 ; // clockwise rotate 270' (>=8 bit)
    class CMFCPixelDeinterlace ; // de-interlace (32 bit)
    class CMFCPixelHalftoneM3 ; // halftone (>=24 bit)
    class CMFCPixelOilPaint ; // oil paint (>=24 bit)
    class CMFCPixelColorTone ; // color tone (>=24 bit)
    class CMFCPixelAddRandomNoise ; // add random noise (>=24 bit)
    class CMFCPixelSplash ; // splash (>=24 bit)
    class CMFCPixelVideo ; // video (>=24 bit)
    class CMFCPixelColorBalance ; // color balance (>=24 bit)
    class CMFCPixelFillGrid ; // fill grid (>=24 bit)
    class CMFCPixel3DGrid ; // add 3D grid (>=24 bit)
    class CMFCPixelMedianFilter ; // Median filter (>=24 bit)
    class CMFCPixelSpliteChannel_RGB ; // splite RGB channel (>=24 bit)
    class CMFCPixelCombineChannel_RGB ; // combine RGB channel (>=24 bit)
    class CMFCPixelConvolute ; // image convolute (>= 24 bit)
        class CMFCPixelGaussianBlur3x3 ; // Standard 3x3 gaussian blur (>=24 bit)
        class CMFCPixelGaussianBlur5x5 ; // Standard 5x5 gaussian blur (>=24 bit)
        class CMFCPixelDetectEdges ; // Detect edges (>=24 bit)
        class CMFCPixelSharp ; // Sharp (laplacian template) (>=24 bit)
    class CMFCPixelGradientBase ; // base class of gradient fill (>=24 bit)
        class CMFCPixelGradientLine ; // gradient fill linear (>=24 bit)
            class CMFCPixelGradientBiLine ; // gradient fill bilinear (>=24 bit)
            class CMFCPixelGradientConicalSym ; // gradient fill symmetric conical (>=24 bit)
            class CMFCPixelGradientConicalASym ; // gradient fill Anti-symmetric conical (>=24 bit)
        class CMFCPixelGradientRect ; // gradient fill rect (>=24 bit)
        class CMFCPixelGradientRadial ; // gradient fill radial (>=24 bit)
    class CMFCPixelBilinearDistord ; // bilinear distord (>=24 bit)
        class CMFCPixelCylinder ; // cylinder (>=24 bit)
        class CMFCPixelWave ; // wave (>=24 bit)
        class CMFCPixelWhirlPinch ; // whirl & pinch (>=24 bit)
        class CMFCPixelFractalTrace ; // Fractal trace (>=24 bit)
        class CMFCPixelLens ; // lens (>=24 bit)
        class CMFCPixelSkew ; // skew transform (>=24 bit)
        class CMFCPixelPerspective ; // perspective transform (>=24 bit)
        class CMFCPixelRotate ; // rotate (>=24 bit)
        class CMFCPixelRibbon ; // ribbon (>=24 bit)
        class CMFCPixelRipple ; // ripple (>=24 bit)
        class CMFCPixelSmallTile ; // tile (>=24 bit)
    class CMFCPixelLUTRoutine ; // LUT(look up table) routine (>=24 bit)
        class CMFCPixelBrightness ; // adjust brightness (>=24 bit)
        class CMFCPixelContrast ; // adjust contrast (>=24 bit)
        class CMFCPixelGamma ; // adjust gamma (>=24 bit)
        class CMFCPixelInvert ; // negate (>=24 bit)
        class CMFCPixelSolarize ; // Solarize (>=24 bit)
        class CMFCPixelPosterize ; // posterize (>=24 bit)
    class CMFCPixelColorsCount ; // count image's number of color (>=24 bit)
        class CMFCPixelGetKeyColor ; // Find a color unused in image (>=24 bit)
    class CMFCPixelExportAscII ; // save a ASCII text file (>=24 bit)

//=============================================================================
// Base class of processor.
class CMFCBasePixelProcess : public CMFCIPixelProcessor
{
public:
    CMFCBasePixelProcess() : m_pImgOld(0) {}
    virtual ~CMFCBasePixelProcess() {if(m_pImgOld)  delete m_pImgOld;}

    // whether the image can be disposed by this processor.
    // default test image's bpp >= 24
    virtual bool ValidateColorBits(const CMFCImageObj* pImg)
    {
        return pImg->IsValidImage() && (pImg->GetImgColorBits() >= 24) ;
    }

protected:
    void SetBackupImage(const CMFCImageObj* pImg)
    {
        if(pImg)
        {
            if (m_pImgOld)
                delete m_pImgOld ;
            m_pImgOld = new CMFCImageObj(*pImg) ;
        }
    }
    CMFCImageObj* GetBackupImage() const {return m_pImgOld;}

private:
    CMFCImageObj * m_pImgOld ; // backup image
};

//=============================================================================
// 1, 4, 8, 24, 32 ==> 16.
// example:
//        CMFCPixelConvertTo16Bit aCmd ;
//        img.PixelProcessProc(aCmd) ;
class CMFCPixelConvertTo16Bit : public CMFCBasePixelProcess
{
    virtual bool ValidateColorBits (const CMFCImageObj* pImg) {return pImg->IsValidImage() && (pImg->GetImgColorBits() != 16) ;}
    virtual void OnEnterProcess(CMFCImageObj* pImg)
    {
        SetBackupImage(pImg) ;

        // make it easier, now we only need 24(32) ==> 16
        if(pImg->GetImgColorBits() <= 8)
            GetBackupImage()->ConvertTo24Bit() ;
        pImg->Create (pImg->GetImgWidth(), pImg->GetImgHeight(), 16) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        *(WORD*)pPixel = Combine16Bit_555 (GetBackupImage()->GetBits(x,y)) ; // 24,32 ==> 16
    }
    static WORD Combine16Bit_555 (const void* pRGB)
    {
        const WORD   wR = ((PCL_R(pRGB) >> 3) << 10),
                     wG = ((PCL_G(pRGB) >> 3) << 5),
                     wB =  (PCL_B(pRGB) >> 3) ;
        return (wR | wG | wB) ;
    }
};

//=============================================================================
/**
 *  1, 4, 8, 16, 24, 32 ==> 8bit gray.
@verbatim
    example:
        CMFCPixelConvertTo8BitGray   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelConvertTo8BitGray : public CMFCBasePixelProcess
{
    virtual bool ValidateColorBits (const CMFCImageObj* pImg) {return pImg->IsValidImage();}
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage(pImg) ;

        // make it easier, now we only need 24(32) ==> 8bit gray
        if (pImg->GetImgColorBits() <= 16)
            GetBackupImage()->ConvertTo24Bit() ;
        pImg->Create (pImg->GetImgWidth(), pImg->GetImgHeight(), 8) ; // default to set a gray palette
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        *pPixel = FCColor::GetGrayscale (GetBackupImage()->GetBits(x,y)) ;
    }
};

//=============================================================================
/**
 *  Gray scale image (>=24 bit).
 *  all channel are same after process.
@verbatim
    example:
        CMFCPixelGrayscale   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGrayscale : public CMFCBasePixelProcess
{
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        PCL_R(pPixel) = PCL_G(pPixel) = PCL_B(pPixel) = FCColor::GetGrayscale(pPixel) ;
    }
};

//=============================================================================
/**
 *  Fill color (>=24 bit).
@verbatim
    example:
        const RGBQUAD      cr = PCL_RGBA(0,0,255) ;
        CMFCPixelFillColor   aCmd (cr, 192) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelFillColor : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nAlpha == -1, not fill alpha
    CMFCPixelFillColor (RGBQUAD crFill, int nAlpha=-1) : m_crFill(crFill), m_nAlpha(nAlpha), m_bIsFillAlpha(false) {}
private:
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        if (pImg->IsValidImage() && (pImg->GetImgColorBits() >= 24))
        {
            m_bIsFillAlpha = ((m_nAlpha != -1) && (pImg->GetImgColorBits() == 32)) ;
            PCL_A(&m_crFill) = m_bIsFillAlpha ? FClamp0255(m_nAlpha) : 0 ;
            return true ;
        }
        return false ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        FCColor::CopyPixel (pPixel, &m_crFill, m_bIsFillAlpha ? 4 : 3) ;
    }

    RGBQUAD   m_crFill ;
    int       m_nAlpha ;
    bool      m_bIsFillAlpha ;
};

//=============================================================================
/**
 *  Adjust image's hue & saturation (>=24 bit).
@verbatim
    example:
        CMFCPixelHueSaturation   aCmd (100, 150) ; // not change hue, add 50% saturation
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelHueSaturation : public CMFCBasePixelProcess
{
public:
    /// Constructor (param's unit is percentage).
    CMFCPixelHueSaturation (int nPercentHue, int nPercentSat) : m_nPercentHue(FMax(0,nPercentHue)), m_nPercentSat(FMax(0,nPercentSat)) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        double      H, L, S ;
        FCColor::RGBtoHLS (pPixel, &H, &L, &S) ;
        if (m_nPercentHue != 100)  H = H * m_nPercentHue / 100 ;
        if (m_nPercentSat != 100)  S = S * m_nPercentSat / 100 ;
        RGBQUAD     cr = FCColor::HLStoRGB (H, L, S) ;
        FCColor::CopyPixel (pPixel, &cr, 3) ;
    }

    int   m_nPercentHue, m_nPercentSat ;
};

//=============================================================================
/**
 *  Left-Right mirror image (>=8 bit).
@verbatim
    example:
        CMFCPixelMirror   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelMirror : public CMFCBasePixelProcess
{
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        return pImg->IsValidImage() && (pImg->GetImgColorBits() >= 8) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if (x < pImg->GetImgWidth()/2)
        {
            BYTE   * pRight = pImg->GetBits (pImg->GetImgWidth()-1-x, y) ;
            for (int i=0 ; i < pImg->GetImgColorBits()/8 ; i++)
                FSwap (pPixel[i], pRight[i]) ; // bytes of per pixel
        }
    }
};

//=============================================================================
/**
 *  Top-Bottom flip image (>=8 bit).
@verbatim
    example:
        CMFCPixelFlip   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelFlip : public CMFCBasePixelProcess
{
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        return pImg->IsValidImage() && (pImg->GetImgColorBits() >= 8) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if (y < pImg->GetImgHeight()/2)
        {
            BYTE   * pBottom = pImg->GetBits (x, pImg->GetImgHeight()-1-y) ;
            for (int i=0 ; i < pImg->GetImgColorBits()/8 ; i++)
                FSwap (pPixel[i], pBottom[i]) ; // bytes of per pixel
        }
    }
};

//=============================================================================
/**
 *  Shift (>=24 bit).
@verbatim
    example:
        ProcessPixel   aCmd(5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelShift : public CMFCBasePixelProcess
{
public:
    CMFCPixelShift (int nAmount) : m_nAmount (FMax(0,nAmount)) {srand((unsigned int)time(0));}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        const int    nSpan = pImg->GetImgColorBits() / 8 ;
        int          nShift = rand() % (m_nAmount+1) ;

        // first pixel every line
        if (x || !m_nAmount || !nShift)
            return ;

        BYTE     crLeft[4], crRight[4] ; // L/R edge pixel color
        FCColor::CopyPixel (crLeft, pImg->GetBits(y), nSpan) ;
        FCColor::CopyPixel (crRight, pImg->GetBits(pImg->GetImgWidth()-1,y), nSpan) ;
        if (rand() % 2)
        {
            // shift right
            if (pImg->GetImgWidth() > nShift)
                memmove (pImg->GetBits(nShift,y), pPixel, (pImg->GetImgWidth()-nShift)*nSpan) ;
            else
                nShift = pImg->GetImgWidth() ;
            for (int i=0 ; i < nShift ; i++, pPixel+=nSpan)
                FCColor::CopyPixel (pPixel, crLeft, nSpan) ;
        }
        else
        {
            // shift left
            if (pImg->GetImgWidth() > nShift)
                memmove (pPixel, pImg->GetBits(nShift,y), (pImg->GetImgWidth()-nShift)*nSpan) ;
            else
                nShift = pImg->GetImgWidth() ;
            pPixel = pImg->GetBits(pImg->GetImgWidth()-1, y) ;
            for (int i=0 ; i < nShift ; i++, pPixel-=nSpan)
                FCColor::CopyPixel (pPixel, crRight, nSpan) ;
        }
    }

    int     m_nAmount ; // max shift pixel
};

//=============================================================================
/**
 *  Auto contrast (>=24 bit).
@verbatim
    example:
        CMFCPixelAutoContrast   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelAutoContrast : public CMFCBasePixelProcess
{
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        BYTE     byCmin[3] = {255, 255, 255},
                 byCmax[3] = {0, 0, 0} ;

        // Get minimum and maximum values for each channel
        for (int y=0 ; y < pImg->GetImgHeight() ; y++)
            for (int x=0 ; x < pImg->GetImgWidth() ; x++)
            {
                BYTE   * pPixel = pImg->GetBits(x,y) ;
                for (int b=0 ; b < 3 ; b++)
                {
                    if (pPixel[b] < byCmin[b])  byCmin[b] = pPixel[b] ;
                    if (pPixel[b] > byCmax[b])  byCmax[b] = pPixel[b] ;
                }
            }

        // Calculate LUTs with stretched contrast
        for (int b=0 ; b < 3 ; b++)
        {
            const int     nRange = byCmax[b] - byCmin[b] ;
            if (nRange)
            {
                for (int x=byCmin[b] ; x <= byCmax[b] ; x++)
                    m_byLut[x][b] = 255 * (x - byCmin[b]) / nRange ;
            }
            else
                m_byLut[byCmin[b]][b] = byCmin[b] ;
        }
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        for (int b=0 ; b < 3 ; b++)
            pPixel[b] = m_byLut[pPixel[b]][b] ;
    }

    BYTE     m_byLut[256][3] ;
};

//=============================================================================
/**
 *  Auto color enhance (>=24 bit).
@verbatim
    example:
        CMFCPixelAutoColorEnhance   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelAutoColorEnhance : public CMFCBasePixelProcess
{
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        m_vhi = 0.0 ; m_vlo = 1.0 ;

        // Get minimum and maximum values
        for (int y=0 ; y < pImg->GetImgHeight() ; y++)
            for (int x=0 ; x < pImg->GetImgWidth() ; x++)
            {
                BYTE    * pPixel = pImg->GetBits(x,y) ;
                int     c = 255 - PCL_B(pPixel),
                        m = 255 - PCL_G(pPixel),
                        y = 255 - PCL_R(pPixel),
                        k = c ;
                if (m < k)  k = m ;
                if (y < k)  k = y ;

                BYTE    byMap[4] = { c-k, m-k, y-k } ;
                double  h, z, v ;
                FCColor::RGBtoHSV (byMap, &h, &z, &v) ;
                if (v > m_vhi)  m_vhi = v ;
                if (v < m_vlo)  m_vlo = v ;
            }
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int nx, int ny, BYTE* pPixel)
    {
        int     c = 255 - PCL_B(pPixel),
                m = 255 - PCL_G(pPixel),
                y = 255 - PCL_R(pPixel),
                k = c ;
        if (m < k)  k = m ;
        if (y < k)  k = y ;

        BYTE    byMap[4] = { c-k, m-k, y-k } ;
        double  h, z, v ;
        FCColor::RGBtoHSV (byMap, &h, &z, &v) ;
        if (m_vhi != m_vlo)
            v = (v-m_vlo) / (m_vhi-m_vlo) ;
        *(RGBQUAD*)byMap = FCColor::HSVtoRGB (h, z, v) ;
        c = byMap[0] ; m = byMap[1] ; y = byMap[2] ;
        c += k ; if (c > 255)  c = 255 ;
        m += k ; if (m > 255)  m = 255 ;
        y += k ; if (y > 255)  y = 255 ;
        PCL_B(pPixel) = 255 - c ;
        PCL_G(pPixel) = 255 - m ;
        PCL_R(pPixel) = 255 - y ;
    }

    double     m_vhi, m_vlo ;
};

//=============================================================================
/**
 *  Emboss effect (>=24 bit).
@verbatim
    example:
        CMFCPixelEmboss   aCmd(5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelEmboss : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelEmboss (int nLevel) : m_nLevel(nLevel) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nGray ;
        if (x == pImg->GetImgWidth()-1)
            nGray = 128 ; // the last right col pixel
        else
        {
            BYTE  * pNext = pImg->GetBits (x+1, y) ;
            int   nSum = PCL_R(pPixel) + PCL_G(pPixel) + PCL_B(pPixel),
                  nSumNx = PCL_R(pNext) + PCL_G(pNext) + PCL_B(pNext) ;
            nGray = FClamp0255 (m_nLevel * (nSum - nSumNx) / 3 + 128) ;
        }
        PCL_R(pPixel) = PCL_G(pPixel) = PCL_B(pPixel) = nGray ;
    }

    int     m_nLevel ;
};

//=============================================================================
/**
 *  Illusion effect (>=24 bit).
@verbatim
    example:
        CMFCPixelIllusion   aCmd(3) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelIllusion : public CMFCBasePixelProcess
{
public:
    CMFCPixelIllusion (int nAmount) : m_nAmount(nAmount) {}
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;

        m_fScale = FHypot ((double)pImg->GetImgWidth(),(double)pImg->GetImgHeight()) / 2.0 ;
        m_fOffset = (int)(m_fScale / 2.0) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        const double   fXCen = pImg->GetImgWidth() / 2.0,
                       fYCen = pImg->GetImgHeight() / 2.0,
                       cx = (x - fXCen) / m_fScale,
                       cy = (y - fYCen) / m_fScale,
                       fTmp = LIB_PI / (double)m_nAmount ;

        double   angle = floor (atan2(cy,cx) / 2.0 / fTmp) * 2.0 * fTmp + fTmp ;
        double   radius = FHypot (cx, cy) ;
        
        int   xx = (int)(x - m_fOffset * cos (angle)),
              yy = (int)(y - m_fOffset * sin (angle)) ;
        xx = FClamp (xx, 0, pImg->GetImgWidth()-1) ;
        yy = FClamp (yy, 0, pImg->GetImgHeight()-1) ;

        const BYTE   * pPixel2 = GetBackupImage()->GetBits (xx, yy) ;
        for (int i=0 ; i < pImg->GetImgColorBits()/8 ; i++)
            pPixel[i] = FClamp0255 (pPixel[i] + (int)(radius * (pPixel2[i] - pPixel[i]))) ;
    }

    int     m_nAmount ;
    double  m_fScale, m_fOffset ;
};

//=============================================================================
/**
 *  Blind effect (>=24 bit).
@verbatim
    example:
        CMFCPixelBlinds   aCmd(AXIS_X, 10, 50, PCL_RGBA(0,0,255)) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelBlinds : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nOpacity : percentage [1,100]
    CMFCPixelBlinds (AXIS_SYS nDirect, int nWidth, int nOpacity, RGBQUAD crBlind) : m_nWidth(nWidth), m_crBlind(crBlind)
    {
        m_nOpacity = FClamp (nOpacity, 1, 100) ;
        m_nDirect = nDirect ;
        if ((m_nDirect != AXIS_X) && (m_nDirect != AXIS_Y))
            m_nDirect = AXIS_X ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        int     nMaxWidth = FMax (pImg->GetImgWidth(), pImg->GetImgHeight()) ;
        m_nWidth = FClamp (m_nWidth, 2, nMaxWidth) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nMod ;
        if (m_nDirect == AXIS_X) // horizontal direction
            nMod = y % m_nWidth ;
        else if (m_nDirect == AXIS_Y) // vertical direction
            nMod = x % m_nWidth ;

        double     fAlphaAdd = 255.0 * m_nOpacity/100.0 / (m_nWidth-1.0) ;
        FCColor::AlphaBlendPixel (pPixel, (BYTE*)&m_crBlind, FClamp0255((int)(nMod * fAlphaAdd))) ;
    }

    AXIS_SYS    m_nDirect ;
    int         m_nWidth ;
    int         m_nOpacity ;
    RGBQUAD     m_crBlind ;
};

//=============================================================================
/**
 *  Mosaic effect (32 bit).
@verbatim
    example:
        CMFCPixelMosaic   aCmd(5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelMosaic : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nBlock : pixel width of block
    CMFCPixelMosaic (int nBlock) : m_nBlock(FMax(2,nBlock)) {}
private:
    virtual bool ValidateColorBits (const CMFCImageObj* pImg) {return pImg->IsValidImage() && (pImg->GetImgColorBits() == 32);}
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if ((x % m_nBlock == 0) && (y % m_nBlock == 0))
        {
            RGBQUAD   cr = GetBlockAverage(x, y) ;
            FCColor::CopyPixel (pPixel, &cr, 4) ;
        }
        else
            FCColor::CopyPixel (pPixel, pImg->GetBits(x/m_nBlock*m_nBlock, y/m_nBlock*m_nBlock), 4) ;
    }
    RGBQUAD GetBlockAverage (int x, int y)
    {
        RECT   rc = {x, y, x+m_nBlock, y+m_nBlock} ;
        GetBackupImage()->BoundRect(rc) ;

        int      nNum = RECTWIDTH(rc) * RECTHEIGHT(rc) ;
        double   nSumR=0, nSumG=0, nSumB=0, nSumA=0 ;
        for (int yy=rc.top ; yy < rc.bottom ; yy++)
        {
            for (int xx=rc.left ; xx < rc.right ; xx++)
            {
                BYTE   * p = GetBackupImage()->GetBits (xx, yy) ;
                nSumB += PCL_B(p)*PCL_A(p) ; nSumG += PCL_G(p)*PCL_A(p) ;
                nSumR += PCL_R(p)*PCL_A(p) ; nSumA += PCL_A(p) ;
            }
        }
        return PCL_RGBA (nSumA ? (int)(nSumR / nSumA) : 0,
                         nSumA ? (int)(nSumG / nSumA) : 0,
                         nSumA ? (int)(nSumB / nSumA) : 0,
                         nNum ? (int)(nSumA / nNum) : 0xFF) ;
    }

private:
    int     m_nBlock ;
};

//=============================================================================
/**
 *  Fill a 3D solid frame (>=24 bit).
@verbatim
    example:
        CMFCPixelFill3DSolidFrame   aCmd (5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelFill3DSolidFrame : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelFill3DSolidFrame (int nWidth) : m_nWidth(FMax(3,nWidth)) {}
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        m_rcOut.left = m_rcOut.top = 0 ;
        m_rcOut.right = pImg->GetImgWidth() ;
        m_rcOut.bottom = pImg->GetImgHeight() ;

        const int     nShadow = (int)ceil(m_nWidth / 4.0),
                      nMid = m_nWidth - 2*nShadow ;
        m_rcO = m_rcOut ; InflateRect (&m_rcO, -nShadow, -nShadow) ;
        m_rcI = m_rcO ; InflateRect (&m_rcI, -nMid, -nMid) ;
        m_rcIn = m_rcI ; InflateRect (&m_rcIn, -nShadow, -nShadow) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        const POINT     ptPixel = {x,y} ;
        if (PtInRect (&m_rcIn, ptPixel))
            return ; // in frame, just return

        static RGBQUAD   crMain = {192,192,192,255},
                         crHighlight = FCColor::crWhite(),
                         crShadow = {128,128,128,255} ;

        RGBQUAD     crFill ;
        if (PtInRect (&m_rcI, ptPixel))
        {
            crFill = __pcl_CalcRoundColor (ptPixel, m_rcI, m_rcIn, crShadow, crHighlight) ;
        }
        else if (PtInRect (&m_rcO, ptPixel))
            crFill = crMain ;
        else if (PtInRect (&m_rcOut, ptPixel))
        {
            crFill = __pcl_CalcRoundColor (ptPixel, m_rcOut, m_rcO, crHighlight, crShadow) ;
        }
        FCColor::CopyPixel (pPixel, &crFill, pImg->GetImgColorBits()/8) ;
    }
    static RGBQUAD __pcl_CalcRoundColor (const POINT& pt, const RECT& rcOut, const RECT& rcIn, RGBQUAD crLT, RGBQUAD crRB)
    {
        if (pt.x < rcIn.left)
            return (pt.y - rcOut.top < RECTHEIGHT(rcOut) + rcOut.left - pt.x) ? crLT : crRB ;
        if (pt.x >= rcIn.right)
            return (pt.y - rcOut.top < rcOut.right - pt.x) ? crLT : crRB ;
        if (pt.y < rcIn.top)
            return crLT ;
        return crRB ; // pt.y >= rcIn.bottom
    }

    int     m_nWidth ; // >=3
    RECT    m_rcOut, m_rcO, m_rcI, m_rcIn ;
};

//=============================================================================
/**
 *  Adjust image's RGB value (>=24 bit).
@verbatim
    example:
        CMFCPixelAdjustRGB   aCmd (-100, 50, 220) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelAdjustRGB : public CMFCBasePixelProcess
{
public:
    /// Constructor (param's unit is delta).
    CMFCPixelAdjustRGB (int nR, int nG, int nB) : m_R(nR), m_G(nG), m_B(nB) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        PCL_B(pPixel) = FClamp0255 (PCL_B(pPixel) + m_B) ;
        PCL_G(pPixel) = FClamp0255 (PCL_G(pPixel) + m_G) ;
        PCL_R(pPixel) = FClamp0255 (PCL_R(pPixel) + m_R) ;
    }
    int     m_R, m_G, m_B ;
};

//=============================================================================
/**
 *  Color level (>=24 bit).
@verbatim
    example:
        CMFCPixelColorLevel   aCmd (false, 20, 230) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelColorLevel : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelColorLevel (bool bAuto, int nInLow, int nInHigh, IMAGE_CHANNEL nChannel = CHANNEL_RGB)
    {
        m_bAuto = bAuto ;
        m_nInputLow[0] = m_nInputLow[1] = m_nInputLow[2] = FClamp0255(nInLow) ;
        m_nInputHigh[0] = m_nInputHigh[1] = m_nInputHigh[2] = FClamp0255(nInHigh) ;
        m_nOutputLow = 0 ; m_nOutputHigh = 255 ;
        m_bChannelR = (nChannel & CHANNEL_RED) ? true : false ;
        m_bChannelG = (nChannel & CHANNEL_GREEN) ? true : false ;
        m_bChannelB = (nChannel & CHANNEL_BLUE) ? true : false ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
        if (m_bAuto)
        {
            FCHistogram     histo(*pImg) ;
            this->AutoColorLevelChannel (histo, CHANNEL_RED) ;
            this->AutoColorLevelChannel (histo, CHANNEL_GREEN) ;
            this->AutoColorLevelChannel (histo, CHANNEL_BLUE) ;
        }
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        double      fInten ;
        if (m_bChannelB)
        {
            fInten = PCL_B(pPixel) - m_nInputLow[0] ;
            if (m_nInputHigh[0] != m_nInputLow[0])
                fInten /= (double)(m_nInputHigh[0] - m_nInputLow[0]) ;
            PCL_B(pPixel) = FClamp0255(FRound(fInten * 255.0)) ;
        }
        if (m_bChannelG)
        {
            fInten = PCL_G(pPixel) - m_nInputLow[1] ;
            if (m_nInputHigh[1] != m_nInputLow[1])
                fInten /= (double)(m_nInputHigh[1] - m_nInputLow[1]) ;
            PCL_G(pPixel) = FClamp0255(FRound(fInten * 255.0)) ;
        }
        if (m_bChannelR)
        {
            fInten = PCL_R(pPixel) - m_nInputLow[2] ;
            if (m_nInputHigh[2] != m_nInputLow[2])
                fInten /= (double)(m_nInputHigh[2] - m_nInputLow[2]) ;
            PCL_R(pPixel) = FClamp0255(FRound(fInten * 255.0)) ;
        }
    }
    void AutoColorLevelChannel (const FCHistogram& histo, IMAGE_CHANNEL nChannel)
    {
        int     nIndex = 0 ;
        switch (nChannel)
        {
            case CHANNEL_RED   : nIndex = 2 ; break ;
            case CHANNEL_GREEN : nIndex = 1 ; break ;
            case CHANNEL_BLUE  : nIndex = 0 ; break ;
            default : assert(false) ; break ;
        }

        const int     nCount = histo.GetCount (nChannel) ;
        if (nCount == 0)
        {
            m_nInputLow[nIndex] = m_nInputHigh[nIndex] = 0 ;
        }
        else
        {
            m_nInputLow[nIndex] = 0 ;
            m_nInputHigh[nIndex] = 255 ;
            // Set the low input
            int     new_count = 0, i ;
            for (i=0 ; i < 255 ; i++)
            {
                new_count += histo.GetValueCount (i, nChannel) ;
                double   percentage = new_count / (double)nCount ;
                double   next_percentage = (new_count + histo.GetValueCount (i+1, nChannel)) / (double)nCount ;
                if (fabs (percentage - 0.006) < fabs (next_percentage - 0.006))
                {
                    m_nInputLow[nIndex] = i + 1 ;
                    break ;
                }
            }
            // Set the high input
            new_count = 0 ;
            for (i=255 ; i > 0 ; i--)
            {
                new_count += histo.GetValueCount (i, nChannel) ;
                double   percentage = new_count / (double)nCount ;
                double   next_percentage = (new_count + histo.GetValueCount (i-1, nChannel)) / (double)nCount ;
                if (fabs (percentage - 0.006) < fabs (next_percentage - 0.006))
                {
                    m_nInputHigh[nIndex] = i - 1 ;
                    break ;
                }
            }
        }
    }
private:
    int      m_nInputLow[3], m_nInputHigh[3] ;
    int      m_nOutputLow, m_nOutputHigh ;
    bool     m_bAuto, m_bChannelR, m_bChannelG, m_bChannelB ;
};

//=============================================================================
/**
 *  Threshold image (>=24 bit).
@verbatim
    example:
        CMFCPixelThreshold   aCmd (128) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelThreshold : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelThreshold (int nLevel) : m_nLevel(FClamp0255(nLevel)) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        RGBQUAD   cr = (FCColor::GetGrayscale(pPixel) > m_nLevel) ? FCColor::crWhite() : FCColor::crBlack() ;
        FCColor::CopyPixel (pPixel, &cr, 3) ;
    }
    int     m_nLevel ;
};

//=============================================================================
/**
 *  Clockwise rotate 90' (>=8 bit).
@verbatim
    example:
        CMFCPixelRotate90   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelRotate90 : public CMFCBasePixelProcess
{
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        return pImg->IsValidImage() && (pImg->GetImgColorBits() >= 8) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        FCColor::CopyPixel (pPixel,
                            GetBackupImage()->GetBits (y, pImg->GetImgWidth()-1-x),
                            pImg->GetImgColorBits()/8) ;
    }
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;

        // create new rotated image
        pImg->Create (pImg->GetImgHeight(), pImg->GetImgWidth(), pImg->GetImgColorBits()) ;
        if (pImg->GetImgColorBits() <= 8)
            pImg->CopyPalette(*GetBackupImage()) ;
    }
};

//=============================================================================
/**
 *  Clockwise rotate 270' (>=8 bit).
@verbatim
    example:
        CMFCPixelRotate270   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelRotate270 : public CMFCPixelRotate90
{
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        FCColor::CopyPixel (pPixel,
                            GetBackupImage()->GetBits (pImg->GetImgHeight()-1-y, x),
                            pImg->GetImgColorBits()/8) ;
    }
};

//=============================================================================
/**
 *  De-interlace (32 bit).
@verbatim
    example:
        CMFCPixelDeinterlace   aCmd (CMFCPixelDeinterlace::SCAN_EVEN) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelDeinterlace : public CMFCBasePixelProcess
{
public:
    enum SCAN_FIELDS { SCAN_EVEN=0, SCAN_ODD=1 };
    /// Constructor.
    /// @param EliminateFields : SCAN_EVEN rebuild even line / SCAN_ODD rebuild odd line.
    CMFCPixelDeinterlace (SCAN_FIELDS EliminateFields) : m_Fields(EliminateFields) {}
private:
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        return pImg->IsValidImage() && (pImg->GetImgColorBits() == 32) ;
    }
    // adapted from GIMP v1.2.5 --- code v1.00
    //   Deinterlace is useful for processing images from video capture cards.
    //   When only the odd or even fields get captured, deinterlace can be used to
    //   interpolate between the existing fields to correct this.
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if ((y == 0) || (y == pImg->GetImgHeight()-1))
            return ;

        if (y % 2 == m_Fields)
        {
            BYTE    * pUp = pImg->GetBits (x, y-1),
                    * pDown = pImg->GetBits (x, y+1) ;
            int     nSumA = PCL_A(pUp) + PCL_A(pDown) ;
            PCL_A(pPixel) = nSumA / 2 ;
            if (PCL_A(pPixel))
                for (int i=0 ; i < 3 ; i++)
                    pPixel[i] = (pUp[i]*PCL_A(pUp) + pDown[i]*PCL_A(pDown)) / nSumA ;
        }
    }
    SCAN_FIELDS     m_Fields ;
};

//=============================================================================
/**
 *  Halftone (>=24 bit), use Limb Pattern M3 algorithm.
@verbatim
    example:
        CMFCPixelHalftoneM3   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelHalftoneM3 : public CMFCBasePixelProcess
{
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        static const BYTE  BayerPattern[8][8] = // 64 level gray
        {
            0,32,8,40,2,34,10,42,
            48,16,56,24,50,18,58,26,
            12,44,4,36,14,46,6,38,
            60,28,52,20,62,30,54,22,
            3,35,11,43,1,33,9,41,
            51,19,59,27,49,17,57,25,
            15,47,7,39,13,45,5,37,
            63,31,55,23,61,29,53,21
        };
        const BYTE     gr = FCColor::GetGrayscale (pPixel) ;
        PCL_R(pPixel) = PCL_G(pPixel) = PCL_B(pPixel) = (((gr>>2) > BayerPattern[y&7][x&7]) ? 0xFF : 0) ;
    }
};

//=============================================================================
/**
 *  Oil paint (>=24 bit).
@verbatim
    example:
        CMFCPixelOilPaint   aCmd (2) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelOilPaint : public CMFCBasePixelProcess
{
public:
    /// Constructor (nRadius >= 1).
    CMFCPixelOilPaint (int nRadius)
    {
        m_nRadius = FMax(1,nRadius) ;
        m_nLength = 2*m_nRadius + 1 ; // Stat. block
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
        GetBackupImage()->ExpandFrame (true, m_nRadius, m_nRadius, m_nRadius, m_nRadius) ;

        // calculate block gray
        m_ImgGray = *GetBackupImage() ;
        CMFCPixelConvertTo8BitGray   aCmd ;
        m_ImgGray.PixelProcessProc(aCmd) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        BYTE    * pSelPixel = NULL ;
        int     nMaxNum = 0,
                pHistogram[256] ;
        memset (pHistogram, 0, sizeof(int) * 256) ;

        // replace every pixel use most frequency
        for (int ny=0 ; ny < m_nLength ; ny++)
        {
            BYTE   * pGray = m_ImgGray.GetBits (x, y+ny) ;
            for (int i=0 ; i < m_nLength ; i++, pGray++)
                if (++pHistogram[*pGray] > nMaxNum)
                {
                    nMaxNum = pHistogram[*pGray] ;
                    pSelPixel = GetBackupImage()->GetBits (x+i, y+ny) ;
                }
        }
        FCColor::CopyPixel (pPixel, pSelPixel, 3) ; // leave alpha=channel
    }

    int     m_nRadius ; // >= 1
    int     m_nLength ; // 2*m_nRadius + 1
    CMFCImageObj   m_ImgGray ;
};

//=============================================================================
/**
 *  Color tone (>=24 bit).
@verbatim
    example:
        CMFCPixelColorTone   aCmd (PCL_RGBA(0,0,255)) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelColorTone : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelColorTone (RGBQUAD crTone)
    {
        //  m_nDeltaR = (PCL_R(&crTone) - 128) / 2 ;
        //  m_nDeltaG = (PCL_G(&crTone) - 128) / 2 ;
        //  m_nDeltaB = (PCL_B(&crTone) - 128) / 2 ;
        double     H, L, S ;
        FCColor::RGBtoHLS (&crTone, &H, &L, &S) ;
        for (int i=0 ; i < 256 ; i++)
        {
            RGBQUAD   cr = FCColor::HLStoRGB (H, i/255.0, S) ;
            final_red_lookup[i]   = i * PCL_R(&cr) / 255 ;
            final_green_lookup[i] = i * PCL_G(&cr) / 255 ;
            final_blue_lookup[i]  = i * PCL_B(&cr) / 255 ;
        }
    }
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        //  int     nGray = FCColor::GetGrayscale (pPixel) ;
        //  PCL_B(pPixel) = FClamp0255(nGray + m_nDeltaB) ;
        //  PCL_G(pPixel) = FClamp0255(nGray + m_nDeltaG) ;
        //  PCL_R(pPixel) = FClamp0255(nGray + m_nDeltaR) ;
        int     nLum = FCColor::GetGrayscale (pPixel) ;
        PCL_B(pPixel) = final_blue_lookup[nLum] ;
        PCL_G(pPixel) = final_green_lookup[nLum] ;
        PCL_R(pPixel) = final_red_lookup[nLum] ;
    }
    int     final_red_lookup[256], final_green_lookup[256], final_blue_lookup[256] ;
};

//=============================================================================
/**
 *  Add random noise (>=24 bit).
@verbatim
    example:
        CMFCPixelAddRandomNoise   aCmd (5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelAddRandomNoise : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelAddRandomNoise (int nLevel) : m_nLevel(nLevel) {srand((unsigned int)time(NULL));}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nDelta[3] ;
        for (int i=0 ; i < 3 ; i++)
        {
            nDelta[i] = FRound ((rand()/(double)RAND_MAX - 0.5) * m_nLevel) ;
        }
        PCL_B(pPixel) = FClamp0255 (PCL_B(pPixel) + nDelta[0]) ;
        PCL_G(pPixel) = FClamp0255 (PCL_G(pPixel) + nDelta[1]) ;
        PCL_R(pPixel) = FClamp0255 (PCL_R(pPixel) + nDelta[2]) ;
    }

    int     m_nLevel ;
};

//=============================================================================
/**
 *  Noisify (>=24 bit).
@verbatim
    example:
        CMFCPixelNoisify   aCmd (5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelNoisify : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nLevel : level (0 <= nLevel <= 100).
    CMFCPixelNoisify (int nLevel) : m_nLevel(FClamp(nLevel,0,100)) {srand((unsigned int)time(NULL));}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nNoise = (int)(m_nLevel * GenGauss() * 127.0 / 100.0) ;
        PCL_B(pPixel) = FClamp0255 (PCL_B(pPixel) + nNoise) ;
        PCL_G(pPixel) = FClamp0255 (PCL_G(pPixel) + nNoise) ;
        PCL_R(pPixel) = FClamp0255 (PCL_R(pPixel) + nNoise) ;
    }
    static double GenGauss()
    {
        return (rand() + rand() + rand() + rand()) * 5.28596089837e-5 - 3.46410161514 ;
    }

    int     m_nLevel ;
};

//=============================================================================
/**
 *  Splash image (>=24 bit).
@verbatim
    example:
        CMFCPixelSplash   aCmd (5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelSplash : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nBlock : splash level (>=3).
    CMFCPixelSplash (int nBlock)
    {
        m_nBlock = FMax (3, nBlock) ;
        srand((unsigned int)time(0)) ;
    }
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int   xCopy = x - m_nBlock/2 + (rand() % m_nBlock),
              yCopy = y - m_nBlock/2 + (rand() % m_nBlock) ;
        xCopy = FClamp (xCopy, 0, pImg->GetImgWidth()-1) ;
        yCopy = FClamp (yCopy, 0, pImg->GetImgHeight()-1) ;
        BYTE   * pSrc = GetBackupImage()->GetBits(xCopy,yCopy) ;
        FCColor::CopyPixel (pPixel, pSrc, pImg->GetImgColorBits()/8) ;
    }
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
    }
    int     m_nBlock ;
};

//=============================================================================
/**
 *  Video (>=24 bit).
@verbatim
    example:
        CMFCPixelVideo   aCmd (CMFCPixelVideo::VIDEO_STAGGERED) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelVideo : public CMFCBasePixelProcess
{
public:
    enum VIDEO_TYPE {VIDEO_STAGGERED=0, VIDEO_TRIPED=1, VIDEO_3X3=2, VIDEO_DOTS=3} ;
    /// Constructor.
    /// @param nVideoType : VIDEO_STAGGERED, VIDEO_TRIPED, VIDEO_3X3, VIDEO_DOTS
    CMFCPixelVideo (VIDEO_TYPE nVideoType) : m_VideoType(nVideoType)
    {
        assert(nVideoType>=VIDEO_STAGGERED && nVideoType<=VIDEO_DOTS);
    }
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        static const int   pattern_width[] = {2, 1, 3, 5} ;
        static const int   pattern_height[] = {6, 3, 3, 15} ;
        static const int   video_pattern[4][15 * 5/* max pattern size */] =
        {
            {
                0, 1,
                0, 2,
                1, 2,
                1, 0,
                2, 0,
                2, 1,
            },
            {
                0,
                1,
                2,
            },
            {
                0, 1, 2,
                2, 0, 1,
                1, 2, 0,
            },
            {
                0, 1, 2, 0, 0,
                1, 1, 1, 2, 0,
                0, 1, 2, 2, 2,
                0, 0, 1, 2, 0,
                0, 1, 1, 1, 2,
                2, 0, 1, 2, 2,
                0, 0, 0, 1, 2,
                2, 0, 1, 1, 1,
                2, 2, 0, 1, 2,
                2, 0, 0, 0, 1,
                1, 2, 0, 1, 1,
                2, 2, 2, 0, 1,
                1, 2, 0, 0, 0,
                1, 1, 2, 0, 1,
                1, 2, 2, 2, 0,
            }
        };

        const int   nWidth = pattern_width[m_VideoType],
                    nHeight = pattern_height[m_VideoType] ;
        for (int i=0 ; i < 3 ; i++)
            if (video_pattern[m_VideoType][nWidth * (y%nHeight) + (x%nWidth)] == i)
                pPixel[i] = FClamp0255 (2 * pPixel[i]) ;
    }

    VIDEO_TYPE   m_VideoType ;
};

//=============================================================================
/**
 *  Color balance (>=24 bit).
@verbatim
    example:
        CMFCPixelColorBalance   aCmd (true, TONE_SHADOWS, -30, 20, 30) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelColorBalance : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelColorBalance (bool bPreLum, TONE_REGION ToneRgn, int cyan_red, int magenta_green, int yellow_blue)
    {
        m_bPreserveLuminosity = bPreLum ;

        int     cyan_red_rgn[3] = {0,0,0},
                magenta_green_rgn[3] = {0,0,0},
                yellow_blue_rgn[3] = {0,0,0} ;
        cyan_red_rgn[ToneRgn] = cyan_red ;
        magenta_green_rgn[ToneRgn] = magenta_green ;
        yellow_blue_rgn[ToneRgn] = yellow_blue ;

        // add for lightening, sub for darkening
        PCL_array<double>  highlights_add(256), midtones_add(256), shadows_add(256),
                           highlights_sub(256), midtones_sub(256), shadows_sub(256) ;
        int     i ;
        for (i=0 ; i < 256 ; i++)
        {
            highlights_add[i] = shadows_sub[255 - i] = (1.075 - 1 / (i / 16.0 + 1)) ;
            midtones_add[i] = midtones_sub[i] = 0.667 * (1 - FSquare ((i - 127.0) / 127.0)) ;
            shadows_add[i] = highlights_sub[i] = 0.667 * (1 - FSquare ((i - 127.0) / 127.0)) ;
        }

        // Set the transfer arrays (for speed)
        double   * cyan_red_transfer[3], * magenta_green_transfer[3], * yellow_blue_transfer[3] ;
        cyan_red_transfer[TONE_SHADOWS] = (cyan_red_rgn[TONE_SHADOWS] > 0) ? shadows_add.get() : shadows_sub.get() ;
        cyan_red_transfer[TONE_MIDTONES] = (cyan_red_rgn[TONE_MIDTONES] > 0) ? midtones_add.get() : midtones_sub.get() ;
        cyan_red_transfer[TONE_HIGHLIGHTS] = (cyan_red_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add.get() : highlights_sub.get() ;
        magenta_green_transfer[TONE_SHADOWS] = (magenta_green_rgn[TONE_SHADOWS] > 0) ? shadows_add.get() : shadows_sub.get() ;
        magenta_green_transfer[TONE_MIDTONES] = (magenta_green_rgn[TONE_MIDTONES] > 0) ? midtones_add.get() : midtones_sub.get() ;
        magenta_green_transfer[TONE_HIGHLIGHTS] = (magenta_green_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add.get() : highlights_sub.get() ;
        yellow_blue_transfer[TONE_SHADOWS] = (yellow_blue_rgn[TONE_SHADOWS] > 0) ? shadows_add.get() : shadows_sub.get() ;
        yellow_blue_transfer[TONE_MIDTONES] = (yellow_blue_rgn[TONE_MIDTONES] > 0) ? midtones_add.get() : midtones_sub.get() ;
        yellow_blue_transfer[TONE_HIGHLIGHTS] = (yellow_blue_rgn[TONE_HIGHLIGHTS] > 0) ? highlights_add.get() : highlights_sub.get() ;

        for (i=0 ; i < 256 ; i++)
        {
            int     r_n = i, g_n = i, b_n = i ;

            r_n += (int)(cyan_red_rgn[TONE_SHADOWS] * cyan_red_transfer[TONE_SHADOWS][r_n]);        r_n = FClamp0255(r_n);
            r_n += (int)(cyan_red_rgn[TONE_MIDTONES] * cyan_red_transfer[TONE_MIDTONES][r_n]);      r_n = FClamp0255(r_n);
            r_n += (int)(cyan_red_rgn[TONE_HIGHLIGHTS] * cyan_red_transfer[TONE_HIGHLIGHTS][r_n]);  r_n = FClamp0255(r_n);

            g_n += (int)(magenta_green_rgn[TONE_SHADOWS] * magenta_green_transfer[TONE_SHADOWS][g_n]);        g_n = FClamp0255(g_n);
            g_n += (int)(magenta_green_rgn[TONE_MIDTONES] * magenta_green_transfer[TONE_MIDTONES][g_n]);      g_n = FClamp0255(g_n);
            g_n += (int)(magenta_green_rgn[TONE_HIGHLIGHTS] * magenta_green_transfer[TONE_HIGHLIGHTS][g_n]);  g_n = FClamp0255(g_n);

            b_n += (int)(yellow_blue_rgn[TONE_SHADOWS] * yellow_blue_transfer[TONE_SHADOWS][b_n]);        b_n = FClamp0255(b_n);
            b_n += (int)(yellow_blue_rgn[TONE_MIDTONES] * yellow_blue_transfer[TONE_MIDTONES][b_n]);      b_n = FClamp0255(b_n);
            b_n += (int)(yellow_blue_rgn[TONE_HIGHLIGHTS] * yellow_blue_transfer[TONE_HIGHLIGHTS][b_n]);  b_n = FClamp0255(b_n);

            m_pLookupR[i] = r_n ;
            m_pLookupG[i] = g_n ;
            m_pLookupB[i] = b_n ;
        }
    }
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        RGBQUAD     rgb ;
        PCL_B(&rgb) = m_pLookupB[PCL_B(pPixel)] ;
        PCL_G(&rgb) = m_pLookupG[PCL_G(pPixel)] ;
        PCL_R(&rgb) = m_pLookupR[PCL_R(pPixel)] ;
        if (m_bPreserveLuminosity) // preserve luminosity
        {
            double   H, L, S ;
            FCColor::RGBtoHLS (&rgb, &H, &L, &S) ;

            // calculate L value
            int   cmax = FMax (PCL_R(pPixel), FMax (PCL_G(pPixel), PCL_B(pPixel))),
                  cmin = FMin (PCL_R(pPixel), FMin (PCL_G(pPixel), PCL_B(pPixel))) ;
            L = (cmax+cmin) / 2.0 / 255.0 ;

            rgb = FCColor::HLStoRGB (H, L, S) ;
        }
        PCL_B(pPixel) = PCL_B(&rgb) ;
        PCL_G(pPixel) = PCL_G(&rgb) ;
        PCL_R(pPixel) = PCL_R(&rgb) ;
    }

    BYTE     m_pLookupR[256], m_pLookupG[256], m_pLookupB[256] ;
    bool     m_bPreserveLuminosity ;
};

//=============================================================================
/**
 *  Fill grid (>=24 bit).
@verbatim
    example:
        CMFCPixelFillGrid   aCmd (PCL_RGBA(0,255,0), PCL_RGBA(0,0,255), 5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelFillGrid : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nPitch : width of grid.
    CMFCPixelFillGrid (RGBQUAD cr1, RGBQUAD cr2, int nPitch) : m_cr1(cr1), m_cr2(cr2), m_nPitch(FMax(1,nPitch)) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nX = x / m_nPitch, nY = y / m_nPitch ;
        FCColor::CopyPixel (pPixel, ((nX + nY) % 2 == 0) ? &m_cr1 : &m_cr2, 3) ;
    }
    RGBQUAD   m_cr1, m_cr2 ;
    int       m_nPitch ;
};

//=============================================================================
/**
 *  Add 3D grid (>=24 bit).
@verbatim
    example:
        CMFCPixel3DGrid   aCmd (32, 60) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixel3DGrid : public CMFCBasePixelProcess
{
public:
    CMFCPixel3DGrid (int nSize, int nDepth) : m_nSize(FMax(1,nSize)), m_nDepth(nDepth) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     nDelta = 0 ;
        if (((y-1) % m_nSize == 0) && (x % m_nSize != 0) && ((x+1) % m_nSize != 0))
            nDelta = -m_nDepth ; // top
        else if (((y+2) % m_nSize == 0) && (x % m_nSize != 0) && ((x+1) % m_nSize != 0))
            nDelta = m_nDepth ; // bottom
        else if (((x-1) % m_nSize == 0) && (y % m_nSize != 0) && ((y+1) % m_nSize != 0))
            nDelta = m_nDepth ; // left
        else if (((x+2) % m_nSize == 0) && (y % m_nSize != 0) && ((y+1) % m_nSize != 0))
            nDelta = -m_nDepth ; // right

        PCL_R(pPixel) = FClamp0255 (PCL_R(pPixel) + nDelta) ;
        PCL_G(pPixel) = FClamp0255 (PCL_G(pPixel) + nDelta) ;
        PCL_B(pPixel) = FClamp0255 (PCL_B(pPixel) + nDelta) ;
    }

    int     m_nSize, m_nDepth ;
};

//=============================================================================
/**
 *  Median filter (>=24 bit).
@verbatim
    example:
        CMFCPixelMedianFilter   aCmd (3) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelMedianFilter : public CMFCBasePixelProcess
{
public:
    /// Constructor (nSize >= 2).
    CMFCPixelMedianFilter (int nSize) : m_nSize(nSize), m_pBlock(0), m_nBlockNum(FSquare(nSize)) {}
    virtual ~CMFCPixelMedianFilter() {if(m_pBlock) delete[] m_pBlock;}
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        m_pBlock = new BlockElem[m_nBlockNum] ;
        SetBackupImage (pImg) ;

        // duplicate edge
        int     nLeftTop = m_nSize/2,
                nRightDown = nLeftTop ;
        GetBackupImage()->ExpandFrame (true, nLeftTop, nLeftTop, nRightDown, nRightDown) ;

        m_BakImage = *GetBackupImage() ;
        // calculate gray
        CMFCPixelConvertTo8BitGray   aCmd ;
        GetBackupImage()->PixelProcessProc (aCmd) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        int     i = 0 ;
        for (int m=0 ; m < m_nSize ; m++)
            for (int n=0 ; n < m_nSize ; n++)
            {
                m_pBlock[i].nGray = *GetBackupImage()->GetBits (x+n, y+m) ;
                FCColor::CopyPixel (&m_pBlock[i].crOrigin, m_BakImage.GetBits(x+n, y+m), 3) ;
                i++ ;
            }

        ::qsort (m_pBlock, m_nBlockNum, sizeof(BlockElem), __CompareGray) ;
        FCColor::CopyPixel (pPixel, &m_pBlock[m_nBlockNum/2], 3) ;
    }

    struct BlockElem
    {
        RGBQUAD   crOrigin ;
        int       nGray ;
    };

    static int __CompareGray (const void* arg1, const void* arg2)
    {
        return ((BlockElem*)arg1)->nGray - ((BlockElem*)arg2)->nGray ;
    }

    int         m_nSize ;
    int         m_nBlockNum ;
    BlockElem   * m_pBlock ;
    CMFCImageObj  m_BakImage ;
};

//=============================================================================
/**
 *  Splite RGB channel (>=24 bit).
@verbatim
    example:
        CMFCImageObj                 imgBlue, imgGreen ;
        CMFCPixelSpliteChannel_RGB   aCmd (NULL, &imgGreen, &imgBlue) ; // get green & blue channel
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelSpliteChannel_RGB : public CMFCBasePixelProcess
{
public:
    /// Constructor (the received image's bpp is 8).
    /// if you needn't some channel, set the param to NULL.
    CMFCPixelSpliteChannel_RGB (CMFCImageObj* pImgR, CMFCImageObj* pImgG, CMFCImageObj* pImgB) : m_pImgR(pImgR), m_pImgG(pImgG), m_pImgB(pImgB) {}
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if (m_pImgB)  *m_pImgB->GetBits(x,y) = PCL_B(pPixel) ;
        if (m_pImgG)  *m_pImgG->GetBits(x,y) = PCL_G(pPixel) ;
        if (m_pImgR)  *m_pImgR->GetBits(x,y) = PCL_R(pPixel) ;
    }
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        if (m_pImgR) m_pImgR->Create (pImg->GetImgWidth(), pImg->GetImgHeight(), 8) ;
        if (m_pImgG) m_pImgG->Create (pImg->GetImgWidth(), pImg->GetImgHeight(), 8) ;
        if (m_pImgB) m_pImgB->Create (pImg->GetImgWidth(), pImg->GetImgHeight(), 8) ;
    }
    CMFCImageObj     * m_pImgR, * m_pImgG, * m_pImgB ;
};

//=============================================================================
/**
 *  Combine RGB channel (>=24 bit).
 *  all channel image must same width & height, bpp == 8.
@verbatim
    example:
        CMFCPixelCombineChannel_RGB  aCmd (&imgRed, &imgGreen, &imgBlue) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelCombineChannel_RGB : public CMFCBasePixelProcess
{
public:
    CMFCPixelCombineChannel_RGB (CMFCImageObj* pImgR, CMFCImageObj* pImgG, CMFCImageObj* pImgB) : m_pImgR(pImgR), m_pImgG(pImgG), m_pImgB(pImgB) {}
private:
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        bool b=m_pImgR && m_pImgG && m_pImgB &&
               (m_pImgR->GetImgColorBits() == 8) && (m_pImgG->GetImgColorBits() == 8) && (m_pImgB->GetImgColorBits() == 8) &&
               (m_pImgR->GetImgWidth() == m_pImgG->GetImgWidth()) && (m_pImgR->GetImgWidth() == m_pImgB->GetImgWidth()) &&
               (m_pImgR->GetImgHeight() == m_pImgG->GetImgHeight()) && (m_pImgR->GetImgHeight() == m_pImgB->GetImgHeight()) ;
        if (b)
            const_cast<CMFCImageObj*>(pImg)->Create (m_pImgR->GetImgWidth(), m_pImgR->GetImgHeight(), 32) ;
        return b ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        PCL_B(pPixel) = *m_pImgB->GetBits(x,y) ;
        PCL_G(pPixel) = *m_pImgG->GetBits(x,y) ;
        PCL_R(pPixel) = *m_pImgR->GetBits(x,y) ;
        PCL_A(pPixel) = 0xFF ;
    }
    CMFCImageObj     * m_pImgR, * m_pImgG, * m_pImgB ;
};

//=============================================================================
/// Image convolute (>= 24 bit)
class CMFCPixelConvolute : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelConvolute() : m_pElement(0)
    {
        m_iBlock=0 ; m_nOffset=0 ; m_iDivisor=1 ;
    }
    virtual ~CMFCPixelConvolute()
    {
        if (m_pElement)
            delete[] m_pElement ;
    }

    /**
     *  Set convolute kernel.
     *  @param nElements : array from top-left of matrix.
     *  @param iBlockLen : width of matrix.
     */
    void SetKernel (const int* nElements, int iBlockLen, int iDivisor, int nOffset=0)
    {
        if (!nElements || (iBlockLen < 1))
            {assert(false); return;}

        if (m_pElement)
            delete[] m_pElement ;
        m_pElement = new int[FSquare(iBlockLen)] ;

        for (int i=0 ; i < FSquare(iBlockLen) ; i++)
            m_pElement[i] = nElements[i] ;

        m_iBlock = iBlockLen ;
        m_iDivisor = FMax(1,iDivisor) ;
        m_nOffset = nOffset ;
    }

private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;

        // duplicate edge, easier to processs
        int   nLeftTop = m_iBlock/2,
              nRightDown = nLeftTop ;
        GetBackupImage()->ExpandFrame (true, nLeftTop, nLeftTop, nRightDown, nRightDown) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        // calculate the sum of sub-block
        int      nSumR=0, nSumG=0, nSumB=0, i=0 ;
        for (int iy=0 ; iy < m_iBlock ; iy++)
            for (int ix=0 ; ix < m_iBlock ; ix++, i++)
            {
                BYTE   * pOld = GetBackupImage()->GetBits (x+ix,y+iy) ;
                nSumB += PCL_B(pOld) * m_pElement[i] ;
                nSumG += PCL_G(pOld) * m_pElement[i] ;
                nSumR += PCL_R(pOld) * m_pElement[i] ;
            }
        // set pixel
        PCL_B(pPixel) = FClamp0255 (m_nOffset + nSumB / m_iDivisor) ;
        PCL_G(pPixel) = FClamp0255 (m_nOffset + nSumG / m_iDivisor) ;
        PCL_R(pPixel) = FClamp0255 (m_nOffset + nSumR / m_iDivisor) ;
    }

    int     * m_pElement ;
    int     m_iBlock, m_iDivisor, m_nOffset ;
};

//=============================================================================
/**
 *  Standard 3x3 gaussian blur (>=24 bit).
@verbatim
    example:
        CMFCPixelGaussianBlur3x3   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGaussianBlur3x3 : public CMFCPixelConvolute
{
public:
    CMFCPixelGaussianBlur3x3()
    {
        int   arKernel[] = {1,2,1,2,4,2,1,2,1},
              nBlock = 3,
              nDivisor = 16,
              nOffset = 0 ;
        SetKernel (arKernel, nBlock, nDivisor, nOffset) ;
    }
};

//=============================================================================
/**
 *  Standard 5x5 gaussian blur (>=24 bit).
@verbatim
    example:
        CMFCPixelGaussianBlur5x5   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGaussianBlur5x5 : public CMFCPixelConvolute
{
public:
    CMFCPixelGaussianBlur5x5 ()
    {
        int   arKernel[] = {0,1,2,1,0,1,3,4,3,1,2,4,8,4,2,1,3,4,3,1,0,1,2,1,0},
              nBlock = 5,
              nDivisor = 52,
              nOffset = 0 ;
        SetKernel (arKernel, nBlock, nDivisor, nOffset) ;
    }
};

//=============================================================================
/**
 *  Detect edges (>=24 bit).
@verbatim
    example:
        CMFCPixelDetectEdges   aCmd(3) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelDetectEdges : public CMFCPixelConvolute
{
public:
    /// Constructor (nRadius >= 1).
    CMFCPixelDetectEdges (int nRadius = 3)
    {
        int   nBlock = 2*FMax(1,nRadius) + 1,
              nDivisor = 1,
              nOffset = 0,
              nWidth = FSquare(nBlock) ;

        PCL_array<int>   pKernel (nWidth) ;
        for (int i=0 ; i < nWidth ; i++)
            pKernel[i] = -1 ;
        pKernel[nWidth/2] = nWidth - 1 ;
        SetKernel (pKernel.get(), nBlock, nDivisor, nOffset) ;
    }
};

//=============================================================================
/**
 *  Sharp (laplacian template) (>=24 bit).
@verbatim
    example:
        CMFCPixelSharp   aCmd(3) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelSharp : public CMFCPixelConvolute
{
public:
    /// Constructor (nStep >= 1).
    CMFCPixelSharp (int nStep)
    {
        int   arKernel[] = {-1,-1,-1,-1,8+nStep,-1,-1,-1,-1},
              nBlock = 3,
              nDivisor = FMax (1,nStep),
              nOffset = 0 ;
        SetKernel (arKernel, nBlock, nDivisor, nOffset) ;
    }
};

//=============================================================================
/// Base class of gradient fill (>=24 bit)
class CMFCPixelGradientBase : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelGradientBase (RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat=REPEAT_NONE) : m_crStart(crStart), m_crEnd(crEnd), m_nRepeat(nRepeat) {}
protected:
    /// calculate factor of point(x,y)
    virtual double CalculateFactor (int nX, int nY) =0 ;
private:
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        double     fFac = this->CalculateFactor (x, y) ;
        switch (m_nRepeat)
        {
        case REPEAT_NONE : fFac = FClamp (fFac, 0.0, 1.0); break;
        case REPEAT_SAWTOOTH :
            if (fFac < 0.0)
                fFac = 1.0 - FDoubleMod1 (-fFac) ;
            else
                fFac = FDoubleMod1 (fFac) ;
            break ;
        case REPEAT_TRIANGULAR :
            if (fFac < 0.0)
                fFac = -fFac ;
            if ( ((int)fFac) & 1 )
                fFac = 1.0 - FDoubleMod1 (fFac) ;
            else
                fFac = FDoubleMod1 (fFac) ;
            break ;
        }
        PCL_B(pPixel) = (BYTE)(PCL_B(&m_crStart) + (PCL_B(&m_crEnd)-PCL_B(&m_crStart)) * fFac) ;
        PCL_G(pPixel) = (BYTE)(PCL_G(&m_crStart) + (PCL_G(&m_crEnd)-PCL_G(&m_crStart)) * fFac) ;
        PCL_R(pPixel) = (BYTE)(PCL_R(&m_crStart) + (PCL_R(&m_crEnd)-PCL_R(&m_crStart)) * fFac) ;
    }
    double FDoubleMod1 (const double &x)
    {
        // the function <fmod> is extreme slow :-(, so we just do it.
        // the function == fmod (x, 1.0)
        return x - (int)x ;
    }

    RGBQUAD      m_crStart, m_crEnd ;
    REPEAT_MODE  m_nRepeat ; // type of repeat
};

//=============================================================================
/**
 *  Gradient fill linear (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100} ;
        CMFCPixelGradientLine  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientLine : public CMFCPixelGradientBase
{
public:
    /**
     *  Constructor.
     *  @param ptStart : start coordinate on image.
     *  @param ptEnd : end coordinate on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientLine (POINT ptStart, POINT ptEnd, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientBase (crStart, crEnd, nRepeat)
    {
        m_ptStart = ptStart; m_ptEnd = ptEnd;
        m_fDist = FHypot ((double)(m_ptStart.x-m_ptEnd.x), (double)(m_ptStart.y-m_ptEnd.y)) ;
        m_fRatX = (m_ptEnd.x-m_ptStart.x) / m_fDist ;
        m_fRatY = (m_ptEnd.y-m_ptStart.y) / m_fDist ;
    }
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   rat = m_fRatX * (nX-m_ptStart.x) + m_fRatY * (nY-m_ptStart.y) ;
        rat = rat / m_fDist ;
        return (rat < 0.0) ? 0.0 : rat ;
    }
protected:
    POINT      m_ptStart, m_ptEnd ; // coordinate on image
    double     m_fRatX, m_fRatY ;
    double     m_fDist ;
};

//=============================================================================
/**
 *  Gradient fill bilinear (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100} ;
        CMFCPixelGradientBiLine  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientBiLine : public CMFCPixelGradientLine
{
public:
    /**
     *  Constructor.
     *  @param ptStart : start coordinate on image.
     *  @param ptEnd : end coordinate on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientBiLine (POINT ptStart, POINT ptEnd, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientLine (ptStart, ptEnd, crStart, crEnd, nRepeat) {}
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   rat = m_fRatX * (nX-m_ptStart.x) + m_fRatY * (nY-m_ptStart.y) ;
        rat = rat / m_fDist ;
        return fabs(rat) ;
    }
};

//=============================================================================
/**
 *  Gradient fill symmetric conical (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100} ;
        CMFCPixelGradientConicalSym  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientConicalSym : public CMFCPixelGradientLine
{
public:
    /**
     *  Constructor.
     *  @param ptStart : start coordinate on image.
     *  @param ptEnd : end coordinate on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientConicalSym (POINT ptStart, POINT ptEnd, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientLine (ptStart, ptEnd, crStart, crEnd, nRepeat) {}
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   rat ;
        double   dx = nX-m_ptStart.x, dy = nY-m_ptStart.y ;
        if ((dx != 0) || (dy != 0))
        {
            double    dr = FHypot (dx, dy) ;
            rat = m_fRatX * dx / dr + m_fRatY * dy / dr ;
            rat = FClamp (rat, -1.0, 1.0) ;
            rat = acos(rat) / LIB_PI ;
            rat = FClamp (rat, 0.0, 1.0) ;
        }
        else
            rat = 0.5 ;
        return rat ;
    }
};

//=============================================================================
/**
 *  Gradient fill Anti-symmetric conical (>=24 bit).
@verbatim
    example:
        POINT     ptStart={0,0}, ptEnd={100,100} ;
        CMFCPixelGradientConicalSym  aCmd (ptStart, ptEnd, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientConicalASym : public CMFCPixelGradientLine
{
public:
    /**
     *  Constructor.
     *  @param ptStart : start coordinate on image.
     *  @param ptEnd : end coordinate on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientConicalASym (POINT ptStart, POINT ptEnd, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientLine (ptStart, ptEnd, crStart, crEnd, nRepeat) {}
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   rat ;
        double   dx = nX-m_ptStart.x, dy = nY-m_ptStart.y ;
        if ((dx != 0) || (dy != 0))
        {
            double   ang0, ang1, ang ;
            ang0 = atan2 (m_fRatX, m_fRatY) + LIB_PI ;
            ang1 = atan2 (dx, dy) + LIB_PI ;
            ang = ang1 - ang0 ;
            if (ang < 0.0)
                ang += LIB_2PI ;

            rat = ang / LIB_2PI ;
            rat = FClamp (rat, 0.0, 1.0) ;
        }
        else
            rat = 0.5 ;
        return rat ;
    }
};

//=============================================================================
/**
 *  Gradient fill rect (>=24 bit).
@verbatim
    example:
        RECT     rc = {0, 0, 100, 100} ;
        CMFCPixelGradientRect  aCmd (rc, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientRect : public CMFCPixelGradientBase
{
public:
    /**
     *  Constructor.
     *  @param rcRect : rect on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientRect (RECT rcRect, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientBase (crStart, crEnd, nRepeat)
    {
        assert (!IsRectEmpty(&rcRect)) ;
        m_ptCenter.x = (rcRect.left + rcRect.right) / 2.0 ;
        m_ptCenter.y = (rcRect.top + rcRect.bottom) / 2.0 ;
        m_fRadiusX = RECTWIDTH(rcRect) / 2.0 ;
        m_fRadiusY = RECTHEIGHT(rcRect) / 2.0 ;
    }
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   ratX = fabs((nX-m_ptCenter.x) / m_fRadiusX),
                 ratY = fabs((nY-m_ptCenter.y) / m_fRadiusY) ;
        return FMax(ratX, ratY) ;
    }
protected:
    POINT_F     m_ptCenter ;
    double      m_fRadiusX, m_fRadiusY ;
};

//=============================================================================
/**
 *  Gradient fill radial (>=24 bit).
@verbatim
    example:
        RECT     rc = {0, 0, 100, 100} ;
        CMFCPixelGradientRadial  aCmd (rc, PCL_RGBA(0,0,0), PCL_RGBA(0,0,255), REPEAT_NONE) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGradientRadial : public CMFCPixelGradientBase
{
public:
    /**
     *  Constructor.
     *  @param rcEllipse : rect on image.
     *  @param nRepeat : REPEAT_NONE, REPEAT_SAWTOOTH, REPEAT_TRIANGULAR
     */
    CMFCPixelGradientRadial (RECT rcEllipse, RGBQUAD crStart, RGBQUAD crEnd, REPEAT_MODE nRepeat = REPEAT_NONE) : CMFCPixelGradientBase (crStart, crEnd, nRepeat)
    {
        assert (!IsRectEmpty(&rcEllipse)) ;
        m_ptCenter.x = (rcEllipse.left + rcEllipse.right) / 2.0 ;
        m_ptCenter.y = (rcEllipse.top + rcEllipse.bottom) / 2.0 ;
        m_fRadiusX = RECTWIDTH(rcEllipse) / 2.0 ;
        m_fRadiusY = RECTHEIGHT(rcEllipse) / 2.0 ;
    }
protected:
    virtual double CalculateFactor (int nX, int nY)
    {
        double   rat = FHypot((nX-m_ptCenter.x)/m_fRadiusX, (nY-m_ptCenter.y)/m_fRadiusY) ;
        return rat ;
    }
protected:
    POINT_F     m_ptCenter ;
    double      m_fRadiusX, m_fRadiusY ;
};

//=============================================================================
/// Bilinear distord (>=24 bit).
/// if derived class override OnEnterProcess, it must call OnEnterProcess of base.
class CMFCPixelBilinearDistord : public CMFCBasePixelProcess
{
protected:
    virtual bool ValidateColorBits (const CMFCImageObj* pImg)
    {
        return CMFCBasePixelProcess::ValidateColorBits(pImg) && (pImg->GetImgWidth() >= 2) && (pImg->GetImgHeight() >= 2) ;
    }
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        double     un_x, un_y ;
        if (!calc_undistorted_coord (x, y, un_x, un_y))
            return ;
        un_x = FClamp (un_x, 0.0, GetBackupImage()->GetImgWidth() - 1.1) ;
        un_y = FClamp (un_y, 0.0, GetBackupImage()->GetImgHeight() - 1.1) ;

        const int     nSrcX = (int)un_x, nSrcY = (int)un_y,
                      nSrcX_1 = nSrcX+1, nSrcY_1 = nSrcY+1 ;
        const BYTE    * pcrPixel[4] =
        {
            GetBackupImage()->GetBits(nSrcX,nSrcY),
            GetBackupImage()->GetBits(nSrcX_1,nSrcY),
            GetBackupImage()->GetBits(nSrcX,nSrcY_1),
            GetBackupImage()->GetBits(nSrcX_1,nSrcY_1)
        } ;
        RGBQUAD   cr = FCColor::Get_Bilinear_Pixel (un_x-nSrcX, un_y-nSrcY, pImg->GetImgColorBits() == 32, pcrPixel) ;
        FCColor::CopyPixel (pPixel, &cr, pImg->GetImgColorBits()/8) ;
    }

    // returned bool variable to declare continue or not
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const =0 ;
};

//=============================================================================
/**
 *  Cylinder (>=24 bit).
@verbatim
    example:
        CMFCPixelCylinder  aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelCylinder : public CMFCPixelBilinearDistord
{
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess(pImg) ;

        // pos in origin image.
        double   R = pImg->GetImgWidth() / 2.0 ;
        for (int x=0 ; x < pImg->GetImgWidth() ; x++)
        {
            double   fIndex = pImg->GetImgWidth() * acos ((R-x)/R) / LIB_PI ;
            m_ColIndex.push_back (fIndex) ;
        }
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        un_x = m_ColIndex[x] ;
        un_y = y ;
        return true ;
    }
    std::deque<double>   m_ColIndex ;
};

//=============================================================================
/**
 *  Wave (>=24 bit).
@verbatim
    example:
        CMFCPixelWave  aCmd (25, 30) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelWave : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    CMFCPixelWave (int nWavelength, int nAmplitude, double fPhase=0) : m_nWavelength(2*FMax(1,nWavelength)), m_nAmplitude(FMax(1,nAmplitude)), m_fPhase(fPhase) {}
private:
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        const int  nImgWidth = GetBackupImage()->GetImgWidth(),
                   nImgHeight = GetBackupImage()->GetImgHeight() ;
        double     fScaleX = 1.0, fScaleY = 1.0 ;
        if (nImgWidth < nImgHeight)
            fScaleX = nImgHeight / (double)nImgWidth ;
        else if (nImgWidth > nImgHeight)
            fScaleY = nImgWidth / (double)nImgHeight ;

        // Distances to center, scaled
        double   fCenX = GetBackupImage()->GetImgWidth() / 2.0,
                 fCenY = GetBackupImage()->GetImgHeight() / 2.0,
                 dx = (x - fCenX) * fScaleX,
                 dy = (y - fCenY) * fScaleY,
                 amnt = m_nAmplitude * sin (LIB_2PI * FHypot(dx,dy) / (double)m_nWavelength + m_fPhase) ;
        un_x = (amnt + dx) / fScaleX + fCenX ;
        un_y = (amnt + dy) / fScaleY + fCenY ;
        return true ;
    }

    double  m_fPhase ; // [0..2n]
    int     m_nAmplitude ;
    int     m_nWavelength ;
};

//=============================================================================
/**
 *  Whirl & Pinch (>=24 bit).
@verbatim
    example:
        CMFCPixelWhirlPinch  aCmd (1.5, 0.5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelWhirlPinch : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    /// @param fWhirl : [-2n,2n]
    /// @param fPinch : [-1.0,1.0]
    CMFCPixelWhirlPinch (double fWhirl, double fPinch)
    {
        m_fWhirl = FClamp (fWhirl, -LIB_2PI, LIB_2PI) ;
        m_fPinch = FClamp (fPinch, -1.0, 1.0) ;
    }
private:
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        
        int      nImgWidth = GetBackupImage()->GetImgWidth(),
                 nImgHeight = GetBackupImage()->GetImgHeight() ;
        double   fScaleX = 1.0, fScaleY = 1.0 ;
        if (nImgWidth < nImgHeight)
            fScaleX = nImgHeight / (double)nImgWidth ;
        else if (nImgWidth > nImgHeight)
            fScaleY = nImgWidth / (double)nImgHeight ;

        // Distances to center, scaled
        double   fCenX = GetBackupImage()->GetImgWidth() / 2.0,
                 fCenY = GetBackupImage()->GetImgHeight() / 2.0,
                 fRadius = FMax (fCenX, fCenY),
                 dx = (x - fCenX) * fScaleX,
                 dy = (y - fCenY) * fScaleY ;
        double   d = dx*dx + dy*dy ; // Distance^2 to center of *circle* (scaled ellipse)
        double   fSqrtD = sqrt (d) ;

        // If we are inside circle, then distort, else, just return the same position
        bool     bInside = (fSqrtD < fRadius) ;
        // exclude center point
        if (fSqrtD < FLT_EPSILON)
            bInside = false ;
        if (bInside)
        {
            //        double  fDist = sqrt (d / m_fRadiusScale) / m_fRadius ;
            double  fDist = fSqrtD / fRadius ;

            // Pinch
            double  fFactor = pow (sin (LIB_PI / 2.0 * fDist), -m_fPinch) ;
            dx *= fFactor ; dy *= fFactor ;

            // Whirl
            double   fAng = m_fWhirl * FSquare (1.0 - fDist) ;
            double   sina = sin (fAng), cosa = cos (fAng) ;
            un_x = (cosa * dx - sina * dy) / fScaleX + fCenX ;
            un_y = (sina * dx + cosa * dy) / fScaleY + fCenY ;
        }
        else
        {
            un_x = x ; un_y = y ;
        }
        return bInside;
    }

    double  m_fWhirl ; // radian of whirl
    double  m_fPinch ; // [-1.0, 1.0]
    //    double  m_fRadiusScale ; // [0.0, 2.0]
};

//=============================================================================
/**
 *  Fractal trace (>=24 bit).
@verbatim
    example:
        CMFCPixelFractalTrace  aCmd (2) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelFractalTrace : public CMFCPixelBilinearDistord
{
public:
    /// Constructor (nDepth >= 1).
    CMFCPixelFractalTrace (int nDepth) : m_nDepth(FMax(1,nDepth)) {}
private:
    void mandelbrot (const double& x, const double& y, double* u, double* v) const
    {
        int     iter = 0 ;
        double  xx = x, yy = y ;
        double  x2 = FSquare(xx), y2 = FSquare(yy) ;

        while (iter++ < m_nDepth)
        {
            const double   tmp = x2 - y2 + x ;
            yy = 2 * xx * yy + y ;
            xx = tmp ;
            x2 = FSquare(xx) ;
            y2 = FSquare(yy) ;
        }
        *u = xx ; *v = yy ;
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        double   fImgWidth = GetBackupImage()->GetImgWidth(),
                 fImgHeight = GetBackupImage()->GetImgHeight(),
                 fScaleX = 1.5 / fImgWidth,
                 fScaleY = 2.0 / fImgHeight,
                 cy = -1.0 + y * fScaleY,
                 cx = -1.0 + x * fScaleX,
                 px, py ;
        mandelbrot (cx, cy, &px, &py) ;
        un_x = (px + 1.0) / fScaleX ;
        un_y = (py + 1.0) / fScaleY ;

        if ( !(0 <= un_x && un_x < fImgWidth && 0 <= un_y && un_y < fImgHeight) )
        {
            un_x = fmod (un_x, fImgWidth) ;
            un_y = fmod (un_y, fImgHeight) ;
            if (un_x < 0.0)  un_x += fImgWidth ;
            if (un_y < 0.0)  un_y += fImgHeight ;
        }
        return true ;
    }
    int     m_nDepth ; // >=1
};

//=============================================================================
/**
 *  Lens (>=24 bit).
@verbatim
    example:
        CMFCPixelLens  aCmd (1.5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelLens : public CMFCPixelBilinearDistord
{
public:
    /// Constructor (fRefraction >= 1.0).
    CMFCPixelLens (double fRefraction, bool bKeepBk) : m_fRefraction(FMax(1.0,fRefraction)), m_bKeepBk(bKeepBk) {}
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess (pImg) ;

        // clear background ?
        if (!m_bKeepBk)
            memset (pImg->GetMemStart(), 0, pImg->GetImgPitch()*pImg->GetImgHeight()) ;
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        const double   fCenX = GetBackupImage()->GetImgWidth() / 2.0,
                       fCenY = GetBackupImage()->GetImgHeight() / 2.0,
                       asqr = FSquare(fCenX),
                       bsqr = FSquare(fCenY),
                       csqr = FSquare(FMin(fCenX,fCenY)),
                       dy = fCenY - y,
                       ysqr = FSquare(dy),
                       dx = x - fCenX,
                       xsqr = FSquare(dx) ;
        if (ysqr < (bsqr - (bsqr * xsqr) / asqr))
        {
            double  fTmp = sqrt ((1 - xsqr/asqr - ysqr/bsqr) * csqr) ;
            double  fTmpsqr = FSquare(fTmp) ;

            double  nxangle = acos (dx / sqrt(xsqr+fTmpsqr)) ;
            double  theta2 = asin (sin (LIB_PI/2.0 - nxangle) / m_fRefraction) ;
            theta2 = LIB_PI/2.0 - nxangle - theta2 ;
            double  xx = dx - tan (theta2) * fTmp ;

            double  nyangle = acos (dy / sqrt(ysqr+fTmpsqr)) ;
            theta2 = asin (sin (LIB_PI/2.0 - nyangle) / m_fRefraction) ;
            theta2 = LIB_PI/2.0 - nyangle - theta2 ;
            double  yy = dy - tan (theta2) * fTmp ;

            un_x = xx + fCenX ;
            un_y = fCenY - yy ;
            return true ;
        }
        return false ;
    }
    double   m_fRefraction ; // >= 1.0
    bool     m_bKeepBk ;
};

//=============================================================================
/**
 *  Skew transform (>=24 bit).
 */
class CMFCPixelSkew : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    /// @param ptPos, order by LT, RT, RB, LB
    CMFCPixelSkew (const POINT ptPos[4])
    {
        memcpy (m_ptNewPos, ptPos, sizeof(POINT) * 4) ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess (pImg) ;

        // create skewed image
        m_nNewWidth = FMax (abs(m_ptNewPos[0].x-m_ptNewPos[2].x), abs(m_ptNewPos[1].x-m_ptNewPos[3].x)) ;
        m_nNewHeight = FMax (abs(m_ptNewPos[0].y-m_ptNewPos[2].y), abs(m_ptNewPos[1].y-m_ptNewPos[3].y)) ;
        pImg->Create (m_nNewWidth, m_nNewHeight, pImg->GetImgColorBits()) ;
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        if (m_ptNewPos[0].x != m_ptNewPos[3].x)
        {
            // x axis slope
            const int      nDelta = m_ptNewPos[0].x - m_ptNewPos[3].x ;
            un_x = x - ((nDelta > 0) ? (m_nNewHeight - y) : y) * abs(nDelta) / (double)m_nNewHeight ;
            un_y = y * GetBackupImage()->GetImgHeight() / (double)m_nNewHeight ;
        }
        else if (m_ptNewPos[0].y != m_ptNewPos[1].y)
        {
            // y axis slope
            const int     nDelta = m_ptNewPos[0].y - m_ptNewPos[1].y ;
            un_x = x * GetBackupImage()->GetImgWidth() / (double)m_nNewWidth ;
            un_y = y - ((nDelta > 0) ? (m_nNewWidth - x) : x) * abs(nDelta) / (double)m_nNewWidth ;
        }
        else
        {
            un_x=x ; un_y=y;
        }

        if (un_x<0.0 || un_x>GetBackupImage()->GetImgWidth()-1 || un_y<0.0 || un_y>GetBackupImage()->GetImgHeight()-1)
            return false ;
        else
            return true ;
    }
private:
    POINT     m_ptNewPos[4] ; // LT, RT, RB, LB
    int       m_nNewWidth ;
    int       m_nNewHeight ;
};

//=============================================================================
/**
 *  Perspective transform (>=24 bit).
 */
class CMFCPixelPerspective : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    /// @param ptPos, order by LT, RT, RB, LB
    CMFCPixelPerspective (const POINT ptPos[4])
    {
        memcpy (m_ptNewPos, ptPos, sizeof(POINT) * 4) ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess (pImg) ;

        // create sloped image
        m_nNewWidth = FMax (abs(m_ptNewPos[0].x-m_ptNewPos[1].x), abs(m_ptNewPos[2].x-m_ptNewPos[3].x)) ;
        m_nNewHeight = FMax (abs(m_ptNewPos[0].y-m_ptNewPos[3].y), abs(m_ptNewPos[1].y-m_ptNewPos[2].y)) ;
        pImg->Create (m_nNewWidth, m_nNewHeight, pImg->GetImgColorBits()) ;
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        if (m_ptNewPos[0].y != m_ptNewPos[1].y)
        {
            // y axis perspective
            int     nDelta = abs(m_ptNewPos[0].y-m_ptNewPos[3].y) - abs(m_ptNewPos[1].y-m_ptNewPos[2].y) ;
            double  fOffset = fabs(nDelta * ((nDelta > 0) ? x : (m_nNewWidth-x)) / (2.0 * m_nNewWidth)) ;
            un_y = GetBackupImage()->GetImgHeight() * (y - fOffset) / (m_nNewHeight - 2.0 * fOffset) ;
            un_x = GetBackupImage()->GetImgWidth() * x / (double)m_nNewWidth ;
        }
        else if (m_ptNewPos[0].x != m_ptNewPos[3].x)
        {
            // x axis perspective
            int     nDelta = abs(m_ptNewPos[0].x-m_ptNewPos[1].x) - abs(m_ptNewPos[2].x-m_ptNewPos[3].x) ;
            double  fOffset = fabs(nDelta * ((nDelta > 0) ? y : (m_nNewHeight-y)) / (2.0 * m_nNewHeight)) ;
            un_x = GetBackupImage()->GetImgWidth() * (x - fOffset) / (m_nNewWidth - 2.0 * fOffset) ;
            un_y = GetBackupImage()->GetImgHeight() * y / (double)m_nNewHeight ;
        }
        else
        {
            un_x = x ; un_y = y ;
        }

        if (un_x<0.0 || un_x>GetBackupImage()->GetImgWidth()-1 || un_y<0.0 || un_y>GetBackupImage()->GetImgHeight()-1)
            return false ;
        else
            return true ;
    }
private:
    POINT     m_ptNewPos[4] ; // LT, RT, RB, LB
    int       m_nNewWidth ;
    int       m_nNewHeight ;
};

//=============================================================================
/**
 *  Rotate (>=24 bit).
@verbatim
    example:
        CMFCPixelRotate   aCmd (37) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelRotate : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    CMFCPixelRotate (int nAngle)
    {
        nAngle %= 360 ;
        while (nAngle < 0)
            nAngle += 360 ;

        m_fAngle = AngleToRadian(nAngle) ;
        m_fInvAngle = AngleToRadian(360 - nAngle) ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess (pImg) ;

        // calculate new width & height
        const POINT_F   ptCenter = {pImg->GetImgWidth()/2.0, pImg->GetImgHeight()/2.0} ;
        const POINT     ptLT = {0,0}, ptRT = {pImg->GetImgWidth(),0},
                        ptLB = {0,pImg->GetImgHeight()}, ptRB = {pImg->GetImgWidth(), pImg->GetImgHeight()} ;
        double          arX[4], arY[4] ;
        FClockwisePoint (ptLT, ptCenter, m_fAngle, arX[0], arY[0]) ;
        FClockwisePoint (ptRT, ptCenter, m_fAngle, arX[1], arY[1]) ;
        FClockwisePoint (ptLB, ptCenter, m_fAngle, arX[2], arY[2]) ;
        FClockwisePoint (ptRB, ptCenter, m_fAngle, arX[3], arY[3]) ;
        m_nNewWidth = FRound (FFindMAX(arX, 4) - FFindMIN(arX, 4)) ;
        m_nNewHeight = FRound (FFindMAX(arY, 4) - FFindMIN(arY, 4)) ;

        pImg->Create (m_nNewWidth, m_nNewHeight, pImg->GetImgColorBits()) ;
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        const POINT     ptRot = {x, y} ;
        const POINT_F   ptCenter = {m_nNewWidth/2.0, m_nNewHeight/2.0} ;
        FClockwisePoint (ptRot, ptCenter, m_fInvAngle, un_x, un_y) ;

        const double   fOffX = (m_nNewWidth - GetBackupImage()->GetImgWidth()) / 2.0,
                       fOffY = (m_nNewHeight - GetBackupImage()->GetImgHeight()) / 2.0 ;
        un_x -= fOffX ;
        un_y -= fOffY ;

        if (un_x<0.0 || un_x>GetBackupImage()->GetImgWidth()-1 || un_y<0.0 || un_y>GetBackupImage()->GetImgHeight()-1)
            return false ;
        else
            return true ;
    }

    double   m_fAngle, m_fInvAngle ;
    int      m_nNewWidth, m_nNewHeight ;
};

//=============================================================================
/**
 *  Ribbon (>=24 bit).
@verbatim
    example:
        CMFCPixelRibbon   aCmd (80, 30) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelRibbon : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    /// @param nSwing : [0..100],  percentage
    /// @param nFrequency : >=0,  a pi every 10
    CMFCPixelRibbon (int nSwing, int nFrequency)
    {
        m_nSwing = FClamp (nSwing, 0, 100) ;
        m_nFreq = FMax (nFrequency, 0) ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        CMFCPixelBilinearDistord::OnEnterProcess (pImg) ;
        
        // clear image
        memset (pImg->GetMemStart(), 0, pImg->GetImgPitch()*pImg->GetImgHeight()) ;

        m_nDelta = m_nSwing * pImg->GetImgHeight() * 75/100/100 ; // upper, max 75%
        const double   fAngleSpan = m_nFreq * LIB_PI / pImg->GetImgWidth() / 10.0 ;
        for (int x=0 ; x < pImg->GetImgWidth() ; x++)
        {
            int   nPush = FRound ((1.0-cos(x * fAngleSpan)) * m_nDelta / 2.0) ;
            m_ShiftDown.push_back (nPush) ;
        }
    }
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        un_x = x ;
        un_y = y + m_nDelta - m_ShiftDown[x] ;
        if ((un_y < m_nDelta) || (un_y > GetBackupImage()->GetImgHeight()-1))
            return false ;
        return true ;
    }

    int               m_nSwing, m_nFreq ;
    int               m_nDelta ;
    std::deque<int>   m_ShiftDown ;
};

//=============================================================================
/**
 *  Ripple (>=24 bit).
@verbatim
    example:
        CMFCPixelRipple   aCmd (10, 30) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelRipple : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    CMFCPixelRipple (int nWavelength, int nAmplitude, BOOL bSinType = true)
    {
        m_nWavelength = FMax (1, nWavelength) ;
        m_nAmplitude = FMax (1, nAmplitude) ;
        m_bSinType = bSinType ;
    }
private:
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        const double   fWidth = GetBackupImage()->GetImgWidth() ;
        un_x = fmod (x + fWidth + shift_amount(y), fWidth) ;
        un_y = y ;
        return true ;
    }
    double  shift_amount (int nPos) const
    {
        if (m_bSinType)
            return m_nAmplitude * sin(nPos*LIB_2PI/(double)m_nWavelength) ;
        else
            return floor (m_nAmplitude * (fabs ((((nPos % m_nWavelength) / (double)m_nWavelength) * 4) - 2) - 1)) ;
    }

    int     m_nWavelength ;
    int     m_nAmplitude ;
    BOOL    m_bSinType ;
};

//=============================================================================
/**
 *  Tile (>=24 bit).
@verbatim
    example:
        CMFCPixelSmallTile   aCmd (2,2) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelSmallTile : public CMFCPixelBilinearDistord
{
public:
    /// Constructor.
    CMFCPixelSmallTile (int nXNum, int nYNum) : m_nXNum(FMax(1,nXNum)), m_nYNum(FMax(1,nYNum)) {}
private:
    virtual bool calc_undistorted_coord (int x, int y, double& un_x, double& un_y) const
    {
        un_x = (x * m_nXNum) % GetBackupImage()->GetImgWidth() ;
        un_y = (y * m_nYNum) % GetBackupImage()->GetImgHeight() ;
        return true ;
    }

    int     m_nXNum ;
    int     m_nYNum ;
};

//=============================================================================
/// LUT(look up table) routine (>=24 bit)
class CMFCPixelLUTRoutine : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    /// @param nChannel : process channel, use OR to combine
    CMFCPixelLUTRoutine (IMAGE_CHANNEL nChannel = CHANNEL_RGB)
    {
        m_bChannelR = nChannel & CHANNEL_RED ;
        m_bChannelG = nChannel & CHANNEL_GREEN ;
        m_bChannelB = nChannel & CHANNEL_BLUE ;
    }
protected:
    /// Initialize LUT.
    virtual int InitLUTtable (int nLUTIndex) =0 ;
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        for (int i=0 ; i <= 0xFF ; i++)
            m_LUT[i] = this->InitLUTtable (i) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        if (m_bChannelB)  PCL_B(pPixel) = m_LUT[PCL_B(pPixel)] ;
        if (m_bChannelG)  PCL_G(pPixel) = m_LUT[PCL_G(pPixel)] ;
        if (m_bChannelR)  PCL_R(pPixel) = m_LUT[PCL_R(pPixel)] ;
    }
private:
    int      m_LUT[256] ;
    BOOL     m_bChannelR, m_bChannelG, m_bChannelB ;
};

//=============================================================================
/**
 *  Adjust brightness (>=24 bit).
@verbatim
    example:
        CMFCPixelBrightness   aCmd (150) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelBrightness : public CMFCPixelLUTRoutine
{
public:
    /// Constructor (param's unit is percentage).
    CMFCPixelBrightness (int nPercent, IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel), m_nPercent(FMax(0,nPercent)) {}
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        return FClamp0255 (nLUTIndex * m_nPercent / 100) ;
    }
    int     m_nPercent ;
};

//=============================================================================
/**
 *  Adjust contrast (>=24 bit).
@verbatim
    example:
        CMFCPixelContrast   aCmd (150) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelContrast : public CMFCPixelLUTRoutine
{
public:
    /// Constructor (param's unit is percentage).
    CMFCPixelContrast (int nPercent, IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel), m_nPercent(FMax(0,nPercent)) {}
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        return FClamp0255 (128 + (nLUTIndex - 128) * m_nPercent / 100) ;
    }
    int     m_nPercent ;
};

//=============================================================================
/**
 *  Adjust gamma (>=24 bit).
@verbatim
    example:
        CMFCPixelGamma   aCmd (0.5) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelGamma : public CMFCPixelLUTRoutine
{
public:
    /// Constructor (param must >= 0.0).
    CMFCPixelGamma (double fGamma, IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel)
    {
        fGamma = FMax (0.0, fGamma) ;
        m_fInvGamma = 1.0 / fGamma ;
    }
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        double   fMax = pow (255.0, m_fInvGamma) / 255.0 ;
        return FClamp0255 (FRound (pow((double)nLUTIndex, m_fInvGamma) / fMax)) ;
    }
    double     m_fInvGamma ;
};

//=============================================================================
/**
 *  Negate image(>=24 bit).
@verbatim
    example:
        CMFCPixelInvert   aCmd ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelInvert : public CMFCPixelLUTRoutine
{
public:
    CMFCPixelInvert (IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel) {}
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        return (255 - nLUTIndex) ;
    }
};

//=============================================================================
/**
 *  Solarize image(>=24 bit).
@verbatim
    example:
        CMFCPixelSolarize   aCmd (128) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelSolarize : public CMFCPixelLUTRoutine
{
public:
    /// Constructor (nThreshold in [0-255]).
    CMFCPixelSolarize (int nThreshold, IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel), m_nThreshold(FClamp0255(nThreshold)) {}
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        return (nLUTIndex >= m_nThreshold) ? (255 - nLUTIndex) : nLUTIndex ;
    }
    int     m_nThreshold ;
};

//=============================================================================
/**
 *  Posterize image(>=24 bit).
@verbatim
    example:
        CMFCPixelPosterize   aCmd (2) ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelPosterize : public CMFCPixelLUTRoutine
{
public:
    /// Constructor (nLevel >= 2).
    CMFCPixelPosterize (int nLevel, IMAGE_CHANNEL nChannel = CHANNEL_RGB) : CMFCPixelLUTRoutine(nChannel), m_nLevel(FMax(2,nLevel)) {}
private:
    virtual int InitLUTtable (int nLUTIndex)
    {
        double   du1 = 255.0 / (m_nLevel - 1.0) ;
        return FClamp0255 (FRound (du1 * FRound (nLUTIndex / du1))) ;
    }
    int     m_nLevel ;
};

//=============================================================================
/**
*  Count image's number of color (>=24 bit).
@verbatim
    example:
        CMFCPixelColorsCount   aCmd ;
        img.PixelProcessProc (aCmd) ;
        aCmd.GetColorsNumber() ;
@endverbatim
 */
class CMFCPixelColorsCount : public CMFCBasePixelProcess
{
public:
    CMFCPixelColorsCount() : m_pMap(0), m_nCount(0) {}
    virtual ~CMFCPixelColorsCount() {if(m_pMap) delete[] m_pMap;}
    /// Get used number of color.
    unsigned int GetColorsNumber() const {return m_nCount;}
protected:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        int     iMaxColor = 1 << 24 ;
        m_pMap = new BYTE[iMaxColor+1] ;
        memset (m_pMap, 0, iMaxColor+1) ;
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel)
    {
        DWORD     i = 0 ;
        FCColor::CopyPixel (&i, pPixel, 3) ;
        if (m_pMap[i] == 0)
        {
            m_pMap[i] = 1 ;
            m_nCount++ ;
        }
    }

    unsigned int  m_nCount ;
    BYTE          * m_pMap ;
};

//=============================================================================
/**
*  Find a color unused in image (>=24 bit).
@verbatim
    example:
        CMFCPixelGetKeyColor   aCmd ;
        img.PixelProcessProc (aCmd) ;
        aCmd.IsFind() ;
        aCmd.GetKeyColor() ;
@endverbatim
 */
class CMFCPixelGetKeyColor : public CMFCPixelColorsCount
{
public:
    /// Is found a color unused in image.
    bool IsFind() const {return m_bFind;}
    /// Get the color unused in image.
    RGBQUAD GetKeyColor() const {return m_crKey;}
private:
    virtual void OnLeaveProcess (CMFCImageObj* pImg)
    {
        m_bFind = false ;
        for (int i=0 ; i <= 0xFFFFFF ; i++)
            if (m_pMap[i] == 0)
            {
                *(DWORD*)&m_crKey = i ;
                m_bFind = true ;
                break ;
            }
    }

    RGBQUAD   m_crKey ;
    bool      m_bFind ;
};

//=============================================================================
/**
 *  Save a ASCII text file (>=24 bit).
@verbatim
    example:
        CMFCPixelExportAscII   aCmd ("c:\\PhoXo.txt") ;
        img.PixelProcessProc (aCmd) ;
@endverbatim
 */
class CMFCPixelExportAscII : public CMFCBasePixelProcess
{
public:
    /// Constructor.
    CMFCPixelExportAscII (const char* szFileName)
    {
        m_outFile.open (szFileName, std::ios::out|std::ios::binary|std::ios::trunc) ;
        if (!m_outFile.is_open())
            {assert(false); return;}

        char   ch[95] =
        {
            ' ',
            '`','1','2','3','4','5','6','7','8','9','0','-','=','\\',
            'q','w','e','r','t','y','u','i','o','p','[',']',
            'a','s','d','f','g','h','j','k','l',';','\'',
            'z','x','c','v','b','n','m',',','.','/',
            '~','!','@','#','$','%','^','&','*','(',')','_','+','|',
            'Q','W','E','R','T','Y','U','I','O','P','{','}',
            'A','S','D','F','G','H','J','K','L',':','"',
            'Z','X','C','V','B','N','M','<','>','?'
        };
        int   gr[95] =
        {
            0,
            7,22,28,31,31,27,32,22,38,32,40, 6,12,20,38,32,26,20,24,40,
            29,24,28,38,32,32,26,22,34,24,44,33,32,32,24,16, 6,22,26,22,
            26,34,29,35,10, 6,20,14,22,47,42,34,40,10,35,21,22,22,16,14,
            26,40,39,29,38,22,28,36,22,36,30,22,22,36,26,36,25,34,38,24,
            36,22,12,12,26,30,30,34,39,42,41,18,18,22
        };

        // Bubble Sort
        for (int i=0 ; i < 94 ; i++)
            for (int j=i+1 ; j < 95 ; j++)
                if (gr[i] > gr[j])
                {
                    FSwap (ch[i], ch[j]) ;
                    FSwap (gr[i], gr[j]) ;
                }

        memcpy (m_chIndex, ch, 95*sizeof(char)) ;
        memcpy (m_nGray, gr, 95*sizeof(int)) ;
    }
private:
    virtual void OnEnterProcess (CMFCImageObj* pImg)
    {
        SetBackupImage (pImg) ;
        CMFCPixelInvert     aCmd ; // most of image is brightness
        GetBackupImage()->PixelProcessProc (aCmd) ;
        CMFCPixelConvertTo8BitGray   aGray ;
        GetBackupImage()->PixelProcessProc (aGray) ;

        const int     nTransWidth = pImg->GetImgWidth() / 8,
                      nTransHeight = pImg->GetImgHeight() / 16 ;
        for (int y=0 ; y < nTransHeight ; y++)
        {
            for (int x=0 ; x < nTransWidth ; x++)
            {
                int     nGray = 0 ;
                for (int k=0 ; k < 16 ; k++)
                    for(int h=0 ; h < 8 ; h++)
                    {
                        BYTE   * pGray = GetBackupImage()->GetBits (8*x+h, y*16+k) ;
                        nGray += *pGray ;
                    }
                    nGray /= 16*8 ;
                    nGray = m_nGray[94] * nGray / 255 ;
                    int   t = 0 ;
                    while (m_nGray[t+1] < nGray)
                        t++ ;
                    m_outFile.write ((char *)&m_chIndex[t],sizeof(char)) ;
            }
            char    tchar = (char)0x0D ;
            m_outFile.write ((char*)&tchar,sizeof(char)) ;
            tchar = (char)0x0A ;
            m_outFile.write ((char*)&tchar,sizeof(char)) ;
        }
    }
    virtual void ProcessPixel (CMFCImageObj* pImg, int x, int y, BYTE* pPixel) {}

    char     m_chIndex[95] ;
    int      m_nGray[95] ;
    std::ofstream   m_outFile ;
};

//=============================================================================
// inline Implement
//=============================================================================

#endif
