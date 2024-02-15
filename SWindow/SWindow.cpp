
#include "SWindow.hpp"

namespace SW {

	/***************************************************************/
	bool g_bIsExitCallBack = false;
	SWindow* g_pSWindowInstance = nullptr;
	SWindow* SWindow::GetSWindowInstance()
	{
		if (!g_pSWindowInstance) g_pSWindowInstance = new SWindow;
		if (!g_bIsExitCallBack) { g_bIsExitCallBack = true; atexit(ReleaseSWindowInstance); }
		return g_pSWindowInstance;
	}
	void SWindow::ReleaseSWindowInstance()
	{
		delete g_pSWindowInstance;
	}
	/***************************************************************/

	void SWindow::OpenAsync(PCWSTR cwszWindowTitle, int nCmdShow)
	{
		m_WindowThread = std::thread([&]() {
			WNDCLASSW wc;
			memset(&wc, 0, sizeof(WNDCLASS));
			wc.lpfnWndProc = m_WndProc;
			wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
			wc.lpszClassName = L"SWindowClassTim4igLib13213#1233$123";
			wc.hInstance = GetModuleHandleW(NULL);
			RegisterClassW(&wc);

			this->m_hWnd = CreateWindowExW(
				NULL,
				wc.lpszClassName,
				L"Window",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				NULL,
				NULL,
				wc.hInstance,
				NULL
			);

			this->m_bIsOpen = 1;

			MSG msg;
			memset(&msg, 0, sizeof(MSG));

			while (GetMessageW(&msg, NULL, 0, 0) && this->m_bIsOpen) {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
				WaitMessage();
			}

			this->m_bIsOpen = 0;

			return 0;
			});

		while (!this->m_bIsOpen);

		this->SetTitle(cwszWindowTitle);
		this->Show(nCmdShow);

		GetWindowRect(this->m_hWnd, &m_rWndPos);
		m_unWndStyle = GetWindowLongPtrW(this->m_hWnd, GWL_STYLE);

		this->m_bIsResize = false;

	}

	void SWindow::Show(int nCmdShow)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		if(this->m_hWnd) ShowWindow(this->m_hWnd, nCmdShow);
	}

	void SWindow::SetTitle(PCWSTR cwszWindowTitle)
	{
		if (this->m_hWnd) SetWindowTextW(this->m_hWnd, cwszWindowTitle);
	}

	void SWindow::SetMode(Modes::Mode eMode) {
		if (!this->m_hWnd) return;
		if (eMode == m_eMode) return;

		RECT newRect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
		LONG_PTR newStyle = 0;

		m_eMode = eMode;
		switch (m_eMode) {
		case SW::Modes::Windowed:
			newRect = m_rWndPos;
			newStyle = m_unWndStyle;
			break;
		case SW::Modes::Borderless:
			GetWindowRect(this->m_hWnd, &m_rWndPos);
			m_unWndStyle = GetWindowLongPtrW(this->m_hWnd, GWL_STYLE);
			newStyle = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			break;
		}

		SetWindowLongPtrW(this->m_hWnd, GWL_STYLE, newStyle);
		SetWindowPos(this->m_hWnd, NULL, newRect.left, newRect.top, newRect.right - newRect.left, newRect.bottom - newRect.top, SWP_FRAMECHANGED);
		if(eMode == Modes::Borderless) this->Show(SW_SHOWMAXIMIZED);
		else this->Show(SW_SHOWDEFAULT);
	}

	HWND SWindow::GetHWND() const
	{
		return m_hWnd;
	}

	POINT SWindow::GetClientSize()
	{
		RECT clientSize = { 0,0,0,0 };
		if (this->m_hWnd) GetClientRect(this->m_hWnd, &clientSize);
		return { clientSize.right - clientSize.left,clientSize.bottom - clientSize.top };
	}

	PCWSTR SWindow::GetTitle()
	{
		wchar_t str[32] = L"";
		if (this->m_hWnd) GetWindowTextW(this->m_hWnd, str, 32);
		return str;
	}

	bool SWindow::IsActive() const
	{
		HWND hActiveWnd = GetForegroundWindow();
		return (hActiveWnd != NULL && hActiveWnd == this->m_hWnd) ? 1 : 0;
	}

	bool SWindow::IsOpen() const {
		return this->m_bIsOpen;
	}

	bool SWindow::IsResize() {
		RECT client;
		GetClientRect(this->m_hWnd, &client);
		if (client.right - client.left < 10 || client.bottom - client.top < 10) m_bIsResize = 0;
		if (m_bIsResize) {
			m_bIsResize = 0;
			return 1;
		}
		return 0;
	}

	bool* SWindow::GetKeyArray()
	{
		return m_pKeys;
	}

	Modes::Mode SWindow::GetMode() const
	{
		return m_eMode;
	}

	SWindow::~SWindow()
	{
		m_bIsOpen = false;
		if (m_WindowThread.joinable()) {
			m_WindowThread.join();
		}
		if (m_hWnd) {
			CloseWindow(m_hWnd);
			m_hWnd = NULL;
		}
	}

	LRESULT __stdcall SWindow::m_WndProc(HWND hWnd, UINT uMsg, WPARAM wP, LPARAM lP)
	{

		switch (uMsg)
		{
		case WM_DESTROY: { PostQuitMessage(0); GetSWindowInstance()->m_bIsOpen = 0; return 0; }
		case WM_SIZE: {
			WINDOWPLACEMENT placement = { sizeof(WINDOWPLACEMENT) };
			if (GetWindowPlacement(hWnd, &placement) && placement.showCmd != SW_MINIMIZE)
			GetSWindowInstance()->m_bIsResize = 1; return 0;
		}
		case WM_KEYDOWN: {
			GetSWindowInstance()->m_pKeys[wP] = true;
			return 0;
		}
		case WM_KEYUP: {
			GetSWindowInstance()->m_pKeys[wP] = false;
			return 0;
		}
		}

		return DefWindowProcW(hWnd, uMsg, wP, lP);
	}

}
