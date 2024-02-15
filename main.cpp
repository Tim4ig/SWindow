

#include "SWindow/SWindow.hpp"
#include <iostream>

#pragma warning(disable:4996)

using ssf = int(*)();


int wWinMain(HINSTANCE, HINSTANCE, wchar_t*, int) {

	SW::SWindow* pWnd = SW::SWindow::GetSWindowInstance();

	pWnd->OpenAsync(L"Wnd");

	while (pWnd->IsOpen()) { if (pWnd->GetKeyArray()['W']) break; std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}
