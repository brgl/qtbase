# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from paster.pro.

#####################################################################
## paster Binary:
#####################################################################

# special case begin
set(args OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
# special case end

qt_internal_add_executable(paster
    ${args} # special case
    SOURCES
        main.cpp
    LIBRARIES
        Qt::Gui
)

## Scopes:
#####################################################################

#### Keys ignored in scope 2:.:.:paster.pro:WIN32:
# DESTDIR = "../paster"
