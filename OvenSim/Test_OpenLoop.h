
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test_OpenLoop.cpp

#pragma once

// ===== Local ==============================================================
#include "Element.h"
#include "ITest.h"
#include "Oven.h"

class Test_OpenLoop : public ITest
{

public:

    Test_OpenLoop();

    void SetPower(double aPower);

    // ===== ITest ==========================================================
    virtual ~Test_OpenLoop() override;
    virtual bool Tick(double aT_s, double aPeriod_s) override;
    virtual void PrintHeader(FILE* aCSV) const override;
    virtual void PrintLine(double aT_s, FILE* aCSV) const override;

protected:

    Element mElement;
    Oven    mOven;

};
