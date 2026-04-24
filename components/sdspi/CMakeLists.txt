idf_component_register(
    SRCS "sdspi.c"
    INCLUDE_DIRS "."
    REQUIRES fatfs sd_card
    WHOLE_ARCHIVE
)