project('ZPulse', 'c')
src = [
  'src/test.c',
  'src/phys/2d.c',
  'src/phys/init.c',
  'src/phys/mat.c',
  'src/obj/rb_2d.c',
  'src/player/command.c',
  'src/player/input.c',
  'src/input/input.c',
  'src/input/keyboard.c',
  'src/input/mouse.c',
  'src/input/glfw/keyboard.c',
  'src/input/glfw/mouse.c',
  'src/render/glfixed/init.c',
  'src/window/glfw.c',
  'src/render/glfixed/texture.c',
  'src/render/glfixed/render.c',
  'src/render/camera.c',
  'src/libs/stb_image.c',
  'src/utils/vector.c',
  'src/game/game.c',
  'src/game/rb_2d.c',
  'src/game/obj_2d.c',
  'src/game/grid.c',
  'src/obj/2d.c',
  'src/math/f32_v2.c',
  'src/math/f32.c',
  'src/obj/sprite_2d.c',
  'src/utils/color.c',
  'src/math/fu16_v2.c',
  'src/phys/grid.c',
  'src/math/u32.c'
]

args = [
  '-Wextra',
  '-Wall',
  '-Wno-override-init',
  '-g',
  '-Og',
]

largs = [
  '-lGL',
  '-lGLU',
  '-lglfw3',
  '-lX11',
  '-lXxf86vm',
  '-lXrandr',
  '-lpthread',
  '-lXi',
  '-lm',
]

incdir = include_directories('include')

add_global_arguments(args, language : 'c')
executable (
  'ZPulse',
  src,
  include_directories : incdir,
  link_args : largs
)
