

#include "SWindow/SWindow.hpp"
#include <iostream>

#pragma warning(disable:4996)

int main() {

	SW::SWindow* pWnd = SW::SWindow::GetSWindowInstance();

	pWnd->OpenAsync(L"Test!");

	Sleep(1000);

	int tempX = pWnd->GetClientSize().x;

	pWnd->SetMode(SW::Modes::Borderless);
	Sleep(6000);
	pWnd->SetMode(SW::Modes::Windowed);

	if (tempX == pWnd->GetClientSize().x) tempX = 0;
	else tempX = -1;

	Sleep(1000);

	pWnd->IsResize();
	

	while (!pWnd->IsResize() && pWnd->IsOpen()) {
	}

	SW::SWindow::CleanUp();

	return tempX;
}
