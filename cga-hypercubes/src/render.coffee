sph_ga = require "./foreign/sph_ga.js" unless window?  # allow it to be run in the browser and node.js

array_sum = (a) -> a.reduce ((a, b) -> a + b), 0
bits_to_array = (a, n) -> [0...n].map (b, i) -> if 0 == (a >> i & 1) then -1 else 1

any = (a, f) ->
  # array {any -> any} -> any
  # like Array.some but returns the truthy result
  for b in a
    c = f b
    return c if c
  false

array_swap = (a, i, j) ->
  # array integer integer -> unspecified
  b = a[i]
  a[i] = a[j]
  a[j] = b

array_map_depth = (a, depth, f) ->
  a.map (a) ->
    if depth then array_map_depth a, depth - 1, f
    else f a

sort_by_predicate = (a, predicate) ->
  # array {any any -> 0/1/2} -> array
  # 0: no-match, 1: acceptable, 2: optimal
  sorted = [a[0]]
  a = a.slice 1
  while a.length > 0
    previous = sorted[sorted.length - 1]
    next_index = 0
    adjacent = null
    for b, i in a
      match_result = predicate previous, b
      if match_result
        next_index = i
        break if 2 == match_result
    sorted.push a[next_index]
    a.splice next_index, 1
  sorted

get_bit_combinations = (n, k) ->
  # generate all k-combinations of a set of size n as binary bitvectors.
  # algorithm: gospers hack
  result = []
  a = (1 << k) - 1
  while a < (1 << n)
    result.push a
    b = a & -a
    c = a + b
    a = (((c ^ a) >> 2) / b) | c
  result

sort_edges_cyclically = (cells) ->
  # sort edge vertices to form a continuous line
  is_adjacent = (a, b) -> a.some (a) -> a in b
  cells = sort_by_predicate cells, is_adjacent
  link = cells[0].find (a) -> a in cells[1]
  unless link == cells[0][1]
    array_swap cells[0], 0, 1
  for i in [1...cells.length]
    unless cells[i - 1][1] == cells[i][0]
      array_swap cells[i], 1, 0
  cells

group_n_cells = (vertices, indices, n, k, cell_length) ->
  fixed_combinations = get_bit_combinations n, k
  cell_indices = []
  for fixed in fixed_combinations
    cell_vertices = {}
    for i in indices
      key = fixed & vertices[i]
      if cell_vertices[key] then cell_vertices[key].push i
      else cell_vertices[key] = [i]
    new_cell_indices = Object.values(cell_vertices).filter (a) -> cell_length == a.length
    cell_indices = cell_indices.concat new_cell_indices
  cell_indices

get_cells = (vertices, n) ->
  # integer -> array
  # get indices of edges grouped by nested cells.
  subcells = (indices, k) ->
    return indices unless k < n
    indices = group_n_cells vertices, indices, n, k, 2 ** (n - k)
    indices = sort_edges_cyclically indices if k is n - 1
    subcells a, k + 1 for a in indices
  subcells [0...vertices.length], 1

get_projector = (space, projection_distance, projection_angle) ->
  # perspective projection.
  cos_half = Math.cos(projection_angle / 2)
  sin_half = Math.sin(projection_angle / 2)
  rotation = space.rotor [cos_half, sin_half, space.normal, space.no(1)]
  coeff = 1 / (2 * projection_distance)
  perspective = space.rotor [1, coeff, space.normal, space.ni(1)]
  projection = space.gp rotation, perspective
  (point) -> space.sp projection, point

get_rotator = (space, n, rotation_dimensions, rotation_speed) ->
  # object integer integer rational -> {multivector:vertex -> multivector:vertex}
  # rotation
  # R = cos(angle / 2) + B * sin(angle / 2)
  bivector_magnitude = Math.sin rotation_speed / 2
  rotor_data = Array n + 1
  rotor_data[0] = Math.cos rotation_speed / 2
  rotors = for a, i in rotation_dimensions
    continue unless a
    rotor_data = rotor_data.fill 0, 1
    rotor_data[i + 1] = bivector_magnitude
    space.rotor rotor_data
  (a) -> rotators.reduce ((a, r) -> a.sp(r)), a

triangulate_squares = (indices, n) ->
  array_map_depth indices, n - 3, (a) ->
    [[a[0][0], a[0][1], a[2][1]], [a[1][0], a[1][1], a[2][1]]]

sort_vertices = (space, n, vertices, cells) ->
  # sort edges counter clockwise.
  # assumes that edges are already sorted cyclically.
  i = space.pseudoscalar 1
  p_ref = space.point [0, 0, 2]
  array_map_depth cells, n - 2, (a) ->
    [p1, p2, p3] = a.map (a) -> vertices[a]
    b = p2.subtract(p1).ep(p3.subtract(p1))
    v = p1.subtract(p_ref)
    o = b.ip(v)
    console.log (a for a in p1)
    console.log (a for a in p2)
    console.log (a for a in p3)

