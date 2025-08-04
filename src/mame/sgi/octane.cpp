// license:BSD-3-Clause
// copyright-holders:Ryan Holtz
/**********************************************************************

    SGI Octane workstation driver (WIP)

    GOAL: Boot SGI IP30 PROM and reach IRIX installer

    Memory map (early WIP):
    00000000 - 0fffffff : RAM (256MB)
    1fc00000 - 1fcfffff : Boot PROM
    1f800000 - 1fbfffff : HEART (stub)
    1f000000 - 1f3fffff : XBow (stub)
    1f120000 - 1f12ffff : IOC3 (stub)

**********************************************************************/

#include "emu.h"
#include "cpu/mips/mips3.h"
#include "machine/ram.h"
#include "machine/serial.h"
#include "machine/rs232.h"

#define LOG_UNKNOWN     (1U << 1)
#define LOG_ALL         (LOG_UNKNOWN)

#define VERBOSE         (0)
#include "logmacro.h"

namespace {

class octane_state : public driver_device
{
public:
	octane_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
	{
	}

	void octane(machine_config &config);

protected:
	void mem_map(address_map &map) ATTR_COLD;

	required_device<r10000be_device> m_maincpu;
};

void octane_state::mem_map(address_map &map)
{
	map(0x00000000, 0x0fffffff).ram(); // 256MB RAM
	map(0x1fc00000, 0x1fcfffff).rom().region("user1", 0); // Boot PROM

	// Stubbed devices with log-only handlers
	map(0x1f800000, 0x1fbfffff).lrw8([](address_space &space, offs_t offset) {
		logerror("HEART: Read @ %08x\n", offset);
		return uint8_t(0);
	});
	map(0x1f800000, 0x1fbfffff).lrw8([](address_space &space, offs_t offset, uint8_t data) {
		logerror("HEART: Write @ %08x = %02x\n", offset, data);
	});

	map(0x1f000000, 0x1f3fffff).lrw8([](address_space &space, offs_t offset) {
		logerror("XBOW: Read @ %08x\n", offset);
		return uint8_t(0);
	});
	map(0x1f000000, 0x1f3fffff).lrw8([](address_space &space, offs_t offset, uint8_t data) {
		logerror("XBOW: Write @ %08x = %02x\n", offset, data);
	});

	map(0x1f120000, 0x1f12ffff).lrw8([](address_space &space, offs_t offset) {
		logerror("IOC3: Read @ %08x\n", offset);
		return uint8_t(0);
	});
	map(0x1f120000, 0x1f12ffff).lrw8([](address_space &space, offs_t offset, uint8_t data) {
		logerror("IOC3: Write @ %08x = %02x\n", offset, data);
	});
}

static INPUT_PORTS_START( octane )
INPUT_PORTS_END

void octane_state::octane(machine_config &config)
{
	R10000BE(config, m_maincpu, 195'000'000);
	m_maincpu->set_icache_size(32768);
	m_maincpu->set_dcache_size(32768);
	m_maincpu->set_addrmap(AS_PROGRAM, &octane_state::mem_map);

	rs232_port_device &rs232(RS232_PORT(config, "rs232", default_rs232_devices, "terminal"));
	rs232.rxd_handler().set("ser", FUNC(serial_port_device::rx));

	serial_port_device &ser(SERIAL_PORT(config, "ser", 0));
	ser.tx_handler().set("rs232", FUNC(rs232_port_device::write_txd));
}

ROM_START( octane )
	ROM_REGION64_BE( 0x100000, "user1", 0 )
	ROMX_LOAD( "ip30prom.rev4.9.bin", 0x000000, 0x100000, CRC(10bafb52) SHA1(de250875c608add63749d3f9fb81a82cb58c3586), ROM_GROUPDWORD )
ROM_END

} // anonymous namespace

COMP( 1997, octane, 0, 0, octane, octane, octane_state, empty_init, "Silicon Graphics Inc", "Octane (Version 6.5 Rev 4.9 05/22/03)", MACHINE_NOT_WORKING | MACHINE_NO_SOUND )
