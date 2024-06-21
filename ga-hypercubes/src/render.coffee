random_color = -> [Math.random(), Math.random(), Math.random()]
color_to_string = (a) -> "rgb(#{a[0]}, #{a[1]}, #{a[2]})"
available_colors = [0...12].map (a) -> random_color()
adjust_color_brightness = (a, factor) -> a.map (a) -> Math.min 255, Math.max(0, a * factor)
array_sum = (a) -> a.reduce ((a, b) -> a + b), 0

any_square = (cells, f) ->
  # array {array -> any} -> any
  # apply Array.find only on 2-cells and return the first truthy result
  if 4 == cells.length && 2 == cells[0].length && Number.isInteger cells[0][0]
    f cells
  else any cells, (a) -> any_square a, f

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

get_cell_indices = (vertices, n) ->
  # integer -> array
  # get edges grouped by nested cells
  subcells = (indices, k) ->
    return indices unless k < n
    indices = group_n_cells vertices, indices, n, k, 2 ** (n - k)
    indices = sort_edges_cyclically indices if k is n - 1
    subcells a, k + 1 for a in indices
  subcells [0...vertices.length], 1

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
  position_attribute_location = gl.getAttribLocation program_wireframe, 'position'
  gl.enableVertexAttribArray position_attribute_location
  gl.vertexAttribPointer position_attribute_location, 3, gl.FLOAT, false, 0, 0
  gl.enable gl.CULL_FACE
  gl

get_projector = (space, options) ->
  # perspective projection
  # r1 = e ** ((angle / 2) * n * eo) = cos(angle / 2) + sin(angle / 2) * n * eo
  # r2 = e ** ((1 / (2 * d)) * n * eo) = 1 + ep 1 / (2 * d) * n, eo
  rotors = []
  ## reflection rotor r1
  angle = options.projection_angle
  normal = space.e1 1
  eo = space.eo 1
  scalar_part = space.s Math.cos angle / 2
  vector_part = normal.gp(eo).gp space.s Math.sin angle / 2
  rotors.push scalar_part.add vector_part
  ## inversion rotor r2
  d = options.projection_distance
  scalar_part = space.s 1
  vector_part = normal.op space.s(1 / (2 * d)).gp(normal), eo
  rotors.push scalar_part.add vector_part
  (a) -> rotors.reduce ((a, b) -> a.sp b), a

get_rotator = (space, options) ->
  # object object -> {versor_object:vertex -> versor_object:vertex}
  # rotation
  # R = cos(angle / 2) + B * sin(angle / 2)
  bivector_magnitude = Math.sin options.rotation_speed / 2
  rotor_data = Array options.dimensions + 1
  rotor_data[0] = Math.cos options.rotation_speed / 2
  rotors = for a, i in options.rotate_dimensions
    continue unless a
    rotor_data = rotor_data.fill 0, 1
    rotor_data[i + 1] = bivector_magnitude
    space.new "rotor", rotor_data

generate_axis_combinations = (dimension) ->
  combinations = []
  generate_combinations = (prefix, start, depth) ->
    if depth
      for i in [start...dimension]
        generate_combinations prefix + (i + 1), i + 1, depth - 1
    else combinations.push prefix
  generate_combinations "e", 0, 2
  combinations

create_versor_space = (n) ->
  en = [1..(n + 2)]
  types = [
    {name: "rotor", bases: ["s"].concat(generate_axis_combinations(n))}
    {name: "point", bases: en.map((a) -> "e#{a}")}
    {name: "pseudoscalar", bases: ["e" + en.join("")]}
    {name: "eo", bases: ["e" + (n + 1)]}
    {name: "ei", bases: ["e" + (n + 2)]}
  ]
  space = versor.create
    metric: Array(n).fill(1).concat([1, -1])
    conformal: true
    types: types
  space.new = (type, values) ->
    # allows less values to be passed and sets the rest to zero, which versor.js does not do by default
    bases_length = @types[type].bases.length
    if bases_length > values.length
      values = values.concat Array(bases_length - values.length).fill 0
    @[type].apply @, values
  space.point_from_cartesian = (a) ->
    eic = 0.5 * array_sum a.map (a) -> a * a
    @new "point", a.concat [1, eic]
  space.point_to_cartesian = (a) -> a.toArray().slice 0, n
  space

triangulate_squares = (indices, n) ->
  array_map_depth indices, n - 3, (a) ->
    [[a[0][0], a[0][1], a[2][1]], [a[1][0], a[1][1], a[2][1]]]

indices_sorter = (space, n, vertices) ->
  pss = space.pseudoscalar 1
  (indices) ->
    # sort edges counter clockwise.
    # assumes that edges are already sorted cyclically
    array_map_depth indices, n - 2, (a) ->
      [p3, p2, p1] = a.map (a) -> vertices[a]
      #a = [a[2], a[0], a[1]]
      #points = [[1, 0, 0], [0, 1, 0], [0, 0, 1]]
      #points = [[0, 1, 0], [1, 0, 0], [0, 0, 1]]
      #[p1, p2, p3] = points.map (a) -> space.point_from_cartesian a
      normal = p2.sub(p1).op(p3.sub(p1))
      console.log normal.toArray()
      orientation = normal.ip(space.e5(1))
      #console.log normal, orientation
      if 0 < orientation then a else [a[0], a[2], a[1]]

render_rotating_cube = (options) ->
  # object -> interval
  # repeatedly draw and rotate a cube.
  # various vector formats are used:
  # - cell finding: integer bitvectors
  # - transformations: versor.js vectors
  # - vertex sorting: integer arrays
  # - webgl: float32arrays
  gl = gl_initialize options.canvas
  n = options.dimensions
  space = create_versor_space n
  rotator = get_rotator space, options
  projector = get_projector space, options
  vertices = [0...2 ** n]
  indices = get_cell_indices vertices, n
  vertices = vertices.map (a) -> space.point_from_cartesian bits_to_array(a, n)
  indices = triangulate_squares indices, n
  sorter = indices_sorter space, n, vertices
  sorter indices
  return

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
  return
  on_frame = (time) ->
    if options.refresh <= time - previous_time
      previous_time = time
      draw()
    requestAnimationFrame on_frame
  requestAnimationFrame on_frame
