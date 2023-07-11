
// Author    KMS - Martin Dubois, P. Eng.
// Copyright (C) 2023 KMS
// License   http://www.apache.org/licenses/LICENSE-2.0
// Product   KMS-Samples
// File      Firmware/STM32/F373xC/ModbusDevice/Core/Src/Device.cpp

#include "Component.h"

// ===== Local ==============================================================
#include "Device.h"

using namespace KMS;

// Public
// //////////////////////////////////////////////////////////////////////////

Device::Device(Embedded::USART* aUSART) : mUSART(aUSART) {}

void Device::Run() {}
