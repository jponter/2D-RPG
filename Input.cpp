#include "Input.hpp"
#include <iostream>

Input::Input()
{
	m_joystick = sf::Joystick::isConnected(0);
	if (m_joystick)
	{
		//Debug::Log("Joystick Connected");
		std::cout << "Joystick Connected" << std::endl;
	}
}

void Input::Update()
{

	lastFrameKeys.SetMask(thisFrameKeys); // 1

	//todo Joystick input here
	if (m_joystick)
	{
		x = sf::Joystick::getAxisPosition(0, sf::Joystick::X);
		y = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);
		joy_left = false;
		joy_right = false;
		joy_up = false;
		joy_down = false;


		if (x < -sensitivity)
		{
			joy_left = true;
		}
		else if (x > sensitivity)
		{
			joy_right = true;
		}

		if (y < -sensitivity)
		{
			joy_up = true;
		}
		else if (y > sensitivity)
		{
			joy_down = true;
		}

		button0 = sf::Joystick::isButtonPressed(0, 0); //A
		button1 = sf::Joystick::isButtonPressed(0, 1); //B
		button2 = sf::Joystick::isButtonPressed(0, 2); //X
		

	}



	
	
	//movement
	thisFrameKeys.SetBit((int)Key::Left,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) || 
		joy_left); // 2

	



	thisFrameKeys.SetBit((int)Key::Right,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) ||
		joy_right);

	thisFrameKeys.SetBit((int)Key::Up,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) ||
		joy_up);

	thisFrameKeys.SetBit((int)Key::Down,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) ||
		joy_down);

	//camera zoom
	thisFrameKeys.SetBit((int)Key::LBracket,
		sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket));

	thisFrameKeys.SetBit((int)Key::RBracket,
		sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket));

	thisFrameKeys.SetBit((int)Key::Esc,
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));

	//shoot
	thisFrameKeys.SetBit((int)Key::E, sf::Keyboard::isKeyPressed(sf::Keyboard::E) || button1);

	thisFrameKeys.SetBit((int)Key::R, sf::Keyboard::isKeyPressed(sf::Keyboard::R));

	thisFrameKeys.SetBit((int)Key::T, sf::Keyboard::isKeyPressed(sf::Keyboard::T));
	thisFrameKeys.SetBit((int)Key::P, sf::Keyboard::isKeyPressed(sf::Keyboard::P));
	thisFrameKeys.SetBit((int)Key::I, sf::Keyboard::isKeyPressed(sf::Keyboard::I) || button2);

	thisFrameKeys.SetBit((int)Key::SPACE, sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || button0);
}

// Return true if the specified key is currently being pressed.
bool Input::IsKeyPressed(Key keycode)
{
	return thisFrameKeys.GetBit((int)keycode);
}

// Returns true if the key was just pressed 
// (i.e. registered as pressed this frame but not the previous).
bool Input::IsKeyDown(Key keycode)
{
	bool lastFrame = lastFrameKeys.GetBit((int)keycode);
	bool thisFrame = thisFrameKeys.GetBit((int)keycode);

	return thisFrame && !lastFrame;
}

// Returns true if the key was just released (i.e. registered as 
// pressed last frame but not the current frame).
bool Input::IsKeyUp(Key keycode)
{
	bool lastFrame = lastFrameKeys.GetBit((int)keycode);
	bool thisFrame = thisFrameKeys.GetBit((int)keycode);

	return !thisFrame && lastFrame;
}


