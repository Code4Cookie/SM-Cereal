cmake_minimum_required(VERSION 3.19)

set(SM_SDK "" CACHE PATH "")

#IF( ${SM_SDK} STREQUAL ""  )
#   message( FATAL_ERROR "You must set SM_SDK to the path of the Sourcemod SDK!" )
#ENDIF()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(using_clang TRUE)
else()
  set(using_clang FALSE)
endif()

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(using_gcc TRUE)
else()
  set(using_gcc FALSE)
endif()

set(compiler_version ${CMAKE_CXX_COMPILER_VERSION})

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SOURCE_FILES
	"${SOURCE_DIR}/include/extension.h"
	"${SOURCE_DIR}/include/smsdk_config.h"
	"${SOURCE_DIR}/include/cereal_archiver.h"
	
	"${SOURCE_DIR}/src/extension.cpp"
	"${SOURCE_DIR}/src/cereal_natives.cpp"
	
	# TODO: Create seperate target for obj file generation so we dont have to include these for linkage
	"${SM_SDK}/public/smsdk_ext.h"
	"${SM_SDK}/public/smsdk_ext.cpp"
)

set(LINK_OPTIONS "")

project(
	SMCereal
	LANGUAGES CXX
)

add_library(
	"${PROJECT_NAME}"
	SHARED
	${SOURCE_FILES}
)

target_include_directories( 
	"${PROJECT_NAME}"
	
	PUBLIC
	"${SOURCE_DIR}/include"
	"${SM_SDK}/public"
	"${SM_SDK}/public/extensions"
	"${SM_SDK}/public/amtl"
	"${SM_SDK}/public/amtl/amtl"
	"${SM_SDK}/sourcepawn/include"
	
	"${SOURCE_DIR}/extern/cereal/include"
)

IF(MSVC)
	IF (${CMAKE_VS_PLATFORM_NAME} STREQUAL "x64")
		message( FATAL_ERROR "64-bit is not supported!" )
	ENDIF()
	
    add_compile_options(
        "/W3"
		"/GR-"
		"/TP"
		"/Oy-"
        "$<$<CONFIG:Debug>:/MTd>"
        "$<$<CONFIG:Release>:/MT>"
    )
	
	add_compile_definitions( 
		"_CRT_SECURE_NO_DEPRECATE"
		"_CRT_SECURE_NO_WARNINGS"
		"_CRT_NONSTDC_NO_DEPRECATE"
		"_ITERATOR_DEBUG_LEVEL"
		"SMEXT_ENABLE_ROOTCONSOLEMENU"
	)
	
	target_link_libraries(
		"${PROJECT_NAME}"
		
		PUBLIC
		"kernel32.lib"
		"user32.lib"
		"gdi32.lib"
		"winspool.lib"
		"comdlg32.lib"
		"advapi32.lib"
		"shell32.lib"
		"ole32.lib"
		"oleaut32.lib"
		"uuid.lib"
		"odbc32.lib"
		"odbccp32.lib"
	)
	
	#set(LINK_OPTIONS "${LINK_OPTIONS} /MACHINE:X86")
	list(APPEND LINK_OPTIONS "/NODEFAULTLIB:libcmt") # technically this is for debug only, but w/e
	
elseif (using_gcc OR using_clang) #gcc or clang
	add_compile_definitions( 
		"stricmp=strcasecmp"
		"_stricmp=strcasecmp"
		"_snprintf=snprintf"
		"_vsnprintf=vsnprintf"
		"HAVE_STDINT_H"
		"GNUC"
		"SMEXT_ENABLE_ROOTCONSOLEMENU"
	)
	
	list(APPEND CMAKE_C_FLAGS
		"-pipe"
		"-fno-strict-aliasing"
		"-Wall"
		"-Werror"
		"-Wno-unused"
		"-Wno-switch"
		"-Wno-array-bounds"
		"-msse"
		"-m32"
		"-fvisibility=hidden"
	)
	
	list(APPEND CMAKE_CXX_FLAGS
		"-std=c++20" 	#unsure if we need this...
		"-fno-exceptions"
		"-fno-threadsafe-statics"
		"-Wno-non-virtual-dtor"
		"-Wno-overloaded-virtual"
		"-fvisibility-inlines-hidden"
	)
	
	list(APPEND LINK_OPTIONS "-m32")
ENDIF()

IF (WIN32) #windows
	add_compile_definitions( "WIN32" "_WINDOWS" )
	
	set_target_properties(
		"${PROJECT_NAME}" 
		PROPERTIES 
        OUTPUT_NAME "sm_cereal"
        SUFFIX ".ext.dll"
	)

ELSEIF (UNIX AND NOT APPLE) # linux
	add_compile_definitions( "_LINUX" "POSIX" )
	
	set_target_properties(
		"${PROJECT_NAME}" 
		PROPERTIES 
        OUTPUT_NAME "sm_cereal"
	PREFIX ""
        SUFFIX ".ext.so"
	)
	
	list(APPEND LINK_OPTIONS "-Wl,--exclude-libs,ALL" "-lm")
	
	IF (using_gcc)
		list(APPEND LINK_OPTIONS "-static-libgcc")
	ELSEIF (using_clang)
		list(APPEND LINK_OPTIONS "-lgcc_eh")
	ENDIF()
ELSEIF (APPLE) # mac
	add_compile_definitions( "OSX" "_OSX" "POSIX" )
	list(APPEND CMAKE_C_FLAGS "-mmacosx-version-min=10.5")
	list(APPEND CMAKE_CXX_FLAGS_FLAGS "-stdlib=libstdc++")
	
	list(APPEND LINK_OPTIONS 
		"-mmacosx-version-min=10.5"
		"-arch" 
		"i386"
		"-lstdc++"
		"-stdlib=libstdc++"
	)
ENDIF()

target_link_options("${PROJECT_NAME}" PRIVATE "${LINK_OPTIONS}")

target_compile_features(
	"${PROJECT_NAME}"
	PRIVATE
		cxx_std_20
)


