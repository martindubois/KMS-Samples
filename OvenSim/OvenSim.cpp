
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2025 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      OvenSim/EvenSim.cpp

#include "Component.h"

// ==== Import/Includes =====================================================
#include <KMS/Banner.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

#include "Test0.h"
#include "Test1.h"
#include "Test2.h"
#include "Test3.h"

using namespace KMS;

// Configurations
// //////////////////////////////////////////////////////////////////////////

#define LINE_PERIOD_s  (1.0)
#define PERIOD_s       (1.0/16.0)
#define MAX_DURATION_s (1800.0)

// Static function declarations
// //////////////////////////////////////////////////////////////////////////

static void DisplayUsage();

static int RunTest(ITest* aTest);

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "OvenSim");

    if (2 != aCount)
    {
        printf("USER ERROR  Invalid command line\n");
        DisplayUsage();
        return __LINE__;
    }

    assert(nullptr != aVector[1]);

    int    lResult = __LINE__;
    ITest* lTest   = nullptr;

    auto lTestIndex = strtoul(aVector[1], nullptr, 10);
    switch (lTestIndex)
    {
    case 0: lTest = new Test0(); break;
    case 1: lTest = new Test1(); break;
    case 2: lTest = new Test2(); break;
    case 3: lTest = new Test3(); break;

    default:
        printf("USER ERROR  Invalid test index\n");
        DisplayUsage();
        lResult = __LINE__;
    }

    if (nullptr != lTest)
    {
        lResult = RunTest(lTest);

        delete lTest;
    }

    return lResult;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void DisplayUsage()
{
    printf("Usage: OvenSim.exe {TestIndex}\n");
    printf("    0  Heating at 90 %% - From 22 to ...\n");
    printf("    1  Cooling at  0 %% - From 1154 to ...\n");
    printf("    2  From 22 to 1000\n");
    printf("    3  From 1000 to 500\n");
}

int RunTest(ITest* aTest)
{
    FILE* lCSV = nullptr;

    auto lRet = fopen_s(&lCSV, "Output.csv", "wb");

    aTest->PrintHeader(lCSV);

    double lT_s = 0;

    for (;;)
    {
        if (MAX_DURATION_s <= lT_s)
        {
            printf("The simulation reach the maximum allowed duration\n");
            break;
        }

        auto lRet = aTest->Tick(lT_s, PERIOD_s);
        if (!lRet)
        {
            aTest->PrintLine(lT_s, lCSV);
            break;
        }

        if (0.0 == fmod(lT_s, LINE_PERIOD_s))
        {
            aTest->PrintLine(lT_s, lCSV);
        }

        lT_s += PERIOD_s;

        if (1000 == lT_s)
        {
            printf("1000 s\n");
        }
    }

    if (0 != lRet)
    {
        printf("WARNING  Output.csv not updated\n");
    }

    return 0;
}
