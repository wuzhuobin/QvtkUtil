# @file		TemplateCMakeLists.txt
# @brief	A template CMakeLists.txt which can automatically generate project
# @author	WUZHUOBIN
# @version	1.6.0
# @since	Sep.19.2017
# @date 	Apr.19.2018
# 
# <h1>How to use</h1>
# <ul>
# 	<li>Create a folder to place all header files, implementation files and the 'TemplateCMakeLists.txt'.</li>
# 	<li>Rename the folder as the project name. Since the folder name will become the project name.</li>
# 	<li>Rename 'TemplateCMakeLists.txt' to 'CMakeLists.txt'.</li>
# 	<li>Configure with CMake and follow normal cmake pipeline.</li>
# 	<li>If build executable, create 'main/main.cpp' as the program entry point.</li>
# 	<li>If build library, no need to do the previous procedure.</li>
# 	<li>DO NOT modify things not in 'CHANGE BELOW'. </li>
# 	<li>If need other Qt module, add modules in 'DEPENDENCY' module.</li>
# 	<li>If need other package, add packages in 'DEPENDENCY' module.</li>
# 	<li>If need Qt qrc, add the path in <code>PROJECT_QRC</code> in 'DEPENDENCY' module. </li>
# 	<li>If need resource, add resources in 'DEPENDENCY' module.</li>
# 	<li>If need other libraries and headers, add them in <code>SYSTEM_LIBRARIES</code> and <code>SYSTEM_INCLUDE</code> in 'LIBRARIES' module. </li>
# </ul>
# <h1>What happen</h1>
# <ul>
# 	<li>CMake will find all '*.c*' '*.h*' files for building. </li>
# 	<li>CMake will find all '*.ui' as Qt UI form. </li>
# 	<li>This template can only build static library. </li>
# 	<li>If 'main/main.cpp' exists, the project will create both static library files and executable files. </li>
# </ul>
#
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			Copyright (c) WUZHUOBIN 											    #
#			All rights reserved.												    #
#			See Copyright.txt													    #
#			This software is distributed WITHOUT ANY WARRANTY; without even		    #
#			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	    #
#			PURPOSE.  See the above copyright notice for more information.		    #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
cmake_minimum_required(VERSION 3.1) 
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
get_filename_component(
	FOLDER_NAME 
	${CMAKE_CURRENT_SOURCE_DIR} 
	NAME
)
project(${FOLDER_NAME})

message(
	STATUS 
	"Building submodule ${PROJECT_NAME}..."
)

# multi-processor build
if(${MSVC})
	set(
		CMAKE_CXX_FLAGS 
		"${CMAKE_CXX_FLAGS} /MP"
	)
	set(
		CMAKE_C_FLAGS
		"${CMAKE_C_FLAGS} /MP"
	)
endif()

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# # # # # # # # # # # # # # # # # POLICY  # # # # # # # # # # # # # # # # # # # 
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

# # # # # # # # # # # # # # # # # CHANGE BELOW  # # # # # # # # # # # # # # # # 


# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 
# # # # # # # # # # # # # # # # # # DEPENDENCY  # # # # # # # # # # # # # # # # 
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # 

# Adding dependency library you need
# Please remember to set it to SYSTEM_LIBRARIES as well 

# ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

# VTK
find_package(VTK REQUIRED) 
include(${VTK_USE_FILE})

# QT
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# # # # # # # # # # # # # # # # # CHANGE BELOW  # # # # # # # # # # # # # # # # 
#find_package(Qt5.... REQUIRED)
#find_package(... REQURIED)

# OTHERS
#find_package()
#....

add_subdirectory(QvtkData)
# add_subdirectory(QvtkFilter)
add_subdirectory(QvtkInteractorObserver)
add_subdirectory(QvtkViewer)
# add_subdirectory(QvtkPipeline)
# add_subdirectory(QvtkMainWindow)
configure_file(
    QvtkUtilConfig.cmake.in
    ${CMAKE_BINARY_DIR}/QvtkUtilConfig.cmake
    @ONLY
)
install(
    FILES
    ${CMAKE_BINARY_DIR}/QvtkUtilConfig.cmake
    DESTINATION
    .
)
option(BUILD_EXAMPLE "Build Example" OFF)
if(${BUILD_EXAMPLE})
	add_subdirectory(Example)
endif()
# QT_QRC
#set(
#	PROJECT_QRC
#	#PATH	
#)


# WINDOWS RESOURCES
# set(
# 	WINDOWS_RESOURCES
# 	# PATH
# )
MESSAGE(STATUS "Building submodule ${PROJECT_NAME} finished!")


