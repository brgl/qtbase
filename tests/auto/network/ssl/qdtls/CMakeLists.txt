# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from qdtls.pro.

#####################################################################
## tst_qdtls Test:
#####################################################################

# Collect test data
list(APPEND test_data "certs")

qt_internal_add_test(tst_qdtls
    SOURCES
        tst_qdtls.cpp
    LIBRARIES
        Qt::NetworkPrivate
    TESTDATA ${test_data}
)

## Scopes:
#####################################################################

#### Keys ignored in scope 3:.:.:qdtls.pro:(CMAKE_BUILD_TYPE STREQUAL Debug):
# DESTDIR = "debug"

#### Keys ignored in scope 4:.:.:qdtls.pro:else:
# DESTDIR = "release"
