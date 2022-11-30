cmake_minimum_required(VERSION 2.82)

file(GLOB_RECURSE SOURCES "src/*[.cpp|.h]")
file(GLOB_RECURSE SOURCES_POBJS "src/PObjs/*[.cpp|.h]")

option(BUILD_WINDOWS "Build Windows .exe" ON) # Default
option(BUILD_LINUX "Build Linux executable" OFF)
option(BUILD_VITA "Build Vita .vpk" OFF)

if(BUILD_WINDOWS)
	message(STATUS "Building for Windows")
	add_definitions(-DWindows)

	set(BUILD_WINDOWS_PKGS "C:/Users/Hamme/pkgs" CACHE STRING "Path to the packages, only for Windows")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread -std=c++17 -Wl,-subsystem,windows")

	project(Sand-Box2D)

	set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/pics/icon.rc")
	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${SOURCES_POBJS} ${APP_ICON_RESOURCE_WINDOWS})

	target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
		"${BUILD_WINDOWS_PKGS}/include"
	)

	target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    	"${BUILD_WINDOWS_PKGS}/lib"
	)

	target_link_libraries(${CMAKE_PROJECT_NAME}
		jsoncpp
		curl
		box2d

		SDL2_image
		SDL2_mixer
		SDL2_ttf
		SDL2_gfx

		mingw32
		SDL2main

		SDL2
	)

	file(COPY assets DESTINATION .)

elseif(BUILD_LINUX)
	message(STATUS "Building for Linux")
	add_definitions(-DLinux)

	project(Sand-Box2D)

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread -std=c++17")
	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${SOURCES_POBJS})

	target_link_libraries(${CMAKE_PROJECT_NAME}
		jsoncpp
		curl
		box2d

		SDL2_image
		SDL2_mixer
		SDL2_ttf
		SDL2_gfx
		SDL2
	)

	file(COPY assets DESTINATION .)

elseif(BUILD_VITA)
	message(STATUS "Building for Vita")
	add_definitions(-DVita)

	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	  if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	  else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	  endif()
	endif()

	project(Sand-Box2D C CXX)
	include("${VITASDK}/share/vita.cmake" REQUIRED)

	set(VITA_APPNAME  "Sand-Box2D")
	set(VITA_TITLEID  "SANDBOX2D")
	set(VITA_VERSION  "01.00")

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pthread -std=c++17")
	set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

	add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${SOURCES_POBJS})

	find_package(CURL REQUIRED)
	find_package(OpenSSL REQUIRED)

	target_link_libraries(${CMAKE_PROJECT_NAME}
		jsoncpp
		box2d

		${CURL_LIBRARIES}
		${OPENSSL_LIBRARIES}

		SDL2_image
		SDL2_mixer
		SDL2_ttf
		SDL2_gfx
		SDL2

		vorbisfile
		vorbis
		ogg
		mikmod
		modplug
		mpg123
		FLAC
		webp
		jpeg
		png
		z
		freetype

		SceAudio_stub
		SceAudioIn_stub
		SceCommonDialog_stub
		SceCtrl_stub
		SceDisplay_stub
		SceGxm_stub
		SceHid_stub
		SceSysmodule_stub
		SceTouch_stub
		SceMotion_stub
	)

	vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

	vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_PROJECT_NAME}.self
		VERSION ${VITA_VERSION}
		NAME ${VITA_APPNAME}

		FILE assets assets
		FILE pics/sce_sys sce_sys
	)

endif()
