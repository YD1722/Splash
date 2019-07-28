//  ===========================================================================
//  File    Splash.cpp
//  Desc    The implementation file for the CSplash class.
//  ===========================================================================
#include <afxwin.h>
#include "splash.h"
#include "windowsx.h"
#include "wtypes.h"

//  ===========================================================================
//  The following is used for layering support which is used in the 
//  splash screen for transparency. In VC 6 these are not defined in the headers
//  for user32.dll and hence we use mechanisms so that it can work in VC 6.
//  We define the flags here and write code so that we can load the function
//  from User32.dll explicitely and use it. This code requires Win2k and above
//  to work.
//  ===========================================================================
typedef BOOL (WINAPI *lpfnSetLayeredWindowAttributes)
        (HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);

lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;

#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

//  ===========================================================================
//  Func    ExtWndProc
//  Desc    The windows procedure that is used to forward messages to the 
//          CSplash class. CSplash sends the "this" pointer through the
//          CreateWindowEx call and the pointer reaches here in the 
//          WM_CREATE message. We store it here and use it for message 
//          forwarding.
//  ===========================================================================
static LRESULT CALLBACK ExtWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static CSplash * spl = NULL;
    if(uMsg == WM_CREATE)
    {
        spl = (CSplash*)((LPCREATESTRUCT)lParam)->lpCreateParams;
    }
    if(spl)
        return spl->WindowProc(hwnd, uMsg, wParam, lParam);
    else
        return DefWindowProc (hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK CSplash::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //  We need to handle on the WM_PAINT message
    switch(uMsg)
    {
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
    }

    return DefWindowProc (hwnd, uMsg, wParam, lParam) ;
}

void CSplash:: OnPaint(HWND hwnd)
{
    if (!m_hBitmap)
        return;

    //  =======================================================================
    //  Paint the background by BitBlting the bitmap
    //  =======================================================================
    PAINTSTRUCT ps ;
    HDC hDC = BeginPaint (hwnd, &ps) ;

    RECT   rect;
    ::GetClientRect(m_hwnd, &rect);
    
    HDC hMemDC      = ::CreateCompatibleDC(hDC);
    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, m_hBitmap);
    
    BitBlt(hDC, 0, 0, m_dwWidth, m_dwHeight, hMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hMemDC, hOldBmp);

    ::DeleteDC(hMemDC);
    
    EndPaint (hwnd, &ps) ;
}

void CSplash::Init()
{
    //  =======================================================================
    //  Initialize the variables
    //  =======================================================================
    m_hwnd = NULL;
    m_lpszClassName = TEXT("SPLASH");
    m_colTrans = 0;

    //  =======================================================================
    //  Keep the function pointer for the SetLayeredWindowAttributes function
    //  in User32.dll ready
    //  =======================================================================
    HMODULE hUser32 = GetModuleHandle(TEXT("USER32.DLL"));

    g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}

CSplash::CSplash()
{
    Init();
}

CSplash::CSplash(LPCTSTR lpszFileName, COLORREF colTrans, LPCTSTR message)
{
    Init();

    SetBitmap(lpszFileName, message);
}

CSplash::~CSplash()
{
    FreeResources();
}

HWND CSplash::RegAndCreateWindow()
{
    //  =======================================================================
    //  Register the window with ExtWndProc as the window procedure
    //  =======================================================================
    WNDCLASSEX wndclass;
    wndclass.cbSize         = sizeof (wndclass);
    wndclass.style          = CS_BYTEALIGNCLIENT | CS_BYTEALIGNWINDOW;
    wndclass.lpfnWndProc    = ExtWndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = DLGWINDOWEXTRA;
    wndclass.hInstance      = ::GetModuleHandle(NULL);
    wndclass.hIcon          = NULL;
    wndclass.hCursor        = ::LoadCursor( NULL, IDC_WAIT );
    wndclass.hbrBackground  = (HBRUSH)::GetStockObject(LTGRAY_BRUSH);
    wndclass.lpszMenuName   = NULL;
    wndclass.lpszClassName  = m_lpszClassName;
    wndclass.hIconSm        = NULL;

    if(!RegisterClassEx (&wndclass))
        return NULL;


	//RECT desktop;
	//const HWND hDesktop = GetDesktopWindow();

	//GetWindowRect(hDesktop, &desktop);

	//int horizontal = desktop.right;
	//int vertical = desktop.bottom;

	RECT xy;
	BOOL fResult = SystemParametersInfo(SPI_GETWORKAREA, 0, &xy, 0);

	DWORD nScrHeight = xy.bottom - xy.top;
	DWORD  nScrWidth = xy.right - xy.left;
	

    //  =======================================================================
    //  Create the window of the application, passing the this pointer so that
    //  ExtWndProc can use that for message forwarding
    //  =======================================================================
    //DWORD nScrWidth  = ::GetSystemMetrics(SM_CXFULLSCREEN);
    //DWORD nScrHeight = ::GetSystemMetrics(SM_CYSCREEN);

    int x = (nScrWidth  - m_dwWidth) / 2;
    int y = (nScrHeight - m_dwHeight) / 2;
    m_hwnd = ::CreateWindowEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW, m_lpszClassName, 
                              TEXT("Banner"), WS_POPUP, x, y, 
                              m_dwWidth, m_dwHeight, NULL, NULL, NULL, this);

    //  =======================================================================
    //  Display the window
    //  =======================================================================
    if(m_hwnd)
    {
        ShowWindow   (m_hwnd, SW_SHOW) ;
        UpdateWindow (m_hwnd);
    }
    return m_hwnd;
}

