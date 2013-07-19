#!/bin/sh
DATA_DIR=./data
GCOM=comgt
GCOM_MODEMDETECT="/usr/share/3gmodem/model.gcom"
MODEM_INFO="modemname.sh"
RETURN_APN=$DATA_DIR/apnprovider
usbreset=/usr/bin/usbreset
GCOM_CMD="/tmp/cmds.gcom"

detect_model()
{
	$GCOM -d $1 $2 | tail -n 1
}

modem_info()
{
	$MODEM_INFO "$1" $2
}

gcomscr_start()
{
	rm $GCOM_CMD
	echo "opengt
 set com 115200n81
 set comecho off
 set senddelay 0.02
 waitquiet 0.2 0.2

 send \"AT^m\"
 waitfor 1 \"OK\",\"ERROR\" " >> $GCOM_CMD 
}

gcomscr_add_cmd()
{
	echo "
 let \$c=\"$1^m\"
 gosub readatcmdnr
" >> $GCOM_CMD
}

gcomscr_end()
{
	echo -e "
 exit 0
 
:readatcmdnr
 let i=10
 send \$c
:loop3
 get 1 \"^m\" \$s
 if len(\$s) < 2 goto loop5
 if \$mid(\$s,1,2) = \"ER\" goto loop4
 if \$mid(\$s,1,2) = \"OK\" goto loop5
 if \$mid(\$s,1,2) = \"AT\" goto loop5
 if \$mid(\$s,1,2) = \"TE\" goto loop5
 if \$mid(\$s,1,1) = \"+\"  goto loop5
 if \$mid(\$s,1,1) = \"\^\"  goto loop5
 let l=len(\$s)
 let \$s=\$mid(\$s,1,l)
 return

:loop4
 let \$s=\"\"
 return

:loop5
 if i = 0 return
 let i=i-1
 sleep 0.25
 goto loop3
 " >> $GCOM_CMD
}

gcomscr_run()
{
	$GCOM -d $1 $GCOM_CMD
}

skip_cycles=0
skip_passed=0
known_modems=""
for port in 0 1 2 3 4 5 6 7 8 9; do
	for tty in $(find /sys/devices/ -name "ttyUSB$port" -type d | sort -u); do
		[ -f "$tty/../../idProduct" ] || continue
		if [ $skip_cycles -gt 0 ]; then
			skip_cycles=$((skip_cycles-1))
			skip_passed=1
			continue
		fi
		dir="$(cd "$tty/../.."; pwd)"
		uid="$(basename "$dir")"
		dev="/dev/$(basename "$tty")"
		vid="$(cat "$tty/../../idVendor")"
		pid="$(cat "$tty/../../idProduct")"
		#echo $dir
		#echo $uid
		echo "DEVICE:"$dev
		#echo $vid":"$pid
		
		ret=$(detect_model "$dev" $GCOM_MODEMDETECT)
		#echo $ret
		manuf=$(echo $ret | cut -d',' -f1 | tr '[A-Z]' '[a-z]' )
		model=$(echo $ret | cut -d',' -f2 | tr '[A-Z]' '[a-z]' )
		serialnum=$(echo $ret | cut -d',' -f3 )
		echo "MODEL: "$model
		echo "MANUF: "$manuf
		echo "SERIAL:"$serialnum
		
		#FILE="/tmp/modem3g.$serialnum"
		#echo "FILE:"$FILE
		if [ -f $FILE ]; then
			found=1
		else
			found=0
		fi

		if [ "$model" != "" ] && \
			[ "$manuf" != "" ]; then
			if [ $found -eq 0 ]; then
				touch /tmp/modem3g.$serialnum
				manuf="${manuf//[[:space:]]/}"
				ret=$(modem_info "$manuf" $model)
				reti="$?"
				##echo "modem_info == "$ret
				#echo "reti"$reti
				if [ "$reti" == "0" ];then
					echo "FOUND 3G MODEM "$model" "$manuf"?"
					#echo "ret="$ret
					#read usbid TMP TMP serialportnum serialport cmds reset<<<$(IFS=",";echo $ret)
					tmp="/tmp/tmp.txt"
					echo "$ret" >> $tmp
					usbid=`awk -F "," '{print $1}' $tmp`
					serialportnum=`awk -F "," '{print $4}' $tmp`
					serialport=`awk -F "," '{print $5}' $tmp`
					cmds=`awk -F "," '{print $6}' $tmp`
					reset=`awk -F "," '{print $7}' $tmp`
					rm -f $tmp
					
					
					
					#echo usbid "$usbid"
					#echo serialportnum "$serialportnum"
					#echo serialport "$serialport"
					
					if [ "$reset" == "1" ]; then
						echo "RESET MODEM"
						$usbreset "$vid:$pid" >/dev/null
					fi
					
					#echo "cmds="$cmds
					if [ "$cmds" != "" ]; then
						
						gcomscr_start
						old_ifs="$IFS"
						IFS=$';'
						for cmd in $cmds; do
							#echo "CMD : "$cmd
							gcomscr_add_cmd $cmd
						done
						IFS="$old_ifs"
						gcomscr_end
						gcomscr_run $dev
					fi
				fi
			fi
		fi
	done
done
rm -f /tmp/modem3g.*

exit 1


