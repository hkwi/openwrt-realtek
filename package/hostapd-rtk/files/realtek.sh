#!/bin/sh
append DRIVERS "realtek"

find_realtek_phy() {
	local device="$1"

	local macaddr="$(config_get "$device" macaddr | tr 'A-Z' 'a-z')"
	config_get phy "$device" phy
	[ -z "$phy" -a -n "$macaddr" ] && {
		cd /sys/class/net
		for phy in $(ls -d wlan[0-9] 2>&-); do
			[ "$macaddr" = "$(cat /sys/class/net/${phy}/address)" ] || continue
			config_set "$device" phy "$phy"
			break
		done
		config_get phy "$device" phy
	}
	[ -n "$phy" -a -d "/sys/class/net/$phy" ] || {
		echo "phy for wifi device $1 not found"
		return 1
	}
	[ -z "$macaddr" ] && {
		config_set "$device" macaddr "$(cat /sys/class/net/${phy}/address)"
	}
	return 0
}

scan_realtek() {
	local device="$1"
	local mainvif
	local wds

	[ ${device%[0-9]} = "wlan" ] && config_set "$device" phy "$device" || find_realtek_phy "$device" || {
		config_unset "$device" vifs
		return 0
	}
	config_get phy "$device" phy

	config_get vifs "$device" vifs
	local _c=0
	for vif in $vifs; do
		config_get_bool disabled "$vif" disabled 0
		[ $disabled = 0 ] || continue

		config_get mode "$vif" mode
		case "$mode" in
			adhoc|sta|ap|monitor)
				# Only one vif is allowed on AP, station, Ad-hoc or monitor mode
				[ -z "$mainvif" ] && {
					mainvif="$vif"
					config_set "$vif" ifname "$phy"
				}
			;;
			wds)
				config_get ssid "$vif" ssid
				[ -z "$ssid" ] && continue
				config_set "$vif" ifname "${phy}wds${_c}"
				_c=$(($_c + 1))
				addr="$ssid"
				${addr:+append wds "$vif"}
			;;
			*) echo "$device($vif): Invalid mode, ignored."; continue;;
		esac
	done
	config_set "$device" vifs "${mainvif:+$mainvif }${wds:+$wds}"
}

disable_realtek() (
	local device="$1"

	find_realtek_phy "$device" || return 0
	config_get phy "$device" phy

	echo disable_realtek phy=$phy device=$device

	#doesn't work?
	set_wifi_down "$device"

	include /lib/network
#	while read line < //${phy}/wds; do
#		set $line
#		[ -f "/var/run/wifi-${1}.pid" ] &&
#			kill "$(cat "/var/run/wifi-${1}.pid")"
#		ifconfig "$1" down
#		unbridge "$1"
#		iwpriv "$phy" wds_del "$2"
#	done

	echo unbridge "$phy"
	unbridge "$phy"
	ifconfig "$phy" down

	return 0
)

