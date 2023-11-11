#include <checksum.h>

#include <QScopedPointer>

int main(int argc, char* argv[])
{
	QScopedPointer<ChecksumGen> checksumGen(new ChecksumGen(argc, argv));
	return checksumGen->result;
}
