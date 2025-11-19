#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define SET_SIZE 365
//Run: clear && gcc BallParadox.c -lm -o g.o && ./g.o
int BallCollision(int setSize)
{
	bool oddBalls[SET_SIZE] = {false};   
	bool evenBalls[SET_SIZE] = {false}; 
	bool collisionFound = false;
	int totalPicks = 0;

	while(!collisionFound)
	{
		int ball = rand() % setSize;  
		totalPicks++;

		if(totalPicks % 2 == 1)  // Odd step
		{  
			if(evenBalls[ball]){collisionFound = true;}
			oddBalls[ball] = true;  
		}
		else  // Even step
		{                   
			if(oddBalls[ball]){collisionFound = true;}
			evenBalls[ball] = true; 
		}
	}
	return totalPicks;
}

int main()
{
	long totalPicks = 0;
	int trials = 1000;
	for(int i = 0; i < trials; i++)
	{
		totalPicks += BallCollision(SET_SIZE);
	}

	double averagePicks = (double)totalPicks / trials;
	printf("On average, %.2f picks are needed for a collision.\n", averagePicks);

	return 0;
}

