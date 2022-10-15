#include <math.h>
#include <vector>
#include <list>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include"imgui/imgui.h"
#include"imgui/imgui-SFML.h"

#include"Planet.h"
#include"Menu.h"
#include"Game.h"
#include"GlobalVariable.h"


//typedef sf::Vector2f vec2;



void Start() {
	srand(time(NULL));
	double never_use = rand();
}

int main() {
	Start();
	
	Game game;

	// цикл выполнения программы
	while (game.isRunning()) {
		game.update();
		game.render();		
	}
}