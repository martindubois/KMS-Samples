
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      Firmware/STM32/F3/ModbusDevice/Core/Inc/Device.h

#pragma once

// ===== Import/Includes ====================================================
#include <KMS/Embedded/USART.h>

class Device
{

public:

    Device(KMS::Embedded::USART* aUSART);

    void Run();

private:

    KMS::Embedded::USART* mUSART;

};