#include "../include/LidarDriver.h"
#include <random>
#include <string>

std::random_device rnd;
std::mt19937 gen(rnd());
std::uniform_real_distribution<double> dist(0., 10.);

std::vector<double> random_line(unsigned len) {
	std::vector<double> res(len);

	for (int i = 0; i < len; i++) {
		res[i] = dist(gen);
	}

	return res;
}

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

void print_buffer(LidarDriver &lidar, unsigned lineLen) {

	double *buf = lidar.get_buf();

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < lineLen; j++) {
			std::cout << buf[i * lineLen + j] << " ";
		}
		std::cout << "\n";
	}
	std::cout << "\n\n";
}

int main(int argc, char **argv) {

	double precision = 10;
	unsigned lineLen = (int)(180 / precision) + 1;
	LidarDriver lidar(precision);

	std::vector<double> first_scan = random_line(lineLen);
	lidar.new_scan(first_scan);
	std::vector<double> first_scan_readback = lidar.get_scan();

	// Checking for correct extraction of the last value
	for (int i = 0; i < first_scan_readback.size(); i++) {
		std::cout << "wrote: " << first_scan[i] << ", read back: " << first_scan_readback[i] << "\n";
	}

	std::cout << "\n";

	std::vector<double> second_scan = random_line(lineLen);
	lidar.new_scan(second_scan);

	std::cout << "buffer after 2nd scan:\n";
	print_buffer(lidar, lineLen);

	for (int i = 0; i < 3; i++) {
		std::vector<double> diocancaro = random_line(lineLen);
		lidar.new_scan(diocancaro);
	}

	std::cout << "buffer after 3 more scans:\n";
	print_buffer(lidar, lineLen);

	first_scan_readback = lidar.get_scan();

	for (int i = 0; i < first_scan_readback.size(); i++) {
		std::cout << first_scan_readback[i] << " , " << second_scan[i] << '\n';
		if (first_scan_readback[i] != second_scan[i]) {
			// std::cout << "[!] Extraction failed!\n";
		}
	}
}
