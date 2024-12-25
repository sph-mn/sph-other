class ui_class
  # creates the html for the controls and default options

  options:
    dimensions: 3
    # zero elements are not rotated
    rotation_dimensions: [1, 0, 1, 1]
    # in milliseconds
    refresh: 15000
    # in radians
    rotation_speed: 0.2
    canvas_width: 1000
    canvas_height: 800
    projection_distance: 3
    projection_angle: Math.PI / 4
    light_position: [0, -1, 0]

  dom: {}
  label: (text, content) -> label = crel "label", text, content
  warning_shown: false
  false_if_nan: (a) -> if isNaN a then false else a

  reset: () =>
    # start rendering with a new configuration
    @options.dimensions = Math.max 1, (@false_if_nan(parseInt(@dom.dimensions.value)) || @options.dimensions)
    if 7 < @options.dimensions
      alert "unfortunately, the current maximum number of dimensions 7 because of limitations in a support library"
      @dom.dimensions.value = 7
      return
    if not @warning_shown and 6 is @options.dimensions
      count = 2 ** @options.dimensions
      alert "increasing dimensions can easily overload the browser. now continuing to create " + count + " vertices", "notice"
      @warning_shown = true
    @options.rotation_dimensions = @dom.rotation_axes.map (a) -> if a.checked then 1 else 0
    rotation_axes = document.getElementById "rotation_axes"
    rotation_axes.innerHTML = ""
    @dom.rotation_axes = @rotation_axes_new()
    @dom.rotation_axes.forEach (a) -> rotation_axes.appendChild a
    @options.rotation_speed = Math.PI * (@false_if_nan(parseFloat(@dom.rotation_speed.value)) || @options.rotation_speed)
    @options.canvas = document.getElementsByTagName("canvas")[0]
    @cube_interval and clearInterval(@cube_interval)
    @cube_interval = @draw @options

  rotation_axes_new: =>
    # create a new array of checkboxes
    Array(@options.dimensions).fill(0).map (a, index) =>
      a = crel "input", {type: "checkbox", value: index}
      a.checked = not (@options.rotation_dimensions[index] is 0)
      a.addEventListener "change", @reset
      a

  constructor: (draw) ->
    # create input fields and container
    @draw = draw
    dimensions = crel "input", {type: "number", value: @options.dimensions}
    rotation_speed = crel "input", {type: "number", step: "0.001", value: @options.rotation_speed}
    rotation_axes = @rotation_axes_new()
    rotation_axes_div = crel "div", @label("rotate"), crel("span", {id: "rotation_axes"}, rotation_axes)
    rotation_speed_label = @label "speed", rotation_speed
    dimensions_label = @label "dimensions", dimensions
    @dom =
      dimensions: dimensions
      rotation_axes: rotation_axes
      rotation_speed: rotation_speed
    [dimensions, rotation_speed].forEach (a) => a.addEventListener "change", @reset
    container = crel "div", dimensions_label, rotation_axes_div, rotation_speed_label
    document.getElementById("controls").appendChild container
    @reset()
