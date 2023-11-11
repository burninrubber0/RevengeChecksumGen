#pragma once

#include <binary-io/data-stream.h>

#include <cstdint>

class ChecksumGen
{
public:
	ChecksumGen(int argc, char* argv[]);

	int result = 0;

private:
	typedef DataStream::Platform Platform;

	uint saveDataSize = 0; // Set at runtime
	Platform platform = Platform::PC; // Set at runtime
	QFile file;
	DataStream stream;
	bool profilesUsed[3] = { false, false, false };

	uint calcChecksum(int profileIndex);
	int validate(int argc, char* argv[]);
	void printUsage();
};
