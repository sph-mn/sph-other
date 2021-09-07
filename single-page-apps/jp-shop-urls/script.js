const config = {
  translations: {
    cuffs: "お袖とめ",
    hair_ties: "ヘアゴム",
    gloves: "グローブ",
    accessories: "アクセサリー",
    hair_accessories: "ヘアアクセサリー",
    legwear: "レッグウェア",
    one_piece: "ワンピース",
    skirts: "スカート",
    small_items: "小物",
    tops: "トップス",
    alice_and_the_pirates: "アリス アンド ザ パイレーツ",
    angelic_pretty: "アンジェリック プリティ",
    baby_the_stars_shine_bright: "ベイビーザスターズシャインブライト",
    innocent_world: "イノセントワールド",
    metamorphose: "メタモルフォーゼ タンドゥフィーユ",
    none: "なし"
  },
  keywords: [
    ["お袖とめ", "cuffs"],
    ["ヘアゴム", "hair_ties"],
    ["グローブ", "gloves"]
  ],
  categories: [
    "accessories",
    "hair_accessories",
    "legwear",
    "one_piece",
    "skirts",
    "small_items",
    "tops"
  ],
  shops: {
    mercari: {
      make_url: function(brand, category, query) {
        category_child = config.shops.mercari.categories[category] || ""
        brand_config = config.shops.mercari.brands[brand]
        brand_name = encodeURIComponent(brand_config.name)
        brand_id = brand_config.id
        keyword = query
        return `https://www.mercari.com/jp/search/?page=1&sort_order=created_desc&keyword=${keyword}&category_root=1&category_child=${category_child}&brand_name=${brand_name}&brand_id=${brand_id}&size_group=&price_min=&price_max=&status_on_sale=1`
      },
      brands: {
        alice_and_the_pirates: {
          name: "アリス+アンド+ザ+パイレーツ",
          id: 154
        },
        angelic_pretty: {
          name: "アンジェリック プリティ",
          id: 188
        },
        baby_the_stars_shine_bright: {
          name: "ベイビーザスターズシャインブライト",
          id: 3897
        },
        innocent_world: {
          name: "イノセントワールド",
          id: 4046
        },
        metamorphose: {
          name: "メタモルフォーゼ タンドゥフィーユ",
          id: 1223
        }
      },
      categories: {
        accessories: 21,
        bags: 20,
        hair_accessories: 22,
        legwear: 18,
        one_piece: 15,
        pants: 13,
        shoes: 16,
        skirt: 14,
        small_items: 23,
        tops: 11
      }
    },
    rakuma: {
      make_url: (brand, category, query) => {
        brand_id = config.shops.rakuma.brands[brand]
        category_id = config.shops.rakuma.categories[category] || config.shops.rakuma.root_categories.ladies
        return `https://fril.jp/s?query=${query}&category_id=${category_id}&brand_id=${brand_id}&transaction=selling&sort=created_at&order=desc`
      },
      brands: {
        innocent_world: 384,
        alice_and_the_pirates: 34,
        metamorphose: 533,
        angelic_pretty: 49,
        baby_the_stars_shine_bright: 81
      },
      root_categories: {
        ladies: 10001
      },
      categories: {
        accessories: 8,
        bags: 5,
        hair_accessories: 9,
        legwear: 15,
        one_piece: 2,
        pants: 3,
        shoes: 7,
        skirt: 4,
        small_items: 6,
        tops: 1
      }
    },
    closetchild: {
      make_url: (brand, category, query) => {
        var category_id
        if (category) {
          category_id = config.shops.closetchild.categories[brand][category]
        } else {
          category_id = config.shops.closetchild.brands[brand]
        }
        return `https://www.closetchildonlineshop.com/product-list/${category_id}?available=1`
      },
      brands: {
        alice_and_the_pirates: 213,
        angelic_pretty: 190,
        baby_the_stars_shine_bright: 202,
        innocent_world: 235,
        metamorphose: 279
      },
      categories: {
        alice_and_the_pirates: {
          accessories: 8,
          bags: 5,
          hair_accessories: 9,
          legwear: 15,
          one_piece: 2,
          pants: 3,
          shoes: 7,
          skirt: 4,
          small_items: 6,
          tops: 1
        },
        angelic_pretty: {
          accessories: 8,
          bags: 5,
          hair_accessories: 9,
          legwear: 15,
          one_piece: 2,
          pants: 3,
          shoes: 7,
          skirt: 4,
          small_items: 6,
          tops: 1
        },
        baby_the_stars_shine_bright: {
          accessories: 8,
          bags: 5,
          hair_accessories: 9,
          legwear: 15,
          one_piece: 2,
          pants: 3,
          shoes: 7,
          skirt: 4,
          small_items: 6,
          tops: 1
        },
        innocent_world: {
          accessories: 8,
          bags: 5,
          hair_accessories: 9,
          legwear: 15,
          one_piece: 2,
          pants: 3,
          shoes: 7,
          skirt: 4,
          small_items: 6,
          tops: 1
        },
        metamorphose: {
          accessories: 8,
          bags: 5,
          hair_accessories: 9,
          legwear: 15,
          one_piece: 2,
          pants: 3,
          shoes: 7,
          skirt: 4,
          small_items: 6,
          tops: 1
        }
      }
    },
    wunderwelt: {
      make_url: (brand, category, query) => {
        brand_id = config.shops.wunderwelt.brands[brand]
        category_id = config.shops.wunderwelt.categories[category] || ""
        return `https://www.wunderwelt.jp/brands/${brand_id}/${category_id}?keywords=${query}`
      },
      brands: {
        alice_and_the_pirates: "alice-and-the-pirates",
        angelic_pretty: "angelic-pretty",
        baby_the_stars_shine_bright: "baby-the-stars-shine-bright",
        innocent_world: "innocent-world",
        metamorphose: "metamorphose"
      },
      categories: {
        accessories: 8,
        bags: 5,
        hair_accessories: 9,
        legwear: 15,
        one_piece: "one-piece",
        pants: 3,
        shoes: 7,
        skirt: 4,
        small_items: 6,
        tops: 1
      }
    }
  }
}

