
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/Oven.h

#pragma once

class Oven
{

public:

    Oven();

    double GetInWallTemp() const;

    double GetOutWallTemp() const;

    double GetTemp() const;

    void SetElementTemp(double aElementTemp_C);

    void SetInWallTemp(double aInWallTemp_C);

    void SetOutWallTemp(double aOutWallTemp_C);

    void SetTemp(double aTemp_C);

    bool Tick(double aT_s, double aPeriod_s);

private:

    double mElement_C;
    double mInWall_C;
    double mOutWall_C;
    double mTemp_C;

};

// Public
// //////////////////////////////////////////////////////////////////////////

inline double Oven::GetInWallTemp() const { return mInWall_C; }

inline double Oven::GetOutWallTemp() const { return mOutWall_C; }

inline double Oven::GetTemp() const { return mTemp_C; }

inline void Oven::SetElementTemp(double aElementTemp_C) { mElement_C = aElementTemp_C; }

inline void Oven::SetInWallTemp(double aInWallTemp_C) { mInWall_C = aInWallTemp_C; }

inline void Oven::SetOutWallTemp(double aOutWallTemp_C) { mOutWall_C = aOutWallTemp_C; }

inline void Oven::SetTemp(double aTemp_C) { mTemp_C = aTemp_C; }
