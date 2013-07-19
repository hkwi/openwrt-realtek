. /usr/share/libubox/jshn.sh

ret=1

s=`cat /usr/share/3gmodem/modem.json`
json_load "$s"
#echo "JSON file loaded"

#echo "modemname params $1--$2---$3"

json_select data
__idx=1
#json_select  "$((__idx))"
while json_is_a "$__idx" object; do
	json_select  "$((__idx++))"
	json_get_var var1 usbid
	json_get_var var2 manufacture
	json_get_var var3 model
	json_get_var var4 serialportnum
	json_get_var var5 serialport
	json_get_var var6 cmds
	json_get_var var7 reset
	if [ "$var1" != "" ] && \
		[ "$var2" == "$1" ] && \
		[ "$var3" == "$2" ]; then
		echo $var1","$var2","$var3","$var4","$var5","$var6","$var7 
		#echo "-------------------------------------------------"
		ret=0
	fi
	json_select ".."
done 

exit $ret
