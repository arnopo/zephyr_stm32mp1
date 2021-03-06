# SPDX-License-Identifier: Apache-2.0

set_ifndef(ESPRESSIF_TOOLCHAIN_PATH "$ENV{ESPRESSIF_TOOLCHAIN_PATH}")
set(       ESPRESSIF_TOOLCHAIN_PATH ${ESPRESSIF_TOOLCHAIN_PATH} CACHE PATH "")
assert(    ESPRESSIF_TOOLCHAIN_PATH "ESPRESSIF_TOOLCHAIN_PATH is not set")

set(TOOLCHAIN_HOME ${ESPRESSIF_TOOLCHAIN_PATH})

set(COMPILER gcc)
set(LINKER ld)
set(BINTOOLS gnu)

set(CROSS_COMPILE_TARGET xtensa-esp32-elf)
set(SYSROOT_TARGET       xtensa-esp32-elf)

set(CROSS_COMPILE ${TOOLCHAIN_HOME}/bin/${CROSS_COMPILE_TARGET}-)
set(SYSROOT_DIR   ${TOOLCHAIN_HOME}/${SYSROOT_TARGET})

set(TOOLCHAIN_HAS_NEWLIB ON CACHE BOOL "True if toolchain supports newlib")

