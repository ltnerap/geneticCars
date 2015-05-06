#include <iostream>
//car object: balls and links
class Car
{
public:
	//define the car
	int* balls_x; //x coordinate of each ball
	int* balls_y; //y coordinate of each ball
	int** links; //matrix of which ball is connected to which ball

	//just for car race
	int* balls; //array of the balls that make up the car
	int* linkslist; //array of links that make up the car

        int nodes; //how many balls
        int numlinks; //how many links
	int iterations; //race results: iterations = how many frames
	int position; //race results: how far the car got

	Car(int); //constructor with number of nodes as input
	void constructCar(); //given balls_x, balls_y, and links, actually make the balls and links for the car
	void deconstructCar(); //clears the car out of the racetrack

	int getCarPosition(); //tells us how far the car got
	void score(int,int); //sets iterations and positions at end of race
	Car* breed(Car*); //NOT WRITTEN - breeds car with another car, returns a new car
	Car* mutate(); //NOT WRITTEN - makes a new mutant car

	//destructor - destructs the car on delete car
	~Car()
	{
		delete balls_x;
		delete balls_y;
		for(int i=0; i<nodes; i++)
			delete links[i];
		delete links;
	}
};
