#pragma once
#ifndef Input_hpp
#define Input_hpp

#include <SFML/Graphics.hpp>
#include "Bitmask.hpp"

class Input
{
public:
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
		T = 10
	};

	
	
	bool IsKeyPressed(Key keycode); // Returns true if the key is pressed.
	bool IsKeyDown(Key keycode); // Returns true if the key was just pressed.
	bool IsKeyUp(Key keycode); // Returns true if the key was just released.

	
	void Update();


private:
	Bitmask thisFrameKeys;
	Bitmask lastFrameKeys;


};

#endif /* Input_hpp */

