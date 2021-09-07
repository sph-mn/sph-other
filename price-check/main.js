const node_path = require("path")
const config = require("./config")
const fs = require("fs")
const moment = require("moment")
const program = require("commander").program
const puppeteer = require("puppeteer-extra")
const StealthPlugin = require("puppeteer-extra-plugin-stealth")
const notifier = require("node-notifier")
puppeteer.use(StealthPlugin())
program.option("-s, --site <name>", "check only this site")

const price_checker = {
  browser: null,
  page: null,
  log: function(line) {
    const timestamp = moment().format("YYYY-MM-DD HH:mm")
    line = timestamp + " " + line
    console.log(line)
    if (config.write_log_file) {
      fs.writeFileSync(__dirname + "/log.txt", line + "\n", {
        flag: "a"
      })
    }
  },
  media_markt_handler: async function() {
    return await price_checker.page.evaluate(() => {
      const button = document.getElementById("pdp-add-to-cart-button")
      if (button) {
        return {
          available: !button.disabled
        }
      } else {
        return {
          error: "error: media_markt button not found"
        }
      }
    });
  },
  check_url: async function(handler, url, site) {
    const result = await handler(url)
    if ("object" != typeof result) {
      price_checker.log("error: site handler result is not an object")
    } else if (result.error) {
      let line = "error: " + result.error + " (" + site.name + ", " + url + ")"
      price_checker.log(line)
      await price_checker.take_screenshot()
    } else if (result.available) {
      const message = "available! (" + site.name + ")\n  " + url
      price_checker.notify(message, url)
      price_checker.log(message)
    } else {
      price_checker.log("not available (" + site.name + ")")
    }
  },
  check: async function(name) {
    for (const site of config.sites) {
      if (site.disabled) continue;
      if (name && !site.name.includes(name)) continue;
      const handler = price_checker[site.handler + "_handler"]
      for (const url of site.urls) {
        price_checker.browser = await puppeteer.launch({
          headless: config.headless
        })
        price_checker.page = await price_checker.browser.newPage()
        await price_checker.page.setCacheEnabled(false)
        await price_checker.page.goto(url)
        await price_checker.check_url(handler, url, site)
        await price_checker.page.waitForTimeout(config.url_wait_seconds * 1000)
        await price_checker.page.close()
        await price_checker.browser.close()
      }
    }
  },
  check_wait: async function(name) {
    while (true) {
      await price_checker.check(name)
      await price_checker.sleep(config.check_wait_seconds)
    }
  },
  notify: function(message, url) {
    try {
      config.notify_hook(message, url)
      return
      notifier.notify({
        title: "price checker",
        message: message,
        icon: node_path.join(__dirname, "/icon.png"),
        appID: "Snore.DesktopToasts.0.7.0",
        sound: true,
        wait: true
      })
    } catch (exc) {
      price_checker.log(exc)
    }
  },
  take_screenshot: async function(page) {
    try {
      await price_checker.page.setViewport({
        width: 1280,
        height: 1024,
        deviceScalefactor: 1
      })
      return price_checker.page.screenshot({
        path: "error_screenshot.png",
        fullPage: true
      })
    } catch (error) {
      console.log("take_screenshot failed " + error)
    }
  },
  sleep: function(s) {
    return new Promise(resolve => setTimeout(resolve, s * 1000));
  }
}

program.parse(process.argv)
price_checker.check_wait(program.site)
//price_checker.notify("testmessage", "http://google.de")
