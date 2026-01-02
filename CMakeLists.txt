cmake_minimum_required(VERSION 3.31)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

# --- SFML Configuration ---
set(MAIN_SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.0)
set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML-2.6.0/lib/cmake/SFML)

find_package(SFML 2.6 COMPONENTS
        graphics window system audio REQUIRED)

add_executable(Chess
        src/main.cpp
        src/top_classes/Board.cpp
        src/top_classes/Board_SelectionLogic.cpp
        src/top_classes/Board_GeneralMethods.cpp
        src/top_classes/Board_Accessors.cpp
        src/top_classes/Piece.cpp
        src/top_classes/Piece_LaserMovement.cpp
        src/top_classes/Piece_SwitchCases.cpp
        src/top_classes/ChessLogic.cpp
        src/top_classes/ChessLogic_HelperMethods.cpp
        src/helpers/TextureManager.cpp
        src/helpers/FenProcessor.cpp
        src/helpers/BoardDisplay.cpp
        src/helpers/BoardIterator.cpp
        src/helpers/BoardConstIterator.cpp
        src/helpers/BoardSounds.cpp
        src/pieces/Pawn.cpp
        src/pieces/Bishop.cpp
        src/pieces/Rook.cpp
        src/pieces/King.cpp
        src/pieces/Queen.cpp
        src/pieces/Knight.cpp
        src/locators/MovementPath.cpp
        src/locators/Move.cpp
        src/locators/Position.cpp
        src/ai_classes/AICalculator.cpp
        src/ai_classes/ScoreMapper.cpp
        src/ai_classes/ScoreMap.cpp
        src/ai_classes/AiMove.cpp
)

# --- SFML Linking ---
target_link_libraries(Chess PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
)

target_include_directories(Chess PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${MAIN_SFML_DIR}/include
)

# ---SFML .dll File inclusion ---
file(GLOB SFML_LIBS_DEBUG "${MAIN_SFML_DIR}/bin/*-d-2.dll")
file(GLOB SFML_LIBS_RELEASE "${MAIN_SFML_DIR}/bin/*-2.dll")

foreach(SFML_LIB IN LISTS SFML_LIBS_DEBUG SFML_LIBS_RELEASE)
    # Check if the file exists before attempting to install
    if(EXISTS "${SFML_LIB}")
        get_filename_component(LIB_NAME "${SFML_LIB}" NAME)
        install(FILES "${SFML_LIB}" DESTINATION "${CMAKE_BINARY_DIR}/bin")
    endif()
endforeach()

# --- Copy OpenAL DLL ---
add_custom_command(TARGET Chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${MAIN_SFML_DIR}/bin/openal32.dll"
        $<TARGET_FILE_DIR:Chess>
)