get_cube = (options) ->
  n = options.dimensions
  space = new sph_ga [1, 1, 1], conformal: true
  rotation_dimensions = options.rotation_dimensions.slice 0, n
  rotator = get_rotator space, n, rotation_dimensions, options.rotation_speed
  projector = get_projector space, options.projection_distance, options.projection_angle
  bit_vertices = [0...2 ** n]
  vertices = bit_vertices.map (a) -> space.point bits_to_array a, n
  cells = get_cells bit_vertices, n
  cells = triangulate_squares cells, n
  console.log cells
  return
  cells = sort_vertices space, n, vertices, cells
  {space, rotator, projector, vertices}

vertex_shader_source = """
#version 300 es
precision highp float;
in vec4 position;
void main() {
  gl_Position = position;
}
"""

fragment_shader_defaults = """
#version 300 es
precision highp float;
out vec4 fragment_color;
"""

fragment_shader_wireframe_source = fragment_shader_defaults + """
void main() {
  fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
}
"""

fragment_shader_solid_source = fragment_shader_defaults + """
void main() {
  fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}
"""

gl_create_shader = (gl, type, source) ->
  a = gl.createShader gl[type]
  gl.shaderSource a, source
  gl.compileShader a
  unless gl.getShaderParameter a, gl.COMPILE_STATUS
    console.error gl.getShaderInfoLog a
    gl.deleteShader a
  a

gl_create_program = (gl, vertex_shader, fragment_shader) ->
  a = gl.createProgram()
  gl.attachShader a, vertex_shader
  gl.attachShader a, fragment_shader
  gl.linkProgram a
  unless gl.getProgramParameter a, gl.LINK_STATUS
     console.error gl.getProgramInfoLog program
     gl.deleteProgram a
  a

gl_initialize = (canvas) ->
  gl = canvas.getContext "webgl2"
  unless gl
    alert "unable to initialize webgl2. your browser may not support it."
    return
  gl.viewport 0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight
  gl.clearColor 0, 0, 0, 1
  gl.bindBuffer gl.ARRAY_BUFFER, gl.createBuffer()
  gl.bindBuffer gl.ELEMENT_ARRAY_BUFFER, gl.createBuffer()
  vertex_shader = gl_create_shader gl, "VERTEX_SHADER", vertex_shader_source
  fragment_shader_wireframe = gl_create_shader gl, "FRAGMENT_SHADER", fragment_shader_wireframe_source
  fragment_shader_solid = gl_create_shader gl, "FRAGMENT_SHADER", fragment_shader_solid_source
  program_wireframe = gl_create_program gl, vertex_shader, fragment_shader_wireframe
  program_solid = gl_create_program gl, vertex_shader, fragment_shader_solid
  # link position variable to array_buffer
  position_attribute_location = gl.getAttribLocation program_wireframe, "position"
  gl.enableVertexAttribArray position_attribute_location
  gl.vertexAttribPointer position_attribute_location, 3, gl.FLOAT, false, 0, 0
  gl.enable gl.CULL_FACE
  gl

render_rotating_cube = (options) ->
  # object -> interval
  # repeatedly draw and rotate a cube.
  # various vector formats are used:
  # - cell finding: integer bitvectors
  # - transformations: sph-ga vectors
  # - vertex sorting: integer arrays
  # - webgl: float32arrays
  n = options.dimensions
  cube = get_cube options
  return
  gl = gl_initialize options.canvas
  gl.bufferData gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW
  final_vertices = new Float32Array vertices.length
  draw = () ->
    for i in [0...vertices.length]
      vertices[i] = transform vertices[i]
      final_vertices[i] = project vertices[i]
    gl.bufferData gl.ARRAY_BUFFER, vertices, gl.DYNAMIC_DRAW
    gl.clear gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT
    #gl.useProgram program_wireframe
    #gl.drawElements gl.LINES, indices.length, gl.UNSIGNED_SHORT, 0
    #if false
    #  gl.useProgram program_solid
    #  gl.drawElements gl.TRIANGLES, faces.length, gl.UNSIGNED_SHORT, 0
  draw()
  options.canvas.addEventListener "click", (event) -> draw()
  previous_time = -options.refresh
  on_frame = (time) ->
    if options.refresh <= time - previous_time
      previous_time = time
      draw()
    requestAnimationFrame on_frame
  requestAnimationFrame on_frame

node_run = () ->
  options = {
    dimensions: 3
    rotation_dimensions: [1, 0, 1, 1]
    rotation_speed: 0.2
    projection_distance: 3
    projection_angle: Math.PI / 4
  }
  cube = get_cube options
  console.log cube

node_run()
