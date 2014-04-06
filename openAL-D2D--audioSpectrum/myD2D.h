#pragma once
#include <d2d1.h>

template <class DERIVED_TYPE> 
class BaseWindow {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DERIVED_TYPE *pThis = NULL;
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
			pThis->m_hwnd = hwnd;
		} else { pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA); }
		if (pThis) return pThis->HandleMessage(uMsg, wParam, lParam); 
		else return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	BaseWindow() : m_hwnd(NULL) { }
	
	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, HWND hWndParent = 0, HMENU hMenu = 0
	) {
		WNDCLASS wc = {0};
		wc.lpfnWndProc   = DERIVED_TYPE::WindowProc;
		wc.hInstance     = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();
		RegisterClass(&wc);
		m_hwnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);
		return (m_hwnd ? TRUE : FALSE);
	}
	HWND Window() const { return m_hwnd; }

protected:
	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
	HWND m_hwnd;
};

class MainWindow : public BaseWindow<MainWindow> {
	ID2D1Factory			*pFactory;
	ID2D1SolidColorBrush	*brshYellow;
	ID2D1SolidColorBrush	*brshOrange;
	ID2D1SolidColorBrush	*brshCyan;
	ID2D1SolidColorBrush	*brshWhite;
	ID2D1SolidColorBrush	*brshVersatile;
	void					CalculateLayout();
	void					CreateGraphicsResources();
	void					DiscardGraphicsResources();
	void					OnPaint();
	void					Resize();
public:
	ID2D1HwndRenderTarget	*pRenderTarget;
	MainWindow() : pFactory(NULL), pRenderTarget(NULL), brshYellow(NULL) { }
	PCWSTR  ClassName() const { return L"Audio Spectrum"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

template <class T> void SafeRelease(T **ppT) { if (*ppT) { (*ppT)->Release(); *ppT = NULL; } }