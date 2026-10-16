#!/bin/sh
# Fetch a firmware payload over plain HTTP and optionally hand it to
# platform-sd-update.
#
# The download always lands in a .partial file first; a failed download
# removes that file and exits non-zero without running any updater. After a
# successful download the script only performs a dry-run update check unless
# --write is given. The script never reboots the board.
#
# usage: board-fetch-update.sh HTTP_URL [OUTPUT] [--write]

set -eu

DEVICE=/dev/mmcblk0p1
MAX_BYTES=33554432

usage() {
    cat <<'EOF'
usage: board-fetch-update.sh HTTP_URL [OUTPUT] [--write]

  HTTP_URL   http:// URL of the firmware payload
  OUTPUT     destination file (default /tmp/fw_payload.bin)
  --write    run the real update instead of a dry run

Only http:// URLs are accepted. The update target defaults to
/dev/mmcblk0p1 and can be overridden with --device.
EOF
}

URL=
OUTPUT=/tmp/fw_payload.bin
POSITIONAL=0
WRITE=0

while [ "$#" -gt 0 ]; do
    case "$1" in
        --help|-h)
            usage
            exit 0
            ;;
        --write)
            WRITE=1
            shift
            ;;
        --device)
            [ "$#" -ge 2 ] || { echo "board-fetch-update: --device needs an argument" >&2; exit 2; }
            DEVICE=$2
            shift 2
            ;;
        -*)
            echo "board-fetch-update: unknown option: $1" >&2
            usage >&2
            exit 2
            ;;
        *)
            POSITIONAL=$((POSITIONAL + 1))
            if [ "$POSITIONAL" -eq 1 ]; then
                URL=$1
            elif [ "$POSITIONAL" -eq 2 ]; then
                OUTPUT=$1
            else
                echo "board-fetch-update: too many arguments" >&2
                exit 2
            fi
            shift
            ;;
    esac
done

[ -n "$URL" ] || { usage >&2; exit 2; }

case "$URL" in
    http://?*)
        ;;
    *)
        echo "board-fetch-update: only http:// URLs are accepted: $URL" >&2
        exit 2
        ;;
esac

case "$URL" in
    *" "*)
        echo "board-fetch-update: URL must not contain spaces" >&2
        exit 2
        ;;
esac

command -v wget >/dev/null 2>&1 || { echo "board-fetch-update: wget not found" >&2; exit 1; }
command -v platform-sd-update >/dev/null 2>&1 ||
    { echo "board-fetch-update: platform-sd-update not found" >&2; exit 1; }

OUTPUT_DIR=$(dirname "$OUTPUT")
[ -d "$OUTPUT_DIR" ] || { echo "board-fetch-update: $OUTPUT_DIR does not exist" >&2; exit 1; }

PARTIAL="$OUTPUT.partial"
rm -f "$PARTIAL"

echo "board-fetch-update: downloading $URL"
if ! wget -O "$PARTIAL" "$URL"; then
    rm -f "$PARTIAL"
    echo "board-fetch-update: download failed; no update attempted" >&2
    exit 1
fi

if [ ! -s "$PARTIAL" ]; then
    rm -f "$PARTIAL"
    echo "board-fetch-update: download produced an empty file; no update attempted" >&2
    exit 1
fi

SIZE=$(wc -c < "$PARTIAL" | tr -d ' ')
if [ "$SIZE" -gt "$MAX_BYTES" ]; then
    rm -f "$PARTIAL"
    echo "board-fetch-update: payload is $SIZE bytes, larger than $MAX_BYTES" >&2
    exit 1
fi

mv "$PARTIAL" "$OUTPUT"
echo "board-fetch-update: downloaded $SIZE bytes to $OUTPUT"

if [ "$WRITE" -ne 1 ]; then
    echo "board-fetch-update: DRY-RUN update check (pass --write to flash)"
    platform-sd-update --device "$DEVICE" --image "$OUTPUT" --dry-run
    exit 0
fi

platform-sd-update --device "$DEVICE" --image "$OUTPUT"
echo "board-fetch-update: update applied; reboot the board manually when ready"