/*
    baby_the_stars_shine_bright: {},
    angelic_pretty_paris: {},
    angelic_pretty: {},
    innocent_world: {},
    metamorphose: {}
*/

const shops = Object.keys(config.shops)

function make_links(query, category) {
  const links = []
  shops.forEach(shop => {

    const brands = Object.keys(config.shops[shop].brands).sort()
    brands.forEach(brand => {
      const url = config.shops[shop].make_url(brand, category, query)
      const a = document.createElement("a")
      const div = document.createElement("div")
      a.href = url
      a.innerHTML = [shop, config.translations[brand], config.translations[category], query].filter(a => a).join(" - ")
      a.target = "_blank"
      div.appendChild(a)
      links.push(div)
    })
  })
  return links
}

const dom = {
  query: document.getElementById("query"),
  links: document.getElementById("links"),
  keywords: document.getElementById("keywords"),
  categories: document.getElementById("categories"),
  query_reset: document.getElementById("query_reset")
}

function update_links() {
  const links = make_links(dom.query.value.trim(), dom.query.getAttribute("data-category"))
  dom.links.innerHTML = ""
  links.forEach(link => {
    dom.links.appendChild(link)
  })
}

function make_keyword_element(value, name) {
  const a = document.createElement("a")
  a.href = "#"
  a.innerHTML = name
  a.addEventListener("click", event => {
    dom.query.value = value
    update_links()
  })
  const div = document.createElement("div")
  div.appendChild(a)
  return div
}

function update_keywords() {
  dom.keywords.innerHTML = ""
  config.keywords.forEach(keyword => {
    dom.keywords.appendChild(make_keyword_element(keyword[0], config.translations[keyword[1]]))
  })
}

function make_category_element(value, name) {
  const a = document.createElement("a")
  a.href = "#"
  a.innerHTML = name
  a.addEventListener("click", event => {
    dom.query.setAttribute("data-category", value)
    update_links()
  })
  const div = document.createElement("div")
  div.appendChild(a)
  return div
}

function update_categories() {
  dom.categories.innerHTML = ""
  dom.categories.appendChild(make_category_element("", config.translations["none"]))
  config.categories.forEach(category => {
    dom.categories.appendChild(make_category_element(category, config.translations[category]))
  })
}

dom.query.addEventListener("keyup", event => {
  update_links()
})

dom.query_reset.addEventListener("click", event => {
  dom.query.value = ""
  update_links()
})

update_links()
update_keywords()
update_categories()
