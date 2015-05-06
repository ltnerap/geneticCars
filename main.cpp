//GENETIC CARS
//
//Template: Michael Black, 2014
//
//main.cpp does the following in order:
//	1) creates a population of cars
//	2) creates a racecourse
//	3) genetically improves the cars by racing them
//	4) creates the window
//	5) shows the winning cars

#include <iostream>
#include <QtGui>
#include <math.h>
#include "main.h"
#include "physicsengine.h"
#include "car.h"

const int GENERATIONS=25;		//how many breeding generations
const int MAXCARS=1000;			//maximum # of cars.  more than this will segfault
const int KILLMAX=20;			//kill all but this many cars
const int INITIAL_POPULATION=30;	//how many cars we start with
const int BREEDRATE = 2;		//2 out of 10
const int DOMUTATERATE = 5;		//5 out of 100
const int MUTATERATE = 3;		//3 out of 100

int WIDTH=500,HEIGHT=500;		//screen width and height
QGraphicsScene* thescene;		//window component
WindowView* view;			//window
int timeunit=1000/660;			//when we're actually showing the car, do a frame every this many milliseconds

Car* car[MAXCARS];			//cars are stored here
int CarCount=INITIAL_POPULATION;	//keeps track of how many valid cars there are
int currentCar;				//which car we're currently racing

int iterations;				//how many frames we have simulated so far

extern bool simulating;			//TRUE if actively racing a car, FALSE during setup
extern bool dontdographics;		//TRUE if no window is open, FALSE if we want to see the cars in the window

void makeRaceCourse(int);

using namespace std;

int randint(int,int);

//sets up a timer for visualization
TimerHandler::TimerHandler(int t)
{
	QTimer* timer=new QTimer(this);
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	timer->start(t);
}

//when a timer goes off, ready to show a frame
//this only happens where we're ready to see the winning cars
void TimerHandler::onTimer()
{
	if(!simulating) return;

	doFrame();

	int pos=car[currentCar]->getCarPosition();

	iterations++;

	if(iterations>=2000 || pos>=WIDTH)
	{
		if(pos < 0){
			pos = 0;
		}
		qDebug() << iterations<<" iterations, position="<<pos<<endl;
		car[currentCar]->score(iterations,pos);
		car[currentCar]->deconstructCar();

		currentCar++;
		if(currentCar>=CarCount)
		{
			currentCar = 0;
			makeRaceCourse(0);
			//simulating=FALSE;
			for (int i=0; i<CarCount; i++)
			{
				qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations<<", pos: "<<car[i]->position<<endl;
			}
			car[currentCar]->constructCar();
		}
		else
		{
			car[currentCar]->constructCar();
		}
		iterations=0;
	}
}

WindowView::WindowView(QGraphicsScene *scene, QWidget* parent):QGraphicsView(scene, parent) { }

void WindowView::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton)
	{
		simulating=!simulating; return;
	}
}

void WindowView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
		currentCar=0;
		car[currentCar]->constructCar();
		simulating=TRUE;
	}
}

void kill(){

	//bubble sort cars by ending position
	for(int i=0; i<CarCount; i++){
		for(int j=0; j<CarCount-1; j++){
			//swap cars if they're out of order
			if(car[j+1]->position > car[j]->position){
				Car* temp = car[j];
				car[j] = car[j+1];
				car[j+1] = temp;
			}
		}
	}
	for(int i=KILLMAX; i<CarCount; i++){
		delete car[i];
	}
	
	CarCount = KILLMAX;
}

void breed(){
	int breedPopulation = CarCount;
	for(int i=0; i<breedPopulation; i++){
		for(int j=0; j<breedPopulation; j++){
			if(i == j){
				continue; //don't breed with yourself
			}
			if(rand() % 10 <= BREEDRATE){
				//create twist point for "dna"
				int twistPoint = rand() % car[i]->nodes;
				Car* daughter = new Car(car[i]->nodes);
				//copy positions from first car up to twist point
				for(int k=0; k<twistPoint; k++){
					daughter->balls[k] = car[i]->balls[k];
					daughter->balls_x[k] = car[i]->balls_x[k];
					daughter->balls_y[k] = car[i]->balls_x[k];
				}
				//copy links from first car up to twist point
				for(int m=0; m<twistPoint; m++){
					for(int n=0; n<twistPoint; n++){
						daughter->links[m][n] = car[i]->links[m][n];
					}
				}
				//copy positions from second car after twist point
				for(int k=twistPoint; k<car[i]->nodes; k++){
					daughter->balls[k] = car[j]->balls[k];
					daughter->balls_x[k] = car[j]->balls_x[k];
					daughter->balls_y[k] = car[j]->balls_y[k];
				}
				//copy links from second car after twist point
				for(int m=twistPoint; m<car[i]->nodes; m++){
					for(int n=twistPoint; n<car[i]->nodes; n++){
						daughter->links[m][n] = car[j]->links[m][n];
					}
				}
				car[CarCount++] = daughter;
			}
		}
	}
}

