#ifndef _MFC_BASEFUNC_060701_H_
#define _MFC_BASEFUNC_060701_H_

#include "MFCStructs.h"
#include "MFCBaseTemplate.h"



//////////////////////////////////////////////////////////////////////////
//
// inline�ͻ�������
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//
// ����
//
//=============================================================================
// ���ΰ������β���
inline bool IsRectInRect(const RECT & rcOut,const RECT & rcIn)
{
	return (rcIn.left >= rcOut.left) && (rcIn.top >= rcOut.top) && (rcIn.right <= rcOut.right) && (rcIn.bottom <= rcOut.bottom) ;
}
//=============================================================================
// ���Ե��ھ�����
inline bool IsPtInRect(const RECT* pRC, const POINT& pt)
{
	return pRC && (pt.x >= pRC->left) && (pt.x < pRC->right) &&
		(pt.y >= pRC->top) && (pt.y < pRC->bottom) ;
}
//=============================================================================
// ���Բ���ȡ�������ε��ཻ����
inline bool IsIntersectRect(RECT* pDst,const RECT* pSrc1,const RECT* pSrc2)
{
	if(pDst && pSrc1 && pSrc2)
	{
		pDst->left = MFCMax(pSrc1->left, pSrc2->left) ;
		pDst->top  = MFCMax(pSrc1->top, pSrc2->top) ;
		pDst->right  = MFCMin(pSrc1->right, pSrc2->right) ;
		pDst->bottom = MFCMin(pSrc1->bottom, pSrc2->bottom) ;

		if(IsRectEmpty(pDst))
		{
			memset (pDst, 0, sizeof(RECT)) ; // set rect empty
			return false ;
		}
		return true ;
	}
	return false ;
}
#ifndef WIN32 // ��û����ʱ�Զ������º���
	//=============================================================================
	// ���ε���Ч�Բ���
	inline bool IsRectEmpty(const RECT* pRC)
	{
		return (pRC ? ((pRC->right <= pRC->left)||(pRC->bottom <= pRC->top)):true);
	}
	//=============================================================================
	// �Ծ��ν���һ����ƫ�ƴ���
	inline RECT & OffsetRect(RECT& rRect,const POINT& ptOff)
	{
		rRect.left+=ptOff.x ; rRect.top+=ptOff.y ;
		rRect.right+=ptOff.x ; rRect.bottom+=ptOff.y ;
		return rRect;
	}
	inline RECT * OffsetRect(RECT* pRC,int dx, int dy)
	{
		if(pRC)
		{
			pRC->left+=dx ; pRC->top+=dy ;
			pRC->right+=dx ; pRC->bottom+=dy ;
		}
		return pRC;
	}
	//=============================================================================
	// ���ž�������
	inline RECT * InflateRect(RECT* pRC, int dx, int dy)
	{
		if (pRC)
		{
			pRC->left-=dx ; pRC->top-=dy ;
			pRC->right+=dx ; pRC->bottom+=dy ;
		}
		return pRC;
	}
	//=============================================================================
	// ���Ͼ���
	inline RECT * UnionRect(RECT* pDst,const RECT* pSrc1,const RECT* pSrc2)
	{
		if (pDst && pSrc1 && pSrc2)
		{
			pDst->left = MFCFMin(pSrc1->left, pSrc2->left) ;
			pDst->top  = MFCFMin(pSrc1->top, pSrc2->top) ;
			pDst->right  = MFCFMax(pSrc1->right, pSrc2->right) ;
			pDst->bottom = MFCFMax(pSrc1->bottom, pSrc2->bottom) ;

			if(IsRectEmpty(pDst))
				memset (pDst,0, sizeof(RECT)) ; // set rect empty
		}
		return pDst;
	}
	//=============================================================================
	// �������
	inline RECT * SetRect(RECT* pRC,int xLeft,int yTop,int xRight,int yBottom)
	{
		if (pRC)
		{
			pRC->left  = xLeft  ; pRC->top    = yTop ;
			pRC->right = xRight ; pRC->bottom = yBottom ;
		}
		return pRC;
	}
