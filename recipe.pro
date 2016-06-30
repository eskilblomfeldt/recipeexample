QT += qml quick sql androidextras

CONFIG += c++11

SOURCES += main.cpp \
    recipemodel.cpp \
    recipe.cpp \
    foodimageprovider.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    recipemodel.h \
    recipe.h \
    foodimageprovider.h

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
