#!/bin/sh

reset=/usr/bin/usbreset
comgt=/usr/bin/gcom
script=/usr/share/query3g.gcom

last_reset="x"

wan_proto="$(uci -q get network.wan.proto)"

if [ "$wan_proto" = "3g" ]; then
	ifdown wan
fi

for port in 0 1 2 3 4 5 6 7 8 9; do
	for tty in $(find /sys/devices/ -name "ttyUSB$port" -type d | sort -u); do
		[ -f "$tty/../../idProduct" ] || continue

		local dir="$(cd "$tty/../.."; pwd)"
		local uid="$(basename "$dir")"
		local dev="/dev/$(basename "$tty")"
		local vid="$(cat "$tty/../../idVendor")"
		local pid="$(cat "$tty/../../idProduct")"

		if [ "$last_reset" != "$vid:$pid" ]; then
			last_reset="$vid:$pid"
			$reset "$vid:$pid" >/dev/null

			local try=0
			while [ $((try++)) -lt 5 ] && [ ! -e "$dev" ]; do sleep 1; done 
		fi

		if $comgt -s "$script" -d "$dev" 2>/dev/null; then
			echo "PORT:$dev"
			echo "VID:$vid"
			echo "PID:$pid"
			echo "UID:$uid"
			exit 0
		fi
	done
done
exit 1
