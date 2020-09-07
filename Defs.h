#pragma once

#include <vector>
#include "QDateTime"
#include "QColor"

constexpr int BadIndex{ -1 };								// индекс вне диапазона

constexpr QColor ColorOrange{ 255, 137, 27, 255 };			// оранжевый
constexpr QColor ColorBlue{ 43, 181, 255 };					// синий
constexpr QColor ColorBlueTransp{ 43, 181, 255, 95 };		// синий прозрачный
constexpr QColor ColorGreenTransp{ 0, 255, 0, 110 };		// зеленый прозрачный
constexpr QColor ColorRedTransp{ 255, 0, 0, 110 };			// красный прозрачный
constexpr double ViewBorder{ 20 };							// бордюр для отображения графического представления

enum class ViewType { Horizontal, Vertical, Column };		// режим отображения графического представления

struct sqlRow
{
	QDateTime date{ QDateTime::fromSecsSinceEpoch(0u) };
	double value{ 0.0 };
	int group{ 0 };
};

//////////////////////////////////////////////////////////////////////////
// Удобные юзинги
//////////////////////////////////////////////////////////////////////////

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;
using VectorSql = std::vector<sqlRow>;
using VectorDouble = std::vector<double>;
using ListInt = std::list<int>;

struct CentralDataStruct
{
	VectorSql vectorSql;
	int currentIndex{ BadIndex };
	bool checkCurrentIndex() { return (currentIndex != BadIndex) && (!vectorSql.empty()); }
};
