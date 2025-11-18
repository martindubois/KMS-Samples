
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/ITest.cpp

#pragma once

class ITest
{

public:

    virtual ~ITest();

    virtual bool Tick(double aT_s, double aPeriod_s) = 0;

    virtual void PrintHeader(FILE* aCSV) const = 0;

    virtual void PrintLine(double aT_s, FILE* aCSV) const = 0;

};

// Public
// //////////////////////////////////////////////////////////////////////////

inline ITest::~ITest() {};
