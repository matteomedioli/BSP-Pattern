/**
 * \file Utimer.cpp
 * \author Matteo Medioli
 */

#include "../include/utimer.hpp"


Utimer::Utimer (const std::string m, int v) : message(m),verbose(v) {
	start = std::chrono::high_resolution_clock::now();
}

Utimer::~Utimer () {
	stop = std::chrono::high_resolution_clock::now();
	elapsed_time = stop - start;
	auto usec = std::chrono::duration_cast<usecs>(elapsed_time).count();
	auto msec = std::chrono::duration_cast<msecs>(elapsed_time).count();
	if(verbose)
		std::cout << message << " computed in " << usec << " usec  (" << msec << " milliseconds)" << std::endl;
	else
	{
		if(message=="TISKIN:")
			std::cout << msec <<std::endl;
		else 
			std::cout << msec << ",";
	}
}