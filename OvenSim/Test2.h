
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test2.cpp

#pragma once

// ===== Local ==============================================================
#include "Test_PID.h"

class Test2 : public Test_PID
{

public:

    Test2();

    // ===== ITest ==========================================================
    virtual ~Test2() override;

};

inline Test2::Test2()
{
    SetConsign(1000.0);
}

inline Test2::~Test2() {}
