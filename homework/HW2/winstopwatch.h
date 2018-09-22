// winstopwatch.h
// Joe Knox
// Created: 2010-2-18
// Updated: 2012-1-26
// High-resolution stopwatch class for benchmarking code; Windows-only

#ifndef WINSTOPWATCH_H
#define WINSTOPWATCH_H

#include <windows.h>

class WinStopWatch {
public:
	// default ctor
	// Creates a blank StopWatch and sets the frequency
	WinStopWatch() {
		startTime.QuadPart = 0;
		stopTime.QuadPart = 0;
		QueryPerformanceFrequency(&frequency);
	}

	// function start
	// Sets the start time
	void start() { QueryPerformanceCounter(&startTime); }

	// function stop
	// Sets the stop time
	void stop() { QueryPerformanceCounter(&stopTime); }

	// function clear
	// Clears both the start and stop time
	void clear() {
		startTime.QuadPart = 0;
		stopTime.QuadPart = 0;
	}

	// function time
	// Returns the difference of the stop and start times
	// Pre: start and stop have each been called at least once
	double time() { return ((stopTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart); }

private:
	LARGE_INTEGER startTime, stopTime, frequency;
};

#endif // WINSTOPWATCH_H