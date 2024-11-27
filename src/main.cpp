#include "../include/LidarDriver.h"
#include <iostream>
#include <vector>

std::vector<double> test_line(unsigned len) { // the 1st item is how many lines were created until the function was called, the others match their angle
	std::vector<double> vec(len);
	srand((unsigned) time(NULL));

	for (int i = 0; i < len; i++) {
		// vec[i] = i * 180.0 / (len - 1);
		vec[i] = (double) (rand() % 100) / 10;
	}

	return vec;
}

void pb(LidarDriver lidar, unsigned lineLen) {
    
    double *buf = lidar.getBuf();

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

	std::vector<double> first_scan = test_line(lineLen);
	lidar.new_scan(first_scan);

	// Chcking for correct extraction of the last value
	std::vector<double> t = lidar.get_scan();
	for (int i = 0; i < t.size(); i++) {
	    std::cout << t[i];
	    if (t[i] != first_scan[i]) {
			std::cout << "[!] Extraction failed!\n";
	    }
	}

	std::cout << std::endl;

	std::vector<double> second_scan = test_line(lineLen);
	lidar.new_scan(second_scan);

    pb(lidar, lineLen);

	for (int i = 0; i < 3; i++) { // 10 is the buffer dim, maybe it's a good idea to write a getter for it?
		std::vector<double> diocancaro = test_line(lineLen);
      
      lidar.new_scan(diocancaro);
	}

    pb(lidar, lineLen);


	t = lidar.get_scan();
	for (int i = 0; i < t.size(); i++) {
	    std::cout << t[i] << " , " << second_scan[i]<< '\n';
	    if (t[i] != second_scan[i]) {
			// std::cout << "[!] Extraction failed!\n";
	    }
	}



}
