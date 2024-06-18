# depends on https://github.com/weshoke/versor.js, https://github.com/KoryNunn/crel.
# * the first part of the file defines helper routines
# * the render_rotating_cube function is the main drawing loop function
# * ui_controls is for the html controls form

array_new = (size, init) ->
  a = Array(size)
  if init? then a.fill(init) else a

array_copy = (a) -> a.slice()
false_if_nan = (a) -> if isNaN a then false else a

binomial = (n, k) ->
  # binomial coefficient (n choose k)
  if k == 0 or k == n then 1
  else (n * binomial(n - 1, k - 1)) / k

k_cube_vectors = (k) ->
  # create bit patterns up to 2 ** k as integers
  vertices = [0...2 ** k]
  edge_indices = []
  cell_indices = []
  # get edges
  for a in vertices
    for b in vertices
      # filter if hamming distance equal to 1
      c = a ^ b
      edge_indices.push [a, b] if 0 != c and 0 == (c & (c - 1))
  # map bits to arrays
  vertices = vertices.map (a) ->
    [0...k].map (b, i) -> if 0 == (a >> i & 1) then -1 else 1
  # get cell/face vertices
  for i in [0..k]
    for fixed in [0, 1]
      cell_indices.push vertices.filter (a) -> fixed == a[i]
  [vertices, edge_indices, cell_indices]

render_rotating_cube = (options) ->
  # object -> interval
  # repeatedly draw and rotate a cube on an html canvas.
  dimensions = options.dimensions || 3
  # elements set to zero are not rotated
  rotate_dimensions = options.rotate_dimensions || []
  # in milliseconds
  refresh = options.refresh || 20
  # in radians
  rotation_speed = options.rotation_speed || 0.008
  # in pixel
  canvas_width = options.canvas_width || 800
  canvas_height = options.canvas_height || 400
  projection_distance = 3
  projection_fov = 400

  get_rotator = (space) ->
    # one rotation function per plane
    rotators = array_new(dimensions, 0).map (a, index) ->
      if 0 is rotate_dimensions[index] then return (a, angle) -> a
      data = array_new dimensions, 0
      ia = (index + 1) % dimensions
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

  project = (a, width, height, fov, distance) ->
    # perspective projection
    factor = fov / (distance + (a[2] or 1))
    x = a[0] * factor + width / 2
    y = a[1] * factor + height / 2
    [x, y].concat array_copy a

  draw = (ctx, vertices, edge_indices, rotate, angle) ->
    width = ctx.canvas.width
    height = ctx.canvas.height
    ctx.fillRect 0, 0, width, height
    vertices = vertices.map (a) ->
      project rotate(a, angle), width, height, projection_fov, projection_distance
    edge_indices.forEach (a, index) ->
      start = vertices[a[0]]
      end = vertices[a[1]]
      ctx.beginPath()
      ctx.moveTo start[0], start[1]
      ctx.lineTo end[0], end[1]
      ctx.closePath()
      ctx.stroke()
    angle + rotation_speed

  canvas = document.getElementById "cube"
  return unless canvas && canvas.getContext
  canvas.width = canvas_width
  canvas.height = canvas_height
  ctx = canvas.getContext "2d"
  ctx.strokeStyle = "#8096bf"
  ctx.fillStyle = "#000"
  space = versor.create metric: array_new(dimensions, 1)
  rotate = get_rotator space
  angle = 0
  [vertices, edge_indices] = k_cube_vectors dimensions
  # called repeatedly and updates angle
  f = -> angle = draw ctx, vertices, edge_indices, rotate, angle
  interval = setInterval f, refresh
  interval


class ui_controls
  # creates the html for the controls and default options

  options:
    dimensions: 4
    rotate_dimensions: [1, 0, 1, 1]
    refresh: 20
    rotation_speed: 0.005
    canvas_width: 1000
    canvas_height: 800

  dom: {}
  label: (text, content) -> label = crel "label", text, content
  warning_shown: false

  update: =>
    # start rendering with a new configuration
    @options.dimensions = Math.max 1, (false_if_nan(parseInt(@dom.in.dim.value)) || @options.dimensions)
    if not @warning_shown and 9 is @options.dimensions
      count = 2 ** @options.dimensions
      alert("increasing dimensions can easily overload the browser. now continuing to create " + count + " vertices", "notice")
      @warning_shown = true
    @options.rotate_dimensions = @dom.in.rot_dim.map (a) -> if a.checked then 1 else 0
    rot_dim = document.getElementById "rot_dim"
    rot_dim.innerHTML = ""
    @dom.in.rot_dim = @in_rot_dim_new()
    @dom.in.rot_dim.forEach (a) -> rot_dim.appendChild a
    @options.rotation_speed = false_if_nan(parseFloat(@dom.in.rot_speed.value)) || @options.rotation_speed
    @cube_interval and clearInterval(@cube_interval)
    @cube_interval = render_rotating_cube @options

  in_rot_dim_new: =>
    # create a new array of checkboxes
    axes = binomial @options.dimensions, 2
    array_new(axes, 0).map (a, index) =>
      a = crel "input", {type: "checkbox", value: index}
      a.checked = not (@options.rotate_dimensions[index] is 0)
      a.addEventListener "change", @update
      a

  constructor: ->
    # create input fields and container
    in_dim = crel "input", {type: "number", value: @options.dimensions}
    in_rot_speed = crel "input", {type: "number", step: "0.001", value: @options.rotation_speed}
    in_rot_dim = @in_rot_dim_new()
    rot_dim = crel "div", @label("rotate"), crel("span", {id: "rot_dim"}, in_rot_dim)
    rot_speed = @label "speed", in_rot_speed
    dim = @label "dimensions", in_dim
    @dom.in =
      dim: in_dim
      rot_dim: in_rot_dim
      rot_speed: in_rot_speed
    [in_dim, in_rot_speed].forEach (a) => a.addEventListener "change", @update
    container = crel "div", dim, rot_dim, rot_speed
    document.getElementById("controls").appendChild container
    @update()

new ui_controls
