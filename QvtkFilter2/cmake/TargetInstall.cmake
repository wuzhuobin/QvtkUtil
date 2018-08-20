# # # # # # # # # # # # # # # # # EXPORT # # # # # # # # # # # # # # # # # # # # # 
# installation to be continue.
# comment following for normal source project
if(TARGET ${PROJECT_NAME})
	install(
		TARGETS ${PROJECT_NAME}
		EXPORT ${PROJECT_NAME} 
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
	)
	export(
		EXPORT
		${PROJECT_NAME}
		FILE
		${CMAKE_BINARY_DIR}/lib/cmake/${PROJECT_NAME}.cmake
	)
	install(
		FILES 
		${PROJECT_BINARY_DIR}/${PROJECT_NAME}_export.h
		${PROJECT_INC}
		DESTINATION
		include
	)
	install(
		EXPORT ${PROJECT_NAME}
		DESTINATION lib/cmake
	)
endif()