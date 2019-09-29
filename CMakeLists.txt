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
		src/builder.h
		src/sdl_fun.c
		src/main_menu.c
		src/main.c
		src/levels.c
		src/get_assets.c
		src/get_next_line.c
		src/get_next_line.h
		src/render.c
		src/modes.c
		src/math.c
		src/rewrite.c
		src/render_grid.c
		src/fill_polygon.c
		src/editor.c
		src/prog.c
		src/add_to_media.c
		src/zoom.c
		src/textures.c
		src/useful.c
		src/move.c
		src/edit_menu.c
		src/values.c
		src/init_editor_buttons.c
		src/edit_walls.c
		src/items.c
		src/sel_item.c
		src/fill_grid.c
		src/grid_main.c
		src/find_in_media.c
		src/sector_logic.c
		src/delete_world.c
		src/delete_sector.c
		src/validate_media.c
		src/file_fun.c
		src/memory_fun.c
		src/line_math.c
		src/vec_ops.c
		src/render_btn.c
		src/manage_btn.c
		src/delete_vector.c
		src/add_door.c
		src/sort.c
		src/get_input.c
		src/drawing.c
		src/states.c
		src/get_sec_v.c
		src/get_sec_items.c
		src/select_lit.c
		src/char_ops.c
		src/add_sector.c
		src/texture_edit.c
		src/edit_world.c
		src/editor_render.c
		src/editor_update.c
		src/render_items.c
		src/move_item.c
		src/render_elements.c
		src/move_player.c
		src/move_vector.c
		src/update_sec.c src/render_grid_iso.c src/render_grid_elems.c src/delete_useless.c src/replace.c src/validate_clockwise.c src/write_section.c src/write_level.c)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -Werror ")
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