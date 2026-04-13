idf_component_register(
    SRCS "tcp_demo.c"
    INCLUDE_DIRS "."
    PRIV_REQUIRES  unity nvs_flash esp_netif
)
