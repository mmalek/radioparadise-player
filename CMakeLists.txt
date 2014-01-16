cmake_minimum_required(VERSION 2.8.12)
project(radioparadise-player)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wpedantic -Werror")
endif()

add_definitions(-DQT_NO_KEYWORDS)

find_package(Qt5
	REQUIRED COMPONENTS Multimedia Network Quick Widgets
	OPTIONAL_COMPONENTS DBus
)

if( Qt5DBus_FOUND )
	set( BUILD_MPRIS TRUE )
endif()

configure_file(config.hpp.in config.hpp)

set(sources
	main.qml
	main.cpp
	artworkdownloader.cpp
)

set(libraries
	Qt5::Network
	Qt5::Qml
	Qt5::Quick
	Qt5::Widgets
)

qt5_add_resources(sources resources.qrc)

if( BUILD_MPRIS )
	list( APPEND sources mpris2.cpp )
	list( APPEND libraries Qt5::DBus )

	qt5_add_dbus_adaptor(sources
		org.mpris.MediaPlayer2.xml
		mpris2.hpp Mpris2
		mpris2_root Mpris2Root
	)

	qt5_add_dbus_adaptor(sources
		org.mpris.MediaPlayer2.Player.xml
		mpris2.hpp Mpris2
		mpris2_player Mpris2Player
	)
endif()

add_executable(radioparadise-player ${sources})

target_include_directories(radioparadise-player PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${Qt5Multimedia_INCLUDE_DIRS}
)

target_link_libraries(radioparadise-player ${libraries})

set_property(TARGET radioparadise-player PROPERTY AUTOMOC TRUE)
