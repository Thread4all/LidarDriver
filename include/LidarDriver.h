#ifndef LIDARDRIVER_H
#define LIDARDRIVER_H

#include <iostream>
#include <vector>

class LidarDriver {
	static constexpr unsigned BUFFER_DIM = 10; // random value arbitrarily chosen by asking god
	const double resolution;				   // angle between readings, in degrees
	const unsigned lineLen;					   // values per lidar sweep (can be calculated from resolution, but it's used very often and requires a division)
	unsigned bufNextLine = 0;				   // next buffer line to write data into
	unsigned bufUsedLines = 0;				   // how many rows of the buffer contain values yet to be consumed
	double *buf;

public:
	LidarDriver(const double res) : resolution{res > 0 ? res : throw std::invalid_argument("resolution field in LidarDriver constructor expects a positive value")},
									lineLen{(unsigned)(180 / resolution) + 1},
									buf{new double[lineLen * BUFFER_DIM]} {};
	~LidarDriver() { delete[] buf; };

	void new_scan(const std::vector<double> &data);
	std::vector<double> get_scan();
	void clear_buffer();
	double get_distance(const double deg) const;

	const unsigned get_buf_dim() const { return BUFFER_DIM; }
	// cursed and unsafe getters that must absolutely be removed
	const double *get_buf() const { return buf; }
	const unsigned get_next_line_index() const { return bufNextLine; }
	const unsigned get_last_line_index() const { return (bufNextLine - bufUsedLines + BUFFER_DIM) % BUFFER_DIM; }

	// it makes no sense to declare getters for just this one helper function to use; it's easier to have it be a friend
	friend std::ostream &operator<<(std::ostream &os, LidarDriver &lidar);
};

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar);

#endif