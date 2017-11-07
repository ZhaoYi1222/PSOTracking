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
	int isseed;
	Particle() {}
	Particle(int _px, int _py, int _vx, int _vy, int _pbestx, int _pbesty, int _gbestx, int _gbesty,double _fitness,double _pbest,double _gbest,int _isseed)
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
		isseed = _isseed;
	}
	bool operator<(const Particle &oth) const
	{
		return fitness > oth.fitness;
	 }
};
extern Particle pa[25][45];

struct target
{
	Point pl;
	Point pr;
	int width;
	int height;
	double vxmin;
	double vymin;
	double vxmax;
	double vymax;
	target() {}
	target(Point _pl, Point _pr,int _width,int _height,double _vxmin,double _vymin,double _vxmax,double _vymax)
	{
		pl = _pl;
		pr = _pr;
		width = _width;
		height = _height;
		vxmin = _vxmin;
		vymin = _vymin;
		vxmax = _vxmax;
		vymax = _vymax;
	}
};
extern target area[20];