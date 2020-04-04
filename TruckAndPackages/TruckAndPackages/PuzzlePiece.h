#pragma once
#include <string>

using namespace std;

class PuzzlePiece {
private:
	int height;
	int width;
	int x;
	int y;

public:

	PuzzlePiece();
	PuzzlePiece(int x, int y, int width, int height);

	int getHeight() const;
	int getWidth() const;
	int getX() const;
	int getY() const;
	void setX(int x);
	void setY(int y);

	void rotatePiece();
	void moveX(int amount);
	void moveY(int amount);

	string toString();

};
