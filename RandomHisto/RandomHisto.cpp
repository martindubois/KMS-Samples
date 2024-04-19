
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      ByteTool/ByteTool.cpp

#include <KMS/Base.h>

// ==== Import/Includes =====================================================
#include <KMS/Banner.h>
#include <KMS/Cfg/MetaData.h>
#include <KMS/DI/Dictionary.h>
#include <KMS/DI/String_Expand.h>
#include <KMS/FileFormat/BMP.h>
#include <KMS/Graph/Histogram.h>
#include <KMS/Main.h>
#include <KMS/Result.h>
#include <KMS/Thread/Thread.h>
#include <KMS/WGDI/Window_Bitmap.h>

#include <KMS/DI/UInt.h>

// ===== Local ==============================================================
#include "../Common/Version.h"

using namespace KMS;

// Constants
// //////////////////////////////////////////////////////////////////////////

#define SIZE_X_px (512)
#define SIZE_Y_px (512)

static const Cfg::MetaData MD_FILE_NAME("FileName = {Path}");
static const Cfg::MetaData MD_N        ("N = {Value}");
static const Cfg::MetaData MD_PERIOD_ms("Period_ms = {Value}");
static const Cfg::MetaData MD_ZOOM     ("Zoom = {Value}");

// Main class
// //////////////////////////////////////////////////////////////////////////

class RandomHisto : public DI::Dictionary
{

public:

    static const char   * FILE_NAME_DEFAULT;
    static const uint32_t N_DEFAULT;
    static const uint32_t N_MIN;
    static const uint32_t PERIOD_DEFAULT_ms;

    DI::String_Expand  mFileName;
    DI::UInt<uint32_t> mN;

    DI::UInt_Ptr<uint32_t> mPeriod_ms;
    DI::UInt_Ptr<uint8_t>  mZoom;

    RandomHisto();

    int Run();

    unsigned int OnIterate(void* aSender, void* aData);

    // ===== DI::Container ==================================================
    virtual void Validate() const;

    Graph::Histogram    mHistogram;
    Thread::Thread      mThread;
    WGDI::Window_Bitmap mWindow;

};

// Entry point
// //////////////////////////////////////////////////////////////////////////

int main(int aCount, const char** aVector)
{
    KMS_BANNER("KMS-Framework", "RandomHisto");

    KMS_MAIN_BEGIN;
    {
        RandomHisto lRH;

        lConfigurator.AddConfigurable(&lRH);

        KMS_MAIN_PARSE_ARGS(aCount, aVector);

        KMS_MAIN_VALIDATE;

        lResult = lRH.Run();

        std::cout << lRH.mHistogram;
    }
    KMS_MAIN_END;

    KMS_MAIN_RETURN;
}

// Public
// //////////////////////////////////////////////////////////////////////////

const char   * RandomHisto::FILE_NAME_DEFAULT = "";
const uint32_t RandomHisto::N_DEFAULT         = 1;
const uint32_t RandomHisto::N_MIN             = 1;
const uint32_t RandomHisto::PERIOD_DEFAULT_ms = 100;

RandomHisto::RandomHisto()
    : mFileName(FILE_NAME_DEFAULT)
    , mN       (N_DEFAULT)
    , mPeriod_ms(&mWindow.mPeriod_ms)
    , mZoom     (&mWindow.mZoom)
    , mHistogram(0.0, 256.0, 256)
{
    Ptr_OF<DI::Object> lEntry;

    lEntry.Set(&mFileName , false); AddEntry("FileName" , lEntry, &MD_FILE_NAME);
    lEntry.Set(&mN        , false); AddEntry("N"        , lEntry, &MD_N);
    lEntry.Set(&mPeriod_ms, false); AddEntry("Period_ms", lEntry, &MD_PERIOD_ms);
    lEntry.Set(&mZoom     , false); AddEntry("Zoom"     , lEntry, &MD_ZOOM);

    mWindow.mPeriod_ms = PERIOD_DEFAULT_ms;
    mWindow.mTitle     = "RandomHisto";
}

int RandomHisto::Run()
{
    FileFormat::BMP* lFile = nullptr;

    if (mFileName.GetString().empty())
    {
        mHistogram.mBitmap.Init(SIZE_X_px, SIZE_Y_px);
    }
    else
    {
        lFile = new FileFormat::BMP(File::Folder::NONE, mFileName, true);

        lFile->Create(SIZE_X_px, SIZE_Y_px);

        mHistogram.mBitmap.Init(lFile);
    }

    mHistogram.Prepare();

    mWindow.SetBitmap(&mHistogram.mBitmap);

    Callback<RandomHisto> lOnIterate(this, &RandomHisto::OnIterate);

    mThread.mOnIterate = &lOnIterate;

    mThread.Start();

    mWindow.Run();

    mThread.StopAndWait(1000);

    if (nullptr != lFile)
    {
        delete lFile;
    }

    return 0;
}

unsigned int RandomHisto::OnIterate(void*, void*)
{
    uint32_t lN = mN;
    double   lSum = 0.0;

    for (unsigned int i = 0; i < lN; i++)
    {
        lSum += rand() % 256;
    }

    mHistogram.AddData(lSum / static_cast<double>(lN));

    return 0;
}

// ===== DI::Container ======================================================

void RandomHisto::Validate() const
{
    DI::Dictionary::Validate();

    KMS_EXCEPTION_ASSERT(N_MIN                         <= mN   , RESULT_INVALID_CONFIG, "The value of N is below the minimum value"   , "");
    KMS_EXCEPTION_ASSERT(WGDI::Window_Bitmap::ZOOM_MAX >= mZoom, RESULT_INVALID_CONFIG, "The value of zoom is above the maximum value", "");
    KMS_EXCEPTION_ASSERT(WGDI::Window_Bitmap::ZOOM_MIN <= mZoom, RESULT_INVALID_CONFIG, "The value of zoom is below the minimum value", "");
}
