# SPDX-License-Identifier: Apache-2.0

zephyr_library()

zephyr_library_sources_ifdef(CONFIG_AMG88XX amg88xx.c)
zephyr_library_sources_ifdef(CONFIG_AMG88XX_TRIGGER amg88xx_trigger.c)
