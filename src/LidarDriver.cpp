#include "../include/LidarDriver.h"
#include <vector>

unsigned uround(double num) {
    return (num - (unsigned) num) > .5 ? (unsigned) ++num : (unsigned) num;
}

void LidarDriver::new_scan(const std::vector<double> &data) {

	for (unsigned i = 0; i < lineLen; i++) { // fill row
		buf[lineLen * bufNextLine + i] = (i < data.size()) ? data[i] : .0;
	}

	bufNextLine = (bufNextLine + 1) % BUFFER_DIM; // go to next buffer line

	if (bufUsedRows < BUFFER_DIM) {
		++bufUsedRows;
	}
}

std::vector<double> LidarDriver::get_scan() {

    std::vector<double> out(lineLen);
    if (!bufUsedRows) {  // Returns a -1 filled vector when no row is available
        for (int i = 0; i < lineLen; i++) {
            out[i] = -1.0;
        }
        return out;
    }

    unsigned start_index = bufUsedRows > bufNextLine
        ? bufNextLine + (BUFFER_DIM - bufUsedRows) - 1
        : (bufNextLine - bufUsedRows) * lineLen;

    for (int i = 0; i < lineLen; i++) {
        out[i] = buf[start_index + i];
    }

    bufUsedRows--;

    return out;
}

void LidarDriver::clear_buffer() {}

double LidarDriver::get_distance(const double deg) const {

    if (!bufNextLine && !bufUsedRows ) return -1;

    unsigned offset = uround(deg/resolution);

    unsigned row = !bufNextLine ? BUFFER_DIM - 1 : bufNextLine - 1;
    unsigned index = row * lineLen + offset;

    return buf[index];
}

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar) {}
