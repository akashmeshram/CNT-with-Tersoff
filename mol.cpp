#include <iostream>
#include "vec.h"
#include "mol.h"    
using namespace std;

Mol::Mol(float x , float y , float z ) {
	pos.setX(x);
	pos.setY(y);
	pos.setZ(z);
}

void Mol::setPos(float x, float y , float z ) {
	pos.setX(x);
	pos.setY(y);
	pos.setZ(z);
}

void Mol::setVel(float x, float y , float z ) {
	vel.setX(x);
	vel.setY(y);
	vel.setZ(z);
}

void Mol::setAcc(float x, float y , float z ) {
	acc.setX(x);
	acc.setY(y);
	acc.setZ(z);
}

createVector Mol::getPos()  {
	return this->pos;
}

createVector Mol::getVel()  {
	return this->vel;
}

createVector Mol::getAcc() {
	return this->acc;
}