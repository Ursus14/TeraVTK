#include "Line.h"

class Lines
{
public:
	Lines();

public:
	void add(Line* line);

	bool intersect(Line* a, Line* b);

	double* getIntersectionPoint(Line* that, Line* other);

private:
	double findDeterminant(double a, double b, double c, double d);
	bool between(double a, double b, double c);
	bool intersect1(double a, double b, double c, double d);

private:
	std::vector<Line*> lines;
	double eps;
};

