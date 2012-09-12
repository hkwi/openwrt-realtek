PART_NAME=firmware
platform_check_image() {
	[ "$ARGC" -gt 1 ] && return 1

	#case "$(get_magic_word "$1")" in
	#	# u-boot
	#	2705) return 0;;
	#	*)
	#		echo "Invalid image type. Please use only u-boot files"
	#		return 1
	#	;;
	#esac
	return 0
}

# use default for platform_do_upgrade()
platform_do_upgrade() {
	default_do_upgrade "$ARGV"
}
