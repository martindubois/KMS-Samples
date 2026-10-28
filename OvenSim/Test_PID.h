
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test_PID.cpp

#pragma once

// ===== KMS-uC =============================================================
#include <Filter_SP.h>
#include <PID_Oven.h>

// ===== Local ==============================================================
#include "Element.h"
#include "ITest.h"
#include "Oven.h"

class Test_PID : public ITest
{

public:

    Test_PID();

    void SetSetpoint(double aSetpoint_C);

    // ===== ITest ==========================================================
    virtual ~Test_PID() override;
    virtual bool Tick(double aT_s, double aPeriod_s) override;
    virtual void PrintHeader(FILE* aCSV) const override;
    virtual void PrintLine(double aT_s, FILE* aCSV) const override;

// Internal

    int32_t GetFilteredSetpoint_FP() const;
    int32_t GetOvenTemperature_FP () const;
    int32_t GetSetpoint_FP        () const;

protected:

    Element mElement;
    Oven    mOven;

    Filter_SP mSetpointFilter;
    PID_Oven  mPID;

};
