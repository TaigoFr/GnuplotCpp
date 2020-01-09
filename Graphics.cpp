#include "Graphics.hpp"
#include <sstream> //stringstream

Graphics::Graphics(bool persist): gp(nullptr)
{
	restart(true, persist);
}
Graphics::~Graphics()
{
	clear();
}

void Graphics::clear()
{
	if (gp != nullptr) {
		delete gp;
		gp = nullptr;
	}
}

Graphics& Graphics::restart(bool newWindow, bool persist)
{
	if (!newWindow) {
		add_count = 0;
		return *this;
	}

	clear();

	if (gp == nullptr) {
		gp = new Gnuplot(persist ? "gnuplot -persist" : "gnuplot");
		add_count = 0;
		user_scaled = false;
		text_offset_x = 0;
		text_offset_y = 0;
		text_font = "Verdana";
		text_size = 14;
		plottingTo2ndAxis = false;
		setLegend();
	}

	return *this;
}

Graphics& Graphics::write(const std::string& str, bool newLine)
{
	*gp << str;
	if(newLine)
		*gp << std::endl;
	return *this;
}

Graphics& Graphics::setWindowSize(unsigned width, unsigned height)
{
	*gp << "set term x11 size " << width << "," << height << std::endl;
	return *this;
}


Graphics& Graphics::setXRange(double min, double max)
{
	user_scaled = true;
	*gp << "set xrange [" << min << ":" << max << "]" << std::endl;
	return *this;
}

Graphics& Graphics::setYRange(double min, double max)
{
	user_scaled = true;

	*gp << "set y" << (plottingTo2ndAxis ? "2" : "") << "range [" << min << ":" << max << "]" <<
	    std::endl;

	return *this;
}

Graphics& Graphics::setAutoscale()
{
	user_scaled = true;
	*gp << "set autoscale xy" << std::endl;
	return *this;
}

Graphics& Graphics::setXLabel(const std::string& label)
{
	*gp << "set xlabel '" << label << "' font '" << text_font << "," << text_size
	    << "' offset " << text_offset_x << "," << text_offset_y << std::endl;
	return *this;
}

Graphics& Graphics::setYLabel(const std::string& label)
{
	*gp << "set y" << (plottingTo2ndAxis ? "2" : "") << "label '" << label << "' font '" << text_font <<
	    "," <<
	    text_size
	    << "' offset " << text_offset_x << "," << text_offset_y << std::endl;
	return *this;
}

Graphics& Graphics::setXTics(unsigned n)
{
	*gp << "set mxtics " << n << std::endl;
	return *this;
}
Graphics& Graphics::setYTics(unsigned n)
{
	*gp << "set my" << (plottingTo2ndAxis ? "2" : "") << "tics " << n << std::endl;
	return *this;
}

Graphics& Graphics::setXInterval(float interval)
{
	*gp << "set xtics " << interval << std::endl;
	return *this;
}
Graphics& Graphics::setYInterval(float interval)
{
	*gp << "set y" << (plottingTo2ndAxis ? "2" : "") << "tics " << interval << std::endl;
	return *this;
}

Graphics& Graphics::setGrid(bool x, bool y, const std::string& color)
{
	assert(x || y);

	*gp << "set grid " << (x ? "xtics " : "") << (y ? "ytics " : "") << "lt 0 lw 1 lc rgb '" << color <<
	    "'" << std::endl;
	return *this;
}

Graphics& Graphics::setTitle(const std::string& title)
{
	*gp << "set title '" << title << "' font '" << text_font << "," << text_size
	    << "' offset " << text_offset_x << "," << text_offset_y << std::endl;
	return *this;
}

Graphics& Graphics::setTextOffset(float off_x, float off_y)
{
	text_offset_x = off_x;
	text_offset_y = off_y;
	return *this;
}
Graphics& Graphics::setTextFont(const std::string& font)
{
	text_font = font;
	setLegend();
	return *this;
}
Graphics& Graphics::setTextSize(unsigned size)
{
	text_size = size;
	setLegend();
	return *this;
}

Graphics& Graphics::setLogScaleX()
{
	*gp << "set logscale x" << std::endl;
	return *this;
}
Graphics& Graphics::setLogScaleY()
{
	*gp << "set logscale y" << (plottingTo2ndAxis ? "2" : "") << std::endl;
	return *this;
}

