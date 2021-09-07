const child_process = require("child_process")

module.exports = {
  sites: [{
    disabled: false,
    handler: "media_markt",
    name: "Media Markt 3080",
    urls: [
      "https://www.mediamarkt.de/de/product/_gigabyte-geforce-rtx%E2%84%A2-3080-gaming-oc-10gb-gv-n3080gaming-oc-10gd-2683937.html",
      "https://www.mediamarkt.de/de/product/_zotac-geforce-rtx%E2%84%A2-3090-trinity-24gb-zt-a30900d-10p-2683242.html"
    ]
  }],
  check_wait_seconds: 60,
  url_wait_seconds: 0.5,
  write_log_file: false,
  headless: true,
  notify_hook: function(message, url) {
    //child_process.execFile("firefox", [url])
    child_process.execFile("smplayer", ["sound.mp3"])
    process.exit()
  }
}
