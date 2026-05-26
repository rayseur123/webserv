#!/usr/bin/env python3

import time
import sys

print("Content-Type: text/plain\r\n\r\n")
sys.stdout.flush()

i = 0

while True:
    print(f"Loop: {i}")
    sys.stdout.flush()
    i += 1
    time.sleep(1)