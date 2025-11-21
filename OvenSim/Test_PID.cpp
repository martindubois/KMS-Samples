
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

static const int16_t FILTER_TABLE_DEC[] =
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

static const int16_t FILTER_TABLE_INC[] =
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

static const Filter_MD_Table FILTER_TABLE =
{
    FILTER_TABLE_DEC,
    FILTER_TABLE_INC,
    100,
    13,
    100
};

// Public
// //////////////////////////////////////////////////////////////////////////

Test_PID::Test_PID()
{
    Filter_MD_Init(&mConsignFilter, &FILTER_TABLE);
    Filter_MD_Reset(&mConsignFilter, static_cast<int32_t>(ROOM_TEMP_C * 256));

    Filter_FIR_Init(&mTempFilter);

    PID_Init(&mPID, &mConsignFilter, &mTempFilter);
    PID_SetParams(&mPID, 1000, 5, 1000);
}

void Test_PID::SetConsign(double aConsign_C)
{
    Filter_MD_SetInput(&mConsignFilter, static_cast<int32_t>(aConsign_C * 256));
}

// ===== ITest ==============================================================

Test_PID::~Test_PID() {}

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

    Filter_FIR_NewSample(&mTempFilter, static_cast<int32_t>(lOvenTemp_C * 256.0));

    Filter_MD_Tick(&mConsignFilter, lPeriod_ms);

    PID_Tick(&mPID, lPeriod_ms);

    auto lPower = (static_cast<double>(PID_GetOutput_FP(&mPID)) / 256.0) / 10000.0;

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
    auto lFCon_C = static_cast<double>(Filter_MD_GetOutput_FP(&mConsignFilter)) / 256.0;
    auto lOven_C = mOven.GetTemp();
    auto lPower  = (static_cast<double>(PID_GetOutput_FP(&mPID)) / 256.0) / 10000.0;

    printf("%6.1f  %6.1f  %6.4f  %6.1f  %6.1f  %6.1f  %6.1f\n",
        aT_s,
        lFCon_C,
        lPower,
        mElement.GetTemp(),
        lOven_C,
        mOven.GetInWallTemp(),
        mOven.GetOutWallTemp());

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "%f;%f;%f;%f\n", aT_s, lFCon_C, lPower, lOven_C);
    }
}
