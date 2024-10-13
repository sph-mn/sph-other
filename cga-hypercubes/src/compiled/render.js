// Generated by CoffeeScript 2.7.0
var adjust_color_brightness, any, any_square, array_map_depth, array_map_leafs, array_sum, array_swap, available_colors, binomial, bits_to_array, bits_to_float32_array, color_to_string, context, create_versor_space, edge_equal, false_if_nan, fragment_shader_defaults, fragment_shader_solid_source, fragment_shader_wireframe_source, fs, generate_axis_combinations, get_bit_combinations, get_cells, get_cube, get_projector, get_rotator, gl_create_program, gl_create_shader, gl_initialize, group_n_cells, intersection, line_midpoint, node_run, perspective_project, random_color, render_rotating_cube, script, sort_by_predicate, sort_edges_cyclically, sort_vertices, triangulate_squares, versor, vertex_distance, vertex_shader_source, vm,
  indexOf = [].indexOf;

if (typeof window === "undefined" || window === null) {
  fs = require("fs");
  vm = require("vm");
  script = fs.readFileSync("./src/foreign/versor.js", 'utf8');
  context = {};
  vm.runInNewContext(script, context);
  versor = context.versor;
}

random_color = function() {
  return [Math.random(), Math.random(), Math.random()];
};

adjust_color_brightness = function(a, factor) {
  return a.map(function(a) {
    return Math.min(255, Math.max(0, a * factor));
  });
};

array_sum = function(a) {
  return a.reduce((function(a, b) {
    return a + b;
  }), 0);
};

available_colors = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11].map(function(a) {
  return random_color();
});

bits_to_array = function(a, n) {
  return (function() {
    var results = [];
    for (var l = 0; 0 <= n ? l < n : l > n; 0 <= n ? l++ : l--){ results.push(l); }
    return results;
  }).apply(this).map(function(b, i) {
    if (0 === (a >> i & 1)) {
      return -1;
    } else {
      return 1;
    }
  });
};

bits_to_float32_array = function(a, n) {
  return (function() {
    var results = [];
    for (var l = 0; 0 <= n ? l < n : l > n; 0 <= n ? l++ : l--){ results.push(l); }
    return results;
  }).apply(this).map(function(b, i) {
    if (0 === (a >> i & 1)) {
      return -1;
    } else {
      return 1;
    }
  });
};

color_to_string = function(a) {
  return `rgb(${a[0]}, ${a[1]}, ${a[2]})`;
};

edge_equal = function(a, b) {
  return (a[0] === b[0] && a[1] === b[1]) || (a[0] === b[1] && a[1] === b[0]);
};

false_if_nan = function(a) {
  if (isNaN(a)) {
    return false;
  } else {
    return a;
  }
};

line_midpoint = function(a, b) {
  return [(a[0] + b[0]) / 2, (a[1] + b[1]) / 2];
};

vertex_distance = function(a) {
  return Math.sqrt(a.map(function(a) {
    return a * a;
  }));
};

binomial = function(n, k) {
  // binomial coefficient (n choose k)
  if (k === 0 || k === n) {
    return 1;
  } else {
    return (n * binomial(n - 1, k - 1)) / k;
  }
};

any = function(a, f) {
  var b, c, l, len;
// array {any -> any} -> any
// like Array.some but returns the truthy result
  for (l = 0, len = a.length; l < len; l++) {
    b = a[l];
    c = f(b);
    if (c) {
      return c;
    }
  }
  return false;
};

array_swap = function(a, i, j) {
  var b;
  // array integer integer -> unspecified
  b = a[i];
  a[i] = a[j];
  return a[j] = b;
};

array_map_leafs = function(a, f) {
  return a.map(function(a) {
    if (Array.isArray(a)) {
      return array_map_leafs(a, f);
    } else {
      return f(a);
    }
  });
};

array_map_depth = function(a, depth, f) {
  return a.map(function(a) {
    if (depth) {
      return array_map_depth(a, depth - 1, f);
    } else {
      return f(a);
    }
  });
};

