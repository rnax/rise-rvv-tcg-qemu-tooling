# Makefile to generate vstart tests
#
# Copyright (C) 2024 SiFive Limited
# Contributor Max Chou <max.chou@sifive.com>

QEMU=qemu-riscv64
CC=riscv64-unknown-linux-gnu-gcc
CFLAGS=-march=rv64gcv -O0 -static

.PHONY: all
all: test_vstart_us

run: test_vstart_us
	${QEMU} -cpu max,vlen=128 test_vstart_us

test_vstart_us: unit_stride_vstart.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONE: clean
clean:
	$(RM) test_vstart_us