enable_realtek() {
	local device="$1"

	find_realtek_phy "$device" || return 0
	config_get phy "$device" phy

	config_get channel "$device" channel
	[ -n "$channel" ] && iwconfig "$phy" channel "$channel" >/dev/null 2>/dev/null


	config_get vifs "$device" vifs
	local first=1
	echo device=$device vifs=$vifs
	for vif in $vifs; do
		config_get ifname "$vif" ifname
		config_get ssid "$vif" ssid
		config_get mode "$vif" mode

		echo vif=$vif ifname=$ifname

		[ "$mode" = "wds" ] || iwconfig "$phy" essid ${ssid:+-- }"${ssid:-any}"

		case "$mode" in
			sta)
				iwconfig "$phy" mode managed
				config_get addr "$device" bssid
				[ -z "$addr" ] || {
					iwconfig "$phy" ap "$addr"
				}
			;;
			ap) iwconfig "$phy" mode master;;
			wds) iwpriv "$phy" wds_add "$ssid";;
			adhoc) iwconfig "$phy" mode ad-hoc;;
			*) iwconfig "$phy" mode "$mode";;
		esac

		[ "$first" = 1 ] && {
			config_get rate "$vif" rate
			[ -n "$rate" ] && iwconfig "$phy" rate "${rate%%.*}"

			config_get_bool hidden "$vif" hidden 0
			iwpriv "$phy" set_mib hiddenAP="$hidden"

			config_get frag "$vif" frag
			[ -n "$frag" ] && iwconfig "$phy" frag "${frag%%.*}"

			config_get rts "$vif" rts
			[ -n "$rts" ] && iwconfig "$phy" rts "${rts%%.*}"

			config_get maclist "$vif" maclist
			[ -n "$maclist" ] && {
				# flush MAC list
				iwpriv "$phy" maccmd 3
				for mac in $maclist; do
					iwpriv "$phy" addmac "$mac"
				done
			}
			config_get macpolicy "$vif" macpolicy
			case "$macpolicy" in
				allow)
					iwpriv "$phy" maccmd 2
				;;
				deny)
					iwpriv "$phy" maccmd 1
				;;
				*)
					# default deny policy if mac list exists
					[ -n "$maclist" ] && iwpriv "$phy" maccmd 1
				;;
			esac
			# kick all stations if we have policy explicitly set
			[ -n "$macpolicy" ] && iwpriv "$phy" maccmd 4
		}

		config_get enc "$vif" encryption
		case "$enc" in
			WEP|wep)
				for idx in 1 2 3 4; do
					config_get key "$vif" "key${idx}"
					iwconfig "$ifname" enc "[$idx]" "${key:-off}"
				done
				config_get key "$vif" key
				key="${key:-1}"
				case "$key" in
					[1234]) iwconfig "$ifname" enc "[$key]";;
					*) iwconfig "$ifname" enc "$key";;
				esac
			;;
			psk*|wpa*)
				start_hostapd=1
				config_get key "$vif" key
			;;
		esac

		local net_cfg bridge
		net_cfg="$(find_net_config "$vif")"
		echo net_cfg=$net_cfg > /dev/ttyS0
		# dirty workaround for wlan0 not appearing when router is started
		bridge="br-lan"

		# set mac address from /etc/config/wireless
		local macaddr
		config_get macaddr "$device" macaddr
		[ -z "$macaddr" ] || ifconfig "$device" hw ether "$macaddr"

		# set led to link/tx/rx (data,management) mode
		# have to be here because router will lock in wlan0 will be up durig this
		iwpriv "$ifname" set_mib led_type=11

		sleep 4
		brctl show > /dev/ttyS0
		[ -z "$net_cfg" ] || {
			#bridge="$(bridge_interface "$net_cfg")"
			echo vif=$vif bridge=$bridge > /dev/ttyS0
			echo tree config_set "$vif" bridge "$bridge" > /dev/ttyS0
			config_set "$vif" bridge "$bridge"
			echo house start_net "$ifname" "$net_cfg" > /dev/ttyS0

			set | grep CONFIG_ > /dev/ttyS0
			start_net "$ifname" "$net_cfg"
		}
		sleep 1
		brctl show > /dev/ttyS0

		# start hostapd anyway for now, it's much more stable than native ap mode
		start_hostapd=1

		[ -n "$start_hostapd" ] || {
			local htmode
			config_get htmode "$device" htmode
			case "$htmode" in
				*HT40-*)
					iwpriv "$ifname" set_mib use40M=1
					iwpriv "$ifname" set_mib shortGI40M=1
					iwpriv "$ifname" set_mib 2ndchoffset=1
				;;
				*HT40+*)
					iwpriv "$ifname" set_mib use40M=1
					iwpriv "$ifname" set_mib shortGI40M=1
					iwpriv "$ifname" set_mib 2ndchoffset=2
				;;
			esac

			# for 40MHz only mode
			#TODO: enable after realtek merge with hostapd?
			#iwpriw "$ifname" set_mib coexist=0

			# enable Space-Time Block Coding for better throughput
			#iwpriv "$ifname" set_mib stbc=1

			#sleep 1
			ifconfig "$ifname" up
		}

		#doesn't work?
		set_wifi_up "$vif" "$ifname"

		case "$mode" in
			ap)
				if [ -n "$start_hostapd" ] && eval "type hostapd_setup_vif" 2>/dev/null >/dev/null; then
					hostapd_setup_vif "$vif" realtek || {
						echo "enable_realtek($device): Failed to set up hostapd for interface $ifname" >&2
						# make sure this wifi interface won't accidentally stay open without encryption
						ifconfig "$ifname" down
						continue
					}
				fi
			;;
			wds|sta)
				if eval "type wpa_supplicant_setup_vif" 2>/dev/null >/dev/null; then
					wpa_supplicant_setup_vif "$vif" realtek || {
						echo "enable_realtek($device): Failed to set up wpa_supplicant for interface $ifname" >&2
						ifconfig "$ifname" down
						continue
					}
				fi
			;;
		esac
		first=0

		# enable client-to-client connections
		iwpriv "$ifname" set_mib block_relay=0
	done

}

check_realtek_device() {
	[ ${1%[0-9]} = "wlan" ] && config_set "$1" phy "$1"
	config_get phy "$1" phy
	[ -z "$phy" ] && {
		find_realtek_phy "$1" >/dev/null || return 0
		config_get phy "$1" phy
	}
	[ "$phy" = "$dev" ] && found=1
}

detect_realtek() {
	devidx=0
	config_load wireless
	while :; do
		config_get type "wlan$devidx" type
		[ -n "$type" ] || break
		devidx=$(($devidx + 1))
	done
	cd /sys/class/net/
	[ -d wlan[0-9] ] || return
	for dev in $(ls -d wlan[0-9] 2>&-); do
		found=0
		config_foreach check_realtek_device wifi-device
		[ "$found" -gt 0 ] && continue
		cat <<EOF
config wifi-device wlan$devidx
	option type	realtek
	option channel  11
	option macaddr	$(rtkmib --get wmac0)
	#TODO: enable after realtek merge with hostapd
	#option hwmode	11ng
	option htmode	HT40-

	# UNCOMMENT THIS LINE TO DISABLE WIFI:
	# option disabled 1

config wifi-iface
	option device	wlan$devidx
	option network	lan
	option mode	ap
	option ssid	OpenWrt
	option encryption none

EOF
	devidx=$(($devidx + 1))
	done
}
