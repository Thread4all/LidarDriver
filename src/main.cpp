#include "../include/LidarDriver.h"

std::vector<double> test_line(unsigned len) { // the 1st item is how many lines were created until the function was called, the others match their angle
	static int index = 0;
	std::vector<double> vec(len);
	vec[0] = index;

	for (int i = 1; i < len; i++) {
		vec[i] = i * 180.0 / (len - 1);
	}

	index++;
	return vec;
}

int main(int argc, char **argv) {

	double precision = 0.01;
	unsigned lineLen = (int)(180 / precision) + 1;
	LidarDriver lidar(precision);

	// fill buffer twice (??)
	for (int i = 0; i < 10; i++) { // 10 is the buffer dim, maybe it's a good idea to write a getter for it?
		lidar.new_scan(test_line(lineLen));
	}

	// no way to print stuff yet, so we need to have trust

	return 0;
}