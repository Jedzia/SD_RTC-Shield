#[=======================================================================[.rst:
helpers.cmake
-----------------------------

This module provides functions helping with default project tasks.



.. command:: add_run_target

Adds a application runner named TARGET_run to TARGET. WORK_DIRECTORY specifies
the startup path/working directory for the application.

    add_run_target(<TARGET> <WORK_DIRECTORY>)



.. command:: add_list_target

Uses objdump to produce an Assembler-Listing of the target and provides
a new target named TARGET_list. For example the target my_application would
produce my_application.list in OUTPUT_DIRECTORY.

    add_list_target(<TARGET> <OUTPUT_DIRECTORY>)

#]=======================================================================]

#set(_HELPER_COPY_ENABLED (${WIN32} AND NOT ${MINGW}))
set(_HELPER_COPY_ENABLED FALSE)
if (WIN32 OR UNIX)
    set(_HELPER_COPY_ENABLED TRUE)
endif ()
# Cache it, for when it is actually needed, at build time
set(_HELPER_COPY_ENABLED TRUE CACHE INTERNAL "")
#mark_as_advanced(_HELPER_COPY_ENABLED)

if (NOT HELPERS_COPY_CMAKE_DEBUG)
    set(HELPERS_COPY_CMAKE_DEBUG OFF)
endif ()

if (HELPERS_COPY_CMAKE_DEBUG)
    set(_HELPERS_COPY_CMAKE_DEBUG_STR "--debug-output")
endif ()

macro(copy_file target file dest)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -E copy_if_different "${file}" "${dest}"
                DEPENDS ${file}
                COMMENT "Copy ${file} to ${dest}."
        )
        #message(FATAL_ERROR "FUUUUUUUUCK")
        #target_link_libraries(${target} Copy-${target})
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_dll_to_target target libname)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt="${PROJECT_BINARY_DIR}/" -Dreleasesrc="${${libname}_DLL_RELEASE}" -Ddebugsrc="${${libname}_DLL_DEBUG}" -P "${P_MODULE_PATH}/copy.cmake"
                #DEPENDS ${target}
                COMMENT "Copy ${libname} DLL."
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_dll_to_targetdep target libname dependency)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt="${PROJECT_BINARY_DIR}/" -Dreleasesrc="${${libname}_DLL_RELEASE}" -Ddebugsrc="${${libname}_DLL_DEBUG}" -P "${P_MODULE_PATH}/copy.cmake"
                DEPENDS ${dependency}
                COMMENT "Copy ${libname} DLL."
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_releasedll_to_target target libname)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt="${PROJECT_BINARY_DIR}/" -Dreleasesrc="${${libname}_DLL_RELEASE}" -Ddebugsrc="${${libname}_DLL_RELEASE}" -P "${P_MODULE_PATH}/copy.cmake"
                DEPENDS Templater
                COMMENT "Copy ${libname} DLL."
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_debugdll_to_target target libname)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt="${PROJECT_BINARY_DIR}/" -Dreleasesrc="${${libname}_DLL_DEBUG}" -Ddebugsrc="${${libname}_DLL_DEBUG}" -P "${P_MODULE_PATH}/copy.cmake"
                DEPENDS Templater
                COMMENT "Copy ${libname} DLL."
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_files_to_target target files)
    if (_HELPER_COPY_ENABLED)
        foreach (f ${files})
            copy_file_to_target(${target} ${f})
        endforeach ()
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_file_to_target target file)
    if (_HELPER_COPY_ENABLED)
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt="${PROJECT_BINARY_DIR}/" -Dreleasesrc="${file}" -Ddebugsrc="${file}" -P "${P_MODULE_PATH}/copy.cmake"
                DEPENDS Templater
                COMMENT "Copy ${libname} DLL."
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(copy_boost_dll_to_target target libname)
    if (_HELPER_COPY_ENABLED)
        set(_boost_my_lib ${libname})
        string(TOUPPER ${_boost_my_lib} UPPERCOMPONENT)
        set(BoostDebugLibPath ${Boost_${UPPERCOMPONENT}_LIBRARY_DEBUG})
        set(BoostReleaseLibPath ${Boost_${UPPERCOMPONENT}_LIBRARY_RELEASE})
        get_filename_component(_boost_my_lib_path ${BoostDebugLibPath} PATH)
        get_filename_component(_boost_my_lib_name ${BoostDebugLibPath} NAME_WE)
        set(_boost_my_debugdll "${_boost_my_lib_path}/${_boost_my_lib_name}.dll")
        get_filename_component(_boost_my_lib_path ${BoostReleaseLibPath} PATH)
        get_filename_component(_boost_my_lib_name ${BoostReleaseLibPath} NAME_WE)
        set(_boost_my_releasedll "${_boost_my_lib_path}/${_boost_my_lib_name}.dll")

        # Boost_*_DLL checking.
        #set(Boost_${UPPERCOMPONENT}_DLL_DEBUG ${_boost_my_debugdll} CACHE PATH "boost ${_boost_my_lib} debug dll.")
        #set(Boost_${UPPERCOMPONENT}_DLL_RELEASE ${_boost_my_releasedll} CACHE PATH "boost ${_boost_my_lib} release dll.")
        #MARK_AS_ADVANCED(Boost_${UPPERCOMPONENT}_DLL_DEBUG)
        #MARK_AS_ADVANCED(Boost_${UPPERCOMPONENT}_DLL_RELEASE)

        set(tgt "${PROJECT_BINARY_DIR}/")
        add_custom_command(
                TARGET ${target}
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} ${_HELPERS_COPY_CMAKE_DEBUG_STR} -Dconfig=${CMAKE_CFG_INTDIR} -Dtgt=${tgt} -Dreleasesrc=${_boost_my_releasedll} -Ddebugsrc=${_boost_my_debugdll} -P "${CMAKE_SOURCE_DIR}/build/cmake/Modules/copy.cmake"
                COMMENT "source copy dlls and essential files"
        )
    endif (_HELPER_COPY_ENABLED)
endmacro()

macro(add_run_target target working_directory)
    add_custom_target(${target}_run
            COMMAND ${target}
            DEPENDS ${target}
            WORKING_DIRECTORY ${working_directory}
            )
    set_target_properties(${target}_run
            PROPERTIES
            FOLDER ${target}
            )
ENDMACRO()

macro(add_list_target target output_directory)
    add_custom_target(${target}_list
            COMMAND ${CMAKE_OBJDUMP} "$<TARGET_FILE:${target}>" -S > ${target}.list
            DEPENDS ${target}
            WORKING_DIRECTORY ${output_directory}
            BYPRODUCTS "${output_directory}/${target}.list"
            )
    #message(FATAL_ERROR "BYPRODUCTS ${output_directory}/${target}.list")
ENDMACRO()
