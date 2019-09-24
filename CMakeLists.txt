cmake_minimum_required(VERSION 3.14)
project(builder C)

set(CMAKE_C_STANDARD 99)

find_library(libsdl SDL2)
find_library(libsdl_img SDL2_image)
find_library(libsdl_ttf SDL2_ttf)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2.framework/Headers)
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2_image.framework/Headers)
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2_ttf.framework/Headers  )
ENDIF()

set(LIB_PATH   ./)
set(SRC_PATH   ./)
set(INC_PATH   ./)
set(LFT_PATH   ./libft)
set(LFT_NAME   ft)
set(LFT_LIB    ${LFT_PATH})

aux_source_directory( ${LFT_LIB} LFT_SRC)
aux_source_directory( ${SRC_PATH} PROJECT_SOURCE_FILES)
add_library(   ${LFT_NAME} STATIC ${LFT_SRC})

add_executable(${PROJECT_NAME}
        ${PROJECT_SOURCE_FILES}
        builder.h
        sdl_fun.c
        main_menu.c
        levels.c
        get_assets.c
        get_next_line.c
        get_next_line.h
        render.c
        modes.c
		math.c
        rewrite.c
        render_grid.c
        fill_polygon.c
        editor.c
        prog.c
        add_to_media.c
        buttons.c
        zoom.c
        textures.c
        walls.c
        useful.c
        move.c
        font.c
        edit_menu.c
        values.c
        init_editor_buttons.c
        edit_door.c
        edit_walls.c
        items.c
        sel_item.c
        fill_grid.c
        grid_main.c
        find_in_media.c
        sector_logic.c
        delete_from_media.c
        delete_world.c
        delete_sector.c
        validate_media.c)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror")
#set(GCC_COVERAGE_COMPILE_FLAGS "-fsanitize=\"address\" -g -fno-omit-frame-pointer")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )
#set(dir ${CMAKE_CURRENT_SOURCE_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${dir}/bin")
target_link_libraries(${PROJECT_NAME} ${LFT_NAME}
        ${libsdl}
        ${libsdl_img}
        ${libsdl_ttf}
        -lm
        )