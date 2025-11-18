
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Element.h

#pragma once

class Element
{

public:

    Element();

    double GetTemp() const;

    void SetOvenTemp(double aOvenTemp_C);

    void SetPower(double aPower_W);

    void SetTemp(double aTemp_C);

    bool Tick(double aT_s, double aPeriod_s);

private:

    double mOven_C;
    double mPower_W;
    double mTemp_C;

};

// Public
// //////////////////////////////////////////////////////////////////////////

inline double Element::GetTemp() const { return mTemp_C; }

inline void Element::SetOvenTemp(double aOvenTemp_C) { mOven_C = aOvenTemp_C; }

inline void Element::SetPower(double aPower_W) { mPower_W = aPower_W; }

inline void Element::SetTemp(double aTemp_C) { mTemp_C = aTemp_C; }
