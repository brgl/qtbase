# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from ico.pro.

#####################################################################
## QICOPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QICOPlugin
    OUTPUT_NAME qico
    PLUGIN_TYPE imageformats
    SOURCES
        main.cpp
        qicohandler.cpp qicohandler.h
    LIBRARIES
        Qt::Core
        Qt::CorePrivate
        Qt::Gui
)

#### Keys ignored in scope 1:.:.:ico.pro:<TRUE>:
# OTHER_FILES = "ico.json"
