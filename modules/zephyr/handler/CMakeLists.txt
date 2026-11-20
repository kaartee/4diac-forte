# ############################################################################
# Copyright (c) 2023 KT Elektronik GmbH This program and the accompanying
# materials are made available under the terms of the Eclipse Public License
# 2.0 which is available at http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Dirk Kaar - initial API and implementation and/or initial
# documentation
# ############################################################################

target_include_directories(forte-zephyr PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(forte-zephyr PRIVATE
    IODeviceController.h
    IODeviceController.cpp
)
