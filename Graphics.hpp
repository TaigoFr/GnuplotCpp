#pragma once

#include "gnuplot-iostream.h"
#include <Eigen/Dense>

class Graphics
{
public:
	// persist: whether or not to keep window open after object is destroyed
	Graphics(bool persist = true);
	~Graphics();

	Graphics& plot();
	Graphics& restart(bool newWindow = true, bool persist = true);

	// 'setWindowSize' not working: when window size is set, font sizes go all
	// to default in CSD3, version 4.6 of gnuplot didn't have that problem
	// mine (5.2.2) does
	Graphics& setWindowSize(unsigned width, unsigned height);
	Graphics& setXRange(double min, double max);
	Graphics& setYRange(double min, double max);
	Graphics& setXLabel(const std::string& label);
	Graphics& setYLabel(const std::string& label);
	Graphics& setXTics(unsigned n);
	Graphics& setYTics(unsigned n);
	Graphics& setXInterval(float interval);
	Graphics& setYInterval(float interval);
	Graphics& setLogScaleX();
	Graphics& setLogScaleY();
	Graphics& setTitle(const std::string& title);
	Graphics& setGrid(bool x, bool y,
	                  const std::string& color = "red"); //also allows hexadecimal color "#880000"
	Graphics& setLegendPosition(bool left = false, bool bottom = false);

	Graphics& setBMargin(float margin);
	Graphics& setUMargin(float margin);
	Graphics& setLMargin(float margin);
	Graphics& setRMargin(float margin);

	Graphics& setTextOffset(float off_x, float off_y);
	Graphics& setTextFont(const std::string& font);
	Graphics& setTextSize(unsigned size);

	Graphics& plotTo2ndAxis(bool yes, float ytics = -1.f);

	Graphics& add(const std::string file_path, unsigned col_x, unsigned col_y,
	              int col_ex = -1, int col_ey = -1,
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              bool if_lines_also_points = false,
	              bool use_x_errors_not_asymetric_y = true);

	Graphics& add(const std::string& function,
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              unsigned Npoints = 0,
	              const std::string& var = "x");

	Graphics& add(const std::vector<std::vector<double>>& data,
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              bool if_lines_also_points = false,
	              bool use_x_errors_not_asymetric_y = true);

	Graphics& add(const std::vector<double>& data_x,
	              const std::vector<double>& data_y,
	              const std::vector<double>& data_ex = {},
	              const std::vector<double>& data_ey = {},
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              bool if_lines_also_points = false,
	              bool use_x_errors_not_asymetric_y = true);

	Graphics& add(const Eigen::MatrixXd& data,
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              bool if_lines_also_points = false,
	              bool use_x_errors_not_asymetric_y = true);
	Graphics& add(const Eigen::VectorXd& data_x,
	              const Eigen::VectorXd& data_y,
	              const Eigen::VectorXd& data_ex,
	              const Eigen::VectorXd& data_ey,
	              const std::string& legend = "",
	              const std::string& color = "",
	              unsigned size = 0,
	              bool if_lines_also_points = false,
	              bool use_x_errors_not_asymetric_y = true);

	// write raw to GNUPLOT
	Graphics& write(const std::string& str, bool newLine = true);

private:
	Gnuplot* gp;
	unsigned add_count;
	bool user_scaled;

	float text_offset_x;
	float text_offset_y;
	std::string text_font;
	unsigned text_size;

	bool plottingTo2ndAxis;

private:
	// assumes 'mat' is organised by columns
	void clear();
	static std::vector<std::vector<double>> EigenMatrixToSTDVector(const Eigen::MatrixXd mat);
	static std::vector<double> 				EigenVectorToSTDVector(const Eigen::VectorXd vec);

	Graphics& setAutoscale();
	Graphics& setLegend();

	Graphics& post_add(bool ex, bool ey, const std::string& legend, const std::string& color,
	                   unsigned size, bool if_lines_also_points);
};