#include "../include/LidarDriver.h"

void LidarDriver::new_scan(const std::vector<double> &data) {

	for (unsigned i = 0; i < lineLen; i++) { // fill row
		buf[lineLen * bufNextLine + i] = (i < data.size()) ? data[i] : .0;
	}

	bufNextLine = (bufNextLine + 1) % BUFFER_DIM; // go to next buffer line

	if (bufUsedRows < BUFFER_DIM) {
		++bufUsedRows;
	}
}

std::vector<double> LidarDriver::get_scan() {}

void LidarDriver::clear_buffer() {}

double LidarDriver::get_distance(const double deg) const {}

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar) {}
