# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from debugplugin.pro.

#####################################################################
## debugplugin Generic Library:
#####################################################################

qt_internal_add_cmake_library(debugplugin
    MODULE
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../plugins"
    SOURCES
        main.cpp
    LIBRARIES
        Qt::Core
)

#### Keys ignored in scope 1:.:.:debugplugin.pro:<TRUE>:
# TEMPLATE = "lib"

qt_autogen_tools_initial_setup(debugplugin)
