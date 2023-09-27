
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      ByteTool/ByteTool.cpp

#include <KMS/Base.h>

// ==== Import/Includes =====================================================
#include <KMS/Banner.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/Dbg/Log.h>
#include <KMS/File/Folder.h>
#include <KMS/Main.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define CONFIG_FILE ("ByteTool.cfg")

// Constants
// //////////////////////////////////////////////////////////////////////////

#define BYTE_QTY (256)

static const char* LABELS[16] =
{
    "x0_", "x1_", "x2_", "x3_", "x4_", "x5_", "x6_", "x7_", "x8_", "x9_", "xA_", "xB_", "xC_", "xD_", "xE_", "xF_"
};

static const Cfg::MetaData MD_SOURCE("Source = {FileName}");

// Main class
// //////////////////////////////////////////////////////////////////////////

class ByteTool : public DI::Dictionary
{

public:

    static const char* SOURCE_DEFAULT;

    DI::File mSource;

    ByteTool();

    int Run();

    // ===== DI::Container ==================================================
    virtual void Validate() const;

// Internal

    void Display(std::ostream& aOut) const;

private:

    unsigned int mHistogram[256];

};

static std::ostream& operator << (std::ostream& aOut, const ByteTool& aBT);

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

static void DisplayValue(std::ostream& aOut, unsigned int aValue);

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "ByteTool");

    KMS_MAIN_BEGIN;
    {
        ByteTool lBT;

        lConfigurator.AddConfigurable(&lBT);

        lConfigurator.ParseFile(File::Folder::CURRENT, CONFIG_FILE);

        KMS_MAIN_PARSE_ARGS(aCount, aVector);

        KMS_MAIN_VALIDATE;

        lResult = lBT.Run();

        std::cerr << lBT;
    }
    KMS_MAIN_END;

    KMS_MAIN_RETURN;
}

// Public
// //////////////////////////////////////////////////////////////////////////

const char* ByteTool::SOURCE_DEFAULT = "stdin";

std::ostream& operator << (std::ostream& aOut, const ByteTool& aBT)
{
    aBT.Display(aOut);

    return aOut;
}

ByteTool::ByteTool() : mSource(stdin, SOURCE_DEFAULT)
{
    AddEntry("Source", &mSource, false, &MD_SOURCE);

    memset(&mHistogram, 0, sizeof(mHistogram));
}

int ByteTool::Run()
{
    uint8_t lByte;

    while (1 == fread(&lByte, sizeof(lByte), 1, mSource))
    {
        mHistogram[lByte] ++;
    }

    return 0;
}

// ===== DI::Container ==================================================

void ByteTool::Validate() const
{
    DI::Dictionary::Validate();

    KMS_EXCEPTION_ASSERT(File::Folder::NONE.DoesFileExist(mSource), RESULT_INVALID_CONFIG, "The source file does not exist", mSource);
}

// Internal
// /////////////////////////////////////////////////////////////////////////

void ByteTool::Display(std::ostream& aOut) const
{
    assert(NULL != mHistogram);

    unsigned int lMax = mHistogram[0];
    unsigned int lMin = mHistogram[0];
    unsigned int lSum = mHistogram[0];

    aOut << "      x_0   x_1   x_2   x_3   x_4   x_5   x_6   x_7   x_8   x_9   x_A   x_B   x_C   x_D   x_E   x_F" << "\n";
    aOut << LABELS[0];

    DisplayValue(aOut, mHistogram[0]);

    for (unsigned int i = 1; i < BYTE_QTY; i++)
    {
        if (0 == i % 16)
        {
            aOut << "\n" << LABELS[i / 16];
        }

        if (lMax < mHistogram[i]) { lMax = mHistogram[i]; }
        if (lMin > mHistogram[i]) { lMin = mHistogram[i]; }

        lSum += mHistogram[i];

        DisplayValue(aOut, mHistogram[i]);
    }

    aOut << "\n";
    aOut << "Min : " << lMin << "\n";
    aOut << "Avg : " << lSum / 256.0 << "\n";
    aOut << "Max : " << lMax << "\n";
    aOut << "Sum : " << lSum << std::endl;
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void DisplayValue(std::ostream& aOut, unsigned int aValue)
{
    char lStr[8];

    sprintf_s(lStr, "%6u", aValue);

    aOut << lStr;
}
