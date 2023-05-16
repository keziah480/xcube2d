#include "MyGame.h"

MyGame::MyGame() : AbstractGame(), score(0), lives(3), numKeys(5), gameWon(false) {
	TTF_Font* font = ResourceManager::loadFont("res/fonts/arial.ttf", 20);
	gfx->useFont(font);
	gfx->setVerticalSync(true);

	//newMesh = Mesh(-7, 200, 800, "cat.txt", 0, 150, 150);
	//newMesh.RotateMesh(0, M_PI);
	
	canvas = new Pixel * [800];

	for (int i = 0; i < 800; i++) {
		canvas[i] = new Pixel[600];
	}
	mainCube = Cuboid(0, 0, 1, 1, 1, 1, 200, 100, 0);

	timeT = 0;
	cubeXVel = 0.1;
	cubeYVel = 0.1;
	
}

MyGame::~MyGame() {

}

void MyGame::handleKeyEvents() {
	int speed = 3;

	if (eventSystem->isPressed(Key::W)) {
	}
	if (eventSystem->isPressed(Key::S)) {
	}
	if (eventSystem->isPressed(Key::A)) {
	}
	if (eventSystem->isPressed(Key::D)) {
	}
	if (eventSystem->isPressed(Key::LEFT)) {
	}
	if (eventSystem->isPressed(Key::RIGHT)) {
	}
	if (eventSystem->isPressed(Key::UP)) {
	}
	if (eventSystem->isPressed(Key::DOWN)) {
	}
	if (eventSystem->isPressed(Key::SPACE)) {
	}
}

void MyGame::update() {
	
	timeT += 0.1;

	if (timeT > 2 * M_PI) {
		timeT -= 2 * M_PI;
	}
	mainCube.centerPoint.x = sinf(timeT);
	mainCube.centerPoint.y = sinf(timeT);
	mainCube.centerPoint.z = cosf(timeT);

	mainCube.RotateCuboid(0, 0.01);
	mainCube.RotateCuboid(1, 0.02);

	mainCube.RenderMesh(canvas, lightSource, eye, fov, offsetX, offsetY);

}

void MyGame::render() {


	for (int row = 0; row < DEFAULT_WINDOW_HEIGHT; row++) {
		for (int col = 0; col < DEFAULT_WINDOW_WIDTH; col++) {
			
			int averageR = 0;
			int averageG = 0;
			int averageB = 0;

			if (antiAliasingActive) {
				// Anti-aliasing
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (col + i >= 0 && col + i < DEFAULT_WINDOW_WIDTH && row + j >= 0 && row + j < DEFAULT_WINDOW_HEIGHT) {
							averageR += canvas[col + i][row + j].r;
							averageG += canvas[col + i][row + j].g;
							averageB += canvas[col + i][row + j].b;
						}
					}
				}

				averageR = round(averageR / 9);
				averageG = round(averageG / 9);
				averageB = round(averageB / 9);
			}
			else {
				averageR = canvas[col][row].r;
				averageG = canvas[col][row].g;
				averageB = canvas[col][row].b;
			}
			

			// Drawing
			gfx->setDrawColor(toSDLColor(averageR, averageG, averageB, 255));

			gfx->drawPoint(Point2(col, row));
		}
	}

	for (int row = 0; row < DEFAULT_WINDOW_HEIGHT; row++) {
		for (int col = 0; col < DEFAULT_WINDOW_WIDTH; col++) {

			
			if (blurActive) {
				// Motion Blur
				canvas[col][row].r = int((canvas[col][row].r * 0.8));
				canvas[col][row].g = int((canvas[col][row].g * 0.8));
				canvas[col][row].b = int((canvas[col][row].b * 0.8));
			}
			
			else {
				canvas[col][row].r = 0;
				canvas[col][row].g = 0;
				canvas[col][row].b = 0;
			}
			
			canvas[col][row].depth = 99999;
		}
	}

	for (auto key : gameKeys)
		if (key->isAlive)
			gfx->drawCircle(key->pos, 5);
}

void MyGame::renderUI() {
	gfx->setDrawColor(SDL_COLOR_AQUA);
	std::string scoreStr = std::to_string(score);
	
	gfx->drawText("DANIEL_MCCOURT-CI517-2022", scoreStr.length() * 50, 25);
}