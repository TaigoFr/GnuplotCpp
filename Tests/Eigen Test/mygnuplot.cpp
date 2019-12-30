
#include "gnuplot-iostream.h"
#include <Eigen/Dense>

#include <vector>
#include <cmath>

double func(double x){
	return x*x - 10.*cos(2.*M_PI*x);
}

void plot(){
	const int num_steps = 100;
	const double min = -5., max = 5.;

	Eigen::MatrixXd mat(4,num_steps);
	double x = min;
	double step = (max-min)/num_steps;
	for(int i=0; i<num_steps; ++i) {
		mat(0,i) = x;
		mat(1,i) = func(x);
		mat(2,i) = 0.05;
		mat(3,i) = 0.2;
		x += step;
	}

	std::cout << mat << std::endl;

	std::vector<std::vector<double>> pts(num_steps);
	for(int i=0; i<pts.size(); ++i)
	{
		auto start = mat.data()+i*mat.rows();
		pts[i] = std::vector<double>(start, start + mat.rows());
		std::cout << pts[i][0] << " " << pts[i][1] << std::endl;
	}

	Gnuplot gp;
	gp << "set xrange [-5:5]\n";
	gp << "set title 'Random plot with Sample Data x^2_{fancy code}'\n";
	gp << "plot ";
	gp << gp.binFile1d(pts, "record") << "with xyerrorbars title 'MyTest (I hope Miguel likes it)'";
	// gp << ", ";
	// gp << gp.binFile1d(pts2, "record") << "with xyerrorbars title 'MyTest2 (did he?)'";
	gp << std::endl;
}

int main(){
	plot();

	return 0;
}