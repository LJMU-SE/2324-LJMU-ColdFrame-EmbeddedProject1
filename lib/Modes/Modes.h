#ifndef MODES_H
#define MODES_H

// Stores operating modes
enum class ModeType
{
    WINTER,
    GERMINATING,
    VEGATATIVE,
    FRUITING,
    NIGHT,
    NULLMODE
};

// Stores sensor ranges
struct MinMax
{
    int min;
    int max;
};

class Mode
{
    ModeType type;
    MinMax tempRange, humidityRange, moistureLevel;

public:
    Mode(ModeType type, int tempMin, int tempMax, int humMin, int humMax, int soilMin, int soilMax)
    {
        this->type = type;
        this->tempRange.min = tempMin;
        this->tempRange.max = tempMax;
        this->humidityRange.min = humMin;
        this->humidityRange.max = humMax;
        this->moistureLevel.min = soilMin;
        this->moistureLevel.max = soilMax;
    }
    Mode() : Mode(ModeType::NULLMODE, -1, -1, -1, -1, -1, -1) {}

    MinMax getTempRange()
    {
        return this->tempRange;
    }

    MinMax getHumidityRange()
    {
        return this->humidityRange;
    }

    MinMax getSoilRange()
    {
        return this->moistureLevel;
    }

    ModeType getModeType(){
        return this->type;
    }
};
#endif 
