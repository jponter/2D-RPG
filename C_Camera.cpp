
#include "C_Camera.hpp"
#include "Object.hpp"

C_Camera::C_Camera(Object* owner) : Component(owner) { }

void C_Camera::LateUpdate(float deltaTime)
{
	
		sf::View view = owner->context->window->GetView();

		const sf::Vector2f& playerPos = owner->transform->GetPosition();
		//TODO: remove hard-coding of y value
		view.setCenter(playerPos.x, playerPos.y);;

		owner->context->window->SetView(view);
	
}

//void C_Camera::SetWindow(Window* gameWindow)
//{
//	window = gameWindow;
//}