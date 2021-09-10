// https://github.com/KoryNunn/crel 4.2.1
(e => {
  const t = "function",
    n = "isNode",
    o = (e, t) => typeof e === t,
    r = (e, t) => {
      null !== t && (Array.isArray(t) ? t.map(t => r(e, t)) : (i[n](t) || (t = document.createTextNode(t)), e.appendChild(t)))
    };

  function i(e, a) {
    let d, f, l = arguments,
      c = 1;
    if (e = i.isElement(e) ? e : document.createElement(e), o(a, "object") && !i[n](a) && !Array.isArray(a))
      for (d in c++, a) f = a[d], o(d = i.attrMap[d] || d, t) ? d(e, f) : o(f, t) ? e[d] = f : e.setAttribute(d, f);
    for (; c < l.length; c++) r(e, l[c]);
    return e
  }
  i.attrMap = {}, i.isElement = (e => e instanceof Element), i[n] = (e => e instanceof Node), i.proxy = new Proxy(i, {
    get: (e, t) => (!(t in i) && (i[t] = i.bind(null, t)), i[t])
  }), e(i, t)
})((e, t) => {
  "object" == typeof exports ? module.exports = e : typeof define === t && define.amd ? define(() => e) : this.crel = e
});

const config = {
  defaults: {
    brands: [
      "innocent_world",
      "alice_and_the_pirates",
      "metamorphose",
      "angelic_pretty",
      "baby_the_stars_shine_bright"
    ]
  },
  keyword_presets: [
    "any",
    "ブラウス",
    "お袖とめ",
    "ヘアゴム",
    "手袋",
    "グローブ",
    "ボレロ"
  ],
  categories: [
    "any",
    "accessories",
    "hair_accessories",
    "legwear",
    "one_piece",
    "skirts",
    "small_items",
    "tops",
    "blouse"
  ],
  shops: {
    mercari: {
      make_url: function(brand, category, query) {
        let category_ids = config.shops.mercari.categories[category] || ""
        let category_child
        if (Array.isArray(category_ids)) {
          category_child = category_ids[0] + "&" + category_ids.slice(1).map(x => `category_grand_child[${x}]=1`).join("&")
        }
        else {
          category_child = category_ids
        }
        brand_config = config.shops.mercari.brands[brand]
        brand_name = brand_config.name
        brand_id = brand_config.id
        keyword = query
        return `https://www.mercari.com/jp/search/?page=1&sort_order=created_desc&keyword=${keyword}&category_root=1&category_child=${category_child}&brand_name=${brand_name}&brand_id=${brand_id}&size_group=&price_min=&price_max=&status_on_sale=1`
      },
      brands: {
        any: {
          name: "",
          id: ""
        },
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
        tops: 11,
        blouse: [11, 121, 122]
      }
    },
    rakuma: {
      make_url: (brand, category, query) => {
        brand_id = config.shops.rakuma.brands[brand]
        category_id = config.shops.rakuma.categories[category] || config.shops.rakuma.root_categories.ladies
        return `https://fril.jp/s?query=${query}&category_id=${category_id}&brand_id=${brand_id}&transaction=selling&sort=created_at&order=desc`
      },
      brands: {
        any: "",
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
        let categories = config.shops.closetchild.categories
        let brands = config.shops.closetchild.brands
        let category_id
        if (category && brand && categories[brand] && categories[brand][category]) {
          category_id = categories[brand][category]
        }
        else if (category && categories[category]) {
          category_id = categories[category]
        }
        else if (brand && brands[brand]) {
          category_id = brands[brand]
        }
        else {
          category_id = ""
        }
        let url = `https://www.closetchildonlineshop.com/product-list/${category_id}?available=1`
        if (query) {
          url += "&keyword=" + query
        }
        return url
      },
      brands: {
        any: "",
        alice_and_the_pirates: 213,
        angelic_pretty: 190,
        baby_the_stars_shine_bright: 202,
        innocent_world: 235,
        metamorphose: 279
      },
      categories: {
        one_piece: 1,
        skirt: 3,
        blouse: 2,
        tops: 5,
        pants: 10,
        jacket: 6,
        shoes: 51,
        bags: 51,
        accessories: 52,
        hair_accessories: 52,
        legwear: 52,
        small_items: 223,
        alice_and_the_pirates: {
          one_piece: 214,
          skirt: 215,
          blouse: 216,
          tops: 217,
          pants: 219,
          jacket: 220,
          shoes: 220,
          bags: 222,
          accessories: 223,
          hair_accessories: 223,
          legwear: 223,
          small_items: 223
        },
        angelic_pretty: {
          one_piece: 191,
          skirt: 192,
          blouse: 193,
          tops: 194,
          pants: 196,
          jacket: 197,
          shoes: 200,
          bags: 200,
          accessories: 201,
          hair_accessories: 201,
          legwear: 201,
          small_items: 201
        },
        baby_the_stars_shine_bright: {
          one_piece: 203,
          skirt: 204,
          blouse: 205,
          tops: 206,
          pants: 208,
          jacket: 209,
          shoes: 211,
          bags: 211,
          accessories: 212,
          hair_accessories: 212,
          legwear: 212,
          small_items: 212
        },
        innocent_world: {
          one_piece: 236,
          skirt: 237,
          blouse: 238,
          tops: 239,
          pants: 241,
          jacket: 242,
          shoes: 244,
          bags: 244,
          accessories: 245,
          hair_accessories: 245,
          legwear: 245,
          small_items: 245
        },
        metamorphose: {
          one_piece: 280,
          skirt: 281,
          blouse: 282,
          tops: 283,
          pants: 285,
          jacket: 286,
          shoes: 288,
          bags: 288,
          accessories: 289,
          hair_accessories: 289,
          legwear: 289,
          small_items: 289
        }
      }
    },
    wunderwelt: {
      make_url: (brand, category, query) => {
        if (!query) return
        var brand_id = config.shops.wunderwelt.brands[brand]
        var category_id = config.shops.wunderwelt.categories[category] || ""
        return `https://www.wunderwelt.jp/brands/${brand_id}/${category_id}?keywords=${query}`
      },
      brands: {
        any: "lolita-fashion",
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
    },
    baby_the_stars_shine_bright: {
      make_url: (brand, category, query) => {
        if (!query) return
        return `https://babyssb.shop/language/ja?q=${query}`
      },
      brands: {
        baby_the_stars_shine_bright: true,
        alice_and_the_pirates: true
      }
    },
    angelic_pretty_paris: {
      make_url: (brand, category, query) => {
        if (!query) return
        query = i18n.translations.en[query] || query
        return `https://angelicpretty-paris.com/gb/recherche?controller=search&orderby=position&orderway=desc&search_query=${query}`
      },
      brands: {
        angelic_pretty: true
      }
    },
    angelic_pretty: {
      make_url: (brand, category, query) => {
        if (!query) return
        return `https://angelicpretty-onlineshop.com/?orderby=date&s=${query}&post_type=product`
      },
      brands: {
        angelic_pretty: true
      }
    },
    metamorphose: {
      make_url: (brand, category, query) => {
        if (!query) return
        return `https://metamorphose.gr.jp/ja/product/all?metamorphose_search=${query}`
      },
      brands: {
        metamorphose: true
      }
    },
    innocent_world: {
      make_url: (brand, category, query) => {
        if (!query) return
        return `https://innocent-w.jp/fs/innocentworld/GoodsSearchList.html?_e_k=%EF%BC%A1&keyword=${query}&x=14&y=10`
      },
      brands: {
        innocent_world: true
      }
    },
  }
}

const i18n = {
  language: "en",
  translate: (key) => {
    return i18n.translations[i18n.language][key] || (key && key.replace(/_/g, " "))
  },
  translations: {
    jp: {
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
      any: "フリー",
      keywords: "キーワード",
      brands: "ブランド",
      clear: "クリア",
      categories: "カテゴリー",
      links: "リンク",
      rakuma: "ラクマ",
      mercari: "メルカリ",
      wunderwelt: "ワンダーウェルト"
    },
    en: {
      "ブラウス": "blouse",
      "ボレロ": "bolero",
      "お袖とめ": "cuffs",
      "ヘアゴム": "hair ties",
      "手袋": "gloves1",
      "グローブ": "gloves2"
    }
  }
}

const helper = {
  make_section: (id, label) => {
    let div = crel("div", {
      "class": "label"
    }, label)
    return crel("section", {
      id: id
    }, div)
  },
  make_link_button: (label) => {
    return crel("span", {
      "class": "link-button"
    }, label)
  }
}

class keyword_selection_class {
  dom = {}
  get_selections() {
    return this.dom.input.value
  }
  build() {
    let preset_links = config.keyword_presets.map(string => {
      let link = helper.make_link_button(i18n.translate(string))
      link.addEventListener("click", event => {
        this.dom.input.value = "any" == string ? "" : string
        this.app.link_list.update()
      })
      return link
    })
    this.dom.presets = crel("div", {
      "class": "presets hlist"
    }, preset_links)
    this.dom.input = crel("input", {
      "class": "input"
    })
    this.dom.input.addEventListener("keyup", event => {
      this.app.link_list.update()
    })
    let section = helper.make_section("keyword_selection", i18n.translate("keywords"))
    section.appendChild(this.dom.presets)
    section.appendChild(this.dom.input)
    this.dom.container = section
  }
  constructor(app) {
    this.app = app
    this.build()
  }
}

class brand_selection_class {
  dom = {}
  build() {
    let brands = Object.keys(config.shops).map(shop => {
      return Object.keys(config.shops[shop].brands)
    }).flat()
    brands = [...new Set(brands)]
    let checkboxes = brands.map(brand => {
      const input = crel("input", {
        type: "checkbox",
        value: brand
      })
      if (config.defaults.brands.includes(brand)) {
        input.checked = true
      }
      input.addEventListener("change", event => {
        this.app.link_list.update()
      })
      return crel("label", input, i18n.translate(brand))
    })
    this.dom.checkboxes = crel("div", {
      "class": "checkboxes hlist"
    }, checkboxes)
    let section = helper.make_section("brand_selection", i18n.translate("brands"))
    section.appendChild(this.dom.checkboxes)
    this.dom.container = section
  }
  get_selections() {
    let checkboxes = this.dom.checkboxes.querySelectorAll("input:checked")
    return Array.from(checkboxes).map(x => x.value)
  }
  constructor(app) {
    this.app = app
    this.build()
  }
}

class category_selection_class {
  dom = {}
  get_selections() {
    return this.currentCategory
  }
  build() {
    let links = config.categories.map(name => {
      let link = helper.make_link_button(i18n.translate(name))
      link.addEventListener("click", event => {
        this.currentCategory = name
        this.app.link_list.update()
      })
      return link
    })
    this.dom.links = crel("div", {
      "class": "links hlist"
    }, links)
    let section = helper.make_section("category_selection", i18n.translate("categories"))
    section.appendChild(this.dom.links)
    this.dom.container = section
  }
  constructor(app) {
    this.app = app
    this.build()
  }
}

class link_list_class {
  dom = {}
  make_links(brands, category, keyword) {
    if ("any" == category) category = ""
    return Object.keys(config.shops).map(shop_key => {
      let shop = config.shops[shop_key]
      let links = brands.map(brand => {
        if (!shop.brands || undefined == shop.brands[brand]) return
        const url = shop.make_url(brand, category, keyword)
        if (!url) return
        let label_brand = "any" == brand ? "" : brand
        let label_category = shop.categories ? category : ""
        const label = [shop_key, label_brand, label_category, keyword].filter(a => a).map(i18n.translate).join(" - ")
        return crel("a", {
          href: url,
          target: "_blank"
        }, label)
      })
      return links.filter(a => a)
    }).flat()
  }
  build() {
    this.dom.links = crel("div", {
      "class": "links vlist"
    })
    let section = helper.make_section("link_list", i18n.translate("links"))
    section.appendChild(this.dom.links)
    this.dom.container = section
  }
  update() {
    let keyword = this.app.keyword_selection.get_selections()
    let category = this.app.category_selection.get_selections()
    let brands = this.app.brand_selection.get_selections()
    const links = this.make_links(brands, category, keyword)
    this.dom.links.innerHTML = ""
    links.forEach(link => {
      this.dom.links.appendChild(link)
    })
  }
  constructor(app) {
    this.app = app
    this.build()
    this.update()
  }
}

class app_class {
  dom = {
    container: document.querySelector("body")
  }
  constructor() {
    this.keyword_selection = new keyword_selection_class(this)
    this.dom.container.appendChild(this.keyword_selection.dom.container)
    this.brand_selection = new brand_selection_class(this)
    this.dom.container.appendChild(this.brand_selection.dom.container)
    this.category_selection = new category_selection_class(this)
    this.dom.container.appendChild(this.category_selection.dom.container)
    this.link_list = new link_list_class(this)
    this.dom.container.appendChild(this.link_list.dom.container)
  }
}

new app_class
