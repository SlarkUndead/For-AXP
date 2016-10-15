
#ifndef _MFC_BASETEMPLATE_060701_H_
#define _MFC_BASETEMPLATE_060701_H_

#include "MFCStdDefine.h"
#include "MFCStructs.h"

//////////////////////////////////////////////////////////////////////////
//
// 基本的template
//
//////////////////////////////////////////////////////////////////////////
//=============================================================================
// 将数值填充到描述颜色的结构里
template<class T> inline RGBQUAD MFC_RGBA (T r, T g, T b, T a=0xFF)
{
	RGBQUAD   crRet ;
	MFC_R(&crRet)=r ; MFC_G(&crRet)=g ; MFC_B(&crRet)=b ; MFC_A(&crRet)=a ;
	return crRet ;
}
//=============================================================================
// 从数组里得到最大/最小值
template<class T> inline T MFCFindMAXOfArray(T * pList, int nNum)
{
	T tMax = pList[0] ;
	for(int i=1 ; i < nNum ; i++)
	{
		if(pList[i] > tMax)
			tMax = pList[i] ;
	}
	return tMax ;
}
//=============================================================================
// 从数组里得到最小值
template<class T> inline T MFCFindMINOfArray(T * pList, int nNum)
{
	T tMin = pList[0] ;
	for(int i=1 ; i < nNum ; i++)
	{
		if(pList[i] < tMin)
			tMin = pList[i] ;
	}
	return tMin ;
}
//=============================================================================
// 求最大最小值||数值交换||数值平方||Bound in [tLow,tHigh]
template<class T> inline const T& MFCMax(const T& _X,const T& _Y){return (_X < _Y ? _Y : _X);}
template<class T> inline const T& MFCMin(const T& _X,const T& _Y){return (_Y < _X ? _Y : _X);}
template<class T> inline void MFCSwap(T& t1, T& t2){ const T tmp=t1 ; t1=t2 ; t2=tmp ;}
template<class T> inline T MFCSquare(const T& nValue){return nValue*nValue ;}
template<class T> inline T MFCClamp(const T& tValue,const T& tLow,const T& tHigh)
{
	return MFCMax(tLow,MFCMin(tHigh,tValue));
}
//=============================================================================
// 计算直角三角形斜边长度Hypotenuse, c2 = a2 + b2
template<class T> inline T MFCHypot(const T& x,const T& y)
{
	return (T)sqrt(MFCSquare((double)x) + MFCSquare((double)y));
}
#endif