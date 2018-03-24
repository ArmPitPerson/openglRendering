# Module Name
SET(MODULE_NAME PCG)
SET(MODULE_EXPORT_NAME ${MODULE_NAME}Targets)

# Create library and alias for building as a sub-project
ADD_LIBRARY(${MODULE_NAME} INTERFACE)
ADD_LIBRARY(${MODULE_NAME}::${MODULE_NAME} ALIAS ${MODULE_NAME})

# Add include directories
TARGET_INCLUDE_DIRECTORIES(${MODULE_NAME}
                           INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:>
                           )

# Set Install Targets
INSTALL(TARGETS ${MODULE_NAME} EXPORT ${MODULE_EXPORT_NAME}
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

INSTALL(EXPORT ${MODULE_EXPORT_NAME}
        FILE ${MODULE_EXPORT_NAME}.cmake
        NAMESPACE ${MODULE_NAME}::
        DESTINATION lib/cmake/${MODULE_NAME}
        )
