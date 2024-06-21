// Generated by CoffeeScript 2.7.0
var any, array_map_depth, array_map_leafs, array_swap, binomial, bits_to_array, bits_to_float32_array, edge_equal, false_if_nan, get_bit_combinations, gl_create_program, gl_create_shader, intersection, line_midpoint, perspective_project, sort_by_predicate, vertex_distance;

false_if_nan = function(a) {
  if (isNaN(a)) {
    return false;
  } else {
    return a;
  }
};

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

vertex_distance = function(a) {
  return Math.sqrt(a.map(function(a) {
    return a * a;
  }));
};

line_midpoint = function(a, b) {
  return [(a[0] + b[0]) / 2, (a[1] + b[1]) / 2];
};

edge_equal = function(a, b) {
  return (a[0] === b[0] && a[1] === b[1]) || (a[0] === b[1] && a[1] === b[0]);
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
