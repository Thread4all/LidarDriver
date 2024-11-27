#include "../include/LidarDriver.h"
#include <vector>

unsigned uround(double num) {
	return (num - (unsigned)num) > .5 ? (unsigned)++num : (unsigned)num;
}

void LidarDriver::new_scan(const std::vector<double> &data) {

	for (unsigned i = 0; i < lineLen; i++) { // fill row, eventually padding with 0s if the input is too short
		buf[lineLen * bufNextLine + i] = (i < data.size()) ? data[i] : .0;
	}

	bufNextLine = (bufNextLine + 1) % BUFFER_DIM; // go to next buffer line

	if (bufUsedLines < BUFFER_DIM) {
		++bufUsedLines;
	}
}

std::vector<double> LidarDriver::get_scan() {

	std::vector<double> out(lineLen);

	if (!bufUsedLines) { // Returns a -1 filled vector when no row is available
		for (int i = 0; i < lineLen; i++) {
			out[i] = -1.0;
		}
		return out;
	}

	unsigned startIndex = (bufUsedLines > bufNextLine ? bufNextLine - bufUsedLines + BUFFER_DIM : (bufNextLine - bufUsedLines)) * lineLen;

	for (int i = 0; i < lineLen; i++) {
		out[i] = buf[startIndex + i];
	}

	bufUsedLines--;

	return out;
}

void LidarDriver::clear_buffer() { // resetting just the indexes is enough to mark the whole buffer as empty
	bufNextLine = 0;
	bufUsedLines = 0;
}

double LidarDriver::get_distance(const double deg) const {

	if (!bufUsedLines) return -1;

	unsigned offset = uround(deg / resolution);

	unsigned row = !bufNextLine ? BUFFER_DIM - 1 : bufNextLine - 1;
	unsigned index = row * lineLen + offset;

	return buf[index];
}

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar) {

	if (!lidar.bufUsedLines) {
		os << "\x1b[34m[empty buffer]\x1b[0m";
		return os;
	}

	unsigned row = lidar.bufNextLine ? lidar.bufNextLine - 1 : LidarDriver::BUFFER_DIM - 1;
	unsigned index = row * lidar.lineLen;

	os << "[ ";
	for (int i = 0; i < lidar.lineLen - 1; i++) {
		os << lidar.buf[index + i] << ", ";
	}
	os << lidar.buf[index + lidar.lineLen - 1];
	os << " ]";

	return os;
}

void LidarDriver::print_buffer() const {

	for (int i = 0; i < BUFFER_DIM; i++) {

		for (int j = 0; j < lineLen; j++) {
			std::cout << buf[i * lineLen + j] << " ";
		}
		if (i == bufNextLine) {
			std::cout << "\x1b[33mNEXT\x1b[0m "; // gets printed after the line that will get written on upon the next invocation of get_scan()
		}
		if (i == (bufNextLine - bufUsedLines + BUFFER_DIM) % BUFFER_DIM) {
			std::cout << "\x1b[33mLAST\x1b[0m"; // gets printed after the oldest line in the buffer
		}
		std::cout << "\n";
	}
}