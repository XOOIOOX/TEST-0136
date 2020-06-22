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

#define ColorOrange				255, 137, 27, 255			// оранжевый
#define ColorBlue				43, 181, 255				// синий
#define ColorBlueTransp			43, 181, 255, 125			// синий прозрачный

#define ViewBorder				20							// бордюр для отображения графического представления

enum class ViewType { Horizontal, Vertical, Column };

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
