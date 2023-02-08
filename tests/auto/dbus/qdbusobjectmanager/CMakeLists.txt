# Copyright (C) 2023 Bartosz Golaszewski <brgl@bgdev.pl>
# SPDX-License-Identifier: BSD-3-Clause

#####################################################################
## tst_qdbusobjectmanager Test:
#####################################################################

qt_internal_add_test(tst_qdbusobjectmanager
    SOURCES
        tst_qdbusobjectmanager.cpp tst_qdbusobjectmanager.h
    LIBRARIES
        Qt::DBus
)
