# depends on https://github.com/weshoke/versor.js, https://github.com/KoryNunn/crel.
# * the first part of the file defines helper routines
# * the "cube" function is the main drawing function
# * ui_controls is for the html controls form

array_new = (size, init) ->
  a = Array(size)
  if init? then a.fill(init) else a

array_copy = (a) -> a.slice()

array_equals = (a, b) ->
  # note that arrays are passed by reference
  i = a.length;
  while i -= 1
    if not (a[i] is b[i]) then return(false)
  true

array_includes_array = (a, value) ->
  # array array -> boolean
  a.some (a) -> array_equals a, value

array_numeric_increment_le = (a, base) ->
  # increment array elements like digits of a number.
  # lower endian. 000, 100, 010, 110, 001, etc
  a = array_copy a
  index = 0
  while(index < a.length)
    if(a[index] < base)
      a[index] += 1
      break
    else
      a[index] = 0
      index += 1
  a

factorial = (n) ->
  result = 1
  while n >= 1
    result = result * n
    n = n - 1
  result

false_if_nan = (a) -> if isNaN a then false else a

vector_diff = (a, b) ->
  # array array -> array
  a.map (a, index) -> a - b[index]

vector_diff_count = (a, b) ->
  # count unequal elements
  diff = vector_diff(a, b)
  f = (result, a) -> if 0 is a then result else result + 1
  diff.reduce f, 0

k_cubes_at_vertices_count = (k, dim) ->
  factorial(dim) / (factorial(k) * factorial(dim - k))

k_cube_count = (k, dim) ->
  Math.pow(2, dim - k) * k_cubes_at_vertices_count(k, dim)

cube = (options) ->
  # object -> interval
  # continuously draw and rotate a cube on an html canvas.
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

  get_vertices = ->
    # every distinct n-tuple of -1 and 1
    result = []
    count = 2 ** dimensions
    elements = [-1, 1]
    a = array_new dimensions, 0
    result.push a.map((a) -> elements[a])
    index = 0
    while index < count - 1
      a = array_numeric_increment_le(a, 1)
      result.push a.map((a) -> elements[a])
      index += 1
    result

  get_lines = (vertices) ->
    # produce all possible vertex pairings and filter
    result = []
    ia = 0
    while ia < vertices.length
      ib = 0
      while ib < vertices.length
        unless ib is ia
          a = vertices[ia]
          b = vertices[ib]
          is_adjacent = vector_diff_count(a, b) is 1
          if is_adjacent
            if not array_includes_array result, [ib, ia]
              result.push [ia, ib]
        ib += 1
      ia += 1
    result

  get_rotator = ->
    # create one rotation function per plane
    rotators = array_new(dimensions, 0).map (a, index) ->
      # if 0 then return an identity function
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
    # scale and project to simulate depth
    factor = fov / (distance + (a[2] or 1))
    x = a[0] * factor + width / 2
    y = a[1] * factor + height / 2
    [x, y].concat array_copy a

  draw = (ctx, vertices, lines, rotate, angle) ->
    width = ctx.canvas.width
    height = ctx.canvas.height
    ctx.fillRect 0, 0, width, height
    # apply vertex transformations
    v = vertices.map (a) ->
      a = rotate a, angle
      project a, width, height, 400, 5
    # draw edges
    lines.forEach (a, index) ->
      start = a[0]
      end = a[1]
      ctx.strokeStyle = "hsl(298, " + (40 + (index / lines.length) * 60) + "%, 61%)"
      ctx.beginPath()
      ctx.moveTo v[start][0], v[start][1]
      ctx.lineTo v[end][0], v[end][1]
      ctx.closePath()
      ctx.stroke()
    angle + rotation_speed

  canvas = document.getElementById "cube"
  return unless canvas && canvas.getContext
  canvas.width = canvas_width
  canvas.height = canvas_height
  ctx = canvas.getContext "2d"
  ctx.strokeStyle = "rgb(255,55,255)"
  ctx.fillStyle = "rgb(0,0,0)"
  vertices = get_vertices dimensions
  lines = get_lines vertices
  space = versor.create metric: array_new(dimensions, 1)
  rotate = get_rotator()
  angle = 0
  # called repeatedly and updates angle
  f = -> angle = draw ctx, vertices, lines, rotate, angle
  interval = setInterval f, refresh
  interval


class ui_controls
  # the html for the controls and default options

  options:
    dimensions: 4
    rotate_dimensions: [1, 0, 1, 1]
    refresh: 20
    rotation_speed: 0.008
    canvas_width: 1000
    canvas_height: 800

  dom: {}
  label: (text, content) -> label = crel "label", text, content
  warning_shown: false

  update: =>
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
    @cube_interval = cube @options

  in_rot_dim_new: =>
    # create a new array of checkboxes
    array_new(@options.dimensions, 0).map (a, index) =>
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
