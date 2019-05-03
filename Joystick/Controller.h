#pragma once

#include <windows.h>
#include <chrono>

#define LOW 0x0000
#define HIGH 0xffff
#define INCREMENT 10
#define TOGGLE_JOYSTICK_MSG 9999

using namespace std::chrono;

struct Vector2 {
	int x;
	int y;
	Vector2(int x, int y) :x(x), y(y) {};

};
class Controller
{
public:
	Controller(HWND hWnd, UINT joystickId, HINSTANCE hInst, NOTIFYICONDATA* niData);
	~Controller();


	void HandleWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	void UpdateMousePosition();

	void AddMouseInput(Vector2 inputAxis);

	void SimulateMouseEvent(UINT mosueEvent);

	void SendVolumeInput(UINT input);

	void ClosePreviousBalloon();


	void ShowBalloonMessage(LPCWSTR message);


protected:
	HWND hWnd;
	Vector2 MousePosition;
	milliseconds LastTimePressed;
	bool JoystickOn;
	HINSTANCE hInstance;
	NOTIFYICONDATA* niData;



	milliseconds Now();
	bool PressingLeftButton;
};

