#ifndef Window_hpp
#define Window_hpp

#include <SFML/Graphics.hpp>

class Window
{
public:
	Window(const std::string& windowName);

	void Update();

	void BeginDraw();

	void Draw(const sf::Drawable& drawable);
	void Draw(const sf::Vertex* vertices,
		std::size_t vertexCount, sf::PrimitiveType type);
	
	void EndDraw();

	bool IsOpen() const;
	bool HasFocus() const;
	sf::Vector2u GetCentre() const;
	sf::FloatRect GetViewSpace() const;

	//camera stuff
	const sf::View& GetView() const;
	void SetView(const sf::View& view);
	sf::Vector2u GetSize() const;

	void SetTitle(std::string title);

	void resetGLStates();
	void pollEvent();
	void imGuiUpdate(sf::Clock clock);
	void imGuiRender();
	void imGuiInit();
	float fElapsedtime;
	bool isSnowing;

private:
	sf::RenderWindow window;
	sf::RenderTexture renderTextture;

	sf::Shader shader;

};

#endif /* Window_hpp */