#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <functional>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;
const float PI = 3.14159265359f;
const Vector2f unitLeft = Vector2f(0.866, 0.5); //unit vector for movement to the left
const Vector2f unitRight = Vector2f(-0.866, 0.5); // unit vector for movement to the right
const Vector2f unitDown = Vector2f(0.0, -1.0); //unit vector for movemet up

vector<Vertex> makeDestination(vector<Vertex> arr) {
	vector<Vertex> tempArray(arr.size());
	Vector2f tempVec;
	//calculating lentgh for triangle to travel by calculating magnitude of two vectors. We can get away using only x coordinates because we know that two first points are on the same plane.
	float length = (arr[1].position.x - arr[0].position.x) * 1.73;
	//3 first points use UnitLeft, next 3 unitRight,last 3 unitDown, and then the cycle repeats	
	for (int i = 0; i < arr.size(); i += 9)
	{
		for (int j = i; j < i + 3; j++)
		{
			tempVec.x = arr[j].position.x + unitLeft.x * length;
			tempVec.y = arr[j].position.y + unitLeft.y * length;
			tempArray[j] = Vertex(tempVec, arr[j].color);
		}
		for (int j = i + 3; j < i + 6; j++)
		{
			tempVec.x = arr[j].position.x + unitRight.x * length;
			tempVec.y = arr[j].position.y + unitRight.y * length;
			tempArray[j] = Vertex(tempVec, arr[j].color);
		}
		for (int j = i + 6; j < i + 9; j++)
		{
			tempVec.x = arr[j].position.x + unitDown.x * length;
			tempVec.y = arr[j].position.y + unitDown.y * length;

			tempArray[j] = Vertex(tempVec, arr[j].color);
		}
	}
	return tempArray;
}

vector<Vertex> makeTriangles(vector<Vertex> arr, Color color) {
	vector<Vertex> tempArray(0);

	for (size_t i = 0; i < arr.size() - 2; i += 3)
	{
		//previous Triangle left point
		Vector2f var1 = Vector2f(arr[i].position.x, arr[i].position.y);
		// middle point between left and right point of previous triangle
		sf::Vector2f var2 = sf::Vector2f((arr[i].position.x + arr[i + 1].position.x) / 2.0f, ((arr[i].position.y + arr[i + 1].position.y) / 2.0f));
		// right point of previous triangle
		sf::Vector2f var3 = sf::Vector2f(arr[i + 1].position.x, arr[i + 1].position.y);
		// middle point between right and bottom points of previous triangle
		sf::Vector2f var4 = sf::Vector2f((arr[i + 2].position.x + arr[i + 1].position.x) / 2.0f, ((arr[i + 2].position.y + arr[i + 1].position.y) / 2.0f));
		// Bottom point of previous triangle
		sf::Vector2f var5 = sf::Vector2f(arr[i + 2].position.x, arr[i + 2].position.y);
		// middle point between botton and left point
		sf::Vector2f var6 = sf::Vector2f((arr[i + 2].position.x + arr[i].position.x) / 2.0f, ((arr[i + 2].position.y + arr[i].position.y) / 2.0f));
		// creating new triangles with same  point order : left, right , bottom
		tempArray.push_back(Vertex(var1, color)); //left 
		tempArray.push_back(Vertex(var2, color)); //right
		tempArray.push_back(Vertex(var6, color)); //bottom

		tempArray.push_back(Vertex(var2, color)); 
		tempArray.push_back(Vertex(var3, color));
		tempArray.push_back(Vertex(var4, color));

		tempArray.push_back(Vertex(var6, color));
		tempArray.push_back(Vertex(var4, color));
		tempArray.push_back(Vertex(var5, color));
	}
	return tempArray;
}

int main()
{
	vector<Vertex> secondArr;
	vector<Vertex> thirdArr;
	vector<Vertex> test;
	vector<Vertex> test2;
	RenderWindow window(sf::VideoMode(600, 600), "Fractal");
	Vector2u screen = window.getSize();
	Vector2f view = Vector2f((float)screen.x, (float)screen.y);

	float length = view.y / 2.0f;
	
	//Clock and Time borrowed from the examples.
	Clock clock;
	clock.restart();
	float t = 0.0;
	float time = 0.0f;
	float t2 = 0.0;
	
	
	vector<Vertex> tPoint(3);
	Vector2f pos = Vector2f(view.x / 2.0f, 600.0f);
	vector<Vertex> tMove(30);

	float halfLength = length / 2.0f;
	float sHeight = view.y - sin(PI / 3.0f) * length;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {

				window.close();
			}
		}

		//Clock and Time borrowed from the examples.
		vector<Vertex> drawArray(0);
		float dt = clock.restart().asSeconds();
		time += 0.3 * dt;


		if (t < 0.9998f) {
			t = abs(2.0 * (time - floor(time)));

			t = t * t * (3 - 2 * t);
		}
		else {
			t = 1;
		}

		if (t == 1 && t2 == 0) {
			time = 0.3 * dt;
		}

		if (t == 1 && t2 < 0.9998f) {
			t2 = abs(2.0 * (time - floor(time)));

			t2 = t2 * t2 * (3 - 2 * t2);
		}
		else {
			if (t == 1) {
				t2 = 1;
			}
		}
		
		//Using color template from example
		Uint8 r = 255 * t;
		Uint8 g = 255 * t;
		Uint8 b = 255 * t;
		Color color(r, g, b);

		//Main Triangle
		tPoint[0] = sf::Vertex(sf::Vector2f(pos.x - halfLength, sHeight), sf::Color::White);     //left
		tPoint[1] = sf::Vertex(sf::Vector2f(pos.x + halfLength, sHeight), sf::Color::White);     //right
		tPoint[2] = sf::Vertex(sf::Vector2f(pos.x, pos.y), sf::Color::White);                    //bottom 		

		if (secondArr.size() < 1) {;
			secondArr = makeTriangles(tPoint, Color::Green);
			test = makeDestination(secondArr);
		}


		//Interpolation for the first set of extra triangles
		for (int i = 0; i < secondArr.size(); i++)
		{
			tMove[i] = Vertex(Vector2f(secondArr[i].position.x * (1 - t) + test[i].position.x * t, secondArr[i].position.y * (1 - t) + test[i].position.y * t), color);
			drawArray.push_back(tMove[i]);
		}

		//Calculates initial position for smallest triangles
		if (thirdArr.size() < 1) {
			thirdArr = makeTriangles(test, Color::Magenta);
			test2 = makeDestination(thirdArr);
		}

		//Interpolation for the second set of extra triangles
		for (int i = 0; i < thirdArr.size(); i++)
		{
			Uint8 r = 255 * t2;
			Uint8 g = 255 * t2;
			Uint8 b = 255 * t2;
			Color color(r, g, b);
			drawArray.push_back(Vertex(Vector2f(thirdArr[i].position.x * (1 - t2) + test2[i].position.x * t2, thirdArr[i].position.y * (1 - t2) + test2[i].position.y * t2), color));
		}
		
		window.clear();
		
		window.draw(&tPoint[0], 3, Triangles);
		window.draw(&drawArray[0], 9, Triangles);
		
		if (t == 1) {
			window.draw(&drawArray[9], 27, Triangles);
		}

		window.display();
	}
	return 0;
}


