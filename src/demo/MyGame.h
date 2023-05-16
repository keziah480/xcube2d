#ifndef __TEST_GAME_H__
#define __TEST_GAME_H__

#include "../engine/AbstractGame.h"

struct GameKey {
	Point2 pos;
	bool isAlive;
};

class MyGame : public AbstractGame {
	private:
		
		// Camera

		Pixel** canvas;

		float lightSource[3] = { -0.1,0.45,-1 };
		Vertex eye = Vertex(0, 0, -10);
		float fov = 5;
		float offsetX = 400;
		float offsetY = 300;

		// Effects 

		bool blurActive = false;
		bool antiAliasingActive = false;

		// Scene Objects

		Sphere sphere1;
		Cuboid mainCube;
		Cuboid cubes[3];
		Mesh newMesh;

		int selector = 0;

		float timeT;

		float cubeXVel;
		float cubeYVel;


		Vector2i velocity;

		std::vector<std::shared_ptr<GameKey>> gameKeys;

		/* GAMEPLAY */
		int score, numKeys, lives;
		bool gameWon;

		void handleKeyEvents();
		void update();
		void render();
		void renderUI();
	public:
        MyGame();
		~MyGame();
};

#endif