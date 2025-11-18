
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test0.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Test0.h"

// Public
// //////////////////////////////////////////////////////////////////////////

Test0::Test0()
{
    mElement.SetPower(0.9 * 1200 * 3);
}

// ===== ITest ==============================================================

Test0::~Test0() {}

bool Test0::Tick(double aT_s, double aPeriod_s)
{
    auto lResult = true;

    lResult &= mElement.Tick(aT_s, aPeriod_s);
    lResult &= mOven   .Tick(aT_s, aPeriod_s);

    auto lElementTemp_C = mElement.GetTemp();
    auto lOvenTemp_C    = mOven   .GetTemp();

    mElement.SetOvenTemp   (lOvenTemp_C   );
    mOven   .SetElementTemp(lElementTemp_C);

    return lResult;
}

void Test0::PrintHeader(FILE* aCSV) const
{
    printf(" Time   Elemen   Oven   In Wal  Out Wa\n");
    printf("   s       C       C       C       C\n");
    printf("------  ------  ------  ------  ------\n");

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "Time;Oven\n");
    }
}

void Test0::PrintLine(double aT_s, FILE* aCSV) const
{
    auto lOven_C = mOven.GetTemp();

    printf("%6.1f  %6.1f  %6.1f  %6.1f  %6.1f\n",
        aT_s,
        mElement.GetTemp(),
        lOven_C,
        mOven.GetInWallTemp(),
        mOven.GetOutWallTemp());

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "%f;%f\n", aT_s, lOven_C);
    }
}
