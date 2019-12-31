/**
 * \file utimer.cpp
 * \author Matteo Medioli
 */

#include <utimer.hpp>


utimer::utimer (const std::string m) : message(m) {
	start		= std::chrono::high_resolution_clock::now();
}

utimer::~utimer () {
	stop		= std::chrono::high_resolution_clock::now();
	elapsed_time	= stop - start;
	auto usec	= std::chrono::duration_cast<usecs>(elapsed_time).count();
	auto msec	= std::chrono::duration_cast<msecs>(elapsed_time).count();
	
	std::cout << message << " computed in " << usec << " usec  (" << msec << " milliseconds)" << std::endl;
}