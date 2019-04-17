/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
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
#include "Game.h"
#include "MainWindow.h"

#include <ctime>

Game::Game( MainWindow& wnd ) : wnd( wnd ), gfx( wnd ) {

	//Initialise variables and drones

	//Set random number based on current time
	srand((unsigned)time(NULL));

	//Set player drone speed
	playerSpeed = 4;

	//Set player drone spawn position
	player.x = rand() % WIDTH - 1;
	player.y = rand() % HEIGHT - 1;

	//Set drone spawn position and velocity
	for (int i = 0; i < MAX_DRONES; i++) {

		drone[i].x = rand() % WIDTH - 1;
		drone[i].y = rand() % HEIGHT - 1;

		drone[i].xVel = 1;
		drone[i].yVel = 1;

	}

	//Select female drone from male drones
	for (int i = 0; i < MAX_F_DRONES; i++) {

		femaleDrone[i] = rand() % MAX_DRONES;

		//Check if female drone's id is the same as other female drone
		for (int j = 0; j < MAX_F_DRONES; ) {

			//Check if checking drone is same as current drone
			if (j == i) {
				j++;
			}

			//Check if current female drone has the same id as checking female drone
			if (femaleDrone[i] == femaleDrone[j]) {
				//Reset id for female drone if same
				femaleDrone[i] = rand() % MAX_DRONES;
				j = 0;
			}
			else {
				j++;
			}

		}

	}

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

void Game::drawCrosshair(int x, int y, int r, int g, int b) {

	// Draws horizontal lines of the drone
	for (int xx = 0; xx < 10; xx++) {
		gfx.PutPixel(x + xx, y + 5 - 1, r, g, b);
		gfx.PutPixel(x + xx, y + 5, r, g, b);
		gfx.PutPixel(x + xx, y + 5 + 1, r, g, b);
	}

	// Draws vertical lines of the drone
	for (int yy = 0; yy < 10; yy++) {
		gfx.PutPixel(x + 5 - 1, y + yy, r, g, b);
		gfx.PutPixel(x + 5, y + yy, r, g, b);
		gfx.PutPixel(x + 5 + 1, y + yy, r, g, b);
	}

}

void Game::playerControl(Boid& player) {

	// Keyboard control for the players to move its drone

	//Check if ARROW-LEFT key or A is pressed
	if (wnd.kbd.KeyIsPressed('A') || wnd.kbd.KeyIsPressed(VK_LEFT)) {
		if (player.x > 0) { player.x -= playerSpeed; } //Check if player is within world border before moving left
	}

	//Check if ARROW-RIGHT key or D is pressed
	if (wnd.kbd.KeyIsPressed('D') || wnd.kbd.KeyIsPressed(VK_RIGHT)) {
		if (player.x + 11 < WIDTH - 1) { player.x += playerSpeed; } //Check if player is within world border before moving right
	}

	//Check if ARROW-UP key or W is pressed
	if (wnd.kbd.KeyIsPressed('W') || wnd.kbd.KeyIsPressed(VK_UP)) {
		if (player.y > 2) { player.y -= playerSpeed; } //Check if player is within world border before moving up
	}

	//Check if ARROW_DOWN key or S is pressed
	if (wnd.kbd.KeyIsPressed('S') || wnd.kbd.KeyIsPressed(VK_DOWN)) {
		if (player.y + 11 < HEIGHT - 1) { player.y += playerSpeed; } //Check if player is within world border before moving down
	}

}

int Game::computeDistance(Boid drone1, Boid drone2) {

	// Computes a circular distance

	return (int) sqrt( (float) ( (drone1.x - drone2.x) * (drone1.x - drone2.x) + (drone1.y - drone2.y) * (drone1.y - drone2.y) ) );
}

void Game::attraction(Boid drone1, Boid& drone2) {

	// Drones will move towards the attracting drone based on the direction that it's opposite of

	if (drone1.x == drone2.x && drone1.y < drone2.y) { // Player on north
		drone2.y -= 2;
	}
	if (drone1.y == drone2.y && drone1.x < drone2.x) { // Player on west
		drone2.x -= 2;
	}
	if (drone1.x == drone2.x && drone1.y > drone2.y) { // Player on south
		drone2.y += 2;
	}
	if (drone1.y == drone2.y && drone1.x > drone2.x) { // Player on east
		drone2.x += 2;
	}

	if (drone1.x < drone2.x && drone1.y < drone2.y) { // Player on north west
		drone2.x -= 2;
		drone2.y -= 2;
	}
	if (drone1.x < drone2.x && drone1.y > drone2.y) { // Player on south west
		drone2.x -= 2;
		drone2.y += 2;
	}
	if (drone1.x > drone2.x && drone1.y > drone2.y) { // Player on south east
		drone2.x += 2;
		drone2.y += 2;
	}
	if (drone1.x > drone2.x && drone1.y < drone2.y) { // Player on north east
		drone2.x += 2;
		drone2.y -= 2;
	}
	
}

void Game::diversion(Boid drone1, Boid& drone2, int safeZone) {

	// Check radius if other drones are touching
	if (computeDistance(drone1, drone2) < safeZone) {

		// Move the drones when touches other drones based on the direction that it's touching
		if (drone1.x == drone2.x && drone1.y < drone2.y) { // Player on north
			drone2.y += 1;
		}
		else if (drone1.x < drone2.x && drone1.y < drone2.y) { // Player on north west
			drone2.x += 1;
			drone2.y += 1;
		}
		else if (drone1.y == drone2.y && drone1.x < drone2.x) { // Player on west
			drone2.x += 1;
		}
		else if (drone1.x < drone2.x && drone1.y > drone2.y) { // Player on south west
			drone2.x += 1;
			drone2.y -= 1;
		}
		else if (drone1.x == drone2.x && drone1.y > drone2.y) { // Player on south
			drone2.y -= 1;
		}
		else if (drone1.x > drone2.x && drone1.y > drone2.y) { // Player on south east
			drone2.x -= 1;
			drone2.y -= 1;
		}
		else if (drone1.y == drone2.y && drone1.x > drone2.x) { // Player on east
			drone2.x -= 1;
		}
		else if (drone1.x > drone2.x && drone1.y < drone2.y) { // Player on north east
			drone2.x -= 1;
			drone2.y += 1;
		}

		//Change the direction of the intefearing drone
		drone2.xVel = -drone2.xVel;
		drone2.yVel = -drone2.yVel;

	}

}

void Game::cohesion(Boid drone1, Boid& drone2, int startRadius, int endRadius) {

	// Check radius for drones to bo be sttracted to female drone. If endRadius value is not 0, shape of radius is a doughnut
	if (computeDistance(drone1, drone2) >= startRadius && computeDistance(drone1, drone2) < endRadius) {

		// Trigger cohesion
		attraction(drone1, drone2);

		// Trigger alignment
		drone2.xVel = drone1.xVel;
		drone2.yVel = drone1.yVel;

	}

}

void Game::seperation(Boid drone1, Boid& drone2, int collisionRadius, int safeZone) {

	// Reset the space
	for (int i = 0; i < uHEIGHT; i++) {

		for (int j = 0; j < uWIDTH; j++) {

			checkSpace[i][j] = 0;

		}

	}

	// Register the space for each drone
	for (int i = 0; i < MAX_DRONES; i++) {

		uX = (int)(drone[i].x / safeZone);
		uY = (int)(drone[i].y / safeZone);

		checkSpace[uX][uY] = 1;

	}

	// Check collision radius
	if (computeDistance(drone1, drone2) < collisionRadius) {
		// Red zone, collision region

		//Get drone's X and Y unit
		uX = (int)(drone2.x / safeZone);
		uY = (int)(drone2.y / safeZone);

		// Move the drone if in space based on the direction that it's touching
		if (checkSpace[uX + 1][uY]) { // East
			drone2.x -= 1;
		}
		else if (checkSpace[uX + 1][uY - 1]) { // North east
			drone2.x -= 1, drone2.y += 1;
		}
		else if (checkSpace[uX][uY - 1]) { // North
			drone2.y += 1;
		}
		else if (checkSpace[uX - 1][uY - 1]) { // North west
			drone2.x += 1, drone2.y += 1;
		}
		else if (checkSpace[uX - 1][uY]) { // West
			drone2.x += 1;
		}
		else if (checkSpace[uX - 1][uY + 1]) { // South west
			drone2.x += 1, drone2.y -= 1;
		}
		else if (checkSpace[uX][uY + 1]) { // South
			drone2.y -= 1;
		}
		else if (checkSpace[uX + 1][uY + 1]) { // South east
			drone2.x += 1, drone2.y += 1;
		}

	}
	
}

void Game::reflectionMove(Boid& drone, int size) {

	// Gets the drones to move to the opposite direction when touches the border of the world 

	// Moves drones opposite of X axis

	if(drone.x == 1 || drone.x + size == WIDTH - 1) {
		drone.xVel = -drone.xVel;
	}

	drone.x += drone.xVel;

	// Moves drones opposite of Y axis

	if (drone.y == 1 || drone.y + size == HEIGHT - 1) {
		drone.yVel = -drone.yVel;
	}

	drone.y += drone.yVel;

}

void Game::clampNPCinWindow(Boid& drone, int size) {

	// Prevents the drones from flying away from the border of the world

	// Drone gets set back to X position 1 at the most left
	if (drone.x < 0) {
		drone.x = 1;
	}

	// Drone gets set back to the end of the X position at the most right factoring the size of the drone
	if (drone.x + size > WIDTH - 1) {
		drone.x = WIDTH - 1 - size;
	}

	// Drone gets set back to Y position 1 at the most top
	if (drone.y < 0) {
		drone.y = 1;
	}

	// Drone gets set back to the end of the Y position at the most bottom factoring the size of the drone
	if (drone.y + size > HEIGHT - 1) {
		drone.y = HEIGHT - 1 - size;
	}

}

void Game::playerDrone() {

	// Sets the controls of the player to work
	// Display the player's drone
	playerControl(player);
	drawCrosshair(player.x, player.y, 255, 0, 0);

	//Diversion algorithm between the player and male drones

	//Reset controlling variable
	counter = 0;
	feCount = 0;

	for (int i = 0; i < MAX_DRONES; ) {

		//Check if current drone is female drone
		if (i == femaleDrone[feCount]) {
			counter++;
		}

		//Check if all female drones are checked
		if (feCount == (MAX_F_DRONES)) {

			//Check if any encountered female drones
			if (counter == 0) {
				diversion(player, drone[i], 25);
			}

			//Reset counter, proceed to next drone
			feCount = 0;
			counter = 0;
			i++;

		}

		else {

			feCount++;

		}

	}

	//Attraction algorithm between player and female drones

	//Reset counters
	feCount = 0;

	for (int i = 0; i < MAX_DRONES; ) {

		//Check if current drone is female drone
		if (i == femaleDrone[feCount]) {
			//Check attraction radius
			if (computeDistance(player, drone[i]) < 25) {
				attraction(player, drone[i]);
			}
		}

		//Check if all female drones have been gone through
		if (feCount == (MAX_F_DRONES)) {
			feCount = 0;
			i++;
		}
		else {
			feCount++;
		}

	}

}

void Game::drones() {

	//Displays the correct colour to female and male drones
	//Sets the drones to reflect at border and prevent drones from flying away from world

	//Reset controlling variable
	counter = 0;
	feCount = 0;

	for (int i = 0; i < MAX_DRONES; ) {

		//Check if current drone is female drone
		if (i == femaleDrone[feCount]) {
			counter++;
		}

		//Check if all female drones have been checked
		if (feCount == MAX_F_DRONES) {

			clampNPCinWindow(drone[i], 10);

			//Check if current drone is female drone or male drone to display appropriate drone
			if (counter == 0) {
				drawCrosshair(drone[i].x, drone[i].y, 65, 105, 255);
			}
			else {
				drawCrosshair(drone[i].x, drone[i].y, 255, 182, 193);
			}

			reflectionMove(drone[i], 10);

			//Reset counters, proceed to next drone
			feCount = 0;
			counter = 0;
			i++;

		}
		else {
			feCount++;
		}

	}

}

void Game::flockingDrones() {

	//Algorithm to simulate flocking drones between male drones and female drones

	//Reset controlling variable
	counter = 0;
	feCount = 0;
	feCount2 = 0;

	for (int i = 0; i < MAX_DRONES; ) {

		//Check if there are any female drones, check if current drone is the same as checking female drone
		if (feCount == MAX_F_DRONES) { // Temp solution
			break;
		}
		else if (i == femaleDrone[feCount2]) {
			counter++;
		}

		//Check if checking drone has checked all drones
		if (feCount2 == MAX_F_DRONES) {

			//Check if current count has female drone encounter
			if (counter == 0) {
				seperation(drone[femaleDrone[feCount]], drone[i], 50, 10);
				//diversion(drone[femaleDrone[feCount]], drone[i], 12); //Testing, new method
				cohesion(drone[femaleDrone[feCount]], drone[i], 50, 100);
			}

			//Proceed to next checking female drone, reset counters
			feCount++;
			feCount2 = 0;
			counter = 0;

		}
		else {
			feCount2++;
		}

		//Check if all female drones have been checked
		if (feCount == MAX_F_DRONES) {
			i++;
			feCount = 0;
		}

	}

}

void Game::notLasbyi() {

	//Algorithm to repel female drones from other female drones

	//Reset controlling variable
	counter = 0;
	feCount = 0;
	feCount2 = 0;

	for (int i = 0; feCount != MAX_F_DRONES; ) {

		//Check if there are any female drones, check if current female drone is the same as checking female drone
		if (feCount2 == feCount && (feCount2 + 1) == MAX_F_DRONES) { // Temp solution
			break;
		}
		else if (feCount2 == feCount) {
			feCount2++;
		}

		//seperation(drone[femaleDrone[feCount]], drone[i], 100, 10);
		diversion(drone[femaleDrone[feCount]], drone[femaleDrone[feCount2]], 100);

		//Check if current drones have checked with all checking drones
		if (counter == 1) {
			feCount++;
			feCount2 = 0;
			counter = 0;
		}
		else {
			counter++;
			feCount2++;
		}

	}

}

void Game::notGei() {

	//Algorithm to repel male drones from other male drones

	//Reset controlling variable
	counter = 0;
	feCount = 0;
	feCount2 = 0;

	for (int i = 0; i < MAX_DRONES; ) {

		//Check if current drone is female drone
		if (i == femaleDrone[feCount]) {
			i++;
		}

		//Check if checking drone is female drone
		if (counter == femaleDrone[feCount]) {
			counter++;
			feCount2++;
		}

		//Check if current drone is checking drone
		if (counter == i) {
			counter++;
		}

		//Check if all female drones have been checked
		if (feCount == MAX_F_DRONES) {

			//Check if current count has female drone encounter
			if (feCount2 == 0) {
				//seperation(drone[i], drone[counter], 10, 10);
				diversion(drone[i], drone[counter], 12);
			}

			//Reset female drone counter and checker
			feCount = 0;
			feCount2 = 0;

			//Check if checking drone has checked all drones
			if (counter == MAX_DRONES) {
				counter = 0;
				i++;
			}
			else {
				counter++;
			}

		}

		else {

			feCount++;

		}

	}

}

void Game::ComposeFrame() {

	//Flow of the entire simulation

	playerDrone();
	drones();
	flockingDrones();
	notLasbyi();
	notGei();

}