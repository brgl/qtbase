QT_FOR_CONFIG += widgets

TEMPLATE      = subdirs
SUBDIRS       = classwizard \
                extension \
                licensewizard \
                standarddialogs \
                tabdialog \
                trivialwizard

!qtHaveModule(printsupport): SUBDIRS -= licensewizard
!qtConfig(wizard) {
    SUBDIRS -= trivialwizard licensewizard classwizard
}
