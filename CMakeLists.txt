cmake_minimum_required(VERSION 3.15.0)

project (ProjectName
        VERSION 1.0.0
        DESCRIPTION "Description"
)

add_definitions(-D_DEBUG -DDEVELOPMENT)

set(vst3sdk_SOURCE_DIR ../../vst3sdk)
add_subdirectory(${vst3sdk_SOURCE_DIR} ${PROJECT_BINARY_DIR}/vst3sdk)
smtg_enable_vst3_sdk()

smtg_add_vst3plugin(ProjectName
        src/version.h
        src/plugin_cids.h
        src/plugin_processor.h
        src/plugin_processor.cpp
        src/plugin_controller.h
        src/plugin_controller.cpp
        src/plugin_entry.cpp
)

target_link_libraries(ProjectName
        PRIVATE
            sdk
)

smtg_target_configure_version_file(ProjectName)

target_link_libraries(ProjectName
        PRIVATE
            vstgui_support
)

smtg_target_add_plugin_resources(ProjectName
        RESOURCES
            "rsrc/plugin_editor.uidesc"
)


