// heart.cpp
// license:BSD-3-Clause

#include "emu.h"
#include "machine/heart.h"

#define LOG_HEART (1U << 1)
#define VERBOSE (LOG_HEART)
#include "logmacro.h"

DEFINE_DEVICE_TYPE(HEART, heart_device, "heart", "SGI Heart Controller")

heart_device::heart_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock)
    : device_t(mconfig, HEART, tag, owner, clock)
{
}

void heart_device::map(address_map &map)
{
    map(0x000000, 0x3fffff).lrw32(
        NAME([this](offs_t offset) -> uint32_t {
            logerror("[HEART] Read at offset %08x\n", offset << 2);
            return 0;
        }),
        NAME([this](offs_t offset, uint32_t data) {
            logerror("[HEART] Write at offset %08x = %08x\n", offset << 2, data);
        })
    );
}
