
#ifndef _MFC_ARRAY_060701_H_
#define _MFC_ARRAY_060701_H_

//=============================================================================
/**
*  Auto delete array.
*/
template<class T> class CMFCArray
{
public:
	/// Attach pArray, pArray must use <B>new []</B> to create (no bound check).
	CMFCArray (void* pArray)
	{
		m_pArray = 0 ;
		m_pArray = (T*)pArray;
		m_nNumberT = -1 ;
	}
	// Alloc nNumberT T array (with DEBUG-time bound check).
	CMFCArray(int nNumberT)
	{
		if (nNumberT > 0)
		{
			m_pArray = new T[nNumberT] ;
			m_nNumberT = nNumberT ;
		}
		else
		{
			m_pArray = 0 ;
			m_nNumberT = -1;
		}
	}
	/// delete[] array
	~CMFCArray()
	{
		if (m_pArray)
			delete[] m_pArray ;
	}

	// Get element.[NOTE:ÎÞ±ß½ç¼ì²â!!!]
	T& operator[](int n) const
	{
		//assert ((m_nNumberT == -1) ? true : (n < m_nNumberT)) ;
		return m_pArray[n] ;
	}

	/// Get array start pointer.
	T* GetArrayPtr(void) const {return m_pArray;}

private:
	T     * m_pArray ;
	int   m_nNumberT ;
};

#endif