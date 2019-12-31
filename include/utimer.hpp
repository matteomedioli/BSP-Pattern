/**
 * \file utimer.cpp
 * \author Matteo Medioli
 */

#ifndef U_TIMER
#define U_TIMER

#include <iostream>
#include <chrono>
#include <mutex>

class utimer {
	private:
		using usecs		= std::chrono::microseconds;
		using msecs		= std::chrono::milliseconds;
		using timepoint	= std::chrono::system_clock::time_point;
		timepoint start;
		timepoint stop;
		std::chrono::duration<double> elapsed_time;
		std::string message;
		
	public:
		utimer (const std::string m);
		~utimer ();
};

#endif

