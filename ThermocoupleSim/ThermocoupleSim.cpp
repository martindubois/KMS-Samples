
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/EvenSim.cpp

#include <KMS/Base.h>

// ==== Import/Includes =====================================================
#include <KMS/Banner.h>

// ===== KMS-uC =============================================================
extern "C"
{
    #include <Thermocouple.h>
}

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Configurations
// //////////////////////////////////////////////////////////////////////////

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void DisplayCalibration(const Thermocouple& aTC);

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "ThermocoupleSim");

    Thermocouple lTC;

    Thermocouple_Init(&lTC, &Thermocouple_TYPE_R);

    std::cout << "Expected 1.0, - 4 deg.C\n";
    Thermocouple_Calibrate2(&lTC, 26, 22, 1004, 1000);
    DisplayCalibration(lTC);

    std::cout << "Expected 1.0, + 4 deg.C\n";
    Thermocouple_Calibrate2(&lTC, 18, 22, 996, 1000);
    DisplayCalibration(lTC);

    std::cout << "Expected > 1.0, + 4 deg.C\n";
    Thermocouple_Calibrate2(&lTC, 18, 22, 990, 1000);
    DisplayCalibration(lTC);

    return 0;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void DisplayCalibration(const Thermocouple& aTC)
{
    double lGain = aTC.mCal_Gain;

    lGain /= 4096;

    std::cout << "Gain   = " << lGain << " (" << aTC.mCal_Gain << ")\n";
    std::cout << "Offset = " << aTC.mCal_Offset_uV << " uV\n";

    std::cout << std::endl;
}
