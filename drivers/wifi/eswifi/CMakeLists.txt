# SPDX-License-Identifier: Apache-2.0

if(CONFIG_WIFI_ESWIFI)
  zephyr_include_directories(./)

  zephyr_library_include_directories(
  # IP headers
  ${ZEPHYR_BASE}/subsys/net/ip
  )

  zephyr_sources(
    eswifi_core.c
    eswifi_bus_spi.c
    eswifi_offload.c
    eswifi_socket.c
    )
zephyr_sources_ifdef(CONFIG_NET_SOCKETS_OFFLOAD eswifi_socket_offload.c)
endif()
