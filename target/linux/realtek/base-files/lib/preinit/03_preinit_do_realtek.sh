#!/bin/sh

do_realtek() {
	. /lib/realtek.sh

	realtek_board_detect
}

boot_hook_add preinit_main do_realtek
