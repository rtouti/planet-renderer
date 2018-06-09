#include "Noise.h"

#include <cmath>
#include <numeric>
#include <random>
#include <algorithm>
#include <iostream>

Noise3D::Noise3D(int seed){
	this->seed = seed;

	p.resize(256);

	std::iota(p.begin(), p.end(), 0);
	std::default_random_engine rng(seed);
	std::shuffle(p.begin(), p.end(), rng);
	p.insert(p.end(), p.begin(), p.end());
}

Noise3D::~Noise3D(){

}

double Noise3D::Noise(double x, double y, double z){
	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	int Z = (int)std::floor(z) & 255;

	x -= std::floor(x);
	y -= std::floor(y);
	z -= std::floor(z);

	double u = Fade(x);
	double v = Fade(y);
	double w = Fade(z);

	int A = p[X]+Y, AA = p[A]+Z, AB = p[A+1]+Z,
		B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;

	double result = Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z), Grad(p[BA], x-1, y, z)),
										 Lerp(u, Grad(p[AB], x, y-1, z), Grad(p[BB], x-1, y-1, z))),
							Lerp(v, Lerp(u, Grad(p[AA+1], x, y, z-1), Grad(p[BA+1], x-1, y, z-1)),
										 Lerp(u, Grad(p[AB+1], x, y-1, z-1), Grad(p[BB+1], x-1, y-1, z-1))));

	return result;
}

double Noise3D::Lerp(double t, double a, double b){
	return a + t * (b - a);
}

double Noise3D::Fade(double t){
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double Noise3D::Grad(int hash, double x, double y, double z){
	int h = hash & 15;

	switch (h){
	case 0:
		return x+y;
	case 1:
		return x-y;
	case 2:
		return -x+y;
	case 3:
		return -x-y;
	case 4:
		return x+z;
	case 5:
		return x-z;
	case 6:
		return -x+z;
	case 7:
		return -x-z;
	case 8:
		return y+z;
	case 9:
		return y-z;
	case 10:
		return -y+z;
	case 11:
		return -y-z;
	case 12:
		return x+y;
	case 13:
		return x-y;
	case 14:
		return -x+y;
	case 15:
		return -x-y;
	}
}
