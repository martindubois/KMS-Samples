
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

#define LOCK_FILE_NAME   "0:/BL-USB.lck"
#define UPDATE_FILE_NAME "0:/Update.elf"

// Functions
// //////////////////////////////////////////////////////////////////////////

void BootLoader_Run()
{
	FIL lFile;

    osDelay(350); // ticks

	FRESULT lRetF = f_mount(&USBHFatFS, USBHPath, 1);
	if (FR_OK == lRetF)
	{
		lRetF = f_open(&lFile, LOCK_FILE_NAME, FA_READ);
		if (FR_OK == lRetF)
		{
			lRetF = f_close(&lFile);
		}
		else
		{
			lRetF = f_open(&lFile, LOCK_FILE_NAME, FA_WRITE | FA_CREATE_NEW );
			if (FR_OK == lRetF )
			{
				lRetF = f_close(&lFile);
			}

			lRetF = f_open(&lFile, UPDATE_FILE_NAME, FA_READ | FA_OPEN_EXISTING );
			if (FR_OK == lRetF)
			{
				// TODO  Write to flash

				lRetF = f_close(&lFile);
			}

			lRetF = f_unlink(LOCK_FILE_NAME);
		}
	}

	// TODO  Start the application
}
