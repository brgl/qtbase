# Copyright (C) 2022 The Qt Company Ltd.
# SPDX-License-Identifier: BSD-3-Clause

# Generated from eglfs_kms_egldevice.pro.

#####################################################################
## QEglFSKmsEglDeviceIntegrationPlugin Plugin:
#####################################################################

qt_internal_add_plugin(QEglFSKmsEglDeviceIntegrationPlugin
    OUTPUT_NAME qeglfs-kms-egldevice-integration
    PLUGIN_TYPE egldeviceintegrations
    SOURCES
        qeglfskmsegldevice.cpp qeglfskmsegldevice.h
        qeglfskmsegldeviceintegration.cpp qeglfskmsegldeviceintegration.h
        qeglfskmsegldevicemain.cpp
        qeglfskmsegldevicescreen.cpp qeglfskmsegldevicescreen.h
    DEFINES
        QT_EGL_NO_X11
    LIBRARIES
        Libdrm::Libdrm
        Qt::Core
        Qt::CorePrivate
        Qt::EglFSDeviceIntegrationPrivate
        Qt::EglFsKmsSupportPrivate
        Qt::Gui
        Qt::GuiPrivate
        Qt::KmsSupportPrivate
)

#### Keys ignored in scope 1:.:.:eglfs_kms_egldevice.pro:<TRUE>:
# OTHER_FILES = "$$PWD/eglfs_kms_egldevice.json"
