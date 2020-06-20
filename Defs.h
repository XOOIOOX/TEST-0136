#pragma once

#include <vector>
#include "QDateTime"

#define BadIndex	-1

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

struct sqlRow
{
	QDateTime date{ QDateTime::fromSecsSinceEpoch(0u) };
	double value{ 0.0 };
	int group{ 0 };
};

using VectorSql = std::vector<sqlRow>;
using ListInt = std::list<int>;

struct CentralDataStruct
{
	VectorSql vectorSql;
	int currentIndex{ BadIndex };
	bool checkCurrentIndex() { return (currentIndex != BadIndex) && (!vectorSql.empty()); }
};
