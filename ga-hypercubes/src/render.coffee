random_color = -> [Math.floor(Math.random() * 256), Math.floor(Math.random() * 256), Math.floor(Math.random() * 256)]
color_to_string = (a) -> "rgb(#{a[0]}, #{a[1]}, #{a[2]})"
available_colors = [0...12].map (a) -> random_color()
adjust_color_brightness = (a, factor) -> a.map (a) -> Math.min 255, Math.max(0, a * factor)

any_square = (cells, f) ->
  # array {array -> any} -> any
  # apply Array.find only on 2-cells and return the first truthy result
  if 4 == cells.length && 2 == cells[0].length && Number.isInteger cells[0][0]
    f cells
  else any cells, (a) -> any_square a, f

sort_cells_by_edges = (cells, n, k, cell_length) ->
  # array integer integer integer -> array
  if 2 == cell_length
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
  else
    # sort other cells by adjacency and parallelity
    max_shared_edges = 2 ** (n - k - 2)
    includes_edge = (a, b) -> a.find (a) -> edge_equal a, b
    is_adjacent_parallel = (a, b) ->
      any_square a, (a) ->
        any_square b, (b) ->
          shared = intersection a, b, max_shared_edges, includes_edge
          if shared
            # checks that only the x-axis differs. could be extended
            if 1 == (shared[0][0] ^ shared[0][1]) then 2
            else 1
          else 0
    sort_by_predicate cells, is_adjacent_parallel

group_n_cells = (vertices, n, k, cell_length) ->
  fixed_combinations = get_bit_combinations n, k
  cells = []
  for fixed in fixed_combinations
    cell_vertices = {}
    for a in vertices
      key = fixed & a
      if cell_vertices[key] then cell_vertices[key].push a
      else cell_vertices[key] = [a]
    new_cells = Object.values(cell_vertices).filter (a) -> cell_length == a.length
    cells = cells.concat new_cells
  cells

get_n_cube_cells = (n) ->
  # integer -> array
  # get edges grouped by nested cells
  subcells = (cells, k) ->
    if k < n
      cell_length = 2 ** (n - k)
      cells = group_n_cells cells, n, k, cell_length
      cells = (subcells a, k + 1 for a in cells)
      sort_cells_by_edges cells, n, k, cell_length
    else cells
  subcells [0...2 ** n], 1

render_rotating_cube = (options) ->
  # object -> interval
  # repeatedly draw and rotate a cube.
  rotate_f = (space) ->
    # one rotation function per plane
    rotators = Array(options.dimensions).fill(0).map (a, index) ->
      if 0 is options.rotate_dimensions[index] then return (a, angle) -> a
      data = Array(options.dimensions).fill 0
      ia = (index + 1) % options.dimensions
      ib = index
      (a, angle) ->
        angle = angle + ((ia + 1) * 1000)
        # array number -> array
        data[ia] = Math.cos angle
        data[ib] = Math.sin angle
        # create a versorjs object
        rotor = space.Vec.apply this, data
        space.Vec.apply(this, a).sp(rotor).toArray()
    (a, angle) ->
      # apply all rotation functions to the given point vector
      f = (a, rotate) -> a and rotate a, angle
      rotators.reduce f, a

  render_square_path = (a) ->
    midpoint = line_midpoint a[0][0], a[2][0]
    midpoint_distance = vertex_distance midpoint
    [b, distance] = render_vertex a[0][0]
    [b, midpoint_distance]

  draw = (ctx, cells, render_vertex, angle) ->
    ctx.clearRect 0, 0, options.canvas_width, options.canvas_height
    draw_cells = (a, ai) ->
      if 4 == a.length && 2 == a[0].length
        [point, distance] = render_vertex a[0][0]
        midpoint =
        ctx.beginPath()
        ctx.moveTo point[0], point[1]
        point = render_vertex a[0][1]
        ctx.lineTo point[0], point[1]
        for i in [1...a.length]
          point = render_vertex a[i][1]
          ctx.lineTo point[0], point[1]
        ctx.closePath()
        #distance = vertex_distance
        color = available_colors[ai]
        color_string = color_to_string adjust_color_brightness color, 1
        ctx.strokeStyle = color_string
        ctx.fillStyle = color_string
        ctx.stroke()
        #ctx.fill()
        gl.drawElements(gl.LINES, vertex_count, type, offset);
      else a.forEach (a, ai) -> draw_cells a, ai
    draw_cells cells, 0
    angle + options.rotation_speed

  gl = options.canvas.getContext "webgl"
  unless gl
    alert "unable to initialize webgl. your browser may not support it."
    return
  console.log gl
  return
  angle = 0
  space = versor.create metric: Array(options.dimensions).fill(1)
  rotate = rotate_f space
  render_vertex = (a) ->
    a = rotate a, angle
    d = vertex_distance a
    a = perspective_project canvas_width, canvas_height, a, d
    [a, d]
  cells = get_n_cube_cells options.dimensions
  cells = array_map_leafs cells, ((a) -> bits_to_array a, options.dimensions)
  # called repeatedly and updates angle
  f = -> angle = draw ctx, cells, render_vertex, angle
  interval = setInterval f, options.refresh
  interval
