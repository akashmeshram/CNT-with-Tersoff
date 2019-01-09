#include "vec.h"
#include <iostream>
#include <cmath>
using namespace std;

createVector::createVector(float x, float y, float z) : x(x), y(y), z(z) { }

float createVector::getX() const {
   return x;
}

void createVector::setX(float x) {
   this->x = x;
}

float createVector::getY() const {
   return y;
}

void createVector::setY(float y) {
   this->y = y;
}

float createVector::getZ() const {
   return z;
}

void createVector::setZ(float z) {
   this->z = z;
}

const createVector createVector::operator+(const createVector & rhs) const {
   return createVector(x + rhs.x, y + rhs.y, z + rhs.z);
}

const createVector createVector::operator-(const createVector & rhs) const {
   return createVector(x - rhs.x, y - rhs.y, z - rhs.z);
}

const createVector createVector::operator*(float value) const {
   return createVector(x*value, y*value, z*value);
}

const createVector createVector::operator/(float value) const {
   return createVector(x/value, y/value, z/value);
}

void createVector::setXYZ(float x, float y, float z) {
   this->x = x;
   this->y = y;
   this->z = z;
}

double createVector::getMagnitude() const {
   return sqrt(x*x + y*y+ z*z);
}
