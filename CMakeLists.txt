project(Xonix)

find_package(SFML REQUIRED network audio graphics window system)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Add source files - no folders needed
add_executable(xonix 
    main.cpp
    Game.cpp
    Player.cpp
    Enemy.cpp
    Menu.cpp
    Auth.cpp
)

target_link_libraries(xonix PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)