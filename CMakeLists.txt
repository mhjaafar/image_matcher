#-----------------------------
# Author: Mohamad Hafiz Jaafar
#-----------------------------
SET(PLUGIN_NAME plugin_ImageMatcher)
PROJECT ( ${PLUGIN_NAME} )

# this is the directory where the currently processed CMakeLists.txt is located in 
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/libmilan/include
	${CMAKE_SOURCE_DIR}/libmilan/utils
	${CMAKE_SOURCE_DIR}/libmilan/logging
	${CMAKE_SOURCE_DIR}/libmilan/imgproc
	${CMAKE_SOURCE_DIR}/libmilan/cv
	${Milan_SOURCE_DIR}/milan/main
	${Milan_SOURCE_DIR}/milan/main/milan_core
	${Milan_SOURCE_DIR}/milan/plugins
	${CMAKE_CURRENT_SOURCE_DIR}
	${OpenCV_INCLUDE_DIRS}
)

FIND_PACKAGE( Qt5 5.4.1 CONFIG REQUIRED  Core Widgets Gui Xml)

IF ( Qt5Widgets_FOUND AND Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5Xml_FOUND )

	ADD_DEFINITIONS (
		${Qt5Core_DEFINITIONS}
		${Qt5Widgets_DEFINITIONS}
		${Qt5Gui_DEFINITIONS}
		${Qt5Xml_DEFINITIONS}
	)

	INCLUDE_DIRECTORIES (
		${QT_INCLUDES}
	)

	SET ( PLUGIN_SRC_FILES
  		src/cmd_image_matcher.cpp
		src/ImageMatcherWidgetModel.cpp
		src/ImageMatcherToolBoxWidget.cpp
		src/facialRecognition.cpp
		src/robustMatcher.cpp
		src/matcher.cpp
		src/mangaFanartPurger.cpp
	)

	SET ( PLUGIN_MOC_SRCS 
		headers/cmd_image_matcher.hpp
		headers/ImageMatcherWidgetModel.hpp
		headers/ImageMatcherToolBoxWidget.hpp
		headers/facialRecognition.hpp
		headers/robustMatcher.hpp
		headers/matcher.hpp
		headers/mangaFanartPurger.hpp
		headers/image_matcher_constants.hpp
	)

	SET ( PLUGIN_MOC_HDRS 
		headers/cmd_image_matcher.hpp
		headers/ImageMatcherWidgetModel.hpp
		headers/ImageMatcherToolBoxWidget.hpp
		headers/facialRecognition.hpp
		headers/robustMatcher.hpp
		headers/matcher.hpp
		headers/mangaFanartPurger.hpp
		headers/image_matcher_constants.hpp
	)

	SET ( PLUGIN_UI_SRCS 
		ui/ImageMatcherToolBoxWidget.ui
	)
	SET( PLUGIN_RESOURCES ImageMatcher.qrc )

	QT5_ADD_RESOURCES ( RESOURCES
		${PLUGIN_RESOURCES}
	)

	# reroute output (*.h and *.cxx -files) to source tree
	SET (TMP_CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
	SET (CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR})

		QT5_WRAP_CPP ( PLUGIN_MOC_GEN_SRCS
			${PLUGIN_MOC_HDRS}
		)

	#	MESSAGE( "PLUGIN_UI_SRCS: " ${PLUGIN_UI_SRCS} )

		QT5_WRAP_UI ( PLUGIN_UI_LIBS
			${PLUGIN_UI_SRCS}
		)

	SET (CMAKE_CURRENT_BINARY_DIR ${TMP_CMAKE_CURRENT_BINARY_DIR})
	# end of reroute

	ADD_LIBRARY ( ${PLUGIN_NAME} SHARED
		${PLUGIN_SRC_FILES}
		${PLUGIN_MOC_SRCS}
		${PLUGIN_MOC_GEN_SRCS}
		${PLUGIN_UI_LIBS}
		${RESOURCES}
	)

	TARGET_LINK_LIBRARIES ( ${PLUGIN_NAME}
		milan_Core
		libmilan_logging
		libmilan_imgproc
        libmilan_cv
        libmilan_utils
		${Qt5Widgets_LIBRARIES}
		${Qt5Core_LIBRARIES}
		${Qt5Gui_LIBRARIES}
		${Qt5Xml_LIBRARIES}
	)

ELSE ( Qt5Widgets_FOUND AND Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5Xml_FOUND )
	MESSAGE ( FATAL_ERROR "-- Qt5 not found!" )
ENDIF ( Qt5Widgets_FOUND AND Qt5Core_FOUND AND Qt5Gui_FOUND AND Qt5Xml_FOUND )