# the exercise name is nothing else than the directory
get_filename_component(EXERCISENAME ${CMAKE_CURRENT_LIST_DIR} NAME)
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# these files are copied only once
file(GLOB TEXTURES
    grid/grass.tga
    grid/rock.tga
    grid/sand.tga
    grid/snow1.tga
	cube/cube_texture.tga)
copy_files_once(${TEXTURES})

# list all the files you want to copy everytime
# you build (ie, you want the shaders there)
file(GLOB SHADERS
  squad/squad_fshader.glsl
  squad/squad_vshader.glsl
  screenquad/screenquad_fshader.glsl
  screenquad/screenquad_vshader.glsl
  grid/grid_vshader.glsl
  grid/grid_fshader.glsl
  water/water_vshader.glsl
  water/water_fshader.glsl
  cube/cube_fshader.glsl
  cube/cube_vshader.glsl)
deploy_shaders_to_build_dir(${SHADERS})

add_executable(${EXERCISENAME} ${SOURCES} ${HEADERS} ${SHADERS})
target_link_libraries(${EXERCISENAME} ${COMMON_LIBS})
