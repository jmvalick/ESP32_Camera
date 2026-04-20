idf_component_register(
    SRCS "tcp.c"
    INCLUDE_DIRS "."
    REQUIRES  unity nvs_flash esp_netif driver
)
