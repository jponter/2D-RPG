#include "Window.hpp"
#include <imgui-SFML.h>
#include <imgui.h>



Window::Window(const std::string& windowName)
	: window(sf::VideoMode(1920, 1080), windowName, sf::Style::Titlebar) // 1
{
	window.setVerticalSyncEnabled(true); // 2
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
	window.clear(sf::Color::White);
}

void Window::Draw(const sf::Drawable& drawable)
{
	window.draw(drawable);
	
	
}

void Window::Draw(const sf::Vertex* vertices,
	std::size_t vertexCount, sf::PrimitiveType type)
{
	window.draw(vertices, vertexCount, type);
}

void Window::EndDraw()
{
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
	sf::Vector2u size = window.getSize();

	return sf::Vector2u(size.x / 2, size.y / 2);
}

const sf::View& Window::GetView() const
{
	return window.getView();
}

void Window::SetView(const sf::View& view)
{
	window.setView(view);
}

void Window::resetGLStates()
{
	window.resetGLStates();
}

void Window::pollEvent()
{
	sf::Event event;
	while (window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);

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
	ImGui::SFML::Render(window);
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