int CSplash::DoLoop()
{
    //  =======================================================================
    //  Show the window
    //  =======================================================================
    if(!m_hwnd)
        ShowSplash();

    //  =======================================================================
    //  Get into the modal loop
    //  =======================================================================
    MSG msg ;
    while (GetMessage (&msg, NULL, 0, 0))
    {
        TranslateMessage (&msg) ;
        DispatchMessage  (&msg) ;
    }

    return msg.wParam ;

}

void CSplash::ShowSplash()
{
    //CloseSplash();
    RegAndCreateWindow();
}


DWORD CSplash::SetBitmap(LPCTSTR lpszFileName, LPCTSTR message)
{
    //  =======================================================================
    //  load the bitmap
    //  =======================================================================
    HBITMAP    hBitmap       = NULL;
    hBitmap = (HBITMAP)::LoadImage(0, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	// get the basic attribute of the bitmap
	BITMAP bm;
	GetObject(hBitmap, sizeof(BITMAP), &bm);
	long width = bm.bmWidth;
	long height = bm.bmHeight;

	//prepare the bitmap attributes
	BITMAPINFO bmInfo;
	memset(&bmInfo.bmiHeader, 0, sizeof(BITMAPINFOHEADER));
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = width;
	bmInfo.bmiHeader.biHeight = height;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 24;

	//create a temporary dc in memory.

	/*he GetDC function retrieves a handle to a device context(DC) for the client area of a specified window or for the entire screen.
		You can use the returned handle in subsequent GDI functions to draw in the DC.The device context 
		is an opaque data structure, whose values are used internally by GDI.*/

	HDC pDC = ::GetDC(0);
	HDC TmpDC = CreateCompatibleDC(pDC); // The CreateCompatibleDC function creates a memory device context (DC) compatible with the specified device.

	//create a new bitmap and select it in the memory dc
	BYTE* pbase;
	HBITMAP TmpBmp = CreateDIBSection(pDC,
		&bmInfo, DIB_RGB_COLORS, (void**)& pbase, 0, 0);
	HGDIOBJ TmpObj = SelectObject(TmpDC, TmpBmp);

	//draw the background
	HDC dcBmp = CreateCompatibleDC(TmpDC);
	HGDIOBJ TmpObj2 = SelectObject(dcBmp, hBitmap);
	BitBlt(TmpDC, 0, 0, width, height, dcBmp, 0, 0, SRCCOPY);
	SelectObject(dcBmp, TmpObj2);
	DeleteDC(dcBmp);

	// Choose the font and properties
	CFont m_Font;
	LOGFONT* m_pLF;
	m_pLF = (LOGFONT*)calloc(1, sizeof(LOGFONT));
	_tcsncpy(m_pLF->lfFaceName, L"Arial", 23);
	m_pLF->lfHeight = 17;
	m_pLF->lfWeight = 800;
	m_Font.CreateFontIndirect(m_pLF);

	// Select the font in the dc
	CDC dc;
	dc.Attach(TmpDC);
	CFont* pOldFont = NULL;
	if (m_Font.m_hObject)
		pOldFont = dc.SelectObject(&m_Font);
	else
		dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));

	//Set text color
	dc.SetTextColor(RGB(255, 255, 255));
	//Set text position;
	//(width / 2 - (lstrlen(message) / 2))
	SIZE sizeText;
	GetTextExtentPoint32(GetDC(NULL) , message, lstrlen(message), &sizeText);

	RECT pos = { ((width / 2)-(sizeText.cx/2)),(height- m_pLF->lfHeight-5),((width / 2) + (lstrlen(message) / 2)),0};
	//draw the text
	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(message, -1, &pos, DT_CALCRECT); 
	dc.DrawText(message, -1, &pos, 0);

	//Cleanup 
	if (pOldFont) dc.SelectObject(pOldFont);
	m_Font.DeleteObject();
	dc.Detach();
	free(m_pLF);

	DeleteObject(hBitmap);
	hBitmap = TmpBmp;

	SelectObject(TmpDC, TmpObj);
	DeleteDC(TmpDC);

    return SetBitmap(hBitmap);
}

DWORD CSplash::SetBitmap(HBITMAP hBitmap)
{
    int nRetValue;
    BITMAP  csBitmapSize;
    
    //  =======================================================================
    //  Free loaded resource
    //  =======================================================================
    FreeResources();
    
    if (hBitmap)
    {
        m_hBitmap = hBitmap;
        
        //  ===================================================================
        //  Get bitmap size
        //  ===================================================================
        nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
        if (nRetValue == 0)
        {
            FreeResources();
            return 0;
        }
        m_dwWidth = (DWORD)csBitmapSize.bmWidth;
        m_dwHeight = (DWORD)csBitmapSize.bmHeight;
    }
       
    return 1;
}

void CSplash::FreeResources()
{
    if (m_hBitmap)
        ::DeleteObject (m_hBitmap);
    m_hBitmap = NULL;
}

int CSplash::CloseSplash()
{
    
    if(m_hwnd)
    {
        DestroyWindow(m_hwnd);
        m_hwnd = 0;
        UnregisterClass(m_lpszClassName, ::GetModuleHandle(NULL));
        return 1;
    }
    return 0;
}


