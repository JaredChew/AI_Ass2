/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"

//Border dimensions
#define WIDTH 800
#define HEIGHT 600
#define uWIDTH 80
#define uHEIGHT 60

//Maximum number of male drones, maximum number of female drones
#define MAX_DRONES 100
#define MAX_F_DRONES 3

class Game {

	public:

		Game( class MainWindow& wnd );
		Game( const Game& ) = delete;
		Game& operator=( const Game& ) = delete;

		void Go();

		/********************************/
		/*  Developer Defined           */

		//Structure of drones
		struct Boid {

			int x;
			int y;

			int xVel;
			int yVel;

		};

		/********************************/

	private:

		void ComposeFrame();
		void UpdateModel();

		/********************************/
		/* Developer Defined Functions  */

		//Functions related to flocking behavior
		void attraction(Boid drone1, Boid& drone2);
		void diversion(Boid player, Boid& drone, int safeZone);
		void cohesion(Boid femaleDrone, Boid& maleDrone, int startRadius, int endRadius);
		void seperation(Boid femaleDrone, Boid& maleDrone, int collisionRadius, int safeZone);

		//Functions related to movement and order
		void playerControl(Boid& player);
		void drawCrosshair(int x, int y, int r, int g, int b);
		void reflectionMove(Boid& drone, int size);
		void clampNPCinWindow(Boid& A, int size);

		//Functions related to setting up drones
		void playerDrone();
		void drones();

		//Functions related to drones behavior
		void flockingDrones();
		void notLasbyi();
		void notGei();

		//Functions related to calculations
		int computeDistance(Boid A, Boid B);

		/********************************/

	private:

		MainWindow& wnd;
		Graphics gfx;

		/********************************/
		/* Developer Defined Variables  */


		//Controlling variables used count and keep track
		int counter;
		int feCount;
		int feCount2;

		//Variables used in seperation
		int uX;
		int uY;
		bool checkSpace[uWIDTH][uHEIGHT];

		//Variables used by the player drones
		Boid player;
		int playerSpeed;

		//Variables used by male and female drones
		Boid drone[MAX_DRONES];
		int femaleDrone[MAX_F_DRONES];

		/********************************/

};