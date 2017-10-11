#include "PSO.h"
struct target
{
	Mat *tarptr;
	int h;
	int w;
	target(){}
	target(Mat *_tarptr, int _h, int _w)
	{
		tarptr = _tarptr;
		h = _h;
		w = _w;
	}
}area[20];