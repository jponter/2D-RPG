#include "Window.hpp"
#include <imgui-SFML.h>
#include <imgui.h>
#include <iostream>



Window::Window(const std::string& windowName)
	: window(sf::VideoMode(1280, 960), windowName, sf::Style::Titlebar) // 1
{
	window.setVerticalSyncEnabled(true); // 2
	isSnowing = false;

	if (!renderTextture.create(1280, 960))
	{
		std::cout << "Cant Create Render Texture" << std::endl;
		exit(2);
	}
	else
	{
		std::cout << "Created Render Texture 1280x960" << std::endl;
	}

	if (!sf::Shader::isAvailable())
	{
		std::cout << "Shaders are not available!" << std::endl;
		exit(3);
	}

	if (!shader.loadFromFile("SnowFragment.glsl", sf::Shader::Fragment))
	{
		std::cout << "Fragment shader failed to load" << std::endl;
	}
	else std::cout << "Snow Fragment Shader Loaded!" << std::endl;

}

void Window::Update()
{
	

	sf::Event event; // 3
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
		}
	}
}

void Window::SetTitle(std::string title)
{
	window.setTitle(title);
}

void Window::BeginDraw() // 4
{
	//window.clear(sf::Color::White);
	renderTextture.clear(sf::Color::White);
}

void Window::Draw(const sf::Drawable& drawable)
{
	//window.draw(drawable);
	renderTextture.draw(drawable);
	
	
}

void Window::Draw(const sf::Vertex* vertices,
	std::size_t vertexCount, sf::PrimitiveType type)
{
	//window.draw(vertices, vertexCount, type);
	renderTextture.draw(vertices, vertexCount, type);
}

void Window::EndDraw()
{
	renderTextture.display();

	const sf::Texture& texture = renderTextture.getTexture();
	sf::Sprite sprite(texture);

	if (isSnowing) {


		shader.setUniform("texture", sf::Shader::CurrentTexture);
		shader.setUniform("iResolution", sf::Vector2f(1280, 960));
		shader.setUniform("iTime", float(fElapsedtime));

		window.clear();
		window.draw(sprite, &shader);
	}
	else
	{
		window.clear();
		window.draw(sprite);
	}
	window.display();
}

bool Window::IsOpen() const
{
	return window.isOpen();
}

bool Window::HasFocus() const
{
	return window.hasFocus();
}

sf::Vector2u Window::GetCentre() const
{
	sf::Vector2u size = renderTextture.getSize();

	return sf::Vector2u(size.x / 2, size.y / 2);
}

const sf::View& Window::GetView() const
{
	return renderTextture.getView();
}

sf::Vector2u Window::GetSize() const
{
	return renderTextture.getSize();
}

void Window::SetView(const sf::View& view)
{
	renderTextture.setView(view);
}

void Window::resetGLStates()
{
	window.resetGLStates();
}

void Window::close()
{
	window.close();
}

void Window::SetMouseCursorVisible(bool visible)
{
	window.setMouseCursorVisible(visible);
}

void Window::pollEvent()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

		/*if (event.type == sf::Event::KeyPressed)
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}*/

		if (event.type == sf::Event::Closed) {
			window.close();
		}
	}
}

void Window::imGuiUpdate(sf::Clock clock)
{
	ImGui::SFML::Update(window, clock.restart());
}

void Window::imGuiRender()
{
	ImGui::SFML::Render(renderTextture);
	//window.display();
}

void Window::imGuiInit()
{
	ImGui::SFML::Init(window);
}

sf::FloatRect Window::GetViewSpace() const
{
	const sf::View& view = GetView();
	const sf::Vector2f& viewCenter = view.getCenter();
	const sf::Vector2f& viewSize = view.getSize();
	sf::Vector2f viewSizeHalf(viewSize.x * 0.5f, viewSize.y * 0.5f);
	sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);
	return viewSpace;
}

void Window::CopyScreen()
{
	screenCopyTexture = sf::Texture(renderTextture.getTexture());
}

sf::Vector2f Window::mapPixelToCoords(sf::Vector2i pixelPos)
{
	return window.mapPixelToCoords(pixelPos);
}

