

#include "SWindow/SWindow.hpp"
#include <iostream>

#pragma warning(disable:4996)

using ssf = int(*)();


int main() {

	SW::SWindow* pWnd = SW::SWindow::GetSWindowInstance();

	pWnd->OpenAsync(L"Wnd");

	while (pWnd->IsOpen()) { if (pWnd->GetKeyArray()['W']) break; Sleep(100); }

	SW::SWindow::ReleaseSWindowInstance();
	return 0;
}
