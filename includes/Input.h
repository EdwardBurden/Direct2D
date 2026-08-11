#pragma once
#include <Windows.h>
#include <functional>

class Input
{
public:
	std::function<void(int x, int y)> Click; //etc maybe
	std::function<void(int x, int y)> Drag;

	void OnMouseInput(UINT msg, int x, int y, DWORD flags); // listen function
	void OnKeyboardInput(); // listen function 
private:

};
