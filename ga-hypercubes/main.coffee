# depends on https://github.com/weshoke/versor.js

array_new = (size, init) ->
  a = Array(size)
  if init then a.fill(init) else a

array_copy = (a) -> a.slice()

array_equals = (a, b) ->
  i = a.length;
  while i -= 1
    if not (a[i] is b[i]) then return(false)
  true

array_includes_array = (a, value) ->
  a.some (a) -> array_equals a, value

array_numeric_increment_le = (a, base) ->
  # increment array elements like digits of a number
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

vector_diff = (a, b) ->
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
  dimensions = options.dimensions || 3
  # elements set to zero are not rotated
  rotate_dimensions = options.rotate_dimensions || []
  # in milliseconds
  refresh = options.refresh || 20
  # in radians
  rotation_speed = options.rotation_speed || 0.002
  # in pixel
  canvas_width = options.canvas_width || 800
  canvas_height = options.canvas_height || 400

  get_vertices = ->
    # cube vertices are every combination of axis start and end
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
    # one rotation function per plane
    rotators = array_new(dimensions, 0).map (a, index) ->
      if 0 is rotate_dimensions[index] then return (a, angle) -> a
      data = array_new(dimensions, 0)
      ia = (index + 1) % dimensions
      ib = index
      (a, angle) ->
        # array number -> array
        data[ia] = Math.cos(angle)
        data[ib] = Math.sin(angle)
        # create a versorjs object
        rotor = space.Vec.apply this, data
        space.Vec.apply(this, a).sp(rotor).toArray()
    (a, angle) ->
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
    v = vertices.map (a) ->
      a = rotate a, angle
      project a, width, height, 400, 5
    lines.forEach (a) ->
      ctx.beginPath()
      start = a[0]
      end = a[1]
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
  vertices = get_vertices(dimensions)
  lines = get_lines vertices
  space = versor.create metric: array_new(dimensions, 1)
  rotate = get_rotator()
  angle = 0
  f = -> angle = draw ctx, vertices, lines, rotate, angle
  setInterval f, refresh

# this function renders on the canvas
###
cube
  dimensions: 4
  rotate_dimensions: [0, 1, 1, 0, 1]
  refresh: 20
  rotation_speed: 0.004
  canvas_width: 1000
  canvas_height: 800
###

class controls
  options:
    dimensions: 4
    rotate_dimensions: [0, 1, 1, 0, 1]
    refresh: 20
    rotation_speed: 0.004
    canvas_width: 1000
    canvas_height: 800

  constructor: ->
    console.log "cons"
    div = crel "div"
      input_rot_dim = array_new(options.dimensions, 0).map (a, index) ->
        crel "input" {type: "radio", name: "rot_dim", value: index}
    document.getElementByTagName("body").appendChild div

  update: ->
    alert("redraw")

controls.new
