# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from watchdog.pro.

#####################################################################
## watchdog Binary:
#####################################################################

qt_internal_add_executable(watchdog
    NO_INSTALL # special case
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} # special case
    SOURCES
        tst_watchdog.cpp
    LIBRARIES
        Qt::Test
)

#### Keys ignored in scope 1:.:.:watchdog.pro:<TRUE>:
# QT_TOOL_ENV = "checkenv"
# checkenv.name = "QTEST_FUNCTION_TIMEOUT"
# checkenv.value = "100"

## Scopes:
#####################################################################

# special case begin
qt_internal_apply_testlib_coverage_options(watchdog)
# special case end
