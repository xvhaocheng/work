test ! -e /tmp/r107-stage111/fw_payload-stage111-r1.bin &&
wget -Y off -T 60 -q -O /tmp/r107-stage111/fw_payload-stage111-r1.bin http://192.168.77.1:18080/fw_payload-stage111-r1.bin &&
printf 'R111_R1_PAYLOAD_SHA='; sha256sum /tmp/r107-stage111/fw_payload-stage111-r1.bin
printf 'R111_R1_PAYLOAD_BYTES='; wc -c < /tmp/r107-stage111/fw_payload-stage111-r1.bin
printf 'R111_UPDATER_SHA='; sha256sum /usr/bin/platform-sd-update
platform-sd-update --device /dev/mmcblk0p1 --image /tmp/r107-stage111/fw_payload-stage111-r1.bin --dry-run
