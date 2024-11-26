#include "../include/LidarDriver.h"

// stuff to implement

void LidarDriver::new_scan(const std::vector<double> &data) {}

std::vector<double> LidarDriver::get_scan() {}

void LidarDriver::clear_buffer() {}

double LidarDriver::get_distance(const double deg) const {}

std::ostream &operator<<(std::ostream &os, LidarDriver &lidar) {}
