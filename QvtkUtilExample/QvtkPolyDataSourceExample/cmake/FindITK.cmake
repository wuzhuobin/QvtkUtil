# ITK
find_package(ITK REQUIRED)
include(${ITK_USE_FILE})
# LIBRARIES
list(
    APPEND
    PROJECT_LIBRARY
    ${ITK_LIBRARIES}
)