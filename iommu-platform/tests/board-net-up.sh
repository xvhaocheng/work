#!/bin/sh
# Configure exactly one board network interface with static addressing.
#
# The script never starts a DHCP client, never reconfigures an interface that
# was not named on the command line, and without --apply it only reports the
# commands it would run.
#
# usage: board-net-up.sh IFNAME --addr ADDR/PREFIX [--gw ADDR] [--apply]

set -eu

usage() {
    cat <<'EOF'
usage: board-net-up.sh IFNAME --addr ADDR/PREFIX [--gw ADDR] [--apply]

  IFNAME           interface to configure, e.g. eth0
  --addr ADDR/LEN  static IPv4 address with prefix length
  --gw ADDR        optional default gateway on that interface
  --apply          run the commands (default: dry run only)

The script refuses to touch any interface other than IFNAME and never starts
a DHCP client.
EOF
}

IFNAME=
ADDR=
GW=
APPLY=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --help|-h)
            usage
            exit 0
            ;;
        --addr)
            [ "$#" -ge 2 ] || { echo "board-net-up: --addr needs an argument" >&2; exit 2; }
            ADDR=$2
            shift 2
            ;;
        --gw)
            [ "$#" -ge 2 ] || { echo "board-net-up: --gw needs an argument" >&2; exit 2; }
            GW=$2
            shift 2
            ;;
        --apply)
            APPLY=1
            shift
            ;;
        -*)
            echo "board-net-up: unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
        *)
            if [ -n "$IFNAME" ]; then
                echo "board-net-up: interface given twice" >&2
                exit 2
            fi
            IFNAME=$1
            shift
            ;;
    esac
done

[ -n "$IFNAME" ] || { usage >&2; exit 2; }
[ -n "$ADDR" ] || { echo "board-net-up: --addr is required" >&2; exit 2; }

case "$IFNAME" in
    *[!A-Za-z0-9_.-]*)
        echo "board-net-up: illegal interface name: $IFNAME" >&2
        exit 2
        ;;
esac

case "$ADDR" in
    *[!0-9./]*)
        echo "board-net-up: illegal address: $ADDR" >&2
        exit 2
        ;;
esac

if [ -n "$GW" ]; then
    case "$GW" in
        *[!0-9.]*)
            echo "board-net-up: illegal gateway: $GW" >&2
            exit 2
            ;;
    esac
fi

command -v ip >/dev/null 2>&1 || { echo "board-net-up: ip not found" >&2; exit 1; }
ip link show dev "$IFNAME" >/dev/null 2>&1 ||
    { echo "board-net-up: interface $IFNAME does not exist" >&2; exit 1; }

echo "board-net-up: interface $IFNAME present"
echo "board-net-up: current addresses:"
ip -o addr show dev "$IFNAME" || true

if [ "$APPLY" -ne 1 ]; then
    echo "board-net-up: DRY-RUN (pass --apply to change state)"
    echo "board-net-up: would run: ip link set dev $IFNAME up"
    echo "board-net-up: would run: ip addr replace $ADDR dev $IFNAME"
    if [ -n "$GW" ]; then
        echo "board-net-up: would run: ip route replace default via $GW dev $IFNAME"
    fi
    exit 0
fi

ip link set dev "$IFNAME" up
ip addr replace "$ADDR" dev "$IFNAME"
if [ -n "$GW" ]; then
    ip route replace default via "$GW" dev "$IFNAME"
fi

# Read back the requested address so a silently ignored configuration fails.
if ! ip -o addr show dev "$IFNAME" | grep -F " $ADDR " >/dev/null 2>&1; then
    echo "board-net-up: FAIL: $ADDR is not present on $IFNAME" >&2
    exit 1
fi

echo "board-net-up: PASS: $IFNAME is configured with $ADDR"
