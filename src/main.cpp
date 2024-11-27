#include "../include/LidarDriver.h"
#include <random>
#include <string>

std::random_device rnd;
std::mt19937 gen(rnd());
std::uniform_real_distribution<double> dist(0., 10.);

std::vector<double> random_line(unsigned len) { // returns len random values
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

std::string vtos(const std::vector<double> &vec) { // vector to string following the same cursed notation as atoi et cetera

	if (!vec.size()) {
		return "[]";
	}

	std::string res = "[ ";

	for (int i = 0; i < vec.size() - 1; i++) {
		res += std::to_string(vec[i]) + ", ";
	}
	res += std::to_string(vec[vec.size() - 1]) + " ]";

	return res;
}

int main(int argc, char **argv) {

	if (argc > 1 && std::string(argv[1]) == "repl") { // repl mode, activated by running the executable passing "repl" as an argument

		double precision;
		std::cout << "precision for lidar: ";
		std::cin >> precision;

		int lineLen = (int)(180 / precision) + 1;
		LidarDriver lidar(precision);
		std::string cmd;

		while (1) {
			std::cout << "\n> ";
			std::cin >> cmd;

			if (cmd == "h" || cmd == "help") {
				std::cout << "h, help   : prints this\n";
				std::cout << "w         : write line in buffer              | calls \x1b[34mnew_scan()\x1b[0m\n";
				std::cout << "r         : get line from buffer              | calls \x1b[34mget_scan()\x1b[0m\n";
				std::cout << "o         : print latest line from buffer     | calls \x1b[34mstd::cout << lidar\x1b[0m\n";
				std::cout << "c         : clear buffer                      | calls \x1b[34mclear_buffer()\x1b[0m\n";
				std::cout << "d <angle> : read last buffer line at <angle>  | calls \x1b[34mget_distance(<angle>)\x1b[0m\n";
				std::cout << "b         : prints the whole buffer           | just for debugging\n";
				std::cout << "q, quit   : quit\n";

			} else if (cmd == "w") {
				std::vector<double> vec = test_line(lineLen);
				lidar.new_scan(vec);
				std::cout << vtos(vec) << "\nadded to buffer\n";

			} else if (cmd == "r") {
				std::cout << vtos(lidar.get_scan()) << "\n";

			} else if (cmd == "o") {
				std::cout << lidar << "\n";

			} else if (cmd == "c") {
				lidar.clear_buffer();
				std::cout << "buffer cleared\n";

			} else if (cmd == "d") {

				std::cin >> cmd; // recycle cmd string to contain the angle
				std::cout << "\n"
						  << std::to_string(lidar.get_distance(std::stod(cmd))) << "\n";

			} else if (cmd == "b") {
				lidar.print_buffer();
				std::cout << "\n";

			} else if (cmd == "q" || cmd == "quit") {
				return 0;

			} else {
				std::cout << "unknown command\n";
			}
		}
	}

	// if the program was not run in repl mode, do standard tests

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
	lidar.print_buffer();

	for (int i = 0; i < 3; i++) {
		std::vector<double> v = random_line(lineLen);
		lidar.new_scan(v);
	}

	std::cout << "buffer after 3 more scans:\n";
	lidar.print_buffer();

	first_scan_readback = lidar.get_scan();

	for (int i = 0; i < first_scan_readback.size(); i++) {
		std::cout << first_scan_readback[i] << " , " << second_scan[i] << '\n';
		if (first_scan_readback[i] != second_scan[i]) {
			std::cout << "\t-> [!] Mismatch found!\n";
		}
	}

	first_scan = test_line(lineLen);
	lidar.clear_buffer();
	lidar.new_scan(first_scan);

	// Retreiveing last scan's value at position n => d = precision * n [deg]
	unsigned n = 3;
	double d = precision * n;
	double expected = first_scan[n];
	double offsets[3] = {0, precision / 3, -precision / 3};

	std::cout << '\n';

	for (int i = 0; i < 3; i++) {
		if (lidar.get_distance(d + offsets[i]) != expected) {
			std::cout << "[!] get_distance failed! (offset: " << offsets[i] << ")\n";
			continue;
		}
		std::cout << "[+] get_distance OK with offset: " << offsets[i] << '\n';
	}
}
