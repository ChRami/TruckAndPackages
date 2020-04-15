#include "PuzzlePiece.h"

PuzzlePiece::PuzzlePiece() {
}

PuzzlePiece::PuzzlePiece(int x, int y, int z, int length, int width, int height, int value) {
	this->length = length;
	this->height = height;
	this->width = width;
	this->x = x;
	this->y = y;
	this->z = z;
	this->value = value;
}

int PuzzlePiece::getHeight() const {
	return this->height;
}

int PuzzlePiece::getWidth() const {
	return this->width;
}

int PuzzlePiece::getLength() const {
	return this->length;
}

int PuzzlePiece::getX() const {
	return this->x;
}

int PuzzlePiece::getY() const {
	return this->y;
}

int PuzzlePiece::getZ() const {
	return this->z;
}

int PuzzlePiece::getValue() const {
	return this->value;
}

void PuzzlePiece::rotatePiece(int choice) {
	switch (choice) {
	case 0: // 0 = X-turn
	{
		int temp = length;
		this->length = width;
		this->width = temp;
		break;
	}
	case 1: // 1 = Y-turn
	{
		int temp = width;
		this->width = height;
		this->height = temp;
		break;
	}
	case 2: // 2 = Z-turn
	{
		int temp = height;
		this->height = length;
		this->length = temp;
		break;
	}
	}
}

void PuzzlePiece::moveX(int amount) {
	x += amount;
}

void PuzzlePiece::moveY(int amount) {
	y += amount;
}

void PuzzlePiece::moveZ(int amount) {
	z += amount;
}

void PuzzlePiece::setX(int x) {
	this->x = x;
}

void PuzzlePiece::setY(int y) {
	this->y = y;
}

void PuzzlePiece::setZ(int z) {
	this->z = z;
}

string PuzzlePiece::toString() {
	string result = "";
	result += "[" + to_string(x) + "; " + to_string(y) + "; " + to_string(z) + "; " + to_string(length) + "; " + to_string(width) + "; " + to_string(height) + "]";

	return result;
}