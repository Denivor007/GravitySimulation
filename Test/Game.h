#pragma once
#include<iostream>
#include"GlobalVariable.h"


class Game {
protected:

	// Service variables
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event event;
	sf::Clock deltaClock;

	// Image variables
	float color[3] = { 0.f, 0.f, 0.f };
	int velositu[2] = { 0,0 };
	sf::Color bgColor;

	// Mouse Position
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Objects
	std::list <Planet> planets;
	Planet sun;
	Planet newObject;

	// Menu Class
	friend Menu;
	Menu menu;


	void initVariables() {
		this->window = nullptr;
	}

	void initWindow(bool screenResolution = false) {
		this->videoMode.height = ScreenResolutionY;
		this->videoMode.width = ScreenResolutionX;

		this->window = new sf::RenderWindow(this->videoMode, "SFML works!",
			sf::Style::Titlebar | sf::Style::Close);

		this->window->setFramerateLimit(60);
	}

	void initMenu() {
		//ImGui::SFML::Init(*this->window);

		menu = Menu(this);
	}

	void initObjects() {
		sf::CircleShape shape(50.f);
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(700, 350);

		Planet Sun(shape, 198000000);

		this->planets.push_back(Sun);

		for (size_t i = 0; i < 100; i++)
			this->planets.push_back(generatePlanet(
				2000, 10000000, -500, 500
			));
		

	}


public:

	Game() {
		this->initVariables();
		this->initWindow();
		this->initMenu();
		this->initObjects();
	}
	~Game(){
		delete this->window;
	}


	//Accessors
	const bool isRunning() const {
		return this->window->isOpen();
	}


	// Functions
	void pollEvents() {
		while (this->window->pollEvent(this->event)) {

			ImGui::SFML::ProcessEvent(event); //обработка ивентов библиотекой ImGui

			switch (this->event.type) {
			case sf::Event::Closed:
				this->window->close();
				break;
			case sf::Event::KeyPressed:
				if (this->event.key.code == sf::Keyboard::Escape)
					this->window->close();
				break;
			case sf::Event::MouseButtonPressed:
				if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
					newObject.shape.setPosition(
						{ 
							(float)this->mousePosWindow.x,
							(float)this->mousePosWindow.y,
						});
					std::cout << "mouse:  (" << mousePosWindow.x << ", " << mousePosWindow.y << ") \t"
						<< "(" << mousePosView.x << ", " << mousePosView.y << ")\t"
						<< "(" << newObject.getPosition().x << ", " << newObject.getPosition().y << ")\n";


					newObject.resize();
					planets.push_back(newObject);
				}
				break;
			}
		}
	}

	void updateMousePosition() {
		this->mousePosWindow = sf::Mouse::getPosition(*this->window);
		this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
	}

	void updateMenu() {
		ImGui::SFML::Update(*this->window, this->deltaClock.restart());

		ImGui::Begin("Sample window"); // создаём окно
		// Инструмент выбора цвета
		ImGui::DragInt("Choose the speed", &SPEED_FOR_MENU,1,1,500);
		ImGui::TextColored({ 205,205,255,100 }, "New Planet:");
		if (ImGui::DragInt2("velosity (x,y)", velositu, 1, -200, 200)) {
			newObject.velosity = { (double)velositu[0], (double)velositu[1] };
		}

		ImGui::DragInt("mass", &(newObject.mass), 10, 10, INT32_MAX);

		if(ImGui::ColorEdit3("color", color)) {
			newObject.shape.setFillColor({
				static_cast<sf::Uint8>(color[0] * 255.f),
				static_cast<sf::Uint8>(color[0] * 255.f),
				static_cast<sf::Uint8>(color[0] * 255.f)
				});
		}
		ImGui::End(); // end window
	}


	void updateObjects() {
		/*
			Заметка!!!
			Метод обещает быть очень неефективным
			- Копирование О(n)
			- Добавление заново елементов О(n)


			Намного удачнее может оказаться замена контейнера на List и удаление
			из него в процессе, т.к. итератор списка не инвалидируется
		*/

		for (
			auto iter = this->planets.begin();
			iter != this->planets.end();
			++iter
			)
		{
			bool beenExecuted = false;
			for (
				auto jter = this->planets.begin();
				jter != this->planets.end();
				jter++
				)
			
			{

				if (iter == jter)
					continue;

				if ((*iter).isColapse(*jter)) {
					if ((*iter).collapse(*jter)) {
						planets.erase(jter);
						continue;
					}
					else {
						planets.erase(iter);
						beenExecuted = true;
					}
				}
				(*iter).doGravity(&*jter);
			}
			if (beenExecuted)
				continue;
			if (distance2d((*iter).getPosition(), { ScreenResolutionX / 2, ScreenResolutionY / 2 }) > 3000) {
				planets.erase(iter);
				continue;
			}
			(*iter).moveGravity();
		}
		
	}

	void updateObjectsOld(); 
	

	void update() {
		this->updateMousePosition();
		this->pollEvents();
		this->updateObjects();
		this->updateMenu();

	}

	void renderObjects() {
		for (Planet& planet : this->planets)
		{
			this->window->draw((planet.shape));
		}
	}
	
	void renderMenu() {
		ImGui::SFML::Render(*window);
	}

	void render() {
		this->window->clear(this->bgColor);

		this->renderObjects();

		this->renderMenu();

		this->window->display();


	}
};



Menu::Menu() {
	this->_game = nullptr;
}

Menu::Menu(Game* game) {
	this->_game = game;
	ImGui::SFML::Init(*_game->window);
}


/*


void Game::updateObjectsOld() {
	
				/*
			Заметка!!!
			Метод обещает быть очень неефективным
			- Копирование О(n)
			- Добавление заново елементов О(n)


			Намного удачнее может оказаться замена контейнера на List и удаление
			из него в процессе, т.к. итератор списка не инвалидируется
		*
std::vector<Planet> temp(this->planets);
this->planets.clear();

for (size_t i = 0; i < temp.size(); i++)
{
	bool flag = true; //добавлять ли елемент назад в массив планет, или он всё

	for (size_t j = 0; j < temp.size(); j++)
	{
		if (i == j) continue;

		if (temp[i].isColapse(temp[j])) {

			if (!temp[i].collapse(temp[j]))
				flag = false;
			continue;
		}
		temp[i].doGravity(&temp[j]);
	}
	if (distance2d(temp[i].getPosition(), { ScreenResolutionX / 2, ScreenResolutionY / 2 }) > 2000)
		flag = false;
	temp[i].moveGravity();
	if (flag)
		this->planets.push_back(temp[i]);
}
}


*/