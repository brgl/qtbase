# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from android.pro.

# special case skip regeneration
# special case begin
# Only build jars during Android build and install the templates for the
# androiddeployqt tool.
# android
if (ANDROID)
    add_subdirectory(jar)
    add_subdirectory(java)
    add_subdirectory(templates)
endif()
# special case end

