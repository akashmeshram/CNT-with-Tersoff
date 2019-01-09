#ifndef MOL_H   
#define MOL_H

class Mol {
public:
   createVector pos;
   createVector vel;
   createVector acc;
   Mol(float x = 0, float y = 0, float z = 0);
   void setPos(float x, float y, float z);
   void setVel(float x, float y, float z);
   void setAcc(float x, float y, float z);
   createVector getPos() ;
   createVector getVel() ;
   createVector getAcc() ;
};

#endif