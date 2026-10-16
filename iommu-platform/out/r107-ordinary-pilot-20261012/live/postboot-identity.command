printf 'R111_UNAME='; uname -r
printf 'R111_BOOT_ID='; cat /proc/sys/kernel/random/boot_id
printf 'R111_ARCH='; uname -m
printf 'R111_TAINT='; cat /proc/sys/kernel/tainted
printf 'R111_CONFIG_SHA='; zcat /proc/config.gz | sha256sum
printf 'R111_NOTES_SHA='; sha256sum /sys/kernel/notes
printf 'R111_DTB_SHA='; sha256sum /sys/firmware/fdt
printf 'R111_CPU_ONLINE='; cat /sys/devices/system/cpu/online
task_cdev_count=0
for task_index in 0 1 2 3; do
    test -c /dev/idma${task_index} && task_cdev_count=$((task_cdev_count + 1))
done
printf 'R111_IDMA_CDEV_COUNT=%s\n' "$task_cdev_count"
printf 'R111_POSTBOOT_IDENTITY_END\n'
