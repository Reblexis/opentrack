# Set the Clearsight SDK path
set(SDK_CLEARSIGHT "" CACHE PATH "Clearsight SDK path")

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
        set(lib ${lib_name}.lib)
        set(dll "${SDK_CLEARSIGHT}/bin/windows/${arch}/${lib_name}.dll")
        target_link_libraries(${self} ${lib})

        # Install the DLL
        install(FILES ${dll} DESTINATION ${opentrack-libexec})
    elseif(APPLE)
        # macOS settings
        target_link_directories(${self} PRIVATE "${SDK_CLEARSIGHT}/lib/macos/${arch}")
        set(lib_name clearsight_client)
        set(lib lib${lib_name}.dylib)
        target_link_libraries(${self} ${lib})

        install(FILES "${SDK_CLEARSIGHT}/lib/macos/${arch}/${lib}" DESTINATION ${opentrack-libexec})
    else()
        # Linux or other UNIX-like systems
        message("Arch: ${arch}")
        target_link_directories(${self} PRIVATE "${SDK_CLEARSIGHT}/lib/linux/${arch}")
        set(lib_name clearsight_client)
        set(lib lib${lib_name}.so)
        target_link_libraries(${self} ${lib})

        install(FILES "${SDK_CLEARSIGHT}/lib/linux/${arch}/${lib}" DESTINATION ${opentrack-libexec})
    endif()
endif()
