#pragma once
#include <string>
#include <vector>
#include "Window.hpp"
#include "ResourceAllocator.hpp"
#include "SharedContext.hpp"

static class DrawText
{
public:
	void showDialog(std::vector<std::string> veclines, Window& window, sf::Vector2f pos);
	void drawText();
	void displayDialog(std::vector<std::string> vecText, Window& window, float x, float y, ResourceAllocator<sf::Font>& fontAllocator, SharedContext& context);
		


	sf::Font font;
	bool m_bShowDialog = false;
	float m_fDialogX = 0.0f;
	float m_fDialogY = 0.0f;
	std::vector<std::string> m_vecDialogToShow;
	//ResourceAllocator<sf::Font>& fontAllocator;
	//SharedContext& context;
	

};

