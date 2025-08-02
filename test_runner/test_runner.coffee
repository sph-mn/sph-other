class test_runner_class
  is_string: (a) -> typeof a is "string"
  to_json: (a) -> JSON.stringify(a).replace /,(?=\S)/g, ", "
  is_plain_object: (a) -> a? and typeof a is "object" and a.constructor is Object
  any_to_array: (a) -> if Array.isArray(a) then a else [a]
  object_merge: (a, b) ->
    for k, v of b
      if @is_plain_object(v) and @is_plain_object(a[k])
        a[k] = @object_merge a[k], v
      else
        a[k] = v
    a
  report_compact_failure_strings: (inp, exp, out) -> [((@to_json a for a in inp).join ", "), @to_json(exp), @to_json(out)]
  report_compact: (results) ->
    for [name, test_results...] in results
      process.stdout.write name
      for [status, index, name, inp, exp, out] in test_results
        if status then process.stdout.write " #{index}"
        else
          [inp_string, exp_string, out_string] = @report_compact_failure_strings inp, exp, out
          process.stdout.write [
            "\n  failure #{name} #{index}"
            "inp #{inp_string}"
            "exp #{exp_string}"
            "out #{out_string}"
          ].join "\n  "
      console.log ""
  constructor: (options) ->
    default_options =
      reporter: @report_compact
    @options = @object_merge default_options, options
    @options.reporter = @options.reporter.bind @
  execute_tests: (tests) ->
    status = true
    for [f, rest...] in tests
      break unless status
      [name, context] =
        if Array.isArray f
          [name_or_context, f] = f
          if @is_string(name_or_context) then [name_or_context, null] else [f.name, name_or_context]
        else [f.name, null]
      results = [name]
      for i in [0...rest.length] by 2
        inp = @any_to_array rest[i]
        exp = rest[i + 1]
        out = f.apply context, inp
        out_string = @to_json out
        exp_string = @to_json exp
        status = out_string == exp_string
        results.push [status, i / 2, name, inp, exp, out]
        break unless status
      results
  execute: (tests) -> @options.reporter @execute_tests tests
