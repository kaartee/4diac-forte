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

option(FORTE_MODULE_ZEPHYR "Zephyr RTOS hardware I/O support" OFF)
option(FORTE_MODULE_ESP32ETHERNETKIT "Espressif ESP32-Ethernet-Kit I/O support" OFF)
option(FORTE_MODULE_ESP32C3DEVKITM "Espressif ESP32-C3-DevKitM-1 I/O support" OFF)

if (NOT FORTE_MODULE_ZEPHYR AND NOT FORTE_MODULE_ESP32ETHERNETKIT AND NOT FORTE_MODULE_ESP32C3DEVKITM)
    return()
endif ()

add_library(forte-zephyr STATIC)
set_property(GLOBAL APPEND PROPERTY FORTE_TARGETS forte-zephyr)
target_link_libraries(forte-zephyr PUBLIC forte-core)
target_link_libraries(forte PUBLIC forte-zephyr)
install(TARGETS forte-zephyr EXPORT forte-export)

target_include_directories(forte-zephyr PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/types/include>
)

target_include_directories(forte-zephyr PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/handler
        ${CMAKE_CURRENT_SOURCE_DIR}/handle
)

target_sources(forte-zephyr PRIVATE
        types/include/forte/ZephyrIOBase.h
        types/include/forte/ZephyrIO_fbt.h
        handler/IODeviceController.h
        handler/IOHandleDescriptor.h
        handler/IOHandleGPIODescriptor.h
        handler/IOHandleADCDescriptor.h
        handler/IOHandlePWMDescriptor.h
        handle/IOHandleGPIO.h
        handle/IOHandleADC.h
        handle/IOHandlePWM.h
        types/src/ZephyrIOBase.cpp
        types/src/ZephyrIO_fbt.cpp
        handler/IODeviceController.cpp
        handle/IOHandleGPIO.cpp
        handle/IOHandleADC.cpp
        handle/IOHandlePWM.cpp
)

if (FORTE_MODULE_ESP32ETHERNETKIT)
    target_sources(forte-zephyr PRIVATE
            types/include/forte/Esp32EthernetKitIO_fbt.h
            types/src/Esp32EthernetKitIO_fbt.cpp
    )
endif ()

if (FORTE_MODULE_ESP32C3DEVKITM)
    target_sources(forte-zephyr PRIVATE
            types/include/forte/Esp32C3DevkitMIO_fbt.h
            types/src/Esp32C3DevkitMIO_fbt.cpp
    )
endif ()
