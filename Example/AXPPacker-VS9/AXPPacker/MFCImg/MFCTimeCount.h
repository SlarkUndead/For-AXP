
#ifndef _MFC_TIMECOUNT_06_07_09_H_
#define _MFC_TIMECOUNT_06_07_09_H_
#include <time.h>

//=============================================================================
// Time counter. unit : millisecond
class CMFCTimeCount
{
public:
	CMFCTimeCount() {SetStartTag();}			// Constructor (will set start tag).
	void SetStartTag() {m_nTick = ::clock();}	// Set start tag.
	// Get passed millisecond from start tag.
	long GetPassMillisecond() const {return tick_to_ms(::clock() - m_nTick);}

	static long tick_to_ms (const clock_t& nTick)
	{
		return (long)(nTick * 1000 / CLOCKS_PER_SEC) ;
	}

private:
	clock_t   m_nTick ;
};
#endif