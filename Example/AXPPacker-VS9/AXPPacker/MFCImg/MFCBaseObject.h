#ifndef _MFC_BASEOBJECT_060701_H_
#define _MFC_BASEOBJECT_060701_H_

#include "MFCStructs.h"
//=============================================================================
//  The Root of all objects
class CMFCObject
{
public:
	virtual ~CMFCObject() {}
	/**
	*  Save object to memory / load object from memory.
	*    @param bSave : save(true) & load(false)
	*    @return return written/loaded bytes.
	*/
	virtual int Serialize (bool bSave, BYTE* pSave) {return 0;}
};

//=============================================================================
//  Basic graphic object (encapsulate object's coordinate).
class CMFCGraphObj : public CMFCObject
{
public:
	CMFCGraphObj() {m_ptObj.x = m_ptObj.y = 0;}
	CMFCGraphObj& operator= (const CMFCGraphObj& GraphObj){m_ptObj = GraphObj.m_ptObj; return *this;}

	/**
	*  Save graphic object to memory / load graphic object from memory.
	*  @see CMFCObject::Serialize
	*/
	virtual int Serialize (bool bSave, BYTE* pSave)
	{
		bSave ? (*(POINT*)pSave = m_ptObj) : (m_ptObj = *(POINT*)pSave) ;
		return sizeof(m_ptObj) ;
	}

	/**
	*  @name Object's position
	*  object's position on canvas.
	*/
	//@{
	// Set position of graph.
	void SetGraphObjPos (int xPos, int yPos) {m_ptObj.x=xPos; m_ptObj.y=yPos;}
	// Set position of graph.
	void SetGraphObjPos (const POINT& pos) {m_ptObj = pos;}
	// Get position of graph.
	POINT GetGraphObjPos() const {return m_ptObj;}
	// Offset position of graph.
	void OffsetGraphObj (int xOff, int yOff) {m_ptObj.x += xOff; m_ptObj.y += yOff;}
	//@}

	/**
	*  @name Coordinate transformation
	*  canvas <==> layer
	*/
	//@{
	/// canvas ==> layer, T can be POINT or POINT_F
	template<class T> void CanvasToLayer (T& pt) const
	{
		pt.x -= m_ptObj.x ; pt.y -= m_ptObj.y ;
	}
	void CanvasToLayer (RECT& rc) const
	{
		CanvasToLayer (*(POINT*)&rc.left) ;
		CanvasToLayer (*(POINT*)&rc.right) ;
	}
	void CanvasToLayer (RECT_F& rc) const
	{
		CanvasToLayer (*(POINT_F*)&rc.left) ;
		CanvasToLayer (*(POINT_F*)&rc.right) ;
	}

	/// layer ==> canvas, T can be POINT or POINT_F
	void LayerToCanvas (POINT& pt) const
	{
		pt.x += m_ptObj.x ; pt.y += m_ptObj.y ;
	}
	void LayerToCanvas (RECT& rc) const
	{
		LayerToCanvas (*(POINT*)&rc.left) ;
		LayerToCanvas (*(POINT*)&rc.right) ;
	}
	//@}

private:
	POINT     m_ptObj ; // object position on canvas
};

#endif