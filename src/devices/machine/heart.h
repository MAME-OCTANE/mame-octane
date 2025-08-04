// license:BSD-3-Clause
#ifndef MAME_MACHINE_HEART_H
#define MAME_MACHINE_HEART_H

#pragma once

#include "emu.h"

class heart_device : public device_t
{
public:
    heart_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock = 0);

    void map(address_map &map);

protected:
    virtual void device_start() override {}
    virtual void device_reset() override {}
};

// device type declaration
DECLARE_DEVICE_TYPE(HEART, heart_device)

#endif // MAME_MACHINE_HEART_H
