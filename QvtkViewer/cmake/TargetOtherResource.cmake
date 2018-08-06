# OTHER RESOURCES
# # # # # # # # # # # # # # # # # CHANGE BELOW  # # # # # # # # # # # # # # # # 
set(
    PROJECT_RESOURCE
#    PATH
    ${PROJECT_SOURCE_DIR}/ColorFiles
)
# Create target
add_custom_target(
    ${PROJECT_NAME}_resource
	ALL
	DEPENDS
	${PROJECT_RESOURCE}
)
foreach(ONE ${PROJECT_RESOURCE})
	get_filename_component(
		FOLDER_NAME 
		${ONE}
		NAME
	)
	if(${MSVC})
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${FOLDER_NAME} 
			COMMENT "Make a directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${FOLDER_NAME} "
		)
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E copy_directory ${ONE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${FOLDER_NAME} 
			COMMENT "Copy ${ONE} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release/${FOLDER_NAME} "
		)
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/${FOLDER_NAME} 
			COMMENT "Make a directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/${FOLDER_NAME} "
		)
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E copy_directory ${ONE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/${FOLDER_NAME} 
			COMMENT "Copy ${ONE} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug/${FOLDER_NAME} "
		)
	else()
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E copy_directory ${ONE} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FOLDER_NAME} 
			COMMENT "Copy ${ONE} to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FOLDER_NAME} "
		)
		add_custom_command(
			TARGET
			${PROJECT_NAME}_resource
			POST_BUILD
			COMMAND
			${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FOLDER_NAME} 
			COMMENT "Make a directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${FOLDER_NAME} "
		)
	endif()
endforeach()
if(TARGET ${PROJECT_NAME})
    add_dependencies(
        ${PROJECT_NAME}
        ${PROJECT_NAME}_resource
    )
elseif(TARGET ${PROJECT_NAME}_main)
    add_dependencies(
        ${PROJECT_NAME}_main
        ${PROJECT_NAME}_resource
    )
endif()