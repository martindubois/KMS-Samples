
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Test3.cpp

#pragma once

// ===== Local ==============================================================
#include "Test_PID.h"

class Test3 : public Test_PID
{

public:

    Test3();

    // ===== ITest ==========================================================
    virtual ~Test3() override;
};

inline Test3::Test3()
{
    auto lElement_C = 1144.5;
    auto lOven_C    = 982.9;

    mElement.SetOvenTemp(lOven_C);
    mElement.SetTemp(lElement_C);

    mOven.SetElementTemp(lElement_C);
    mOven.SetInWallTemp(792.9);
    mOven.SetOutWallTemp(27.1);
    mOven.SetTemp(lOven_C);

    Filter_MD_Reset(&mConsignFilter, static_cast<int32_t>(lOven_C * 256));

    SetConsign(100.0);
}

inline Test3::~Test3() {}
