#include "Controller.h"

#include "windowsx.h"
#include "Resource.h"
#include <sstream>

#define CLICK_BUTTON JOY_BUTTON1
#define VOLUME_UP JOY_BUTTON2
#define VOLUME_DOWN JOY_BUTTON3
#define TOGGLE_APPLICATION JOY_BUTTON4
#define LEFT_CLICK_BUTTON JOY_BUTTON5
#define RIGHT_CLICK 0x000A
#define INTERVAL_CLICK 1
#define JOYSTICK_BUTTON_MSG 0x3a2

using namespace std::chrono;

Controller::Controller(HWND hWnd, UINT joystickId, HINSTANCE hInst, NOTIFYICONDATA* niData) :
	MousePosition(0,0)
{
	this->hWnd = hWnd;
	JoystickOn = TRUE;
	hInstance = hInst;
	this->niData = niData;
	
}

Controller::~Controller()
{
}

void Controller::HandleWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::wstringstream ss;
	HWAVEOUT waveOut;

	switch (msg)
	{
		case MM_JOY1MOVE:

			if (!JoystickOn) break;

			AddMouseInput(Vector2(LOWORD(lParam), HIWORD(lParam)));
			UpdateMousePosition();
			break;

		case JOYSTICK_BUTTON_MSG:
			if ((UINT)wParam == 0)
				break;
			

			if ((UINT)wParam & TOGGLE_APPLICATION)
			{
				JoystickOn = !JoystickOn;

				ShowBalloonMessage(JoystickOn ? L"Joystick On" : L"Joystick off");
				

			}


			

			if (!JoystickOn) break;

			if ((UINT)wParam & LEFT_CLICK_BUTTON)
			{
				SimulateMouseEvent(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP);
				break;
			}
			if ((UINT)wParam & CLICK_BUTTON) {
				PressingLeftButton = true;
				LastTimePressed = Now();
				SimulateMouseEvent(MOUSEEVENTF_LEFTDOWN);
			}
			else if ((UINT)wParam & VOLUME_UP)
			{
				SendVolumeInput(VK_VOLUME_UP);

			}
			else if ((UINT)wParam & VOLUME_DOWN)
			{
				SendVolumeInput(VK_VOLUME_DOWN);
			}
			break;
		case MM_JOY1BUTTONUP:
			if (PressingLeftButton)
			{
				PressingLeftButton = false;
				SimulateMouseEvent(MOUSEEVENTF_LEFTUP);
			}
			break;
		case MOUSEEVENTF_XDOWN:
			MousePosition.x = GET_X_LPARAM(lParam);
			MousePosition.y = GET_Y_LPARAM(lParam);
			UpdateMousePosition();
			break;
		default:
			break;
	}
}

void Controller::UpdateMousePosition()
{
	SetCursorPos(MousePosition.x, MousePosition.y);
}


void Controller::AddMouseInput(Vector2 inputAxis) {
	if (inputAxis.x == HIGH)
	{
		MousePosition.x += INCREMENT;
	}
	else if (inputAxis.x == LOW)
	{
		MousePosition.x -= INCREMENT;
	}

	if (inputAxis.y == HIGH)
	{
		MousePosition.y += INCREMENT;
	}
	else if(inputAxis.y == LOW)
	{
		MousePosition.y -= INCREMENT;
	}
}


void Controller::SimulateMouseEvent(UINT mouseEvent) {
	INPUT input = INPUT();
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = mouseEvent;
	input.mi.dx = MousePosition.x;
	input.mi.dy = MousePosition.y;
	SendInput(1, (LPINPUT)& input, sizeof(input));
}

void Controller::SendVolumeInput(UINT input)
{
	INPUT inp = { 0 };
	inp.type = INPUT_KEYBOARD;
	inp.ki.wVk = input;

	inp.ki.dwFlags &= KEYEVENTF_KEYUP;
	SendInput(1, &inp, sizeof(inp));
}

void Controller::ClosePreviousBalloon()
{
	niData->szInfo[0] = 0;
	Shell_NotifyIcon(NIM_MODIFY, niData);
}

void Controller::ShowBalloonMessage(LPCWSTR message)
{
	ClosePreviousBalloon();
	lstrcpy(niData->szTip, message);

	niData->uTimeout = 1000;
	Shell_NotifyIcon(NIM_MODIFY | NIM_SETFOCUS, niData);
}

milliseconds Controller::Now() {
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch());
}