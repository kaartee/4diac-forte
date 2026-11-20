#############################################################################
# Copyright (c) 2023, 2026 KT Elektronik GmbH
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://eclipse.org.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Dirk Kaar - initial API and implementation and/or initial documentation
#############################################################################

if (NOT FORTE_ARCHITECTURE STREQUAL "Zephyr")
    return()
endif ()

option(FORTE_MODULE_ZEPHYR "Support for Zephyr OS I/O" OFF)
option(FORTE_MODULE_ZEPHYR_TEMPLATE "Support for the Zephyr template board" OFF)
option(FORTE_MODULE_ESP32ETHERNETKIT "Support for the Esp32EthernetKit board" OFF)

# Automatically enable base Zephyr I/O support if any Zephyr board FB is selected
if (FORTE_MODULE_ZEPHYR_TEMPLATE OR FORTE_MODULE_ESP32ETHERNETKIT)
    set(FORTE_MODULE_ZEPHYR ON CACHE BOOL "Support for Zephyr OS I/O" FORCE)
endif ()

if (NOT FORTE_MODULE_ZEPHYR)
    return()
endif ()

add_library(forte-zephyr)

target_sources(forte-zephyr
    PRIVATE
        types/src/ZephyrIOBase.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS types/include
        FILES
            types/include/forte/ZephyrIOBase.h
)

if (FORTE_MODULE_ZEPHYR_TEMPLATE)
    target_sources(forte-zephyr
        PRIVATE
            types/src/ZephyrIO_fbt.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS types/include
            FILES
                types/include/forte/ZephyrIO_fbt.h
    )
endif ()

if (FORTE_MODULE_ESP32ETHERNETKIT)
    target_sources(forte-zephyr
        PRIVATE
            types/src/Esp32EthernetKitIO_fbt.cpp
        PUBLIC
            FILE_SET HEADERS
            BASE_DIRS types/include
            FILES
                types/include/forte/Esp32EthernetKitIO_fbt.h
    )
endif ()

target_link_libraries(forte-zephyr PUBLIC forte-core)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-zephyr,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-zephyr>>)

add_subdirectory(handler)
add_subdirectory(handle)

install(TARGETS forte-zephyr EXPORT forte-export FILE_SET HEADERS)
