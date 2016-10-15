
#ifndef _MFC_TEMPLATE_COMPOSITE_060706_H_
#define _MFC_TEMPLATE_COMPOSITE_060706_H_

//=============================================================================
// composite support. enable your class contain composite object, for example:
// class CMultiImage : public CMFCObject,private CMFCIComposite<CImage>
// {
//    int GetImageCount() const { return GetObjectCount(); }
//    CImage* GetImage (int nIndex) const { return GetObject(nIndex); }
// };
template<class T> class CMFCIComposite
{
public:
	/// Deconstructor will call all DeleteAllObjects to delete all objects.
	virtual ~CMFCIComposite() {DeleteAllObjects();}

	/// Get number of object.
	int GetObjectCount() const{return (int)m_objList.size();}

	/**
	*  Add an object (created by new) into list (add a NULL object is permitted).
	*  after the object be added, you can't delete it any more.
	*/
	void PushObject(T* pObj){m_objList.push_back (pObj);} // push NULL is permitted

	/// Get object in list, nIndex is a zero-based index.
	T* GetObject(int nIndex) const
	{
		if((nIndex >= 0) && (nIndex < GetObjectCount()))
			return m_objList[nIndex] ;
		else
			return 0;
	}

	/// Whether the object has been added in list.
	bool HasInList(const T* pObj)const
	{
		return (std::find (m_objList.begin(), m_objList.end(), pObj) != m_objList.end()) ;
	}

	/// Throw objects' ownership. you must delete object yourself.
	void ThrowOwnership(std::deque<T*>& listObj)
	{
		listObj = m_objList ;
		m_objList.clear() ;
	}

	/// Delete all objects in list.
	void DeleteAllObjects()
	{
		while(!m_objList.empty())
		{
			T * p = m_objList.back() ;
			m_objList.pop_back() ;
			if(p)
				delete p ;
		}
	}

	/// Remove & delete object from list.
	void DeleteObject(int nIndex)
	{
		if((nIndex >= 0) && (nIndex < PCL_GetObjectCount()))
		{
			T   * p = m_objList[nIndex] ;
			m_objList.erase(m_objList.begin() + nIndex) ;
			if (p)
				delete p ;
		}
	}

	/// Delete object in list.
	void DeleteObject(T* pObj)
	{
		if(std::find (m_objList.begin(), m_objList.end(), pObj) != m_objList.end())
		{
			m_objList.erase(std::find (m_objList.begin(), m_objList.end(), pObj)) ;
			if (pObj)
				delete pObj ;
		}
	}

private:
	std::deque<T*>   m_objList ;
};
#endif