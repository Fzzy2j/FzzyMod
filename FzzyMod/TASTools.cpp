#pragma comment(lib,"d3d11.lib")
#include <Windows.h>
#include "TASTools.h"
#include "TF2Binds.h"
#include <chrono>
#include <iostream>
#include "include/MinHook.h"
#include "InputHooker.h"

using namespace std;

auto flipTimestamp = std::chrono::steady_clock::now();
bool flip;

void TASProcessInputProc(UINT &uMsg, WPARAM &wParam, LPARAM &lParam) {
	bool onGround = *(bool*)((uintptr_t)GetModuleHandle("client.dll") + 0x11EED78);

	if (uMsg == WM_KEYDOWN) {
		//int scanCode = (lParam >> 16) & 0xFF;
		//cout << scanCode << endl;
		if (wParam == forwardBinds[0] || wParam == forwardBinds[1]) {
			simulateKeyDown(leftBinds[0]);
			//wParam = leftBinds[0];
			if (!onGround) {
				//uMsg = WM_NULL;
			}
		}
	}
}

void TASProcessXInput(XINPUT_STATE* pState) {
	float velX = *(float*)((uintptr_t)GetModuleHandle("client.dll") + 0xB34C2C);
	float velY = *(float*)((uintptr_t)GetModuleHandle("client.dll") + 0xB34C30);

	float yaw = *(float*)((uintptr_t)GetModuleHandle("engine.dll") + 0x7B6668);

	float toDegrees = (180.0f / 3.14159265f);
	float toRadians = (3.14159265f / 180.0f);
	float magnitude = sqrt(pow(velX, 2) + pow(velY, 2));

	auto elapsed = chrono::steady_clock::now() - flipTimestamp;
	long long since = chrono::duration_cast<chrono::milliseconds>(elapsed).count();
	if (since > 100) {
		flip = !flip;
	}

	if (magnitude > 100) {
		float airSpeed = 50;
		float velDegrees = atan2f(velY, velX) * toDegrees;
		float velDirection = yaw - velDegrees;

		float rightx = sinf((velDirection + 90) * toRadians);
		float righty = cosf((velDirection + 90) * toRadians);

		float offsetx = velX + rightx * airSpeed;
		float offsety = velY + righty * airSpeed;

		float angle = atan2f(offsety, offsetx) - atan2f(velY, velX);
		float offsetAngle = atan2f(righty, rightx) - angle;
		cout << offsetAngle << endl;

		short tx = cosf(offsetAngle * toRadians) * 32767.0f;
		short ty = sinf(offsetAngle * toRadians) * 32767.0f;
		//cout << tx << " : " << ty << endl;

		pState->Gamepad.sThumbLX = tx;
		pState->Gamepad.sThumbLY = ty;
	}
}

bool pressingLurch = false;