intersection = function(a, b, min, includes) {
  var c, included, l, len;
  // array array integer {any any -> boolean} -> array
  // return the shared elements if at least "min" number of elements are shared.
  // "includes" is a custom comparison function.
  if (!includes) {
    includes = function(a, b) {
      return a.includes(b);
    };
  }
  included = [];
  for (l = 0, len = a.length; l < len; l++) {
    c = a[l];
    if (!includes(b, c)) {
      continue;
    }
    included.push(c);
    if (1 === min) {
      return included;
    }
    min -= 1;
  }
  return false;
};

perspective_project = function(plane_width, plane_height, vertex_distance) {
  var effective_distance, scale, x, y, z;
  effective_distance = a.slice(3).reduce((function(sum, a) {
    return sum - a;
  }), vertex_distance);
  scale = vertex_distance / effective_distance;
  x = x * scale;
  y = y * scale;
  z = z * scale;
  return [x, y, z];
};

sort_by_predicate = function(a, predicate) {
  var adjacent, b, i, l, len, match_result, next_index, previous, sorted;
  // array {any any -> 0/1/2} -> array
  // 0: no-match, 1: acceptable, 2: optimal
  sorted = [a[0]];
  a = a.slice(1);
  while (a.length > 0) {
    previous = sorted[sorted.length - 1];
    next_index = 0;
    adjacent = null;
    for (i = l = 0, len = a.length; l < len; i = ++l) {
      b = a[i];
      match_result = predicate(previous, b);
      if (match_result) {
        next_index = i;
        if (2 === match_result) {
          break;
        }
      }
    }
    sorted.push(a[next_index]);
    a.splice(next_index, 1);
  }
  return sorted;
};

get_bit_combinations = function(n, k) {
  var a, b, c, result;
  // generate all k-combinations of a set of size n as binary bitvectors.
  // algorithm: gospers hack
  result = [];
  a = (1 << k) - 1;
  while (a < (1 << n)) {
    result.push(a);
    b = a & -a;
    c = a + b;
    a = (((c ^ a) >> 2) / b) | c;
  }
  return result;
};

gl_create_shader = function(gl, type, source) {
  var a;
  a = gl.createShader(gl[type]);
  gl.shaderSource(a, source);
  gl.compileShader(a);
  if (!gl.getShaderParameter(a, gl.COMPILE_STATUS)) {
    console.error(gl.getShaderInfoLog(a));
    gl.deleteShader(a);
  }
  return a;
};

gl_create_program = function(gl, vertex_shader, fragment_shader) {
  var a;
  a = gl.createProgram();
  gl.attachShader(a, vertex_shader);
  gl.attachShader(a, fragment_shader);
  gl.linkProgram(a);
  if (!gl.getProgramParameter(a, gl.LINK_STATUS)) {
    console.error(gl.getProgramInfoLog(program));
    gl.deleteProgram(a);
  }
  return a;
};

any_square = function(cells, f) {
  // array {array -> any} -> any
  // apply Array.find only on 2-cells and return the first truthy result
  if (4 === cells.length && 2 === cells[0].length && Number.isInteger(cells[0][0])) {
    return f(cells);
  } else {
    return any(cells, function(a) {
      return any_square(a, f);
    });
  }
};

sort_edges_cyclically = function(cells) {
  var i, is_adjacent, l, link, ref;
  // sort edge vertices to form a continuous line
  is_adjacent = function(a, b) {
    return a.some(function(a) {
      return indexOf.call(b, a) >= 0;
    });
  };
  cells = sort_by_predicate(cells, is_adjacent);
  link = cells[0].find(function(a) {
    return indexOf.call(cells[1], a) >= 0;
  });
  if (link !== cells[0][1]) {
    array_swap(cells[0], 0, 1);
  }
  for (i = l = 1, ref = cells.length; (1 <= ref ? l < ref : l > ref); i = 1 <= ref ? ++l : --l) {
    if (cells[i - 1][1] !== cells[i][0]) {
      array_swap(cells[i], 1, 0);
    }
  }
  return cells;
};

