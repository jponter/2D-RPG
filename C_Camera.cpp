
#include "C_Camera.hpp"
#include "Object.hpp"
#include "TileMapParser.hpp"

C_Camera::C_Camera(Object* owner) : Component(owner) { }

void C_Camera::LateUpdate(float deltaTime)
{
	
		sf::View view = owner->context->window->GetView();

		const sf::Vector2f& playerPos = owner->transform->GetPosition();
		
		float OriginalX = view.getCenter().x  ;
		float OriginalY = view.getCenter().y  ;

		sf::Vector2f newPos;

		float WorldSizeX = owner->context->mapParser->WorldX;
		float WorldSizeY = owner->context->mapParser->WorldY;

		sf::Vector2u screenSize = owner->context->window->GetSize();

		newPos = playerPos;

		if (playerPos.x + 32< screenSize.x /2 )
		{
			newPos.x = screenSize.x / 2 - 32;
		}
		
		if (playerPos.y +32 < screenSize.y / 2)
		{
			newPos.y = screenSize.y / 2 - 32;
		}

		if (playerPos.x  +32> WorldSizeX - (screenSize.x / 2))
		{
			newPos.x = WorldSizeX - (screenSize.x / 2) -32;
		}

		if (playerPos.y +32 > WorldSizeY - (screenSize.y / 2))
		{
			newPos.y = WorldSizeY - (screenSize.y / 2) - 32;
		}
		

		
			view.setCenter(newPos.x, newPos.y);;
			owner->context->window->SetView(view);
		
}

//void C_Camera::SetWindow(Window* gameWindow)
//{
//	window = gameWindow;
//}