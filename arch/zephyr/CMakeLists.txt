#/************************************************************************************
# Copyright (c) 2023 - 2024 KT Elektronik GmbH, Jose Cabral
# This program and the accompanying materials are made available under the
# terms of the Eclipse Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors:
#  Dirk Kaar - initial API and implementation and/or initial documentation
# ************************************************************************************/

forte_register_architecture(Zephyr)

# TODO: should be replaced by better arch-independent mechanism
target_include_directories(forte-global INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set(FORTE_LOGLEVEL "NOLOG" CACHE STRING "Loglevel to use" FORCE)

target_sources(forte-arch PRIVATE
    forte_architecture_time.cpp
    forte_thread.cpp forte_thread.h
    forte_sync.cpp forte_sync.h
    forte_sem.cpp forte_sem.h
    zephyrtiha.cpp zephyrtiha.h
    ../genforte_printer.cpp
    zephyrforte_fileio.cpp
    ../common/forte_specific_architecture.cpp
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../fdselecthand.cpp>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../fdselecthand.h>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../bsdsocketinterf.cpp>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../bsdsocketinterf.h>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/../gensockhand.h>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/sockhand.cpp>
    $<$<BOOL:${FORTE_COM_ETH}>:${CMAKE_CURRENT_SOURCE_DIR}/sockhand.h>
)
