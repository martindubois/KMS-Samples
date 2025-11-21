
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test_OpenLoop.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Test_OpenLoop.h"

// Public
// //////////////////////////////////////////////////////////////////////////

Test_OpenLoop::Test_OpenLoop() {}

void Test_OpenLoop::SetPower(double aPower)
{
    mElement.SetPower(aPower * 1200 * 3);
}

// ===== ITest ==============================================================

Test_OpenLoop::~Test_OpenLoop() {}

bool Test_OpenLoop::Tick(double aT_s, double aPeriod_s)
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

void Test_OpenLoop::PrintHeader(FILE* aCSV) const
{
    printf(" Time   Elemen   Oven   In Wal  Out Wa\n");
    printf("   s       C       C       C       C\n");
    printf("------  ------  ------  ------  ------\n");

    if (nullptr != aCSV)
    {
        fprintf(aCSV, "Time;Oven\n");
    }
}

void Test_OpenLoop::PrintLine(double aT_s, FILE* aCSV) const
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
