false_if_nan = (a) -> if isNaN a then false else a
bits_to_array = (a, n) -> [0...n].map (b, i) -> if 0 == (a >> i & 1) then -1 else 1
bits_to_float32_array = (a, n) ->
  [0...n].map (b, i) -> if 0 == (a >> i & 1) then -1 else 1

binomial = (n, k) ->
  # binomial coefficient (n choose k)
  if k == 0 or k == n then 1
  else (n * binomial(n - 1, k - 1)) / k

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

array_swap = (a, i, j) ->
  # array integer integer -> unspecified
  b = a[i]
  a[i] = a[j]
  a[j] = b

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

any = (a, f) ->
  # array {any -> any} -> any
  # like Array.some but returns the truthy result
  for b in a
    c = f b
    return c if c
  false

edge_equal = (a, b) -> (a[0] == b[0] && a[1] == b[1]) || (a[0] == b[1] && a[1] == b[0])

array_map_leafs = (a, f) ->
  a.map (a) ->
    if Array.isArray a then array_map_leafs a, f
    else f a

vertex_distance = (a) -> Math.sqrt a.map (a) -> a * a
line_midpoint = (a, b) -> [(a[0] + b[0]) / 2, (a[1] + b[1]) / 2]

perspective_project = (plane_width, plane_height, vertex_distance) ->
  effective_distance = a.slice(3).reduce ((sum, a) -> sum - a), vertex_distance
  scale = vertex_distance / effective_distance
  x = x * scale
  y = y * scale
  z = z * scale
  [x, y, z]

intersection = (a, b, min, includes) ->
  # array array integer {any any -> boolean} -> array
  # return the shared elements if at least "min" number of elements are shared.
  # "includes" is a custom comparison function.
  unless includes
    includes = (a, b) -> a.includes b
  included = []
  for c in a
    continue unless includes b, c
    included.push c
    return included if 1 == min
    min -= 1
  false
