
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test1.cpp

#pragma once

// ===== Local ==============================================================
#include "Test_OpenLoop.h"

class Test1 : public Test_OpenLoop
{

public:

    Test1();

    // ===== ITest ==========================================================
    virtual ~Test1() override;

};

inline Test1::Test1()
{
    mElement.SetOvenTemp(1154.0);
    mElement.SetTemp    (1187.0);

    mOven.SetElementTemp(1187.0);
    mOven.SetInWallTemp (1043.6);
    mOven.SetOutWallTemp(  36.6);
    mOven.SetTemp       (1154.0);
}

inline Test1::~Test1() {}
