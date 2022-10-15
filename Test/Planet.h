#pragma once
#include"GlobalVariable.h"
class vec2 {
public:
	double x, y;
	vec2(double x, double y) {
		this->x = x;
		this->y = y;
	}
	vec2() {
		this->x = x;
		this->y = y;
	}
	vec2(sf::Vector2f v) {
		this->x = v.x;
		this->y = v.y;
	}
	double length()
	{
		return sqrt((x * x) + (y * y));
	}

	vec2 normalize()
	{
		float locLength = length();
		float inv_length = (1 / locLength);
		x *= inv_length;
		y *= inv_length;

		return *this;
	}
	vec2 operator *(int v) {
		return { this->x * v,this->y * v };
	}
	vec2 operator *(double v) {
		return { this->x * v,this->y * v };
	}
	vec2 operator *(long long int v) {
		return { this->x * v,this->y * v };
	}
	vec2 operator +(vec2 v) {
		return { this->x + v.x,this->y + v.y };
	}
};


double distance2d(const vec2& A, const vec2& B) {
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2));
}

class Planet {
public:
	sf::CircleShape shape;
	int mass;
	vec2 velosity;
	Planet() {
		this->mass = 100;
		this->velosity = { 0,0 };
	}
	Planet(sf::CircleShape shape, long long int mass) {
		this->shape = shape;
		this->mass = mass;
		this->velosity = { 0,0 };
		resize();

	}
	Planet(sf::CircleShape shape, long long int mass, vec2 velosity) {
		this->shape = shape;
		this->mass = mass;
		this->velosity = velosity;
		resize();

	}

	void setVelosity(double x, double y) {
		velosity.x = x;
		velosity.y = y;
	}
	
	void resize() {
		this->mass %= INT32_MAX - 1;
		shape.setRadius((sqrt(sqrt(mass)))/3.14);
	}

	void doGravity(Planet* other) {
		const vec2 thisPosition = getPosition(); //позиция центра масс
		const vec2 otherPosition = other->getPosition();  //позиция центра масс другой планеты
		double distance = distance2d(thisPosition, otherPosition); // расстояние
		double forse = G * ((mass * other->mass) / pow(distance, 2));

		vec2 direction = {
			otherPosition.x - thisPosition.x,
			otherPosition.y - thisPosition.y
		};
		direction.normalize();
		// единичный вектор направления в сторону второго тела

		double scacceleration = (forse / mass); // скалярное ускорение - сила/массу
		vec2 acceleration = {
			direction.x * scacceleration,
			direction.y * scacceleration
		}; // применение скаляра к вектору направления

		velosity = {
			acceleration.x + velosity.x,
			acceleration.y + velosity.y
		}; //применение ускорения к скорости
	}

	vec2 getPosition() { //метод даёт позицию центра окружности
		vec2 res = shape.getPosition();
		double radius = shape.getRadius();
		return { res.x + radius, res.y + radius };
	}

	double getRadius() { //метод даёт позицию центра окружности
		return shape.getRadius();
	}

	void moveGravity() { //движение, исходя из скорости
		double mod = 0.000001 * SPEED_FOR_MENU * SPEED_FOR_MENU; // просто модификатор чтоб регулировать скорость
		shape.move(velosity.x * mod, velosity.y * mod);
	}

	bool isColapse(Planet& other) {
		vec2 pos1 = getPosition();
		vec2 pos2 = other.getPosition();
		double distance = distance2d(pos1, pos2);
		if (distance < ((getRadius() + other.getRadius())/3*2))
			return true;
		return false;
	}

	bool collapse(Planet& other) {
		/*
			Заметка!!!
			Метод херня и надо бы его в класс Гейм, т.к. работа этого метода
			построена на том, что вызывающий класс будет вкурсе как и ним управлятся
		*/

		auto newmass = mass + other.mass;
		if (other.mass > this->mass) {
			return false; //Всё что дальше не обрабатыается в связи с логикой класса Game;

			other.velosity = {
				((double)mass / (double)newmass) * velosity.x + ((double)other.mass / (double)newmass) * other.velosity.x,
				((double)mass / (double)newmass) * velosity.y + ((double)other.mass / (double)newmass) * other.velosity.y
			};
			other.mass += this->mass;

			other.resize();
		}

		velosity =
		{
			((double)mass / (double)newmass) * velosity.x + ((double)other.mass / (double)newmass) * other.velosity.x,
			((double)mass / (double)newmass) * velosity.y + ((double)other.mass / (double)newmass) * other.velosity.y
		};
		this->mass += other.mass;

		resize();
		return true;
	}
};



Planet generatePlanet(
	int startMass,
	int endMass,
	double startVelosity,
	double endVelosity
) {
	int cMass = (startMass + rand() % (int)(endMass - startMass));

	sf::CircleShape shape;
	shape.setFillColor(
		sf::Color::Color(
			1 + rand() % 250,
			1 + rand() % 250,
			1 + rand() % 250,
			1 + rand() % 250
		)
	);
	int x = 50 + rand() % 500;
	int y = 50 + rand() % 500;
	shape.setPosition({
		(float)1 + rand() % ScreenResolutionX,
		(float)1 + rand() % ScreenResolutionY
		});

	return Planet(
		shape,
		cMass,
		{
		startVelosity + rand() % (int)(endVelosity - startVelosity),
		startVelosity + rand() % (int)(endVelosity - startVelosity)
		}
	);

}

