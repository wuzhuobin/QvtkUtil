# VTK
# IF(NOT VTK_FOUND)
	find_package(VTK REQUIRED)
	# LIBRARIES
	list(
	    APPEND
	    PROJECT_LIBRARY
	    ${VTK_LIBRARIES}
	)
# ENDIF()

include(${VTK_USE_FILE})