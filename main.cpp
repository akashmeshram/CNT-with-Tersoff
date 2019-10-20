#include <bits/stdc++.h>
#include "vec.h"
#include "mol.h"
#include "ParamsOfCarbon.h"
using namespace std;

double detaT = 0.002 ;
int stepAvg = 10;
double temperature = 1;
int steps;
double timeNow;
double regionZ = 4.912;

int total = 40;
Mol mole[40]; /*Array for number of Molecules*/
double uSum, virSum;
double kinEnergy, totEnergy, pressure = 0.0;

/* Function Declrations */
void setup();
void initVel();
void simulate();
void checkBoundary();
void computeForce();
void update();
void evaluate();
void PrintSummary();
createVector getVec(int j1, int j2);


/* Main Function */
int main(int argc, char
  const * argv[]) {
  cout << "steps, timeNow, kinEnergy, totEnergy" << endl;
  srand(time(NULL));
  steps = 0;
  setup();
  int loop = 1;
  int i;
  while (loop) {
    simulate();
    if (steps > 1000) loop = 0;
  }
  return 0;
}

/* For formatiing input file */
void split(const string & s, char c,
  vector < string > & v) {
  string::size_type i = 0;
  string::size_type j = s.find(c);

  while (j != string::npos) {
    v.push_back(s.substr(i, j - i));
    i = ++j;
    j = s.find(c, j);

    if (j == string::npos)  v.push_back(s.substr(i, s.length()));
  }
}

/* TO initialize Parameters */
void setup() {
  int index;
  string line;
  ifstream myfile("points"); // File Input
  double offset = 1.228 / 2.0;
  if (myfile.is_open()) {
    vector < string > v;
    index = 0;
    while (getline(myfile, line)) {
      split(line, ' ', v);
      mole[index] = Mol(stof(v[0]), stof(v[1]), stof(v[2]) + offset);
      v.clear();
      index++;
    }
    myfile.close();
  } else cout << "Unable to open file";

  initVel();
}

/* For velocity asignment */
void initVel() {
  for (int j = 0; j < total; ++j) {
    double a, b, c, mag;
    a = rand() % 200 - 100;
    b = rand() % 200 - 100;
    c = rand() % 200 - 100;
    mag = sqrt(a * a + b * b + c * c);
    double velMag = sqrt(2 * (1. - (1. / total)) * temperature);
    // cout << mag << endl;
    a = (a / mag) * velMag;
    b = (b / mag) * velMag;
    c = (c / mag) * velMag;
    mole[j].vel.setXYZ(a, b, c);
  }
}


/* For carring Simulation */
void simulate() {
  ++steps;
  timeNow = steps * detaT;
  update();
  evaluate();
  if (steps % stepAvg == 0) {
    PrintSummary();
  }
}

/* TO check boundary */
void checkBoundary() {
  for (int j = 0; j < total; ++j) {
    createVector br = mole[j].getPos();
    // Only in z direction because cnt is one dimentional
    if (br.getZ() < 0) {
      mole[j].pos.z += regionZ;
    } else
    if (br.getZ() > regionZ) {
      mole[j].pos.z -= regionZ;
    }
  }
}

