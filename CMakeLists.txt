cmake_minimum_required(VERSION 3.14)
project(builder C)

set(CMAKE_C_STANDARD 99)

find_library(libsdl SDL2)
find_library(libsdl_img SDL2_image)
find_library(libsdl_ttf SDL2_ttf)
find_library(libsdl_ttf SDL2_mixer)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2.framework/Headers)
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2_image.framework/Headers)
    include_directories(/Users/mshpakov/Library/Frameworks/SDL2_ttf.framework/Headers  )
	include_directories(/Users/mshpakov/Library/Frameworks/SDL2_mixer.framework/Headers  )
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
		builder_src/builder.h
		builder_src/sdl_fun.c
		builder_src/main_menu.c
		builder_src/main.c
		builder_src/levels.c
		builder_src/get_assets.c
		builder_src/render.c
		builder_src/modes.c
		builder_src/math.c
		builder_src/rewrite.c
		builder_src/render_grid.c
		builder_src/fill_polygon.c
		builder_src/editor.c
		builder_src/prog.c
		builder_src/add_to_media.c
		builder_src/zoom.c
		builder_src/textures.c
		builder_src/useful.c
		builder_src/move.c
		builder_src/edit_menu.c
		builder_src/values.c
		builder_src/init_editor_buttons.c
		builder_src/edit_walls.c
		builder_src/items.c
		builder_src/sel_item.c
		builder_src/fill_grid.c
		builder_src/grid_main.c
		builder_src/find_in_media.c
		builder_src/sector_logic.c
		builder_src/delete_world.c
		builder_src/delete_sector.c
		builder_src/validate_media.c
		builder_src/file_fun.c
		builder_src/memory_fun.c
		builder_src/line_math.c
		builder_src/vec_ops.c
		builder_src/render_btn.c
		builder_src/manage_btn.c
		builder_src/delete_vector.c
		builder_src/add_door.c
		builder_src/sort.c
		builder_src/get_input.c
		builder_src/drawing.c
		builder_src/states.c
		builder_src/get_sec_v.c
		builder_src/get_sec_items.c
		builder_src/select_lit.c
		builder_src/char_ops.c
		builder_src/add_sector.c
		builder_src/texture_edit.c
		builder_src/edit_world.c
		builder_src/editor_render.c
		builder_src/editor_update.c
		builder_src/render_items.c
		builder_src/move_item.c
		builder_src/render_elements.c
		builder_src/move_player.c
		builder_src/move_vector.c
		builder_src/update_sec.c
		builder_src/render_grid_iso.c
		builder_src/render_grid_elems.c
		builder_src/delete_useless.c
		builder_src/replace.c
		builder_src/validate_clockwise.c
		builder_src/write_section.c
		builder_src/write_level.c
		builder_src/identify.c
		builder_src/read_assets_help.c
		builder_src/get_map.c
		builder_src/section.c
		builder_src/free_media.c
		builder_src/update_media_sect.c
		builder_src/read_levels.c
		builder_src/get_map_elems.c
		builder_src/init_world.c
		builder_src/read_line.c
		builder_src/drawing_help.c
		builder_src/edit_walls_help.c
		builder_src/grid_ops.c
		builder_src/btn_press.c
		builder_src/distribute_btns.c
		builder_src/math_ops.c
		builder_src/levels_btn.c
		builder_src/btn_main.c
		builder_src/mode_btn.c
		builder_src/check_for_light.c
		builder_src/clip_line.c
		builder_src/draw_line.c
		builder_src/settings.c
		builder_src/add_door_help.c
		builder_src/texture_name.c
		builder_src/btn_press_settings.c
		builder_src/break_wall.c
		builder_src/get_boxes.c
		builder_src/map_useful.c
		builder_src/settings_help.c
		builder_src/light_sec_type.c)

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