void mutate(){
	int mutatePopulation = CarCount;
	for(int i=0; i<mutatePopulation; i++){
		if(rand() % 100 <= DOMUTATERATE){
			//create a clone
			Car* clone = new Car(car[i]->nodes);
			//copy positions
			for(int j=0; j<car[i]->nodes; j++){
				clone->balls[j] = car[i]->balls[j];
				clone->balls_x[j] = car[i]->balls_x[j];
				clone->balls_y[j] = car[i]->balls_y[j];
			}
			//copy links
			for(int m=0; m<car[i]->nodes; m++){
				for(int n=0; n<car[i]->nodes; n++){
					clone->links[m][n] = car[i]->links[m][n];
				}
			}
			//potentially randomize ball position
			for(int j=0; j<car[i]->nodes; j++){
				if(rand() % 100 <= MUTATERATE){
					clone->balls_x[j] = randint(5,50);
					clone->balls_y[j] = randint(5,50);
				}
			}
			//potentially toggle link
			for(int m=0; m<car[i]->nodes; m++){
				for(int n=0; n<car[i]->nodes; n++){
					if(rand() % 100 <= MUTATERATE){
						clone->links[m][n] = !clone->links[m][n];
					}						
				}
			}
		}	
	}
}

void doCars()
{
	dontdographics=TRUE;

	//for every generation
	for(int gen=0; gen<GENERATIONS; gen++) {
		qDebug() << "****** GENERATION "<<gen<<" **********" <<endl;
	//for every car
		for(int track=0; track<2; track++){
			makeRaceCourse(track);
			for (int i=0; i<CarCount; i++)
			{
				//make a car
				car[i]->constructCar();
	
				simulating=TRUE;
				int t=0;
				int pos=0;
				//do 2000 frames unless car reaches end of track
				for(t=0; t<2000; t++)
				{
					doFrame();
					pos=car[i]->getCarPosition();
					if(pos>=WIDTH) break;
				}
				if(track == 0){
					car[i]->score(t,pos);
				}
				else{
					car[i]->position += pos;
					car[i]->iterations += t;
				}
				//print position and iterations, score car, deconstruct car
				std::cout << t<<" iterations, " << "car " << i <<", " << " position="<<pos<<std::endl;
				simulating=FALSE;
				car[i]->deconstructCar();
			}
		}
		
		for (int i=0; i<CarCount; i++)
		{
			qDebug()<<"Car "<<i<<": itr: "<<car[i]->iterations << ", pos: "<<car[i]->position <<endl;		
		}
	
		//kill, breed, mutate
		kill();
		breed();
		mutate();
	}
	kill();
	dontdographics=FALSE;
}

extern Wall* walls[];
extern int WallCount;
void makeRaceCourse(int trackNum)
{
	WallCount=9;
	thescene->clear();
	if(trackNum == 0){
		//create racetrack
		walls[0]=new Wall(1,500,499,500);
		walls[1]=new Wall(-20,132,123,285);
		walls[2]=new Wall(104,285,203,277);
		walls[3]=new Wall(202,275,271,344);
		walls[4]=new Wall(271,344,320,344);
		walls[5]=new Wall(321,345,354,318);
		walls[6]=new Wall(354,318,394,324);
		walls[7]=new Wall(394,324,429,390);
		walls[8]=new Wall(429,391,498,401);
	}
	else if(trackNum == 1){
		//create racetrack
		walls[0]=new Wall(1,500,499,500);
		walls[1]=new Wall(-20,100,200,380);
		walls[2]=new Wall(194,375,255,350);
		walls[3]=new Wall(250,350,300,350);
		walls[4]=new Wall(295,350,350,320);
		walls[5]=new Wall(345,320,380,345);
		walls[6]=new Wall(375,345,410,385);
		walls[7]=new Wall(410,385,429,365);
		walls[8]=new Wall(429,365,498,400);
	}
	for(int i=0; i<WallCount; i++)
		thescene->addItem(walls[i]);
}

int main(int argc, char **argv)
{
	QApplication app(argc,argv);
	qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

	thescene=new QGraphicsScene();
	thescene->setSceneRect(0,0,WIDTH,HEIGHT);

	//make the cars
	for (int i=0; i<CarCount; i++)
		car[i]=new Car(10);
		//do the genetics stuff to the cars
		doCars();
		makeRaceCourse(1);
	
	
	//construct the current car
	currentCar=0;
	car[currentCar]->constructCar();
	simulating=TRUE;
	
	view=new WindowView(thescene);
	view->setWindowTitle("Genetic Cars");
	view->resize(WIDTH+50,HEIGHT+50);
	view->show();
	view->setMouseTracking(true);

	TimerHandler timer(timeunit);

	return app.exec();
}
