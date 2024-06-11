target_sources(recorder PRIVATE
    main.cpp
    interfaces.cpp
)

add_subdirectory(util)
add_subdirectory(hook)
add_subdirectory(afx)
add_subdirectory(mem)