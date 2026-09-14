##################################################################
#
# cFE Software Bus (SB) module CMake build recipe
#
##################################################################

project(CFE_SB C)

# Software Bus source files
set(sb_SOURCES
    fsw/src/cfe_sb_api.c
    fsw/src/cfe_sb_buf.c
    fsw/src/cfe_sb_init.c
    fsw/src/cfe_sb_priv.c
    fsw/src/cfe_sb_task.c
    fsw/src/cfe_sb_util.c
    fsw/src/cfe_sb_objtab.c
)

# A target may supply an observer for successful Software Bus deliveries.
# No observer is built by default, so flight targets retain the normal SB path
# without toolchain-specific weak symbols or an additional runtime dependency.
if (CFE_SB_OBSERVER_SOURCE)
  list(APPEND sb_SOURCES ${CFE_SB_OBSERVER_SOURCE})
endif()

if (CFE_EDS_ENABLED)
  # Use EDS-based message dispatch
  list(APPEND sb_SOURCES
    fsw/src/cfe_sb_eds_dispatch.c
    fsw/src/cfe_sb_eds_msg_id_util.c
  )
else()
  # Use hardcoded message dispatch
  list(APPEND sb_SOURCES
    fsw/src/cfe_sb_dispatch.c
    fsw/src/cfe_sb_msg_id_util.c
  )
endif()

add_library(sb STATIC ${sb_SOURCES})

if (CFE_SB_OBSERVER_SOURCE)
  target_compile_definitions(sb PRIVATE CFE_SB_OBSERVER_ENABLED)
  target_include_directories(sb PRIVATE
    fsw/src
    ${CFE_SB_OBSERVER_INCLUDE_DIRS}
  )
endif()

target_include_directories(sb PUBLIC fsw/inc)
target_link_libraries(sb PRIVATE core_private)

# Add unit test coverage subdirectory
if(ENABLE_UNIT_TESTS)
    add_subdirectory(ut-coverage)
endif(ENABLE_UNIT_TESTS)

cfs_app_check_intf(${DEP}
    cfe_sb_eventids.h
    cfe_sb_extern_typedefs.h
    cfe_sb_fcncodes.h
    cfe_sb_interface_cfg.h
    cfe_sb_internal_cfg.h
    cfe_sb_mission_cfg.h
    cfe_sb_msgdefs.h
    cfe_sb_msg.h
    cfe_sb_msgids.h
    cfe_sb_msgstruct.h
    cfe_sb_platform_cfg.h
    cfe_sb_topicids.h
)
