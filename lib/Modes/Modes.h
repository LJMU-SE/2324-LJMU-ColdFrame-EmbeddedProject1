#ifndef MODES_H
#define MODES_H

// Stores operating modes
enum ModeType
{
    WINTER,
    GERMINATING,
    VEGATATIVE,
    FRUITING,
    NIGHT,
    CUSTOM
};

String modes[] = {"Winter", "Germinating", "Vegatative", "Fruiting", "Night", "Custom"};

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
    Mode() : Mode(ModeType::CUSTOM, -1, -1, -1, -1, -1, -1) {}

    MinMax getTempRange()
    {
        return this->tempRange;
    }

    MinMax getHumidityRange()
    {
        return this->humidityRange;
    }

    void setTempRange(MinMax _tempRange){

        _tempRange.min = constrain(_tempRange.min,0,40);
        _tempRange.max = constrain(_tempRange.max,0,40);

        if (_tempRange.min > _tempRange.max){
            _tempRange.min = _tempRange.max;
        }
        if (_tempRange.max < _tempRange.min){
            _tempRange.max = _tempRange.min;
        }

        this->tempRange = _tempRange;
    }
    void setHumRange(MinMax _humRange){

        _humRange.min = constrain(_humRange.min,0,100);
        _humRange.max = constrain(_humRange.max,0,100);

        if (_humRange.min > _humRange.max){
            _humRange.min = _humRange.max;
        }
        if (_humRange.max < _humRange.min){
            _humRange.max = _humRange.min;
        }
        this->humidityRange = _humRange;
    }
    void setSoilRange(MinMax _soilRange){
        this->moistureLevel = _soilRange;
    }

    MinMax getSoilRange()
    {
        return this->moistureLevel;
    }

    ModeType getModeType(){
        return this->type;
    }

    String getModeString(){
        return modes[this->type];
    }
};
#endif 
