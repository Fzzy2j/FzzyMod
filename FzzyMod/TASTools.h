#pragma once
#include <Xinput.h>

void TASProcessXInput(XINPUT_STATE* pState);

void TASProcessInputProc(UINT &uMsg, WPARAM &wParam, LPARAM &lParam);

void hookDirectXPresent();
