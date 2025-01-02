# tracker-clearsight/CMakeLists.txt

# Set the Clearsight SDK path
if(NOT DEFINED SDK_CLEARSIGHT)
    set(SDK_CLEARSIGHT $ENV{SDK_CLEARSIGHT} CACHE PATH "Clearsight SDK path")
endif()

if(SDK_CLEARSIGHT)
    # Define the module for the Clearsight tracker
    otr_module(tracker-clearsight)

    # Compiler options for MSVC on Windows
    if(WIN32 AND MSVC)
        add_compile_options(-EHsc)
    endif()

    # Determine architecture (32-bit or 64-bit)
    if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
        set(arch "x86")
    else()
        set(arch "x64")
    endif()

    # Include directories
    target_include_directories(${self} SYSTEM PRIVATE "${SDK_CLEARSIGHT}/include")

    # Platform-specific library settings
    if(WIN32)
        # Windows settings
        target_link_directories(${self} PRIVATE "${SDK_CLEARSIGHT}/lib/windows/${arch}")
        set(lib_name clearsight_client)
        set(lib "${lib_name}.lib")
        set(dll "${SDK_CLEARSIGHT}/bin/windows/${arch}/${lib_name}.dll")
        target_link_libraries(${self} "${lib}")  # Plain signature

        # Install the DLL without 'lib' prefix
        install(FILES "${dll}" DESTINATION ${opentrack-libexec})

    elseif(APPLE)
        # macOS settings
        target_link_directories(${self} PRIVATE "${SDK_CLEARSIGHT}/lib/macos/${arch}")
        set(lib_name clearsight_client)
        set(lib "lib${lib_name}.dylib")
        target_link_libraries(${self} "${lib}")  # Plain signature

        # Install the dylib with 'lib' prefix
        install(FILES "${SDK_CLEARSIGHT}/lib/macos/${arch}/${lib}" DESTINATION ${opentrack-libexec})

    else()
        # Linux or other UNIX-like systems
        message("Arch: ${arch}")
        target_link_directories(${self} PRIVATE "${SDK_CLEARSIGHT}/lib/linux/${arch}")
        set(lib_name clearsight_client)
        set(lib "clearsight_client")  # Corrected spelling

        # Find the library using find_library
        find_library(CLEARSIGHT_CLIENT_LIBRARY
                NAMES "clearsight_client"
                PATHS "${SDK_CLEARSIGHT}/lib/linux/${arch}"
                NO_DEFAULT_PATH
        )

        if(NOT CLEARSIGHT_CLIENT_LIBRARY)
            message(FATAL_ERROR "clearsight_client library not found in ${SDK_CLEARSIGHT}/lib/linux/${arch}")
        endif()

        # Link the library
        target_link_libraries(${self} "${CLEARSIGHT_CLIENT_LIBRARY}")  # Plain signature

        # Set RPATH to locate the shared library at runtime
        set_target_properties(${self} PROPERTIES
                INSTALL_RPATH "$ORIGIN/../../lib/linux/${arch}"
                BUILD_WITH_INSTALL_RPATH TRUE
        )

        # Install the shared library with 'lib' prefix
        install(FILES "${SDK_CLEARSIGHT}/lib/linux/${arch}/lib${lib}.so" DESTINATION "lib/linux/${arch}")

        # Create a symbolic link 'clearsight_client.so' pointing to 'libclearsight_client.so'
        install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink lib${lib}.so clearsight_client.so
                WORKING_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/lib/linux/${arch}\")")
    endif()
endif()