group_n_cells = function(vertices, indices, n, k, cell_length) {
  var cell_indices, cell_vertices, fixed, fixed_combinations, i, key, l, len, len1, new_cell_indices, o;
  fixed_combinations = get_bit_combinations(n, k);
  cell_indices = [];
  for (l = 0, len = fixed_combinations.length; l < len; l++) {
    fixed = fixed_combinations[l];
    cell_vertices = {};
    for (o = 0, len1 = indices.length; o < len1; o++) {
      i = indices[o];
      key = fixed & vertices[i];
      if (cell_vertices[key]) {
        cell_vertices[key].push(i);
      } else {
        cell_vertices[key] = [i];
      }
    }
    new_cell_indices = Object.values(cell_vertices).filter(function(a) {
      return cell_length === a.length;
    });
    cell_indices = cell_indices.concat(new_cell_indices);
  }
  return cell_indices;
};

get_cells = function(vertices, n) {
  var ref, subcells;
  // integer -> array
  // get indices of edges grouped by nested cells.
  subcells = function(indices, k) {
    var a, l, len, results;
    if (!(k < n)) {
      return indices;
    }
    indices = group_n_cells(vertices, indices, n, k, 2 ** (n - k));
    if (k === n - 1) {
      indices = sort_edges_cyclically(indices);
    }
    results = [];
    for (l = 0, len = indices.length; l < len; l++) {
      a = indices[l];
      results.push(subcells(a, k + 1));
    }
    return results;
  };
  return subcells((function() {
    var results = [];
    for (var l = 0, ref = vertices.length; 0 <= ref ? l < ref : l > ref; 0 <= ref ? l++ : l--){ results.push(l); }
    return results;
  }).apply(this), 1);
};

vertex_shader_source = `#version 300 es
precision highp float;
in vec4 position;
void main() {
  gl_Position = position;
}`;

fragment_shader_defaults = `#version 300 es
precision highp float;
out vec4 fragment_color;`;

fragment_shader_wireframe_source = fragment_shader_defaults + `void main() {
  fragment_color = vec4(1.0, 0.0, 0.0, 1.0);
}`;

fragment_shader_solid_source = fragment_shader_defaults + `void main() {
  fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}`;

gl_initialize = function(canvas) {
  var fragment_shader_solid, fragment_shader_wireframe, gl, position_attribute_location, program_solid, program_wireframe, vertex_shader;
  gl = canvas.getContext("webgl2");
  if (!gl) {
    alert("unable to initialize webgl2. your browser may not support it.");
    return;
  }
  gl.viewport(0, 0, gl.drawingBufferWidth, gl.drawingBufferHeight);
  gl.clearColor(0, 0, 0, 1);
  gl.bindBuffer(gl.ARRAY_BUFFER, gl.createBuffer());
  gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, gl.createBuffer());
  vertex_shader = gl_create_shader(gl, "VERTEX_SHADER", vertex_shader_source);
  fragment_shader_wireframe = gl_create_shader(gl, "FRAGMENT_SHADER", fragment_shader_wireframe_source);
  fragment_shader_solid = gl_create_shader(gl, "FRAGMENT_SHADER", fragment_shader_solid_source);
  program_wireframe = gl_create_program(gl, vertex_shader, fragment_shader_wireframe);
  program_solid = gl_create_program(gl, vertex_shader, fragment_shader_solid);
  // link position variable to array_buffer
  position_attribute_location = gl.getAttribLocation(program_wireframe, "position");
  gl.enableVertexAttribArray(position_attribute_location);
  gl.vertexAttribPointer(position_attribute_location, 3, gl.FLOAT, false, 0, 0);
  gl.enable(gl.CULL_FACE);
  return gl;
};

