#pragma once
#ifndef C_Sprite_hpp
#define C_Sprite_hpp

#include "Component.hpp"
#include "C_Drawable.hpp"
#include "ResourceAllocator.hpp"

class C_Sprite : public Component, public C_Drawable
{
public:
	C_Sprite(Object* owner);

	// Loads a sprite from a textire id
	void Load(int id);
	//loads a teture from a file
	void Load(const std::string& filePath);

	//void SetTextureAllocator(ResourceAllocator<sf::Texture>* allocator);

	// We override the draw method so we can draw our sprite.
	void Draw(Window& window) override;

	//use this to update the sprite based on position
	void LateUpdate(float deltaTime) override;

	void SetTextureRect(int x, int y, int width, int height);
	void SetTextureRect(const sf::IntRect& rect);

	void SetScale(float x, float y);

	bool ContinueToDraw() const override;

private:


	//ResourceAllocator<sf::Texture>* allocator;
	sf::Sprite sprite;
	int currentTextureID; // We’ll now keep track of our currently set texture.
};

#endif /* C_Sprite_hpp */

