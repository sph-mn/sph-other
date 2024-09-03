delete_duplicates = (a) -> [...new Set(a)]
random_integer = (min, max) -> Math.floor(Math.random() * (max - min + 1)) + min
random_element = (a) -> a[random_integer 0, a.length - 1]
n_times = (n, f) -> [...Array(n).keys()].map f
median = (a) -> a.slice().sort((a, b) -> a - b)[Math.floor(a.length / 2)]
sum = (a) -> a.reduce ((a, b) -> a + b), 0
mean = (a) -> sum(a) / a.length

delete_duplicates_stable = (a) ->
  result = []
  existing = {}
  a.forEach (a) ->
    unless existing[a]
      existing[a] = true
      result.push a
  result

array_shuffle = (a) ->
  i = a.length
  while 0 < i
    random_index = Math.floor(Math.random() * i)
    i -= 1
    temp = a[i]
    a[i] = a[random_index]
    a[random_index] = temp
  a

object_array_add = (object, key, value) ->
  if object[key] then object[key].push value else object[key] = [value]

sort_by_array_with_index = (a, sorting, index) ->
  a.sort (a, b) -> sorting.indexOf(a[index]) - sorting.indexOf(b[index])

array_intersection = (a, b) -> a.filter (a) -> b.includes(a)
