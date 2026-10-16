printf 'R111_UNAME='; uname -r; printf 'R111_BOOT_ID='; cat /proc/sys/kernel/random/boot_id; /tmp/r107-stage111/idma-finite --caps --device /dev/idma0; printf 'R111_LOG_TAIL\n'; dmesg | tail -n 10
