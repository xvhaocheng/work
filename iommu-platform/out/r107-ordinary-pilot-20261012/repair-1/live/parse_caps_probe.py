#!/usr/bin/env python3
"""Parse actual framed CAPS and boot evidence without opening any device."""
import argparse
import json
from pathlib import Path
import re


def parse_probe(record):
    if record.get('UART_opened') is not True or record.get('board_command_exit_code') != 0:
        raise ValueError('actual UART command did not return successfully')
    begin = record.get('dell_monotonic_start_ns')
    end = record.get('dell_monotonic_end_ns')
    if (type(begin) is not int or type(end) is not int or not 0 < begin < end):
        raise ValueError('independent Dell command brackets are missing')
    raw = record.get('board_stdout')
    if not isinstance(raw, str):
        raise ValueError('original framed CAPS stdout missing')
    lines = raw.replace('\r', '').splitlines()
    fields, caps = {}, []
    for line in lines:
        match = re.fullmatch(r'(R111_UNAME|R111_BOOT_ID)=(.+)', line)
        if match:
            if match[1] in fields:
                raise ValueError('duplicate live boot field')
            fields[match[1]] = match[2]
        elif line.startswith('{'):
            data = json.loads(line)
            if data.get('kind') == 'caps':
                caps.append(data)
    if len(caps) != 1 or set(fields) != {'R111_UNAME', 'R111_BOOT_ID'}:
        raise ValueError('complete unique CAPS/boot evidence missing')
    if not re.fullmatch(r'[0-9a-f]{8}(?:-[0-9a-f]{4}){3}-[0-9a-f]{12}', fields['R111_BOOT_ID']):
        raise ValueError('invalid actual boot UUID')
    c = caps[0]
    if (c.get('schema') != 'r107-ordinary-finite-pilot-receipt-v1' or
            c['ioctl'].get('attempted') is not True or
            c['ioctl'].get('return') != 0 or c['ioctl'].get('errno') != 0):
        raise ValueError('actual CAPS ioctl failed or foreign schema')
    return {'actual_UART_command_succeeded': True,
            'boot_ID': fields['R111_BOOT_ID'], 'kernel_release': fields['R111_UNAME'],
            'caps_JSON': c, 'dell_monotonic_start_ns': begin,
            'dell_monotonic_end_ns': end,
            'dell_begin_ns': begin, 'dell_end_ns': end,
            'scientific_or_frequency_acceptance': 'requires separate raw identity/timer/RUN checks'}


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('record', type=Path)
    p.add_argument('--result', type=Path, required=True)
    a = p.parse_args()
    r = parse_probe(json.loads(a.record.read_text()))
    r['original_record'] = str(a.record.resolve())
    a.result.write_text(json.dumps(r, indent=2) + '\n')
    print(json.dumps(r, indent=2))


if __name__ == '__main__':
    main()
