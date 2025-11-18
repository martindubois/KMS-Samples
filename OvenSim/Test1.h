
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test1.cpp

#pragma once

// ===== Local ==============================================================
#include "Element.h"
#include "ITest.h"
#include "Oven.h"

class Test1 : public ITest
{

public:

    Test1();

    // ===== ITest ==========================================================
    virtual ~Test1() override;
    virtual bool Tick(double aT_s, double aPeriod_s) override;
    virtual void PrintHeader(FILE* aCSV) const override;
    virtual void PrintLine(double aT_s, FILE* aCSV) const override;

private:

    Element mElement;
    Oven    mOven;

};
