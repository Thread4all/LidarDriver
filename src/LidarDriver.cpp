#include "../include/LidarDriver.h"
#include <vector>

unsigned uround(double num) {
	return (num - (unsigned)num) > .5 ? (unsigned)++num : (unsigned)num;
}

void LidarDriver::new_scan(const std::vector<double> &data) {

	for (unsigned i = 0; i < lineLen; i++) { // fill row
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

	unsigned start_index = (bufUsedLines > bufNextLine ? bufNextLine - bufUsedLines + BUFFER_DIM : (bufNextLine - bufUsedLines)) * lineLen;
	// could achieve the same with: unsigned start_index = lineLen * ((bufNextLine - bufUsedRows + BUFFER_DIM) % BUFFER_DIM)

	for (int i = 0; i < lineLen; i++) {
		out[i] = buf[start_index + i];
	}

	bufUsedLines--;

	return out;
}

void LidarDriver::clear_buffer() {}

double LidarDriver::get_distance(const double deg) const {

	if (!bufNextLine && !bufUsedLines) return -1; // --!-- fails if i call new_scan(), followed by get_scan(), followed by get(distance) (reads on deleted 1st line)

	unsigned offset = uround(deg / resolution);

	unsigned row = !bufNextLine ? BUFFER_DIM - 1 : bufNextLine - 1;
	unsigned index = row * lineLen + offset;

	return buf[index];
}

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar) {}
