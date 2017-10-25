######################################################################
# Automatically generated by qmake (3.1) Wed Oct 25 13:59:51 2017
######################################################################
QT += core gui widgets

TARGET = installer

TEMPLATE = app

SOURCES += main.cpp \
	container.cpp \
	main_window.cpp \
	..\share\crypto_key\crypto_key.cpp \
	..\share\crypto_hash\crypto_hash.cpp \
	..\share\crypto_provider\crypto_provider.cpp \
	..\share\windows_os\windows_os.cpp

HEADERS += main_window.h \
	container.h \
	..\share\crypto_key\crypto_key.h \
	..\share\crypto_hash\crypto_hash.h \
	..\share\crypto_provider\crypto_provider.h \
	..\share\windows_os\windows_os.h \
	..\share\error_codes\error_codes.h


unix|win32: LIBS += -LC:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/ -lnetapi32

INCLUDEPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include
DEPENDPATH += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/include

win32:!win32-g++: PRE_TARGETDEPS += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/netapi32.lib
else:unix|win32-g++: PRE_TARGETDEPS += C:/Qt/Tools/mingw530_32/i686-w64-mingw32/lib/libnetapi32.a

QMAKE_CXXFLAGS_RELEASE -= -O3

# Directories
