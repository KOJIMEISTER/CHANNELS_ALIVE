QT += quick quickcontrols2 sql network

android: include(./android_openssl/openssl.pri)

SOURCES += \
        ../../../libs/Qt-AES-1.2/qaesencryption.cpp \
        backapi.cpp \
        channel.cpp \
        databaseio.cpp \
        main.cpp \
        notificationclient.cpp \
        tokenrecieveserver.cpp \
        twitchapi.cpp

resources.files = main.qml
resources.prefix = /$${TARGET}
RESOURCES += resources

RESOURCES += \
        qtquickcontrols2.conf \
        pages/AddChannelPage.qml \
        pages/ChannelsPage.qml \
        pages/MainPage.qml \
        icons \
        imgs \

INCLUDEPATH += \
        C:/C++/libs/Qt-AES-1.2/

target.path = C:/C++/Projects/CHANNELSALIVE/CHANNELSALIVE/
INSTALLS += target

HEADERS += \
        ../../../libs/Qt-AES-1.2/qaesencryption.h \
        backapi.h \
        channel.h \
        databaseio.h \
        main.h \
        notificationclient.h \
        tokenrecieveserver.h \
        twitchapi.h

DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle.properties \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/res/values/libs.xml \
        android/src/NotificationClient.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
