/**
 * \file utimer.hpp
 * \author Matteo Medioli
 */

#ifndef UTIMER_H
#define UTIMER_H
#include <chrono>
#include <string>
#include <iostream>

class Utimer {
	private:
		using usecs		= std::chrono::microseconds;
		using msecs		= std::chrono::milliseconds;
		using timepoint	= std::chrono::system_clock::time_point;
		timepoint start;
		timepoint stop;
		std::chrono::duration<double> elapsed_time;
		std::string message;
		int verbose;
		
	public:
		Utimer (const std::string m, int verbose);
		~Utimer ();
};

#endif

