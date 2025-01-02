opentrack_boilerplate(module-proto-freetrack)
opentrack_qt(Widgets)

if(WIN32)
    target_link_libraries(opentrack-proto-freetrack opentrack-compat)
else()
    target_link_libraries(opentrack-proto-freetrack opentrack-compat atomic)
endif()

if(NOT WIN32)
    # Build trackir-compat for Linux
    add_executable(trackir-compat trackir-compat.cpp)
    target_link_libraries(trackir-compat opentrack-compat)
    install(TARGETS trackir-compat RUNTIME DESTINATION ${opentrack-libexec})
endif()
