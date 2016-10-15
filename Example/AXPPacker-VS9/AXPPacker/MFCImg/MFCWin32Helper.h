#ifndef _MFC_WIN32_HELPER_060705_H_
#define _MFC_WIN32_HELPER_060705_H_

	#ifdef WIN32
		#include "MFCStdDefine.h"
		#include "MFCBaseFunc.h"
		#include "MFCImageObj.h"
		#include "MFCArray.h"
		//=============================================================================
		// WIN32 API wrapper.
		class CMFCWin32Helper
		{
		public:
			//=============================================================================
			/// Test vKey downing now.
			static BOOL IsKeyDown(int vKey)
			{
				return (::GetKeyState(vKey) & 0x8000) ;
			}
			//=============================================================================
			// Convert a UTF-8 string to a ASCII string.
			static std::string UTF8_to_ANSI(const char* szUTF8)
			{
				if (szUTF8 == 0)
					return "" ;

				int nLen = MultiByteToWideChar(CP_UTF8, 0, szUTF8, -1, 0, 0);
				LPWSTR lpwsz = new WCHAR[nLen];
				MultiByteToWideChar(CP_UTF8, 0, szUTF8, -1, lpwsz, nLen);
				int nLen1 = WideCharToMultiByte(CP_ACP, 0, lpwsz, nLen, 0, 0, 0, 0);
				LPSTR lpsz = new CHAR[nLen1];
				WideCharToMultiByte(CP_ACP, 0, lpwsz, nLen, lpsz, nLen1, 0, 0);
				std::string strAnsi(lpsz);
				delete []lpwsz;
				delete []lpsz;
				return strAnsi;
			}
			//=============================================================================
			// Convert a ASCII string to a UTF-8 string.
			static std::string ANSI_to_UTF8 (const char* szAnsi)
			{
				if (szAnsi == 0)
					return "" ;
				int nLen = MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, 0, 0);
				LPWSTR lpwsz = new WCHAR[nLen];
				MultiByteToWideChar(CP_ACP, 0, szAnsi, -1, lpwsz, nLen);
				int nLen1 = WideCharToMultiByte(CP_UTF8, 0, lpwsz, nLen, 0, 0, 0, 0);
				LPSTR pUTF8 = new CHAR[nLen1];
				WideCharToMultiByte(CP_UTF8, 0, lpwsz, nLen, pUTF8, nLen1, 0, 0);
				std::string strUTF(pUTF8) ;
				delete [] lpwsz;
				delete [] pUTF8 ;
				return strUTF ;
			}
			//=============================================================================
			// Autorun at startup of windows.
			static BOOL IsAutoRunEnable(LPCTSTR szValue)//Is autorun enable.
			{
				HKEY hKey = 0 ;
				TCHAR sK[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") ;
				LONG l = ::RegOpenKeyEx(uis_GetRootKey(), sK, 0, KEY_ALL_ACCESS, &hKey) ;
				if (l != ERROR_SUCCESS)
					return FALSE;

				TCHAR szPath[MAX_PATH] = {0} ;
				DWORD dw = sizeof(TCHAR) * MAX_PATH,dwType = REG_SZ ;
				::RegQueryValueEx (hKey, szValue, NULL, &dwType, (BYTE*)szPath, &dw) ;
				::RegCloseKey (hKey) ;
				return ::lstrlen(szPath) ? TRUE : FALSE ;
			}
			// ---------------------------------------------------------------------------------
			static void SetAutoRunEnable(LPCTSTR szValue,BOOL bEnable) // Set autorun enable.
			{
				HKEY hKey = 0 ;
				TCHAR sK[] = TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run") ;
				LONG l = ::RegOpenKeyEx(uis_GetRootKey(), sK, 0, KEY_ALL_ACCESS, &hKey) ;
				if (l != ERROR_SUCCESS)
					return;

				::RegDeleteValue (hKey, szValue) ;
				if(bEnable)
				{
					TCHAR szPath[MAX_PATH] = {0} ;
					::GetModuleFileName(0, szPath, MAX_PATH) ;
					::RegSetValueEx(hKey,szValue, 0,REG_SZ,(BYTE*)szPath,(::lstrlen(szPath)+1)*sizeof(TCHAR)) ;
				}
				::RegCloseKey (hKey) ;
			}
			//=============================================================================
			// IE's menu item.
			static BOOL IsIEMenuItemEnable(LPCTSTR szItemShow) // Is IE's menu item enable.
			{
				TCHAR szGroup[MAX_PATH] = TEXT("Software\\Microsoft\\Internet Explorer\\MenuExt\\") ;
				::lstrcat(szGroup, szItemShow) ;

				HKEY hKey = 0 ;
				BOOL bRet = (::RegOpenKeyEx(HKEY_CURRENT_USER,szGroup,0,KEY_ALL_ACCESS,&hKey) == ERROR_SUCCESS) ;
				::RegCloseKey (hKey) ;
				return bRet ;
			}
			// ---------------------------------------------------------------------------------
			static void SetIEMenuItem(BOOL bAdd,LPCTSTR szItemShow,LPCTSTR szHtmlPath)//Add/Remove IE's menu item.
			{
				TCHAR szKey[MAX_PATH] = TEXT("Software\\Microsoft\\Internet Explorer\\MenuExt\\") ;
				::lstrcat(szKey, szItemShow) ;
				::RegDeleteKey(HKEY_CURRENT_USER, szKey) ;
				if(bAdd)
				{
					HKEY hKey = 0 ;
					LONG l = ::RegCreateKeyEx (HKEY_CURRENT_USER,szKey,0,0,0,KEY_ALL_ACCESS,0, &hKey,0) ;
					if (l != ERROR_SUCCESS)
						return;

					::RegSetValue (hKey,0,REG_SZ,szHtmlPath,::lstrlen(szHtmlPath) * sizeof(TCHAR)) ;
					::RegCloseKey (hKey) ;
				}
			}
			//=============================================================================
			// Window always on top.
			// ---------------------------------------------------------------------------------
			// Test a window have WS_EX_TOPMOST property.
			static BOOL IsWindowAlwaysTop(HWND hWnd)
			{
				return (GetWindowLong(hWnd,GWL_EXSTYLE) & WS_EX_TOPMOST) ;
			}
			// ---------------------------------------------------------------------------------
			// Set/Clear a window's WS_EX_TOPMOST property.
			static void SetWindowAlwaysTop(HWND hWnd, BOOL bTop)
			{
				if (!::IsWindow(hWnd) || (::GetDesktopWindow() == hWnd))
					return ;

				if (bTop)
				{
					if(!IsWindowAlwaysTop(hWnd))
						::SetWindowPos (hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE) ;
				}
				else
				{
					if(IsWindowAlwaysTop(hWnd))
						::SetWindowPos (hWnd, HWND_NOTOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE) ;
				}
			}
			//=============================================================================
			// Create a DDB handle from a CMFCImageObj object.
			static HBITMAP CreateDDBHandle(const CMFCImageObj& img)
			{
				if (!img.IsValidImage()) return NULL;

				// prepare info
				CMFCArray<BITMAPINFOHEADER>   bmfh (img.NewImgInfoWithPalette()) ;

				HDC       hdc = ::GetDC (NULL) ;
				HBITMAP   hDDB = CreateDIBitmap (hdc, bmfh.GetArrayPtr(), CBM_INIT, (VOID*)img.GetMemStart(), (BITMAPINFO*)bmfh.GetArrayPtr(), DIB_RGB_COLORS) ;
				::ReleaseDC (NULL, hdc) ;
				return hDDB ;
			}
			//=============================================================================
			// Load CMFCImageObj object from resource.
			static void LoadImageRes(CMFCImageObj& img,LPCTSTR resName,LPCTSTR resType,MFC_IMAGE_TYPE imgType,HMODULE hMod=0)
			{
				HRSRC res = ::FindResource(hMod, resName, resType) ;
				BYTE * pImgData = (BYTE*)::LockResource (::LoadResource (hMod, res)) ;
				if (pImgData)
					img.Load(pImgData,::SizeofResource(hMod, res),imgType) ;
			}
			//=============================================================================
			// Draw image.
			// ---------------------------------------------------------------------------------
			// Draw entire image to hdc at position (x,y), no scale.
			static bool DrawImage(const CMFCImageObj& img,HDC hdc, int x, int y)
			{
				if(!img.IsValidImage())
					return false;

				// prepare info
				CMFCArray<BITMAPINFO>   bmfh (img.NewImgInfoWithPalette()) ;

				// start draw
				int nOldMode = ::SetStretchBltMode (hdc,COLORONCOLOR) ;
				::SetDIBitsToDevice (hdc, x, y, img.GetImgWidth(), img.GetImgHeight(), 0, 0, 0, img.GetImgHeight(),
					img.GetMemStart(), bmfh.GetArrayPtr(), DIB_RGB_COLORS) ;
				::SetStretchBltMode (hdc, nOldMode) ;
				return true;
			}
			// ---------------------------------------------------------------------------------
			// Draw rcImg of image to rcDrawDC of hdc.
			static bool DrawImage(const CMFCImageObj& img,HDC hdc, RECT rcDrawDC,RECT rcImg)
			{
				if (!img.IsValidImage())
					return false;

				// prepare info
				CMFCArray<BITMAPINFO> bmfh (img.NewImgInfoWithPalette()) ;

				// start draw
				int nOldMode = ::SetStretchBltMode (hdc, COLORONCOLOR),
					nYStart = img.GetImgHeight() - (rcImg.top + GetRectHeight(rcImg)) ;
				::StretchDIBits (hdc, rcDrawDC.left, rcDrawDC.top, GetRectWidth(rcDrawDC), GetRectHeight(rcDrawDC),
					rcImg.left, nYStart, GetRectWidth(rcImg), GetRectHeight(rcImg),
					img.GetMemStart(), bmfh.GetArrayPtr(), DIB_RGB_COLORS, SRCCOPY) ;
				::SetStretchBltMode (hdc, nOldMode) ;
				return true;
			}
			// ---------------------------------------------------------------------------------
			// Draw entire image to rcDrawDC of hdc.
			static bool DrawImage(const CMFCImageObj& img,HDC hdc, RECT rcDrawDC)
			{
				RECT rcImg = {0, 0, img.GetImgWidth(), img.GetImgHeight()} ;
				return DrawImage(img, hdc, rcDrawDC, rcImg) ;
			}
			// ---------------------------------------------------------------------------------
			// Draw(aspect) entire image to center of rcDrawDC.
			static bool DrawImageAspect(const CMFCImageObj& img,HDC hdc,RECT rcDrawDC)
			{
				if(!img.IsValidImage())
					return false;

				int nNewWidth = img.GetImgWidth(),
					nNewHeight = img.GetImgHeight() ;
				if((img.GetImgWidth() > GetRectWidth(rcDrawDC)) || (img.GetImgHeight() > GetRectHeight(rcDrawDC)))
				{
					double fScaleX = GetRectWidth(rcDrawDC)/(double)img.GetImgWidth(),
						fScaleY = GetRectHeight(rcDrawDC)/(double)img.GetImgHeight(),
						fScale = MFCMin(fScaleX, fScaleY) ;
					nNewWidth = MFCMax(1, (int)(img.GetImgWidth() * fScale)) ;
					nNewHeight = MFCMax(1, (int)(img.GetImgHeight() * fScale)) ;
				}
				RECT rcShow = {0, 0, nNewWidth, nNewHeight} ;
				::OffsetRect(&rcShow,(GetRectWidth(rcDrawDC) - nNewWidth) / 2 + rcDrawDC.left,
					(GetRectHeight(rcDrawDC) - nNewHeight) / 2 + rcDrawDC.top) ;
				//assert ((rcShow.left >= 0) && (rcShow.top >= 0)) ;
				return DrawImage(img,hdc,rcShow) ;
			}
			//=============================================================================
			// Capture rcCap of hdc to CMFCImageObj.
			// the received image color format is 24bpp
			static BOOL CaptureDC (CMFCImageObj& img, HDC hdc, RECT rcCap)
			{
				if(IsRectEmpty(&rcCap) || (GetDeviceCaps (hdc, BITSPIXEL) <= 8)) // palette format
					return FALSE;

				HDC hMemDC = CreateCompatibleDC (hdc) ;
				HBITMAP hDDB = CreateCompatibleBitmap (hdc, GetRectWidth(rcCap), GetRectHeight(rcCap)) ;
				if(hDDB == NULL)
				{
					::DeleteDC (hMemDC) ;
					return FALSE;
				}

				// start capture
				HGDIOBJ   hOldBmp = ::SelectObject (hMemDC, hDDB) ;
				::BitBlt (hMemDC, 0, 0, GetRectWidth(rcCap), GetRectHeight(rcCap),
					hdc, rcCap.left, rcCap.top, SRCCOPY) ;
				::SelectObject (hMemDC, hOldBmp) ;

				// attach CMFCImageObj to DDB
				BITMAP bm ;
				GetObject (hDDB, sizeof(BITMAP), &bm) ;
				if(img.Create (bm.bmWidth, bm.bmHeight, 24))
				{
					// dest format
					CMFCArray<BITMAPINFO> bmfh (img.NewImgInfoWithPalette()) ;
					::GetDIBits (hdc, hDDB, 0, bm.bmHeight, img.GetMemStart(), bmfh.GetArrayPtr(), DIB_RGB_COLORS) ;
				}

				DeleteObject (hDDB) ;
				DeleteDC (hMemDC) ;
				return img.IsValidImage() ;
			}
			//=============================================================================
			// Capture rcCap of screen to CMFCImageObj.
			// the received image color format is 24bpp
			static BOOL CaptureScreen(CMFCImageObj& img, RECT rcCap)
			{
				HDC hSrcDC = ::GetDC (NULL) ;
				BOOL bRet = CaptureDC (img, hSrcDC, rcCap) ;
				::ReleaseDC (NULL, hSrcDC) ;
				return bRet ;
			}
			//=============================================================================
			// AlphaBlend img32 on HDC.
			static void AlphaImageOnDC(const CMFCImageObj& img32, HDC hdc, RECT rcOnDC, int nPercent=100)
			{
				if(!img32.IsValidImage() || (img32.GetImgColorBits() != 32))
					return ;

				CMFCImageObj   imgDC ;
				CaptureDC (imgDC, hdc, rcOnDC) ;
				RECT rcImg = {0, 0, img32.GetImgWidth(), img32.GetImgHeight()},
					rcDCImg = {0, 0, imgDC.GetImgWidth(), imgDC.GetImgHeight()} ;
				imgDC.AlphaBlend (img32, rcDCImg, rcImg, nPercent) ;
				DrawImage (imgDC, hdc, rcOnDC) ;
			}
			//=============================================================================
			// Create HRGN object from a 32bpp image.
			// alpha=0 means outter of region.
			static HRGN ConvertAlphaImageToHRGN(const CMFCImageObj& img32)
			{
				if (!img32.IsValidImage() || (img32.GetImgColorBits() != 32))
					return 0;

				HRGN hRgn = ::CreateRectRgn (0, 0, img32.GetImgWidth(), img32.GetImgHeight()) ;
				for(int y=0 ; y < img32.GetImgHeight() ; y++)
				{
					int nLastX = 0 ;
					BOOL bStartCount = FALSE ;
					for(int x=0 ; x < img32.GetImgWidth() ; x++)
					{
						if(MFC_A(img32.GetBits (x,y)))
						{
							if(bStartCount)
							{
								// erase rect
								HRGN   hSingle = ::CreateRectRgn (nLastX, y, x, y+1) ;
								::CombineRgn (hRgn, hRgn, hSingle, RGN_DIFF) ;
								::DeleteObject (hSingle) ;
								bStartCount = FALSE ;
							}
						}
						else
						{
							if (!bStartCount)
							{
								bStartCount = TRUE ;
								nLastX = x ;
							}
						}
					}
					if(bStartCount)
					{
						// erase rect
						HRGN   hSingle = ::CreateRectRgn(nLastX, y, img32.GetImgWidth(), y+1) ;
						::CombineRgn (hRgn, hRgn, hSingle, RGN_DIFF) ;
						::DeleteObject (hSingle) ;
					}
				}
				return hRgn ;
			}
			//=============================================================================
			// Clipboard.
			// ---------------------------------------------------------------------------------
			// Whether current clipboard has a bitmap (DDB / DIB).
			static bool IsPasteAvailable()
			{
				return IsClipboardFormatAvailable(CF_BITMAP) || IsClipboardFormatAvailable(CF_DIB) ;
			}
			// ---------------------------------------------------------------------------------
			// Copy img to clipboard (the image must >= 24bpp).
			static void CopyToClipboard(const CMFCImageObj& img)
			{
				if (img.IsValidImage() && (img.GetImgColorBits() >= 24))
				{
					if (::OpenClipboard(0))
					{
						// create HGLOBAL
						HGLOBAL  hMem = GlobalAlloc (GMEM_MOVEABLE, img.GetImgPitch()*img.GetImgHeight() + sizeof(BITMAPINFOHEADER) + 16) ;
						BYTE * pClipData = (BYTE*)GlobalLock(hMem) ;

						// fill image's info
						CMFCArray<BYTE>     imgInfo (img.NewImgInfoWithPalette()) ;
						CopyMemory (pClipData, imgInfo.GetArrayPtr(), sizeof(BITMAPINFOHEADER)) ;
						pClipData += sizeof(BITMAPINFOHEADER) ;

						CopyMemory(pClipData, img.GetMemStart(), img.GetImgPitch()*img.GetImgHeight()) ;
						GlobalUnlock (hMem) ;
						if (::EmptyClipboard())
							::SetClipboardData (CF_DIB, hMem) ;
						else
							GlobalFree(hMem) ;
						::CloseClipboard() ;
						// GlobalFree (hMem) ; // it is a nightmare :-(
					}
				}
			}
			// ---------------------------------------------------------------------------------
			// Get clipboard's image (the returned image is 32bpp).
			static void GetClipboardImage(CMFCImageObj& img)
			{
				if(IsPasteAvailable())
				{
					if (::OpenClipboard(0))
					{
						// get bitmap
						BITMAPINFOHEADER * bmif = (BITMAPINFOHEADER*)::GetClipboardData(CF_DIB) ;
						if(img.Create (bmif))
						{
							// palette
							BYTE * pCurr = ((BYTE*)bmif) + bmif->biSize ;
							if(img.GetImgColorBits() <= 8)
							{
								int nCount = 1 << img.GetImgColorBits() ;
								img.SetColorTable (0, nCount, (RGBQUAD*)pCurr) ;
								pCurr += (4 * nCount) ;
							}
							else if((bmif->biCompression == BI_BITFIELDS) && (bmif->biSize == sizeof(BITMAPINFOHEADER)))
								pCurr += 12 ;

							// copy pixel
							CopyMemory(img.GetMemStart(), pCurr, img.GetImgPitch()*img.GetImgHeight()) ;

							if(img.GetImgColorBits() == 32)
							{
								if(bmif->biCompression == BI_BITFIELDS)
									img.SetAlphaChannelValue (0xFF) ;
							}
							else	img.ConvertTo32Bit() ;
						}
						::CloseClipboard() ;
					}
				}
			}
			//=============================================================================
			// Get a temp file path.
			static std::string QueryTempFilePath()
			{
				TCHAR szTmpPath[MAX_PATH],szFilename[MAX_PATH] ;
				::GetTempPath(MAX_PATH, szTmpPath) ;
				::GetTempFileName(szTmpPath, TEXT("foo"), 0, szFilename) ;
				return std::string(szFilename) ;
			}
		private:
			static HKEY uis_GetRootKey()
			{
				OSVERSIONINFO osvi = {sizeof(OSVERSIONINFO)} ;
				::GetVersionEx (&osvi) ;
				return (osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER ;
			}
		};
	#endif// WIN32
#endif // _MFC_WIN32_HELPER_060705_H_

