# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from qlalr.pro.

#####################################################################
## qlalr Tool:
#####################################################################

qt_get_tool_target_name(target_name qlalr)
qt_internal_add_tool(${target_name}
    TARGET_DESCRIPTION "Qt Look Ahead LR Parser Generator"
    INSTALL_DIR "${INSTALL_LIBEXECDIR}"
    TOOLS_TARGET Core # special case
    SOURCES
        compress.cpp compress.h
        cppgenerator.cpp cppgenerator.h
        dotgraph.cpp dotgraph.h
        grammar.cpp grammar_p.h
        lalr.cpp lalr.h
        main.cpp
        parsetable.cpp parsetable.h
        recognizer.cpp recognizer.h
    DEFINES
        QT_NO_FOREACH
    LIBRARIES
        Qt::Core # special case
)
qt_internal_return_unless_building_tools()

#### Keys ignored in scope 1:.:.:qlalr.pro:<TRUE>:
# OTHER_FILES = "lalr.g"
# QMAKE_TARGET_DESCRIPTION = "Qt Look Ahead LR Parser Generator"
# _OPTION = "host_build"
