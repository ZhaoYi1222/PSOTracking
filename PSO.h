#pragma once
#include<opencv.hpp>
#include<highgui.h>
using namespace cv;
#include<iostream>
#include <string>
using namespace std;
struct observe
{
	Mat targetobserve;
};
extern observe ob[30];
struct Particle
{
	int px;
	int py;
	int vx;
	int vy;
	int pbestx;
	int pbesty;
	int gbestx;
	int gbesty;
	double fitness;
	double pbest;
	double gbest;
	Particle() {}
	Particle(int _px, int _py, int _vx, int _vy, int _pbestx, int _pbesty, int _gbestx, int _gbesty,double _fitness,double _pbest,double _gbest)
	{
		px = _px;
		py = _py;
		vx = _vx;
		vy = _vy;
		pbestx = _pbestx;
		pbesty = _pbesty;
		gbestx = _gbestx;
		gbesty = _gbesty;
		fitness = _fitness;
		pbest = _pbest;
		gbest = _gbest;
	}
};
extern Particle pa[25][45];