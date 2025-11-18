
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Element.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Element.h"

// Configurations
// //////////////////////////////////////////////////////////////////////////

#define COOLING_s    (0.01)
#define HEATING_C_Ws (0.0005)

#define MAX_TEMP_C (1400.0)

// Public
// //////////////////////////////////////////////////////////////////////////

Element::Element()
    : mOven_C(150.0)
    , mPower_W(0)
    , mTemp_C(150.0)
{}

bool Element::Tick(double aT_s, double aPeriod_s)
{
    auto lResult = true;

    mTemp_C += aPeriod_s * mPower_W * HEATING_C_Ws;

    auto lDelta_C = mTemp_C - mOven_C;
    mTemp_C -= aPeriod_s * lDelta_C * COOLING_s;

    if (MAX_TEMP_C < mTemp_C)
    {
        printf("The element overheated\n");
        lResult = false;
    }

    return lResult;
}
