const list = {
  key: {
    left: 37,
    right: 39,
    up: 38,
    down: 40,
    q: 81,
    space: 32
  },
  selection: 0,
  content_data: null,
  move_distance: null,
  dom: {
    content: document.getElementById("file_content")
  },
  get_content: () => {
    // -> [[string:column, ...], ...]
    result = []
    for (let i = 0; i < list.dom.content.children.length; i += 1) {
      const id = parseInt(list.dom.content.children[i].getAttribute("id"))
      result.push(list.content_data[id])
    }
    return result
  },
  set_content: (data) => {
    // [[string:column, ...], ...]
    if (!data) return
    list.dom.content.innerHTML = ""
    data.forEach(function(line, index) {
      const b = document.createElement("span")
      b.classList.add("hidden")
      b.innerHTML = " " + line.slice(1).join(" ")
      const a = document.createElement("div")
      a.innerHTML = line[0]
      a.appendChild(b)
      a.setAttribute("id", index)
      a.addEventListener("click", function() {
        // find the current index
        for (let i = 0; i < list.dom.content.children.length; i += 1) {
          if (a == list.dom.content.children[i]) {
            list.set_selection(i)
            break
          }
        }
      })
      list.dom.content.appendChild(a)
    })
    list.content_data = data
    list.move_distance = Math.ceil(data.length / 2)
    list.set_selection(0)
  },
  set_selection: (index) => {
    list.dom.content.children[list.selection].classList.remove("selected")
    list.selection = index
    const a = list.dom.content.children[list.selection]
    a.classList.add("selected")
    const b = a.getBoundingClientRect()
    if (b.top < a.offsetHeight || b.bottom > window.innerHeight) {
      a.scrollIntoView({
        block: "center"
      })
    }
  },
  reveal: () => {
    list.dom.content.children[list.selection].children[0].classList.toggle("hidden")
  },
  select_previous: () => {
    list.set_selection(Math.max(0, list.selection - 1))
  },
  select_next: () => {
    list.set_selection(Math.min(list.dom.content.children.length - 1, list.selection + 1))
  },
  move_up: (far) => {
    if (0 == list.selection) return
    const a = list.dom.content.children[list.selection]
    a.classList.remove("selected")
    list.dom.content.insertBefore(a, list.dom.content.children[(far ? 0 : Math.max(0, list.selection - list.move_distance))])
    list.dom.content.children[list.selection].classList.add("selected")
  },
  move_down: (far) => {
    if ((list.dom.content.children.length - 1) <= list.selection) return
    const a = list.dom.content.children[list.selection]
    a.classList.remove("selected")
    const last_index = list.dom.content.children.length - 1
    b_index = (far ? last_index : list.selection + list.move_distance) + 1
    if (b_index >= last_index) list.dom.content.appendChild(a)
    else list.dom.content.insertBefore(a, list.dom.content.children[b_index])
    list.dom.content.children[list.selection].classList.add("selected")
  },
  quit: () => {
    duration.updateMain()
    electron.ipcRenderer.send("quit")
  },
  add_event_listeners: () => {
    document.addEventListener("keydown", event => {
      if (list.key.down == event.keyCode) {
        list.select_next()
      } else if (list.key.up == event.keyCode) {
        list.select_previous()
      } else if (list.key.space == event.keyCode) {
        list.reveal()
      } else if (event.ctrlKey && list.key.left == event.keyCode) {
        list.move_up(true)
      } else if (event.ctrlKey && list.key.right == event.keyCode) {
        list.move_down(true)
      } else if (event.ctrlKey && list.key.q == event.keyCode) {
        list.quit()
      } else {
        return
      }
      event.preventDefault()
    })
    document.addEventListener("dblclick", event => {
      list.reveal()
    })
  },
  init: () => {
    list.add_event_listeners()
  }
}

const app = {
  download: (filename, text) => {
    var element = document.createElement("a")
    element.setAttribute("href", "data:text/plain;charset=utf-8," + encodeURIComponent(text))
    element.setAttribute("download", filename)
    element.style.display = "none"
    document.body.appendChild(element)
    element.click()
    document.body.removeChild(element)
  },
  dom: {
    select_file: document.getElementById("select_file"),
    save_file: document.getElementById("save_file"),
    open_file: document.getElementById("open_file"),
    download_file: document.getElementById("download_file")
  },
  csv_config: {
    delimiter: " ",
  },
  about_init: () => {
    var about_link = document.getElementById("about_link")
    var about = document.getElementById("about")
    about_link.addEventListener("click", function() {
      about.classList.toggle("hidden")
    })
  },
  save_file: data => {
    localStorage.setItem("file_name", app.current_file_name)
    localStorage.setItem("file_content", JSON.stringify(data))
  },
  open_file: () => {
    content = localStorage.getItem("file_content")
    name = localStorage.getItem("file_name")
    if (!content || !name) return
    app.current_file_name = name
    list.set_content(JSON.parse(content))
  },
  download_file: () => {
    const dsv = Papa.unparse(list.get_content())
    app.download(app.current_file_name, dsv)
  },
  upload_file: file => {
    app.current_file_name = file.name
    app.csv_config.complete = data => {
      data.errors.forEach(error => {
        console.error(error)
      })
      list.set_content(data.data)
      app.save_file(list.get_content())
    }
    Papa.parse(file, app.csv_config)
  },
  init: () => {
    app.csv_config.complete = app.load_file
    app.dom.select_file.addEventListener("change", event => {
      const files = event.target.files
      if (!files.length) return
      app.upload_file(files[0])
    })
    app.dom.save_file.addEventListener("click", event => {
      app.dom.save_file.classList.add("active")
      app.save_file(list.get_content())
      app.dom.save_file.classList.remove("active")
    })
    app.dom.download_file.addEventListener("click", event => {
      app.download_file()
    })
    app.dom.open_file.addEventListener("click", event => {
      document.getElementById("select_file").click()
    })
    list.init()
    app.open_file()
    app.about_init()
  }
}

app.init()
