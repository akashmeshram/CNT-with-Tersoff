# Carbon-nanotube-by-tersoff-potential
Carbon nanotubes is allotrope of carbon with a cylindrical nanostructure. Here its molecular dynamics is studied
by simulation by construction tersoff potential from scratch. The Tersoff potential is a three-body 
potential function which icludes an angular contribution of the force.

## Getting Started
The file [main.cpp](main.cpp) will do a molecular dynamics simulation of Carbon Nanotube.

## Requirements
* C++ compiler 
* [mol.cpp](mol.cpp) - *For molecules*
* [vec.cpp](vec.cpp) - *For vectors*

## Configuration
* **total** - *Number of atoms in a axially repeated cnt*
* **detaT** - *Time step*
* **stepAvg** - *Average time for showing result*
* **regionZ** - *region of axial length*
* **temperature** - *Temperature of system*

## Parameters of Tersoff Potential for atoms of carbon
|Parameter| value|
| ------------- |:-------------:|
| c | 3.8049e4 |
|d | 4.3484|
|lam3 | 0|
|Mu | 2.2119|
|Lam(Å<sup>-1</sup>) | 3.4879|
|lam2(Å<sup>-1</sup>) | 2.2119|
|lam1(Å<sup>-1</sup>) | 3.4879|
|beta | 1.5724e-7|
|h | -0.57058|
|R(Å) | 1.8|
|S(Å) | 2.1|
|D | 0.15|
|m | 3|
|n | 0.72751|
|A(eV) | 1393.6|
|B(eV) | 346.74|


## Built With
* [C++](https://isocpp.org/)

## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Author 
* [Akash Meshram](https://github.com/akashmeshram)
