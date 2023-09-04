
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      Firmware/STM32/H7/BL-USB/CM7/Core/Src/BootLoader.cpp

#include "Component.h"

// ===== STM32 ==============================================================
#include "fatfs.h"

// ===== Local ==============================================================
#include "BootLoader.h"

// Configuration
// //////////////////////////////////////////////////////////////////////////

#define USB_PREFIX "0:/"

#define LOCK_FILE_NAME   "BL-USB.lck"
#define UPDATE_FILE_NAME "Update.elf"

// Static function declaration
// //////////////////////////////////////////////////////////////////////////

static void StartApplication();

static void UpdateIfNeeded(const char* aUpdate, const char* aLock);

// Functions
// //////////////////////////////////////////////////////////////////////////

void BootLoader_Run()
{
    osDelay(350); // ticks

	FRESULT lRet = f_mount(&USBHFatFS, USBHPath, 1);
	if (FR_OK == lRet)
	{
		UpdateIfNeeded(USB_PREFIX UPDATE_FILE_NAME, USB_PREFIX LOCK_FILE_NAME);
	}

	StartApplication();
}

// Static functions
// //////////////////////////////////////////////////////////////////////////

void StartApplication()
{
	// TODO
}

void UpdateIfNeeded(const char* aUpdate, const char* aLock)
{
	FIL lFile;

	FRESULT lRet = f_open(&lFile, aLock, FA_READ);
	if (FR_OK == lRet)
	{
		lRet = f_close(&lFile);
	}
	else
	{
		lRet = f_open(&lFile, aLock, FA_WRITE | FA_CREATE_NEW );
		if (FR_OK == lRet )
		{
			lRet = f_close(&lFile);
		}

		lRet = f_open(&lFile, aUpdate, FA_READ | FA_OPEN_EXISTING );
		if (FR_OK == lRet)
		{
			// TODO  Write to flash

			lRet = f_close(&lFile);
		}

		lRet = f_unlink(aLock);

		StartApplication();
	}
}