#endif // WIN32


//=============================================================================
inline long GetRectWidth(const RECT & fRect) {return fRect.right - fRect.left;}
inline long GetRectHeight(const RECT & fRect) {return fRect.bottom - fRect.top;}

//=============================================================================
inline int MFCClamp0255(int nValue){return MFCClamp(nValue,0,0xFF);}						// [0,255]
inline int MFCRound(const double& x){return (int)((x>0.0)?(x+0.5):(x-0.5));}	// ��������

inline double AngleToRadian(int nAngle) {return MFC_PI * nAngle/180.0;}		// angle ==> radian
inline double RadianToAngle(double dRadian){return 180.0 * dRadian/MFC_PI;}	// radian ==> angle

//=============================================================================
// ��������ƽ��
inline int GetMFCPointDistanceSquare(const POINT& pt1,const POINT& pt2)
{
	return MFCSquare(pt1.x - pt2.x) + MFCSquare(pt1.y - pt2.y) ;
}

//=============================================================================
// ����ֱ��֮��ļн�(����н�) [0��n]
inline double GetMFCPointAngle(const POINT& pt1,const POINT& pt2,const POINT& ptCenter)
{
	const int nPt1 = GetMFCPointDistanceSquare(pt1,ptCenter),
		      nPt2 = GetMFCPointDistanceSquare(pt2,ptCenter);
	if((nPt1 == 0) || (nPt2 == 0))
		return -MFC_2PI ; // pt1/pt2 == center

	const int nTemp = nPt1 + nPt2 - GetMFCPointDistanceSquare(pt1,pt2) ;
	return acos(nTemp/(2.0*sqrt((double)nPt1)*sqrt((double)nPt2))) ;
}
//=============================================================================
// ����н�[0��2n]��pt1 --> pt2 ��˳ʱ�뷽��
inline double GetMFCPointAngleClockwise(const POINT &pt1,const POINT &pt2,const POINT &ptCenter)
{
	const double fAngle = GetMFCPointAngle(pt1,pt2,ptCenter) ;
	if (fAngle < 0.0) // ��һ���center�غ�
		return fAngle ;
	if (pt1.x == ptCenter.x) // ��ֱ���
		return (pt2.x >= ptCenter.x) ? fAngle : (MFC_PI - fAngle) ;
	// �ж���ֱ���ϻ�����
	const double yLine = ptCenter.y + (pt2.x - ptCenter.x) * (ptCenter.y - pt1.y)/(double)(ptCenter.x - pt1.x) ;
	if(pt2.y > yLine)
		return (pt1.x >= ptCenter.x) ? fAngle : (MFC_2PI - fAngle) ;
	return (pt1.x >= ptCenter.x) ? (MFC_2PI - fAngle) : fAngle ;
}
//=============================================================================
// pt1��ptCenter˳ʱ����ת [0, 2n] �������
inline POINT & GetMFCCCWPoint(const POINT & pt1,const POINT_F & ptCenter,const double & fAngle,POINT & fOutPnt)
{
	double dx = pt1.x - ptCenter.x, dy = -pt1.y + ptCenter.y,
		   cost = cos(fAngle), sint = sin(fAngle);
	fOutPnt.x = (LONG)(ptCenter.x + (dx*cost + dy*sint));
	fOutPnt.y = (LONG)(ptCenter.y - (dy*cost - dx*sint));
	return fOutPnt;
}
//=============================================================================
 // pt1��ptCenter˳ʱ����ת [0, 2n] �������
inline POINT GetMFCCCWPoint(const POINT &pt1,const POINT &ptCenter,const double & fAngle)
{
	const POINT_F ptCen = {ptCenter.x, ptCenter.y} ;
	POINT ptRet;
	GetMFCCCWPoint(pt1,ptCen,fAngle,ptRet) ;

	ptRet.x = MFCRound(ptRet.x);
	ptRet.y = MFCRound(ptRet.y);
	return ptRet ;
}


#endif