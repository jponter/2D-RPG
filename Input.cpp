#include "Input.hpp"


void Input::Update()
{
	lastFrameKeys.SetMask(thisFrameKeys); // 1
	
	//movement
	thisFrameKeys.SetBit((int)Key::Left,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::A))); // 2

	thisFrameKeys.SetBit((int)Key::Right,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::D)));

	thisFrameKeys.SetBit((int)Key::Up,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::W)));

	thisFrameKeys.SetBit((int)Key::Down,
		(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) ||
		(sf::Keyboard::isKeyPressed(sf::Keyboard::S)));

	//camera zoom
	thisFrameKeys.SetBit((int)Key::LBracket,
		sf::Keyboard::isKeyPressed(sf::Keyboard::LBracket));

	thisFrameKeys.SetBit((int)Key::RBracket,
		sf::Keyboard::isKeyPressed(sf::Keyboard::RBracket));

	thisFrameKeys.SetBit((int)Key::Esc,
		sf::Keyboard::isKeyPressed(sf::Keyboard::Escape));

	//shoot
	thisFrameKeys.SetBit((int)Key::E, sf::Keyboard::isKeyPressed(sf::Keyboard::E));

	thisFrameKeys.SetBit((int)Key::R, sf::Keyboard::isKeyPressed(sf::Keyboard::R));

	thisFrameKeys.SetBit((int)Key::T, sf::Keyboard::isKeyPressed(sf::Keyboard::T));
	thisFrameKeys.SetBit((int)Key::P, sf::Keyboard::isKeyPressed(sf::Keyboard::P));
	thisFrameKeys.SetBit((int)Key::I, sf::Keyboard::isKeyPressed(sf::Keyboard::I));

	thisFrameKeys.SetBit((int)Key::SPACE, sf::Keyboard::isKeyPressed(sf::Keyboard::Space));
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


