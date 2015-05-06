#include "car.h"
#include "physicsengine.h"
#include "ball.h"
#include <QtGui>

//returns random integer between a and b inclusive
int randint(int a, int b)
{
	return (qrand()%(b-a+1))+a;
}

//mutates car - NOT FINISHED
Car* Car::mutate()
{
	Car* newcar=new Car(nodes);
	return newcar;
}

//breeds cars - NOT FINISHED
Car* Car::breed(Car* c)
{
	Car* newcar=new Car(nodes);
	return newcar;
}

//car constructor - makes a random car
Car::Car(int n)
{
	//makes all needed arrays
	nodes=n;
	balls_x=new int[nodes];
	balls_y=new int[nodes];
	balls=new int[nodes];
	links=new int*[nodes];
	for(int i=0; i<nodes; i++) links[i]=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	//makes random ball locations on screen from 5 to 50
	for(int i=0; i<nodes; i++)
	{
		balls_x[i]=randint(5,50);
		balls_y[i]=randint(5,50);
	}
	//making links, 1/3 probability that any two balls will get a link
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			links[i][j]=0;
			if(i<=j) continue;
			if(randint(1,3)==1)
			{
				links[i][j]=1;
			}
		}
	}
}

//graphically construct the car so we can race it
void Car::constructCar()
{
	balls=new int[nodes];
	numlinks=0;
	linkslist=new int[nodes*nodes];
	
	//make balls in balls list
	for(int i=0; i<nodes; i++)
	{
		balls[i]=makeBall(balls_x[i],balls_y[i]);
	}
	//make links based on whether there is a 0 or 1 in index of links
	for(int i=0; i<nodes; i++)
	{
		for(int j=0; j<nodes; j++)
		{
			if(links[i][j])
			{
				linkslist[numlinks++]=makeLink(i,j);
			}
		}
	}
}
extern QGraphicsScene* thescene;
//graphically destroys car so we can race someone else
void Car::deconstructCar()
{

	//remove balls and links from lists
	for(int i=0; i<numlinks; i++)
		removeLink(linkslist[i]-i);
	for(int i=0; i<nodes; i++)
		removeBall(balls[i]-i);

	//delete balls list and linkslist
	delete balls;
	delete linkslist;
}

//find position of car - average of ball positions
int Car::getCarPosition()
{
	int sum2=0;
	//sums the positions of the balls
	
	for (int i=0; i<nodes; i++){
		sum2+=getBall(i)->position->x;
	}
	//divides sum of positions of balls by number of balls
	return sum2 / nodes;
}

//set iterations and position
void Car::score(int itr, int pos)
{
	iterations=itr;
	position=pos;
}
