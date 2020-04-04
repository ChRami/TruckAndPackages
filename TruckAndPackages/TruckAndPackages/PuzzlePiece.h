#pragma once
#include <string>

using namespace std;

class PuzzlePiece {
private:
	int length;
	int width;
	int height;
	int x;
	int y;
	int z;

	int value;

public:

	PuzzlePiece();
	PuzzlePiece(int x, int y, int z, int length, int width, int height, int value);

	int getLength() const;
	int getWidth() const;
	int getHeight() const;

	int getX() const;
	int getY() const;

	int getZ() const;
	int getValue() const;

	void setX(int x);
	void setY(int y);
	void setZ(int z);

	void rotatePiece(int choice);

	void moveX(int amount);
	void moveY(int amount);
	void moveZ(int amount);

	string toString();

};
