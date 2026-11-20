#############################################################################
# Copyright (c) 2023 - 2025 KT Elektronik GmbH, Jose Cabral
#
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#   Dirk Kaar - initial API and implementation and/or initial documentation
#############################################################################

forte_register_architecture(Zephyr)

if(NOT "${FORTE_ARCHITECTURE}" STREQUAL "Zephyr")
    return()
endif()

set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)
set(FORTE_ZEPHYR_INCLUDES "" CACHE STRING "Folders to include from the Zephyr OS")

target_include_directories(forte-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(FORTE_ZEPHYR_INCLUDES)
    target_include_directories(forte-core SYSTEM PRIVATE ${FORTE_ZEPHYR_INCLUDES})
endif()

target_sources(forte-core PRIVATE
    include/forte/arch/forte_thread.h
    forte_thread.cpp
    include/forte/arch/forte_sync.h
    forte_sync.cpp
    include/forte/arch/forte_sem.h
    forte_sem.cpp
    include/forte/arch/zephyrtiha.h
    zephyrtiha.cpp
    forte_architecture_time.cpp
    zephyrforte_fileio.cpp
    ../common/src/forte_specific_architecture.cpp
    ../common/src/genforte_printer.cpp
)

set_property(TARGET forte-core APPEND PROPERTY FORTE_TIMER_HANDLERS "forte::arch::CZephyrTimerHandler:forte/arch/zephyrtiha.h")

if(FORTE_COM_ETH)
    target_sources(forte-core PRIVATE
        ../common/src/fdselecthand.cpp
        ../common/src/bsdsocketinterf.cpp
        include/forte/arch/sockhand.h
        sockhand.cpp
    )
    set_property(TARGET forte-core APPEND PROPERTY FORTE_HANDLERS "forte::arch::CFDSelectHandler:forte/arch/sockhand.h")
endif()