Graphics& Graphics::setBMargin(float margin)
{
	*gp << "set bmargin " << margin << std::endl;
	return *this;
}
Graphics& Graphics::setUMargin(float margin)
{
	*gp << "set umargin " << margin << std::endl;
	return *this;
}
Graphics& Graphics::setLMargin(float margin)
{
	*gp << "set lmargin " << margin << std::endl;
	return *this;
}
Graphics& Graphics::setRMargin(float margin)
{
	*gp << "set rmargin " << margin << std::endl;
	return *this;
}

Graphics& Graphics::setLegend()
{
	*gp << "set key font '" << text_font << "," << text_size << "'" << std::endl;
	*gp << "set xtics font '" << text_font << "," << text_size << "'" << std::endl;
	*gp << "set ytics font '" << text_font << "," << text_size << "'" << std::endl;
	*gp << "set y2tics font '" << text_font << "," << text_size << "'" << std::endl; // do the 2nd as well just in case, it doesn't hurt
	return *this;
}

Graphics& Graphics::setLegendPosition(bool left, bool bottom)
{
	*gp << "set key " << (left ? "left" : "right") << " " << (bottom ? "bottom" : "top") << std::endl;
	return *this;
}

Graphics& Graphics::plotTo2ndAxis(bool yes, float ytics)
{
	plottingTo2ndAxis = yes;

	if (!yes)
		return *this;

	if (ytics > 0) {
		*gp << "set ytics nomirror" << std::endl;
		*gp << "set y2tics " << ytics << std::endl;
	}

	return *this;
}

Graphics& Graphics::plot()
{
	*gp << std::endl;
	return *this;
}


Graphics& Graphics::add(const std::string file_path, unsigned col_x, unsigned col_y,
                        int col_ex, int col_ey,
                        const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        bool if_lines_also_points,
                        bool use_x_errors_not_asymetric_y)
{
	if (!user_scaled)
		setAutoscale();

	std::string plot = (add_count > 0 ? "replot " : "plot ");
	*gp << plot << "'" <<  file_path << "' u " << col_x << ":" << col_y;

	bool ex = use_x_errors_not_asymetric_y && col_ex > 0;
	bool ey = col_ey > 0;

	if (col_ex > 0) {
		if (ex)
			*gp << ":" << col_ex;
		else
			*gp << ":($" << col_y << "-" << "$" << col_ex << ")";
	}

	if (col_ey > 0) {
		if (ex)
			*gp << ":" << col_ey;
		else
			*gp << ":($" << col_y << "+" << "$" << col_ey << ")";
	}


	return post_add(ex, ey, legend, color, size, if_lines_also_points);
}

Graphics& Graphics::add(const std::vector<std::vector<double>>& data, const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        bool if_lines_also_points,
                        bool use_x_errors_not_asymetric_y)
{
	if (!user_scaled)
		setAutoscale();

	if (data.size() == 0) {
		std::cout << "Data added is empty." << std::endl;
		return *this;
	}

	bool ex = use_x_errors_not_asymetric_y && data[0].size() == 4;
	bool ey = data[0].size() >= 3;

	std::string plot = (add_count > 0 ? "replot " : "plot ");

	if (!use_x_errors_not_asymetric_y && data[0].size() == 4) {
		auto data2 = data;

		for (int i = 0; i < data.size(); ++i) {
			data2[i][2] = data[i][1] - data[i][2]; // y- = y - ey-
			data2[i][3] = data[i][1] + data[i][3]; // y+ = y + ey+
		}

		// *gp << plot << gp->binFile1d(data2, "record");
		*gp << plot << gp->file1d(data2);
	}
	else
		// *gp << plot << gp->binFile1d(data, "record");
		*gp << plot << gp->file1d(data);


	return post_add(ex, ey, legend, color, size, if_lines_also_points);
}


