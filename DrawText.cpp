#include "DrawText.h"
#include <iostream>

using namespace std;

 void DrawText::showDialog(std::vector<std::string> veclines, Window& window, sf::Vector2f pos)
{

	m_vecDialogToShow = veclines;
	m_bShowDialog = true;
	
	



}

 void DrawText::drawText()
 {
	 m_bShowDialog = false;
 }

 void DrawText::displayDialog(std::vector<std::string> vecText, Window& window, float x, float y)
 {
	 sf::Text text;
	 sf::Font font;
	 sf::FloatRect textRect;
	 sf::FloatRect tempRect;
	 sf::RectangleShape boxShape;
	 float padding = 5;
	 float outlineThickness = 2;
	 int fontSize = 32;


	 //TODO: Implelemnt the shared context here
	 /*
	 const int fontID = owner->context->fontAllocator->Add(owner->context->workingDir->Get() + "Joystix Monospace.ttf");
	 std::shared_ptr<sf::Font> font = owner->context->fontAllocator->Get(fontID);
	 text.setFont(*font);
	 */

	 if(!font.loadFromFile("sansation.ttf"))
	 {
		 cout << "font not found! " << endl;

	 }
	 

	 text.setFont(font);
	 text.setCharacterSize(fontSize);
	 text.setFillColor(sf::Color::White);
	 

	 int nMaxLineLength = 0;
	 int nLines = vecText.size();

	 
	 for (auto l : vecText)
	 {
		 text.setString(l);

		 tempRect = text.getLocalBounds();

		 
		 if (tempRect.width > textRect.width) textRect.width = tempRect.width;
		 if (tempRect.height > textRect.height) textRect.height = tempRect.height;
		 
	 }
	 
	 

	 //rectShape.setSize(sf::Vector2f(rect.width + rect.left, rect.height + rect.top));
	 boxShape.setSize(sf::Vector2f(textRect.width + textRect.left + padding, (textRect.height + textRect.top) * (nLines +1) + padding * 2));
	 
	 boxShape.setPosition(x, y);
	 boxShape.setFillColor(sf::Color::Blue);
	 boxShape.setOutlineColor(sf::Color::White);
	 boxShape.setOutlineThickness(outlineThickness);

	 

	 window.Draw(boxShape);

	 for (int l = 0; l < vecText.size(); l++)
	 {
		 text.setString(vecText[l]);
		 text.setPosition(x, y + (l * text.getCharacterSize()));
		 //text.move(boxShape.getSize().x / text.getLocalBounds().width, boxShape.getSize().y / text.getLocalBounds().height);
		 text.move(boxShape.getSize().x / text.getLocalBounds().width, 0);

		 window.Draw(text);
	 }

	 

	

	 

 }
