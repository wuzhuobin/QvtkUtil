if(BUILD_TESTING)
# Qt5::Test
    find_package(Qt5Test REQUIRED)
# Enable CTest.
    include(CTest)
    file(GLOB PROJECT_TEST_SRC ${PROJECT_SOURCE_DIR}/Testing/*.c*)
    foreach(ONE ${PROJECT_TEST_SRC})
        get_filename_component(FILE_NAME ${ONE} NAME_WE)
        add_executable(
            ${FILE_NAME}
            ${ONE}
        )
        if(TARGET ${PROJECT_NAME})
            target_link_libraries(
                ${FILE_NAME}
                ${PROJECT_NAME}
                ${PROJECT_LIBRARY}
                Qt5::Test
            )
        else()
            target_link_libraries(
                ${FILE_NAME}
                ${PROJECT_LIBRARY}
                Qt5::Test
            )
        endif()
        target_include_directories(
            ${FILE_NAME}
            PUBLIC
            ${PROJECT_INCLUDE}
        )
# Providing ability to do CTest
        add_test(
            NAME
            ${FILE_NAME}
            COMMAND
            ${FILE_NAME}
        )
# EXPORT test
    install(
        TARGETS ${FILE_NAME}
        RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
		INCLUDES DESTINATION include
    )
    endforeach()
endif()
