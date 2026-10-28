
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test_PID.cpp

#include "Component.h"

// ===== C ==================================================================
#include <stdint.h>

// ===== Local ==============================================================
#include "Test_PID.h"

// Constants
// //////////////////////////////////////////////////////////////////////////

static const int16_t FILTER_TABLE_DEC_VALUES[] =
{
      3, //    0 -  200 C ->  0.1  C/s
      5, //  100 -  200 C ->  0.2  C/s
      7, //  200 -  300 C ->  0.28 C/s
     11, //  300 -  400 C ->  0.42 C/s
     16, //  400 -  500 C ->  0.62 C/s
     18, //  500 -  600 C ->  0.72 C/s
     23, //  600 -  700 C ->  0.9  C/s
     26, //  700 -  800 C ->  1.0  C/s
     28, //  800 -  900 C ->  1.1  C/s
     36, //  900 - 1000 C ->  1.4  C/s
     92, // 1000 - 1100 C ->  3.6  C/s
    184, // 1100 - 1200 C ->  7.2  C/s
    369, // 1200 - 1300 C -> 14.4  C/s
};

static const Table FILTER_TABLE_DEC =
{
    FILTER_TABLE_DEC_VALUES,
    100 << 8,
    13
};

static const int16_t FILTER_TABLE_INC_VALUES[] =
{
    37, //    0 -  200 C -> 1.45 C/s
    33, //  100 -  200 C -> 1.3  C/s
    31, //  200 -  300 C -> 1.2  C/s
    27, //  300 -  400 C -> 1.05 C/s
    24, //  400 -  500 C -> 0.95 C/s
    19, //  500 -  600 C -> 0.75 C/s
    15, //  600 -  700 C -> 0.6  C/s
    12, //  700 -  800 C -> 0.45 C/s
     8, //  800 -  900 C -> 0.3  C/s
     4, //  900 - 1000 C -> 0.15 C/s
     3, // 1000 - 1100 C -> 0.1  C/s
     2, // 1100 - 1200 C -> 0.08 C/s
     2, // 1200 - 1300 C -> 0.07 C/s
};

static const Table FILTER_TABLE_INC =
{
    FILTER_TABLE_INC_VALUES,
    100 << 8,
    13
};

static const Filter_SP_Table FILTER_TABLE =
{
    &FILTER_TABLE_DEC,
    &FILTER_TABLE_INC,
    180,
};

static const int16_t PID_TABLE_VALUES[] =
{
    1000, //    0 -  100 C -> 10 %
    1500, //  100 -  200 C -> 15 %
    2000, //  200 -  300 C -> 20 %
    2500, //  300 -  400 C -> 25 %
    3000, //  400 -  500 C -> 30 %
    3500, //  500 -  600 C -> 35 %
    4000, //  600 -  700 C -> 40 %
    4500, //  700 -  800 C -> 45 %
    5000, //  800 -  900 C -> 50 %
    5500, //  900 - 1000 C -> 55 %
    6000, // 1000 - 1100 C -> 60 %
    6500, // 1100 - 1200 C -> 65 %
    7000, // 1200 - 1300 C -> 70 %
};

static const Table PID_TABLE =
{
    PID_TABLE_VALUES,
    100 << 8,
    13
};

// Variable
// //////////////////////////////////////////////////////////////////////////

static Test_PID* sThis;

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static int32_t GetFilteredSetpoint();
static int32_t GetOvenTemperature();

// Public
// //////////////////////////////////////////////////////////////////////////

Test_PID::Test_PID()
{
    sThis = this;

    Filter_SP_Init(&mSetpointFilter, &FILTER_TABLE, GetOvenTemperature);

    PID_Oven_Init(&mPID, &PID_TABLE, GetFilteredSetpoint, GetOvenTemperature);
    PID_Oven_SetParams(&mPID, 1000, 5, 1000);
}

void Test_PID::SetSetpoint(double aSetpoint_C)
{
    Filter_SP_SetInput(&mSetpointFilter, static_cast<int32_t>(aSetpoint_C * 256));
}

// ===== ITest ==============================================================

Test_PID::~Test_PID() { sThis = nullptr; }

bool Test_PID::Tick(double aT_s, double aPeriod_s)
{
    auto lResult = true;

    lResult &= mElement.Tick(aT_s, aPeriod_s);
    lResult &= mOven   .Tick(aT_s, aPeriod_s);

    auto lElementTemp_C = mElement.GetTemp();
    auto lOvenTemp_C    = mOven   .GetTemp();

    mElement.SetOvenTemp   (lOvenTemp_C   );
    mOven   .SetElementTemp(lElementTemp_C);

    auto lPeriod_ms = static_cast<uint8_t>(aPeriod_s * 1000);

    Filter_SP_Tick(&mSetpointFilter, lPeriod_ms);

    PID_Oven_Tick(&mPID, lPeriod_ms);

    auto lPower = (static_cast<double>(PID_Oven_GetOutput_FP(&mPID)) / 256.0) / 10000.0;

    mElement.SetPower(lPower * 1200 * 3);

    return lResult;
}

void Test_PID::PrintHeader(FILE* aCSV) const
{
    printf(" Time   F. Con   Power  Elemen   Oven   In Wal  Out Wa\n");
    printf("   s       C               C       C       C       C\n");
    printf("------  ------  ------  ------  ------  ------  ------\n");

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "Time;F. Con;Power;Oven\n");
    }
}

void Test_PID::PrintLine(double aT_s, FILE* aCSV) const
{
    auto lSetpoint_C = static_cast<double>(Filter_SP_GetOutput_FP(&mSetpointFilter)) / 256.0;
    auto lOven_C     = mOven.GetTemp();
    auto lPower      = (static_cast<double>(PID_Oven_GetOutput_FP(&mPID)) / 256.0) / 10000.0;

    printf("%6.1f  %6.1f  %6.4f  %6.1f  %6.1f  %6.1f  %6.1f\n",
        aT_s,
        lSetpoint_C,
        lPower,
        mElement.GetTemp(),
        lOven_C,
        mOven.GetInWallTemp(),
        mOven.GetOutWallTemp());

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "%f;%f;%f;%f\n", aT_s, lSetpoint_C, lPower, lOven_C);
    }
}

// Internal
// //////////////////////////////////////////////////////////////////////////

int32_t Test_PID::GetFilteredSetpoint_FP() const { return Filter_SP_GetOutput_FP(&mSetpointFilter); }
int32_t Test_PID::GetOvenTemperature_FP () const { return static_cast<int32_t>(mOven.GetTemp() * 256.0); }
int32_t Test_PID::GetSetpoint_FP        () const { return Filter_SP_GetInput_FP(&mSetpointFilter); }

// Static functions
// //////////////////////////////////////////////////////////////////////////

int32_t GetSetpoint        () { return sThis->GetSetpoint_FP        (); }
int32_t GetFilteredSetpoint() { return sThis->GetFilteredSetpoint_FP(); }
int32_t GetOvenTemperature () { return sThis->GetOvenTemperature_FP (); }
