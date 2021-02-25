#include <SFML/Graphics.hpp>
#include <random>
using namespace std;
using namespace sf;
const float PI = 3.14159265f;


//Gets a random seed every time you run the program
random_device device;
default_random_engine generator(device());
uniform_real_distribution<float> distribution(0.0f, 1.0f);

//Random function used to assign random values to different things, pos, color, radius etc...
float rng(){
	return distribution(generator);
}



//Class expanding on the functionality from SFMLs Circleshape class
//Used to store all variables related to circles
class myCircles : public CircleShape {
private:
	float m = 0.f;
public: 
	Vector2f vel;					//Velocity
	Vector2f force;					//Force
	Vector2f accel;					//Acceleration

	void mass(float r) {			//Calculates mass
		this->m = r * r * PI;
	}
	float getMass() {				//Returns the mass
		return m;
	}
};

//Main Function
int main()
{
	vector<myCircles> drawCircles;  //Array to store circles
	Clock clock;					//Class to pass time to time variable
	float dt;						//Time Variable
	float at;						//Alternative Time Variable
	const float g = 0.10f;			//Gravity acceleration constant
	const float w = -0.2f;			//Wind acceleration constant


	//For loop creating 500 circles and storing their values in an array
	for (int i = 0; i < 500; i++)
	{
		myCircles circle;			//Local class type variable
		circle.setPosition(rng() * 600 + 600, -600 * rng());
		circle.setRadius(25*rng() + 10);
		circle.setPointCount(30);
		circle.setFillColor(Color(255 * rng(),125*rng(),175*rng(), 255));
		circle.mass(circle.getRadius());
		circle.force = Vector2f(0.0f, 0.0f); 
		circle.vel = Vector2f(0.0f + rng()*4.0f , 0.0f + rng() * 4.0f); 
		drawCircles.push_back(circle); //Passing all values to the circle array.
	}

	RenderWindow window(VideoMode(1200, 1200), "Circles Bouncing");
	window.setFramerateLimit(60);	//Frame limit set to 60 to avoid hyperspeed
	Event event;

	//Game loop
	while (window.isOpen()) {

	dt = clock.getElapsedTime().asSeconds();	//Counting seconds
	at = abs(sin(dt));							//Goes between 0 and 1

		//Event Polling
		while (window.pollEvent(event)) {
			switch (event.type)					//Closes the window if close button or esc is pressed.
			{
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
					if (event.type == sf::Event::KeyPressed)
				    {
						if (event.key.code == sf::Keyboard::Escape)
						{
							window.close();
						}
				    }
				break;
			default:
				break;
			}
		}

	window.clear(); //Clear old frame

		//Going through all circles, updating their position and all forces.
		for (unsigned i = 0; i < drawCircles.size(); i++)
		{

			drawCircles[i].force.x = 0.01f * abs(((dt * w) - drawCircles[i].vel.x)) * drawCircles[i].getRadius() * 2;
			drawCircles[i].force.y = drawCircles[i].getMass() * g;
			
			drawCircles[i].accel = drawCircles[i].force / drawCircles[i].getMass();

			drawCircles[i].vel += dt * drawCircles[i].accel;

			drawCircles[i].move(-(dt * drawCircles[i].vel.x), dt * drawCircles[i].vel.y);

			if (drawCircles[i].getPosition().y >= 1200 - drawCircles[i].getRadius()*2)
			{
				drawCircles[i].setPosition(drawCircles[i].getPosition().x, 1200 - drawCircles[i].getRadius() * 2);
				drawCircles[i].setFillColor(Color(255 * rng() * at, 180, 255 * rng() * at));
				drawCircles[i].vel.y = drawCircles[i].vel.y * (-0.9f);
			}
		}

		//Iterating over all circles and drawing them
		for (vector<myCircles>::iterator i  = drawCircles.begin(); i < drawCircles.end(); i++)
		{
			window.draw(*i);
		}
		
		window.display();
	}
}