/* For calculating force and potential */
void computeForce() {
  uSum = 0;
  for (int j = 0; j < total; ++j) {
    mole[j].acc.setX(0);
    mole[j].acc.setY(0);
    mole[j].acc.setZ(0);
  }

  double cSqr = c * c;
  double dSqr = d * d;
  double cdSqr = cSqr / dSqr;
  double invn2 = -1.0 / 2.0 / n;

  double fCsave[40][40], dfCdrsave[40][40], 
         fAsave[40][40], dfAdrsave[40][40], 
         fRsave[40][40], dfRdrsave[40][40],
         rd[40][40];
  vector < int > nPos;
  createVector ru[40][40], rv[40][40];

  double gSave[40][40][40], dgSave[40][40][40], saveTheta[40][40][40];

  createVector dr, fc, fTheta;
  double V, fC, fA, fR, r;
  double Arg;
  int num;
  double Zeta, CosTheta, dCosTheta, g, BetaZN, BetaZN1, b, dgdc, db;
  int i, j, k, n1, n2;

  double bsave[40][40], dbsave[40][40];

  double dfCdr, dfAdr, dfRdr, force;

  memset(saveTheta, 0, sizeof saveTheta);
  memset(gSave, 0, sizeof gSave);
  memset(dgSave, 0, sizeof dgSave);

  for (i = 0; i < total; i++) {

    num = 0;

    for (j = 0; j < total; j++) {

      if (j == i) continue;
      dr = getVec(j, i);
      rv[i][j] = (dr);
      r = dr.getMagnitude();
      rd[i][j] = (r);
      ru[i][j] = (dr / r);
      if (r > S) continue;

      if (r < R) {
        fC = 1;
        dfCdr = 0;
      } else if (r <= S) {
        Arg = 3.1416 * (r - R) / (S - R);
        fC = 0.5 - 0.5 * sin(Arg);
        dfCdr = -0.5 * 3.1416 * cos(Arg) / (S - R);
      } else {
        fC = 0;
        dfCdr = 0;
      }


      fR = A * exp(-Lam * r);
      dfRdr = -Lam * fR;
      fA = -B * exp(-Mu * r);
      dfAdr = -Mu * fA;

      fCsave[i][j] = (fC);
      dfCdrsave[i][j] = (dfCdr);
      fRsave[i][j] = (fR);
      dfRdrsave[i][j] = (dfRdr);
      fAsave[i][j] = (fA);
      dfAdrsave[i][j] = (dfAdr);

      num++;

      nPos.push_back(j);

    }

    for (int n1 = 0; n1 < num; n1++) {
      j = nPos[n1];
      Zeta = 0;

      for (int n2 = 0; n2 < num; n2++) {
        k = nPos[n2];

        if (k == j) continue;

        createVector rjk = getVec(k, j);
        double drjk = rjk.getMagnitude();

        CosTheta = ru[i][j].x * ru[i][k].x + ru[i][j].y * ru[i][k].y + ru[i][j].z * ru[i][k].z;
        dCosTheta = dSqr + (h - CosTheta) * (h - CosTheta);
        g = 1 + cdSqr - cSqr / dCosTheta;
        Zeta += fCsave[i][k] * g;
        dgdc = -2.0 * cSqr * (h - CosTheta) / (dCosTheta * dCosTheta);

        gSave[i][j][k] = g;
        dgSave[i][j][k] = dgdc;
        saveTheta[i][j][k] = CosTheta;
      }
      /*For potential calculation */
      fC = fCsave[i][j];
      fA = fAsave[i][j];
      fR = fRsave[i][j];

      BetaZN = pow((beta * Zeta), n);
      BetaZN1 = pow(beta, n) * pow(Zeta, (n - 1.0));
      b = pow((1.0 + BetaZN), invn2);
      V = 0.5 * fC * (fR + b * fA); // Potential Calculation
      uSum += 0.5 * V;

      db = -0.5 * b * BetaZN1 / (1 + BetaZN);

      bsave[i][j] = b;
      dbsave[i][j] = db;
    }
    nPos.clear();
  }
    
    /*Force calculation */
  double theta[2][4], ktheta;

  for (i = 0; i < total; i++) {
    for (j = 0; j < total; j++) {

      if (i == j) continue;

      for (int m1 = 0; m1 < 2; m1++) {
        for (int m2 = 0; m2 < 4; m2++) {
          theta[m1][m2] = 0;
        }
      }

      ktheta = 0;

      for (k = 0; k < total; k++) {
        if (k == j || k == i) continue;

        theta[0][0] += fCsave[i][k] * dbsave[i][k] * fAsave[i][k] * gSave[i][j][k];
        theta[0][2] += (saveTheta[i][j][k] / rd[i][j] - 1 / rd[i][k]) * fCsave[i][k] * dgSave[i][j][k];
        theta[0][3] += fCsave[i][k] * dbsave[i][k] * fAsave[i][k] * (saveTheta[i][j][k] / rd[i][j] - 1 / rd[i][k]) * dgSave[i][j][k];

        theta[1][0] += fCsave[j][k] * dbsave[j][k] * fAsave[j][k] * gSave[j][i][k];
        theta[1][2] += (saveTheta[j][i][k] / rd[j][i] - 1 / rd[j][k]) * fCsave[j][k] * dgSave[j][i][k];
        theta[1][3] += fCsave[j][k] * dbsave[j][k] * fAsave[j][k] * (saveTheta[j][i][k] / rd[j][i] - 1 / rd[j][k]) * dgSave[j][i][k];

        ktheta += (fCsave[k][i] * fCsave[k][j] / rd[k][i] / rd[k][i]) * dgSave[k][i][j] *
          (dbsave[k][i] * fAsave[k][i] + dbsave[k][j] * fAsave[k][j]);
        
      }

      force =
        dfCdrsave[i][j] * (fRsave[i][j] + 0.5 * (bsave[i][j] + bsave[j][i]) * fAsave[i][j]) +
        fCsave[i][j] * (dfRdrsave[i][j] + 0.5 * (bsave[i][j] + bsave[j][i]) * dfAdrsave[i][j]) + 0.5 * dfCdrsave[i][j] * theta[0][0] +
        0.5 * fCsave[i][j] * dbsave[i][j] * fAsave[i][j] * (theta[0][1] - theta[0][2]) -
        0.5 * fCsave[i][j] * theta[0][3] + 0.5 * dfCdrsave[j][i] * theta[1][0] +
        0.5 * fCsave[j][i] * dbsave[j][i] * fAsave[j][i] * (theta[1][1] - theta[1][2]) -
        0.5 * fCsave[j][i] * theta[1][3] - 0.5 * rd[i][j] * ktheta;

      force *= 1;

      // Applying force
      mole[i].acc = mole[i].acc + ru[i][j] * force;
      mole[j].acc = mole[j].acc - ru[i][j] * force;
    }
  }
  
}


/* Updating parameters */
void update() {

  for (int i = 0; i < total; ++i) {
    mole[i].pos = mole[i].pos + mole[i].vel * 0.5 *  detaT +  mole[i].acc * 0.5 * detaT * detaT;
    mole[i].vel = mole[i].vel + mole[i].acc * 0.5 * detaT;
  }

  checkBoundary();
  computeForce();

  for (int i = 0; i < total; ++i) {
    mole[i].vel = mole[i].vel + mole[i].acc * 0.5 * detaT;
  }

}

/* To create displacement vector */
createVector getVec(int j1, int j2) {
  createVector dr;

  dr = mole[j1].pos - mole[j2].pos;

  if (fabs(dr.getZ()) > 0.5 * regionZ) {
    if (dr.getZ() > 0) {
      dr.setZ(-(regionZ - dr.getZ()));
    } else if (dr.getZ() < 0) {
      dr.setZ(regionZ + dr.getZ());
    }
  }

  return dr;
}

/* To calculate energy */
void evaluate() {
  double sqVel = 0;
  for (int j = 0; j < total; ++j) {
    sqVel += pow(mole[j].vel.getMagnitude(), 2);
  }

  kinEnergy = 0.5 * sqVel  / total;
  totEnergy = kinEnergy + uSum / total;
}

// To print energy
void PrintSummary() {
  printf("%d %4.6lf  %4.6lf  %4.6lf \n", steps, timeNow, kinEnergy, totEnergy);
}

