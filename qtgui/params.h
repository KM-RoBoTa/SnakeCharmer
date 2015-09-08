#ifndef PARAMS_H
#define PARAMS_H

#include <vector>

#define PI 3.14159f

struct Params {
    std::vector<double> offset;
    bool manualoffset;
    std::vector<double> amplitude;
    bool manualamplitude;


    double amplitudeOdd;
    double wavelengthOdd;
    double frequencyOdd;
    double phaseOdd;
    double masteroffsetOdd;

    double amplitudeEven;
    double wavelengthEven;
    double frequencyEven;
    double phaseEven;
    double masteroffsetEven;
};

#endif // PARAMS_H
