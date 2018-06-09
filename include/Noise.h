#ifndef NOISE_H
#define NOISE_H


#include <vector>

class Noise3D {
public:
	Noise3D(int seed);
	~Noise3D();
	double Noise(double x, double y, double z);
private:
	int seed;
	std::vector<int> p;

	double Lerp(double t, double a, double b);
	double Fade(double t);
	double Grad(int hash, double x, double y, double z);

};

#endif // NOISE_H
