#ifndef _MFC_INTERFACE_ZOOMSCALE_06_07_09_H_
#define _MFC_INTERFACE_ZOOMSCALE_06_07_09_H_

//=============================================================================
// Zoom scale support.
class CMFCIZoomScale
{
public:
	CMFCIZoomScale() : m_nScale(1) {}
	virtual ~CMFCIZoomScale() {}

	void SetZoomScale(int nScale){m_nScale = nScale; assert(nScale != 0);}	// Set zoom scale.
	int GetZoomScale()const{return m_nScale;}								// Get zoom scale.

	// Coordinate transformation:scaled <==> actual
	// T : POINT / POINT_F
	template<class T> void  Scaled_to_Actual(T& pt) const
	{
		if (m_nScale > 1)
		{
			pt.x /= m_nScale ;
			pt.y /= m_nScale ;
		}
		else
			if (m_nScale < -1)
			{
				pt.x *= -m_nScale ;
				pt.y *= -m_nScale ;
			}
	}
	void  Scaled_to_Actual (RECT& rc) const
	{
		Scaled_to_Actual (*(POINT*)&rc.left) ;
		Scaled_to_Actual (*(POINT*)&rc.right) ;
	}
	void  Scaled_to_Actual (RECT_F& rc) const
	{
		Scaled_to_Actual (*(POINT_F*)&rc.left) ;
		Scaled_to_Actual (*(POINT_F*)&rc.right) ;
	}

	// T : POINT / POINT_F
	template<class T> void  Actual_to_Scaled (T& pt) const
	{
		if (m_nScale > 1)
		{
			pt.x *= m_nScale ;
			pt.y *= m_nScale ;
		}
		else
			if (m_nScale < -1)
			{
				pt.x /= -m_nScale ;
				pt.y /= -m_nScale ;
			}
	}
	void  Actual_to_Scaled (RECT& rc) const
	{
		Actual_to_Scaled (*(POINT*)&rc.left) ;
		Actual_to_Scaled (*(POINT*)&rc.right) ;
	}

private:
	int     m_nScale ;
};

#endif