Graphics& Graphics::add(const std::string& function,
                        const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        unsigned Npoints,
                        const std::string& var)
{
	if (!user_scaled)
		setAutoscale();

	std::stringstream ss;
	ss << "f" << add_count << "(" << var << ")";
	*gp << ss.str() << " = " << function << std::endl;

	if (Npoints > 0)
		*gp << "set samples " << Npoints << std::endl;

	std::string plot = (add_count > 0 ? "replot " : "plot ");
	*gp << plot << ss.str();

	return post_add(false, false, legend, color, size, Npoints > 0);
}

Graphics& Graphics::post_add(bool ex, bool ey, const std::string& legend, const std::string& color,
                             unsigned size, bool if_lines_also_points)
{
	if (size > 0)
		*gp << " w l" << (if_lines_also_points ? "p" : "") << " lw " << size << " ";

	if (if_lines_also_points) {
		if (size == 0)
			*gp << "w p";

		*gp << " pt 7 ps 0.5 ";
	}

	if (ex && size == 0) {
		if (ey)
			*gp << " with xyerrorbars ";
		else
			*gp << " with xerrorbars ";
	}
	else if (ey && size == 0)
		*gp << " with yerrorbars ";

	if (color != "")
		*gp << " lc rgb '" << color << "'";

	/*
		auto leg = legend;

		if (leg == "") {
			std::stringstream ss;
			ss << "F" << add_count;
			leg = ss.str();
		}

		*gp	<< " title '" << leg << "'" << std::endl;
	*/
	if (legend == "")
		*gp << " notitle ";
	else
		*gp << " title '" << legend << "' ";

	if (plottingTo2ndAxis)
		*gp << " axis x1y2 ";

	*gp << std::endl;

	++add_count;
	return *this;
}


Graphics& Graphics::add(const std::vector<double>& data_x, const std::vector<double>& data_y,
                        const std::vector<double>& data_ex, const std::vector<double>& data_ey,
                        const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        bool if_lines_also_points,
                        bool use_x_errors_not_asymetric_y)
{
	bool has_ex = (data_ex.size() > 0);
	bool has_ey = (data_ey.size() > 0);

	assert(data_x.size() == data_y.size() &&
	       (!has_ex || data_ex.size() == data_x.size()) &&
	       (!has_ey || data_ey.size() == data_y.size())
	      );

	// make transposed "matrix"
	std::vector<std::vector<double>> data(data_x.size(), std::vector<double>(2 + has_ex + 1));

	for (int i = 0; i < data.size(); ++i) {
		data[i][0] = data_x[i];
		data[i][1] = data_y[i];

		if (has_ex)
			data[i][2] = data_ex[i];

		if (has_ey)
			data[i][2 + has_ex] = data_ey[i];
		else
			data[i][2 + has_ex] = 0;
	}

	return add(data, legend, color, size, if_lines_also_points, use_x_errors_not_asymetric_y);
}
Graphics& Graphics::add(const Eigen::MatrixXd& data, const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        bool if_lines_also_points,
                        bool use_x_errors_not_asymetric_y)
{
	return add(EigenMatrixToSTDVector(data), legend, color, size, if_lines_also_points,
	           use_x_errors_not_asymetric_y);
}
Graphics& Graphics::add(const Eigen::VectorXd& data_x, const Eigen::VectorXd& data_y,
                        const Eigen::VectorXd& data_ex, const Eigen::VectorXd& data_ey,
                        const std::string& legend,
                        const std::string& color,
                        unsigned size,
                        bool if_lines_also_points,
                        bool use_x_errors_not_asymetric_y)
{
	return add(EigenVectorToSTDVector(data_x), EigenVectorToSTDVector(data_y),
	           EigenVectorToSTDVector(data_ex), EigenVectorToSTDVector(data_ey),
	           legend, color, size, if_lines_also_points, use_x_errors_not_asymetric_y);
}

std::vector<std::vector<double>> Graphics::EigenMatrixToSTDVector(const Eigen::MatrixXd mat)
{
	std::vector<std::vector<double>> out(mat.cols());

	for (int i = 0; i < out.size(); ++i) {
		auto start = mat.data() + i * mat.rows();
		out[i] = std::vector<double>(start, start + mat.rows());
	}

	return out;
}
std::vector<double> Graphics::EigenVectorToSTDVector(const Eigen::VectorXd vec)
{
	return std::vector<double>(vec.data(), vec.data() + vec.size());
}
