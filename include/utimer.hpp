/**
 * \file utimer.hpp
 * \author Matteo Medioli
 */

#ifndef UTIMER_H
#define UTIMER_H

#include <iostream>
#include <chrono>
#include <mutex>

class Utimer {
	private:
		using usecs		= std::chrono::microseconds;
		using msecs		= std::chrono::milliseconds;
		using timepoint	= std::chrono::system_clock::time_point;
		timepoint start;
		timepoint stop;
		std::chrono::duration<double> elapsed_time;
		std::string message;
		
	public:
		Utimer (const std::string m);
		~Utimer ();
};

#endif

