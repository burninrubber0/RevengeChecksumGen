#include <checksum.h>
#include <mc02.h>

#include <QFile>
#include <QFileInfo>
#include <QString>

ChecksumGen::ChecksumGen(int argc, char* argv[])
{
	qInfo("Checksum generator for Burnout Revenge profiles\nby burninrubber0, 2023-11-11\n");
	
	result = validate(argc, argv);
	if (result != 0)
		return;

	stream.open(QIODeviceBase::ReadWrite);

	// Recalculate checksums for the profiles
	uint sum = 0;
	for (int i = 0; i < 3; ++i)
	{
		if (!profilesUsed[i])
			continue;
		sum = calcChecksum(i);
		stream << sum;
	}

	// Recalculate MC02 header checksums
	MC02 header;
	header.read(stream);
	header.write(stream);

	stream.close();
}

uint ChecksumGen::calcChecksum(int profileIndex)
{
	uint result = 0;
	uint tmp = 0;
	stream.seek(0xDC + saveDataSize * profileIndex);
	for (uint i = 0; i < saveDataSize - 8; i += 4)
	{
		stream >> tmp;
		result += tmp;
	}
	return result;
}

int ChecksumGen::validate(int argc, char* argv[])
{
	// Generic argument count check
	if (argc != 2)
	{
		if (argc < 2)
			qCritical("Too few arguments provided.");
		else if (argc > 2)
			qCritical("Too many arguments provided.");
		printUsage();
		return 1;
	}

	// Check input file validity
	QFileInfo info(argv[1]);
	if (!info.exists() || !info.isFile())
	{
		qCritical("Invalid input file. Ensure the path is correct.");
		return 2;
	}
	else if (!info.isReadable())
	{
		qCritical("Input file is not readable. Check its permissions.");
		return 3;
	}
	else if (info.size() != 0x50BC && info.size() != 0x5344)
	{
		if (info.size() == 0x511C)
		{
			qCritical("Input file appears to be an Xbox savegame. These are unsupported at this time.");
			return 4;
		}
		qCritical("Input file is not a valid savegame.");
		return 5;
	}

	// Setup for data validity checks
	platform = info.size() == 0x50BC ? Platform::PS2 : Platform::X360;
	saveDataSize = platform == Platform::X360 ? 0x1B78 : 0x1AA0;
	file.setFileName(argv[1]);
	stream.setDevice(&file);
	stream.setPlatform(platform);
	stream.open(QIODeviceBase::ReadOnly);

	// Check for MC02 header
	uint test4 = 0;
	stream >> test4;
	if (test4 != 0x4D433032) // MC02
	{
		qCritical("Invalid savegame: header mismatch");
		stream.close();
		return 6;
	}

	// Check that at least one profile exists
	stream.seek(0x1C);
	uint8_t test1 = 0;
	bool hasProfile = false;
	for (int i = 0; i < 3; ++i)
	{
		stream >> test1;
		if (test1 != 0)
		{
			profilesUsed[i] = true;
			hasProfile = true;
		}
		stream.skip(0x3F);
	}
	if (!hasProfile)
	{
		qCritical("No profiles present.");
		stream.close();
		return 7;
	}

	// Check the version number in all profiles is correct
	for (int i = 0; i < 3; ++i)
	{
		if (!profilesUsed[i])
			continue;
		stream.seek(0xDC + saveDataSize * i);
		stream >> test4;
		if ((platform == Platform::X360 && test4 != 31)
			|| (platform == Platform::PS2 && test4 != 24))
		{
			qCritical("Invalid profile version.");
			stream.close();
			return 8;
		}
	}

	stream.close();
	return 0;
}

void ChecksumGen::printUsage()
{
	QString info = "Usage: RevengeChecksumGen <input file>\nSupports PS2 and X360 retail games";
	qInfo(info.toStdString().c_str());
}