get_projector = function(space, projection_distance, projection_angle) {
  var angle, d, eo, normal, rotors, scalar_part, vector_part;
  // perspective projection
  // r1 = e ** ((angle / 2) * n * eo) = cos(angle / 2) + sin(angle / 2) * n * eo
  // r2 = e ** ((1 / (2 * d)) * n * eo) = 1 + ep 1 / (2 * d) * n, eo
  rotors = [];
  //# reflection rotor r1
  angle = projection_angle;
  normal = space.e1(1);
  eo = space.eo(1);
  scalar_part = space.s(Math.cos(angle / 2));
  vector_part = normal.gp(eo).gp(space.s(Math.sin(angle / 2)));
  rotors.push(scalar_part.add(vector_part));
  //# inversion rotor r2
  d = projection_distance;
  scalar_part = space.s(1);
  vector_part = normal.op(space.s(1 / (2 * d)).gp(normal), eo);
  rotors.push(scalar_part.add(vector_part));
  return function(a) {
    return rotors.reduce((function(a, b) {
      return a.sp(b);
    }), a);
  };
};

get_rotator = function(space, n, rotation_dimensions, rotation_speed) {
  var a, bivector_magnitude, i, rotor_data, rotors;
  // object integer integer rational -> {versor_object:vertex -> versor_object:vertex}
  // rotation
  // R = cos(angle / 2) + B * sin(angle / 2)
  bivector_magnitude = Math.sin(rotation_speed / 2);
  rotor_data = Array(n + 1);
  rotor_data[0] = Math.cos(rotation_speed / 2);
  rotors = (function() {
    var l, len, results;
    results = [];
    for (i = l = 0, len = rotation_dimensions.length; l < len; i = ++l) {
      a = rotation_dimensions[i];
      if (!a) {
        continue;
      }
      rotor_data = rotor_data.fill(0, 1);
      rotor_data[i + 1] = bivector_magnitude;
      results.push(space.new("rotor", rotor_data));
    }
    return results;
  })();
  return function(a) {
    return rotators.reduce((function(a, r) {
      return a.sp(r);
    }), a);
  };
};

generate_axis_combinations = function(dimension) {
  var combinations, generate_combinations;
  combinations = [];
  generate_combinations = function(prefix, start, depth) {
    var i, l, ref, ref1, results;
    if (depth) {
      results = [];
      for (i = l = ref = start, ref1 = dimension; (ref <= ref1 ? l < ref1 : l > ref1); i = ref <= ref1 ? ++l : --l) {
        results.push(generate_combinations(prefix + (i + 1), i + 1, depth - 1));
      }
      return results;
    } else {
      return combinations.push(prefix);
    }
  };
  generate_combinations("e", 0, 2);
  return combinations;
};

create_versor_space = function(n) {
  var en, ref, space, types;
  en = (function() {
    var results = [];
    for (var l = 1, ref = n + 2; 1 <= ref ? l <= ref : l >= ref; 1 <= ref ? l++ : l--){ results.push(l); }
    return results;
  }).apply(this);
  types = [
    {
      name: "rotor",
      bases: ["s"].concat(generate_axis_combinations(n))
    },
    {
      name: "point",
      bases: en.map(function(a) {
        return `e${a}`;
      })
    },
    {
      name: "pseudoscalar",
      bases: ["e" + en.join("")]
    },
    {
      name: "eo",
      bases: ["e" + (n + 1)]
    },
    {
      name: "ei",
      bases: ["e" + (n + 2)]
    }
  ];
  space = versor.create({
    metric: Array(n).fill(1).concat([1, -1]),
    conformal: true,
    types: types
  });
  space.new = function(type, values) {
    var bases_length;
    // allows less values to be passed and sets the rest to zero, which versor.js does not do by default
    bases_length = this.types[type].bases.length;
    if (bases_length > values.length) {
      values = values.concat(Array(bases_length - values.length).fill(0));
    }
    return this[type].apply(this, values);
  };
  space.point_from_cartesian = function(a) {
    var eic;
    eic = 0.5 * array_sum(a.map(function(a) {
      return a * a;
    }));
    return this.new("point", a.concat([1, eic]));
  };
  space.point_to_cartesian = function(a) {
    return a.toArray().slice(0, n);
  };
  return space;
};

