
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Element.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Oven.h"

// Configurations
// //////////////////////////////////////////////////////////////////////////

#define COOLING_s          (0.35)
#define HEATING_s          (0.4)
#define IN_WALL_COOLING_s  (0.2)
#define IN_WALL_HEATING_s  (0.8)
#define OUT_WALL_COOLING_s (0.03)
#define OUT_WALL_HEATING_s (0.0002)

#define IN_WALL_MAX_TEMP_C  (1300.0)
#define OUT_WALL_MAX_TEMP_C (70.0)

// Public
// //////////////////////////////////////////////////////////////////////////

Oven::Oven()
    : mElement_C(150.0)
    , mInWall_C (150.0)
    , mOutWall_C(ROOM_TEMP_C)
    , mTemp_C   (150.0)
{}

bool Oven::Tick(double aT_s, double aPeriod_s)
{
    auto lResult = true;

    auto lDelta_C = mElement_C - mTemp_C;
    mTemp_C    += aPeriod_s * lDelta_C * HEATING_s;

    lDelta_C = mTemp_C - mInWall_C;
    mInWall_C  += aPeriod_s * lDelta_C * IN_WALL_HEATING_s;

    lDelta_C = mInWall_C - mOutWall_C;
    mOutWall_C += aPeriod_s * lDelta_C * OUT_WALL_HEATING_s;

    lDelta_C = mTemp_C - mInWall_C;
    mTemp_C    -= aPeriod_s * lDelta_C * COOLING_s;

    lDelta_C = mInWall_C - mOutWall_C;
    mInWall_C  -= aPeriod_s * lDelta_C * IN_WALL_COOLING_s;

    lDelta_C = mOutWall_C - ROOM_TEMP_C;
    mOutWall_C -= aPeriod_s * lDelta_C * OUT_WALL_COOLING_s;

    assert(ROOM_TEMP_C <= mInWall_C);
    assert(mInWall_C >= mOutWall_C);
    assert(mInWall_C <= mTemp_C);

    if (IN_WALL_MAX_TEMP_C < mInWall_C)
    {
        printf("The interior wall of the oven overheated\n");
        lResult = false;
    }

    if (OUT_WALL_MAX_TEMP_C < mOutWall_C)
    {
        printf("The exterior wall of the oven overheated\n");
        lResult = false;
    }

    return lResult;
}
