
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      Firmware/STM32/F373xC/ModbusDevice/Core/Src/ModbusDevice.cpp

#include "Component.h"

// ===== Import/Includes ====================================================
#include <KMS/STM/STM32F.h>

// ===== Local ==============================================================
#include "Device.h"

#include "ModbusDevice.h"

using namespace KMS;

// Static variables
// //////////////////////////////////////////////////////////////////////////

static STM::STM32F sProcessor;

// Entry point
// //////////////////////////////////////////////////////////////////////////

void ModbusDevice_Main()
{
    sProcessor.Clock_64_MHz();

    Embedded::USART* lUSART = sProcessor.USART_Get(1, KMS_STM_ID_PD(6), KMS_STM_ID_PD(5));

    static Device lDevice(lUSART);

    lDevice.Run();
}