triangulate_squares = function(indices, n) {
  return array_map_depth(indices, n - 3, function(a) {
    return [[a[0][0], a[0][1], a[2][1]], [a[1][0], a[1][1], a[2][1]]];
  });
};

sort_vertices = function(space, n, vertices, cells) {
  var reference;
  // sort edges counter clockwise.
  // assumes that edges are already sorted cyclically.
  reference = space.new("point", [0, 0, 2]);
  return array_map_depth(cells, n - 2, function(a) {
    var d, i_inv, m, p1, p2, p3, s;
    [p1, p2, p3] = a.map(function(a) {
      return vertices[a];
    });
    m = p1.op(p2).op(p3).op(space.ei(1));
    i_inv = space.pseudoscalar(1).reverse();
    n = m.ip(i_inv);
    d = p1.sub(reference);
    s = n.ip(d);
    return console.log(s);
  });
};

//console.log p1.toArray(), p2.toArray(), p3.toArray()
get_cube = function(options) {
  var bit_vertices, cells, n, projector, ref, rotator, space, vertices;
  n = options.dimensions;
  space = create_versor_space(n);
  rotator = get_rotator(space, n, options.rotation_dimensions.slice(0, n), options.rotation_speed);
  projector = get_projector(space, options.projection_distance, options.projection_angle);
  bit_vertices = (function() {
    var results = [];
    for (var l = 0, ref = 2 ** n; 0 <= ref ? l < ref : l > ref; 0 <= ref ? l++ : l--){ results.push(l); }
    return results;
  }).apply(this);
  vertices = bit_vertices.map(function(a) {
    return space.point_from_cartesian(bits_to_array(a, n));
  });
  cells = get_cells(bit_vertices, n);
  cells = triangulate_squares(cells, n);
  cells = sort_vertices(space, n, vertices, cells);
  return 0;
  return {space, rotator, projector, vertices, triangles};
};

node_run = function() {
  var cube, options;
  options = {
    dimensions: 3,
    rotation_dimensions: [1, 0, 1, 1],
    rotation_speed: 0.2,
    projection_distance: 3,
    projection_angle: Math.PI / 4
  };
  cube = get_cube(options);
  return console.log(cube);
};

node_run();

render_rotating_cube = function(options) {
  var cube, draw, final_vertices, gl, n, on_frame, previous_time;
  // object -> interval
  // repeatedly draw and rotate a cube.
  // various vector formats are used:
  // - cell finding: integer bitvectors
  // - transformations: versor.js vectors
  // - vertex sorting: integer arrays
  // - webgl: float32arrays
  n = options.dimensions;
  cube = get_cube(n);
  return;
  gl = gl_initialize(options.canvas);
  gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, indices, gl.STATIC_DRAW);
  final_vertices = new Float32Array(vertices.length);
  draw = function() {
    var i, l, ref;
    for (i = l = 0, ref = vertices.length; (0 <= ref ? l < ref : l > ref); i = 0 <= ref ? ++l : --l) {
      vertices[i] = transform(vertices[i]);
      final_vertices[i] = project(vertices[i]);
    }
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.DYNAMIC_DRAW);
    return gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
  };
  //gl.useProgram program_wireframe
  //gl.drawElements gl.LINES, indices.length, gl.UNSIGNED_SHORT, 0
  //if false
  //  gl.useProgram program_solid
  //  gl.drawElements gl.TRIANGLES, faces.length, gl.UNSIGNED_SHORT, 0
  draw();
  options.canvas.addEventListener("click", function(event) {
    return draw();
  });
  previous_time = -options.refresh;
  return;
  on_frame = function(time) {
    if (options.refresh <= time - previous_time) {
      previous_time = time;
      draw();
    }
    return requestAnimationFrame(on_frame);
  };
  return requestAnimationFrame(on_frame);
};