#ifndef _MFC_OBJECTPROGRESS_060705_H_
#define _MFC_OBJECTPROGRESS_060705_H_

//=============================================================================
// Progress interface.
class CMFCObjProgress
{
public:
    CMFCObjProgress() : m_nProgress(0) {}
    virtual ~CMFCObjProgress() {}
    virtual void ResetProgress() {this->SetProgress(0);} // Set current progress to 0
    virtual int GetProgress() const{return m_nProgress;} // Get current progress.
    virtual void SetProgress(int nNew){m_nProgress=nNew;}// Set current progress.

private:
    int     m_nProgress ;
} ;

//=============================================================================
// inline implement
//=============================================================================

#endif
