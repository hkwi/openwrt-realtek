#!/bin/sh

dev=`uci get query3g.dongle.device`
pin=`uci get query3g.dongle.pin`

uci set network.wan.proto=3g
uci set network.wan.ifname=ppp0
uci set network.wan.username=umts
uci set network.wan.password=umts
uci set network.wan.device=$dev
uci set network.wan.pin=$pin

uci commit network
