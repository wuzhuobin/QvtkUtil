# @file	    FindQtConcurrent.cmake
# @brief	CMake script for finding QtConcurrent. 
# @author	WUZHUOBIN
# @version	1.0
# @since	Aug.29.2018
# @date 	Aug.29.2018
# @log
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
#			Copyright (c) WUZHUOBIN 											    #
#			All rights reserved.												    #
#			See Copyright.txt													    #
#			This software is distributed WITHOUT ANY WARRANTY; without even		    #
#			the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR	    #
#			PURPOSE.  See the above copyright notice for more information.		    #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Qt5Concurrent
find_package(Qt5Concurrent REQUIRED)
# LIBRARY
list(
    APPEND
    PROJECT_LIBRARY
    Qt5::Concurrent
)