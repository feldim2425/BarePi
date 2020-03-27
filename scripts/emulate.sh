#! /bin/sh

VERSION=1
PORT=-1
DEBUGGING=0
SEMIHOST=0
BINFILE=""

set_port() {
	if [ $1 -le 0 ]; then 
		echo "Port has to be a positive integer!\n"
		usage
	fi
	PORT=$1
}

usage() {
	cat <<-EOF
	Usage: $0 [<options>] <binary>

	-h              Shows this help message
	-d              Opens a gdb debugging interface
	-p <port>       Sets the port of the debugging interface (Requires -d)      
	-r <version>    Select the raspberry version (default: 1)   
	-s              Enable the semihost interface
	EOF
	exit 2
}

while getopts 'hdp:r:s' opt
do
	case $opt in
		d) DEBUGGING=1 ;;
		p) set_port $OPTARG;;
		r) VERSION=$OPTARG ;;
		s) SEMIHOST=1 ;;
		h|?) usage ;; 
	esac
done

shift $((OPTIND-1))
BINFILE="$@"

if [ -z $BINFILE ]; then
	usage
elif [ ! -f $BINFILE ]; then
	echo "The binary file does not exist!"
	exit 1
fi

if [ $DEBUGGING -eq 0 ] && [ $PORT -gt 0 ]; then
	echo "Warning! Setting the debug port with \"-p\" requires \"-d\". Option will be ignored"
	PORT=-1
fi

QEMU_ARGS=""

echo "Raspberry Version: $VERSION (Currently unused WIP)"

if [ $DEBUGGING -ne 0 ]; then
	echo "Debugging: ENABLED"
	if [ $PORT -gt 0 ]; then
		echo "Debugging port set to $PORT"
		QEMU_ARGS="$QEMU_ARGS -gdb tcp::$PORT"
	else
		echo "Debugging port defaults to 1234"
		QEMU_ARGS="$QEMU_ARGS -s"
	fi
else
	echo "Debugging: DISABLED"
fi

if [ $SEMIHOST -ne 0 ]; then
	echo "Semihost interface: ENABLED"
	QEMU_ARGS="$QEMU_ARGS -semihosting"
else
	echo "Semihost interface: DISABLED"
fi

echo ""
echo "Starting ARM QEMU!"


QEMU_START_CMD="qemu-system-arm $QEMU_ARGS -M raspi2 -bios $BINFILE"
echo "$QEMU_START_CMD"
$($QEMU_START_CMD)


