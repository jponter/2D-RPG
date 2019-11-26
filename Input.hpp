#pragma once
#ifndef Input_hpp
#define Input_hpp

#include <SFML/Graphics.hpp>
#include "Bitmask.hpp"
//#include "Debug.hpp"

class Input
{
public:

	Input();

	enum class Key
	{
		None = 0,
		Left = 1,
		Right = 2,
		Up = 3,
		Down = 4,
		Esc = 5,
		LBracket = 6,
		RBracket = 7,
		E = 8,
		R = 9,
		T = 10,
		SPACE = 11,
		P = 12,
		I = 13
		
	};

	
	
	bool IsKeyPressed(Key keycode); // Returns true if the key is pressed.
	bool IsKeyDown(Key keycode); // Returns true if the key was just pressed.
	bool IsKeyUp(Key keycode); // Returns true if the key was just released.

	
	void Update();

	float getjoyx() { return x; };
	float getjoyy() { return y; };


private:
	Bitmask thisFrameKeys;
	Bitmask lastFrameKeys;
	bool m_joystick = false;

	//joystick

	// sensitivity
	float sensitivity = 50.0f;

	float x;
	float y;
	
	bool button0; // A
	bool button1; // B
	bool button2; // X
	bool joy_left = false;
	bool joy_right = false;
	bool joy_up = false;
	bool joy_down = false;


};

#endif /* Input_hpp */

