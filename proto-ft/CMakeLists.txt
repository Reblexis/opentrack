if(WIN32)
    otr_module(proto-freetrack)
    target_link_libraries(opentrack-proto-freetrack opentrack-csv)
else()
    otr_module(proto-freetrack)
    target_link_libraries(opentrack-proto-freetrack opentrack-csv)
    target_include_directories(opentrack-proto-freetrack PRIVATE ${CMAKE_SOURCE_DIR})
    
    # Build trackir-compat for Linux
    add_executable(trackir-compat trackir-compat.cpp)
    target_link_libraries(trackir-compat opentrack-compat rt)
    target_include_directories(trackir-compat PRIVATE ${CMAKE_SOURCE_DIR})
    install(TARGETS trackir-compat RUNTIME DESTINATION ${opentrack-libexec})
endif()
