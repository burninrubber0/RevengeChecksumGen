#pragma once

#include <binary-io/data-stream.h>

#include <cstdint>

// MC02 header used on EA Xbox 360 games
class MC02
{
public:
	void read(DataStream& file);
	void write(DataStream& buffer);

private:
	void init(DataStream& buffer);
	uint32_t calcSignature(DataStream& buffer, int32_t offset, uint32_t length);
	uint32_t crc32(uint8_t* data, uint32_t length);

	uint32_t fileHeaderVersion = 0;
	uint32_t fileSize = 0;
	uint32_t userHeaderSize = 0;
	uint32_t userBodySize = 0;
	uint32_t userHeaderSignature = 0;
	uint32_t userBodySignature = 0;
	uint32_t fileHeaderSignature = 0;

	static const uint32_t table[256];
};
