#include "PuzzlePiece.h"

PuzzlePiece::PuzzlePiece() {
}

PuzzlePiece::PuzzlePiece(int x, int y, int width, int height) {
	this->height = height;
	this->width = width;
	this->x = x;
	this->y = y;
}

int PuzzlePiece::getHeight() const {
	return this->height;
}

int PuzzlePiece::getWidth() const {
	return this->width;
}

int PuzzlePiece::getX() const {
	return this->x;
}

int PuzzlePiece::getY() const {
	return this->y;
}

void PuzzlePiece::rotatePiece() {
	int temp = height;
	this->height = width;
	this->width = temp;
}

void PuzzlePiece::moveX(int amount) {
	x += amount;
}

void PuzzlePiece::moveY(int amount) {
	y += amount;
}

void PuzzlePiece::setX(int x) {
	this->x = x;
}

void PuzzlePiece::setY(int y) {
	this->y = y;
}

string PuzzlePiece::toString() {
	string result = "";
	result += "[" + to_string(height) + "; " + to_string(width) + "; " + to_string(x) + "; " + to_string(y) + "]";

	return result;
}