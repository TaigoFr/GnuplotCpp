#include "Graphics.hpp"
#include <cmath>
#include <unistd.h>


double func(double x)
{
    return x * x - 10.*cos(2.*M_PI * x);
    // return exp(2.*x);
}

void plot()
{
    const int num_steps = 100;
    const double min = -5., max = 5.;

    Eigen::MatrixXd mat(4, num_steps);
    double x = min;
    double step = (max - min) / num_steps;
    for(int i = 0; i < num_steps; ++i)
    {
        mat(0, i) = x;
        mat(1, i) = func(x);
        mat(2, i) = 0.05;
        mat(3, i) = 0.2;
        x += step;
    }

    Graphics G(true);

    G.setTextSize(10)
    .setTitle("Random plot with Sample Data x^2_{fancy code}")
    .setLegendPosition(true, true)
    .add(mat.row(0), mat.row(1), mat.row(2), {}, "m1")
    .setLogScaleY()
    .plot();

    usleep(2e6);

    G.add(mat * 2, "m2", false, "orange", 3, true)
    // .add(mat * 2, "m2")
    .plot();

    usleep(2e6);

    G.restart(true)
    .setXRange(-10, 10)
    .setTitle("Energy vs. Time for Sample Data")
    .setYTics(4)
    .setYInterval(10)
    .setGrid(true, true)
    .add(mat, "m2", false, "")
    .plot();

    G.restart(true)
    .setXRange(-10, 10)
    .setTitle("Energy vs. Time for Sample Data")
    .add("stats_AH1.out", 1, 2)
    .add("14+0.1*sin(x)", "f(x)", "red", 0, 100)
    .plot();
}

int main()
{
    plot();
    for(unsigned i = 0; i < 100000; ++i)
        // for(unsigned j = 0; j < 50000; ++j)
            // i + j;

    return 0;
}