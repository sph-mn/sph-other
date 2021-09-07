const node_path = require("path")
const notifier = require("node-notifier")
// https://docs.microsoft.com/en-us/windows/win32/properties/props-system-appusermodel-id?redirectedfrom=MSDN

notifier.notify({
  title: "price checker",
  message: "testmessage",
  icon: node_path.join(__dirname, "/icon.png"),
  appID: "SnoreToast",
  sound: true,
  wait: true
})
