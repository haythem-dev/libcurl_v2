######################################################################
# Narcotics common settings
# Frank Naumann
# 08.02.2012
######################################################################

######################################################################
# directories
######################################################################

unix {
	TCACCESS_VERSION        = 8.0.11.4
	SSUK_HOME				= ../../ext/ssuk
	BOOST_HOME				= ../../ext/boost
	LOG4CPLUS_HOME			= ../../ext/log4cplus
	TCACCESS_HOME			= $(DEVLIB_PATH)/marathon/lib/tcaccess/$${TCACCESS_VERSION}
}

win32 {
	SSUK_HOME				= ..\\..\\ext\\ssuk
}

######################################################################
# override QT settings
######################################################################

CONFIG						+= debug_and_release
CONFIG						+= build_all
CONFIG						+= warn_on

QT							-= gui

build_pass:CONFIG(debug, debug|release) {
	DBGDIR					= debug
}
else {
	DBGDIR					= release
	DEFINES					+= NDEBUG
}

unix {
	OBJECTS_DIR				= $${DBGDIR}
	DESTDIR					= $${DBGDIR}
	MOC_DIR					= ./generatedfiles
	UI_DIR					= ./generatedfiles
}

unix {
	QMAKE_CXXFLAGS		+= -std=c++98 -Werror -pedantic -Wno-long-long
}

######################################################################
# includes
######################################################################

INCS_BASAR					= $${SSUK_HOME} $${BOOST_HOME}/include $${SSUK_HOME}/basar/include $${LOG4CPLUS_HOME}/include

######################################################################
# libs
######################################################################

win32 {
	LIBS_DIR_BASAR		= -L$${SSUK_HOME}/basar/lib/$${DBGDIR}
	LIBS_BASAR			= libbasarcmnutil.lib libbasarproperty.lib libbasardbsql.lib libbasardbaspect.lib libbasarlogin.lib libbasarappl.lib 
	LIBS_LOG4CPLUS		= liblog4cplus.lib
}

unix {
	LIBS_DIR_TCACCESS	= -L$${TCACCESS_HOME}/lib
	LIBS_DIR_BASAR		= $${SSUK_HOME}/sharedlib/$${DBGDIR}
	LIBS_DIR_BOOST		= $${BOOST_HOME}/lib
	LIBS_LOG4CPLUS		= -L$${LOG4CPLUS_HOME}/lib/$${OBJECTS_DIR} -llog4cplus

	LIBS_TCACCESS		= $${LIBS_DIR_TCACCESS} -lBml -lbmlApi -lBmlCh
	LIBS_BASAR			= -Wl,-brtl -L$${LIBS_DIR_BASAR} -lbasarcmnutil -lbasarproperty -lbasardbsql -lbasardbaspect -lbasarlogin -lbasarappl
	LIBS_BOOST			= -Wl,-brtl -L$${LIBS_DIR_BOOST} -lboost_system -lboost_filesystem
}
