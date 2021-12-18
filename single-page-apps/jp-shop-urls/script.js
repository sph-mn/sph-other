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
    max_price: 50000,
    brands: [
      "innocent_world",
      "alice_and_the_pirates",
      "metamorphose",
      "angelic_pretty",
      "baby_the_stars_shine_bright"
    ]
  },
  categories: [
    "accessories",
    "any",
    "blouse",
    "pumpkin_pants",
    "bag",
    "bolero",
    "cape",
    "coat",
    "gloves",
    "hair_accessories",
    "hair_ties",
    "half_pants",
    "hat",
    "jacket",
    "jewelry",
    "jumper_skirt",
    "legwear",
    "one_piece",
    "ouji_style",
    "overknee_socks",
    "pants",
    "salopette",
    "shoes",
    "skirt",
    "socks",
    "tights",
    "top",
    "vest",
    "wristbands"
  ],
  example_keywords: [
    "pink",
    "sax",
    "light_blue"
  ],
  shops: {
    rakuma: {
      make_url: (brand, category, keywords) => {
        brand_id = config.shops.rakuma.brands[brand]
        let category_id
        category = config.shops.rakuma.category_alias[category] || category
        if (category) {
          category_id = config.shops.rakuma.categories[category]
          if (!category_id) {
            keywords = helper.keywords_add_category(keywords, category)
            category_id = config.shops.rakuma.root_categories.ladies
          }
        } else {
          category_id = config.shops.rakuma.root_categories.ladies
        }
        return `https://fril.jp/s?query=${keywords}&category_id=${category_id}&brand_id=${brand_id}&transaction=selling&sort=created_at&order=desc&max=${config.defaults.max_price}`
      },
      brands: {
        any: "",
        innocent_world: 384,
        alice_and_the_pirates: 34,
        metamorphose: 533,
        angelic_pretty: 49,
        baby_the_stars_shine_bright: 81
      },
      other_brand_as_keyword: true,
      root_categories: {
        ladies: 10001
      },
      category_alias: {
        blouse: "top",
        socks: "legwear",
        jumper_skirt: "one_piece",
        vest: "top",
        jewelry: "accessories"
      },
      categories: {
        accessories: 8,
        bags: 7,
        hair_accessories: 9,
        legwear: 11,
        one_piece: 3,
        jacket: 2,
        pants: 5,
        shoes: 4,
        skirt: 6,
        top: 1,
        hat: 12
      }
    },
    mercari: {
      make_url: function(brand, category, keywords) {
        category = config.shops.mercari.category_alias[category] || category
        let cat_0 = 1
        let category_params = "&t1_category_id=" + cat_0
        if (category) {
          let category_ids = config.shops.mercari.categories[category]
          if (category_ids) {
            if (Number.isInteger(category_ids)) category_ids = {
              cat_1: category_ids
            }
            category_params += "&t2_category_id=" + category_ids.cat_1
            if (category_ids.cat_2) {
              category_params += "&t3_category_id=" + category_ids.cat_2.join("%2C")
              category_params += "&category_id=" + [category_ids.cat_1].concat(category_ids).join("%2C")
            } else {
              category_params += "&category_id=" + category_ids.cat_1
            }
            if (category_ids.keywords && !keywords) keywords = category_ids.keywords
          } else {
            keywords = helper.keywords_add_category(keywords, category)
            category_params = ""
          }
        }
        const brand_config = config.shops.mercari.brands[brand]
        let brand_id = brand_config.id
        let brand_name = ""
        if (brand_id) {
          brand_name = brand_config.name
        } else {
          keywords = (brand_config.name + " " + keywords).trim()
        }
        return `https://www.mercari.com/jp/search?keyword=${keywords}&page=1&sort_order=created_desc&${category_params}&brand_name=${brand_name}&brand_id=${brand_id}&size_group=&price_min=&price_max=${config.defaults.max_price}&status=on_sale`
      },
      other_brand_as_keyword: true,
      brands: {
        any: {
          name: "",
          id: ""
        },
        alice_and_the_pirates: {
          name: "アリス アンド ザ パイレーツ",
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
        },
        atelier_boz: {
          name: "アトリエ ボズ",
          id: 108
        },
        marble: {
          name: "マーブル",
          id: 6065
        }
      },
      category_alias: {
        socks: "legwear",
        overknee_socks: "legwear",
        tights: "legwear",
        jumper_skirt: "one_piece"
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
        accessories: 23,
        top: 11,
        hat: 19,
        blouse: {
          cat_1: 11,
          cat_2: [121, 122]
        },
        vest: {
          cat_1: 11,
          cat_2: [131]
        }
      }
    },
    closetchild: {
      make_label: (brand, category, keywords) => {
        if (keywords) return ["", "", keywords]
        let original_category = category
        let category_alias = config.shops.closetchild.category_alias
        category = category_alias[category] || category
        let categories = config.shops.closetchild.categories
        let brands = config.shops.closetchild.brands
        if (category && brand && categories[category] && categories[category][brand]) {
          return [brand, original_category, ""]
        } else if (category && !brand && categories[category].any) {
          return ["", original_category, ""]
        } else if (!category && brand && brands[brand]) {
          return [brand, "", ""]
        } else if (category) {
          return ["", "", i18n.translate(category)]
        } else {
          return [brand, "", ""]
        }
      },
      make_url: (brand, category, keywords) => {
        [brand, category, keywords] = config.shops.closetchild.make_label(brand, category, keywords)
        if (keywords) {
          return `https://www.closetchildonlineshop.com/product-list?available=1&keyword=${keywords}`
        }
        category = config.shops.closetchild.category_alias[category] || category
        let category_id = ""
        let categories = config.shops.closetchild.categories
        if (category && brand) {
          category_id = categories[category][brand]
        } else if (category) {
          category_id = categories[category].any
          brand = "any"
        } else if (brand) {
          let brands = config.shops.closetchild.brands
          category_id = brands[brand]
        }
        if ("any" == brand) {
          return `https://www.closetchildonlineshop.com/product-group/${category_id}?available=1`
        } else {
          return `https://www.closetchildonlineshop.com/product-list/${category_id}?available=1`
        }
      },
      brands: {
        any: "",
        alice_and_the_pirates: 213,
        angelic_pretty: 190,
        baby_the_stars_shine_bright: 202,
        innocent_world: 235,
        metamorphose: 279
      },
      other_brand_as_keyword: true,
      category_alias: {
        jumper_skirt: "one_piece",
        bags: "shoes",
        jewelry: "accessories",
        hair_accessories: "accessories",
        legwear: "accessories",
        overknee_socks: "accessories",
        socks: "accessories"
      },
      categories: {
        coat: {
          any: 7
        },
        one_piece: {
          any: 1,
          alice_and_the_pirates: 214,
          angelic_pretty: 191,
          baby_the_stars_shine_bright: 203,
          innocent_world: 236,
          metamorphose: 280
        },
        skirt: {
          any: 3,
          alice_and_the_pirates: 215,
          angelic_pretty: 192,
          baby_the_stars_shine_bright: 204,
          innocent_world: 237,
          metamorphose: 281
        },
        blouse: {
          any: 2,
          alice_and_the_pirates: 216,
          angelic_pretty: 193,
          baby_the_stars_shine_bright: 205,
          innocent_world: 238,
          metamorphose: 282
        },
        top: {
          any: 5,
          alice_and_the_pirates: 217,
          angelic_pretty: 194,
          baby_the_stars_shine_bright: 206,
          innocent_world: 239,
          metamorphose: 283
        },
        pants: {
          any: 10,
          alice_and_the_pirates: 219,
          angelic_pretty: 196,
          baby_the_stars_shine_bright: 208,
          innocent_world: 241,
          metamorphose: 285
        },
        jacket: {
          any: 6,
          alice_and_the_pirates: 220,
          angelic_pretty: 197,
          baby_the_stars_shine_bright: 209,
          innocent_world: 242,
          metamorphose: 286
        },
        shoes: {
          any: 51,
          alice_and_the_pirates: 220,
          angelic_pretty: 200,
          baby_the_stars_shine_bright: 211,
          innocent_world: 244,
          metamorphose: 288
        },
        accessories: {
          any: 52,
          alice_and_the_pirates: 223,
          angelic_pretty: 201,
          baby_the_stars_shine_bright: 212,
          innocent_world: 245,
          metamorphose: 289
        }
      }
    },
    wunderwelt: {
      make_label: (brand, category, keywords) => {
        if (keywords) {
          return ["", "", keywords]
        }
        if (category) {
          let category_data = config.shops.wunderwelt.categories[category]
          if (category_data) {
            if (brand && category_data[brand]) return [brand, category, ""]
            else {
              return ["", category, ""]
            }
          }
        } else if (brand) {
          return [brand, "", ""]
        }
      },
      make_url: (brand, category, keywords) => {
        let category_id
        let brand_id
        if (keywords) {
          return `https://www.wunderwelt.jp/en/pages/search-results-page?q=${keywords}`
        }
        if (category) {
          let category_data = config.shops.wunderwelt.categories[category]
          if (category_data) {
            if (brand && category_data[brand]) category_id = category_data[brand]
            else {
              category_id = category_data.any
            }
          }
        } else if (brand) {
          category_id = config.shops.wunderwelt.brands[brand]
        }
        return `https://www.wunderwelt.jp/collections/${category_id}`
      },
      brands: {
        any: "lolita-fashion",
        alice_and_the_pirates: "alice-and-the-pirates-used",
        angelic_pretty: "angelic-pretty-used",
        baby_the_stars_shine_bright: "baby-the-stars-shine-bright-used",
        innocent_world: "innocent-world-used",
        metamorphose: "metamorphose-used"
      },
      categories: {
        coat: {
          any: "coat-used"
        },
        jewelry: {
          any: "accessories-used"
        },
        bags: {
          any: "bag-used"
        },
        hair_accessories: {
          any: "hair_accessory-used"
        },
        legwear: {
          any: "legwear-used"
        },
        one_piece: {
          any: "one-piece-used"
        },
        jumper_skirt: {
          any: "jumper-skirt-used"
        },
        cutsew: {
          any: "cut-sew-used"
        },
        pants: {
          any: "bottoms-used"
        },
        shoes: {
          any: "shoes-used"
        },
        skirt: {
          any: "bottoms-used"
        },
        accessories: {
          any: "accessories-used"
        },
        blouse: {
          any: "blouse-used"
        }
      }
    },
    baby_the_stars_shine_bright: {
      make_url: (brand, category, keywords) => {
        const categories = config.shops.baby_the_stars_shine_bright.categories
        if (category) {
          let category_id = categories[category]
          return `https://babyssb.shop/category/select/cid/${category_id}`
        }
        if (!keywords) return
        return `https://babyssb.shop/language/ja?q=${keywords}&search_button.x=11&search_button.y=8`
      },
      brands: {
        baby_the_stars_shine_bright: true,
        alice_and_the_pirates: true
      },
      categories: {
        skirt: 343
      }
    },
    angelic_pretty_paris: {
      make_url: (brand, category, keywords) => {
        if (!keywords) return
        shop_keywords = i18n.translations.en[keywords] || keywords
        return `https://angelicpretty-paris.com/gb/recherche?controller=search&orderby=position&orderway=desc&search_query=${shop_keywords}`
      },
      brands: {
        angelic_pretty: true
      }
    },
    angelic_pretty: {
      make_url: (brand, category, keywords) => {
        if (!keywords) return
        return `https://angelicpretty-onlineshop.com/?orderby=date&s=${keywords}&post_type=product`
      },
      brands: {
        angelic_pretty: true
      }
    },
    metamorphose: {
      make_url: (brand, category, keywords) => {
        if (!keywords && category) keywords = i18n.translations.jp[category]
        if (!keywords) return
        return `https://metamorphose.gr.jp/ja/product/all?metamorphose_search=${keywords}&op=+`
      },
      brands: {
        metamorphose: true
      }
    },
    innocent_world: {
      make_url: (brand, category, keywords) => {
        if (keywords) {
          return `https://innocent-w.jp/fs/innocentworld/GoodsSearchList.html?_e_k=%EF%BC%A1&keyword=${keywords}&x=14&y=10`
        } else if (category) {
          let category_id = config.shops.innocent_world.categories[category]
          if (!category_id) return
          return `https://innocent-w.jp/fs/innocentworld/c/${category_id}`
        }
      },
      brands: {
        innocent_world: true
      },
      categories: {
        one_piece: "one-piece",
        jumper_skirt: "jumperskirt",
        blouse: "top",
        top: "top",
        pants: "bottom",
        skirt: "bottom",
        jacket: "outer",
        shoes: "shoes",
        bags: "bag",
        accessories: "ac",
        hair_accessories: "hd",
        legwear: "shoes",
        socks: "shoes",
        overknee_socks: "shoes",
        jewelry: "other"
      }
    },
    atelier_pierrot: {
      make_url: (brand, category, keywords) => {
        // keyword argument must be EUC-JP encoded, no idea how to achieve that
        return
        if (!keywords) return
        window.location.href = `https://atelier-pierrot.shop-pro.jp/?mode=srh&sort=n&cid=&keyword=${keywords}`
        return `https://atelier-pierrot.shop-pro.jp/?mode=srh&sort=n&cid=&keyword=${keywords}`
      },
      brands: {
        any: "",
        marble: "マーブル"
      }
    }
  }
}

const i18n = {
  language: "jp",
  translate: (key) => {
    return i18n.translations[i18n.language][key] || (key && key.replace(/_/g, " "))
  },
  translations: {
    jp: {
      half_pants: "ハーフパンツ",
      jacket: "ジャケット",
      wristbands: "お袖とめ",
      bag: "バッグ",
      tights: "タイツ",
      socks: "ソックス",
      skirt: "スカート",
      salopette: "サロペット",
      pants: "パンツ",
      pumpkin_pants: "カボチャパンツ",
      overknee_socks: "オーバーニー",
      ouji_style: "皇子系",
      bolero: "ボレロ",
      cape: "ケープ",
      shoes: "靴",
      coat: "コート",
      mantle: "マント",
      accessories: "小物",
      alice_and_the_pirates: "アリス アンド ザ パイレーツ",
      angelic_pretty: "アンジェリック プリティ",
      any: "フリー",
      baby_the_stars_shine_bright: "ベイビーザスターズシャインブライト",
      blouse: "ブラウス",
      brands: "ブランド",
      categories: "カテゴリー",
      clear: "クリア",
      cuffs: "お袖とめ",
      gloves: "グローブ",
      hat: "帽子",
      hair_accessories: "ヘアアクセサリー",
      hair_ties: "ヘアゴム",
      innocent_world: "イノセントワールド",
      jewelry: "アクセサリー",
      keywords: "キーワード",
      legwear: "レッグウェア",
      links: "リンク",
      marble: "マーブル",
      mercari: "メルカリ",
      metamorphose: "メタモルフォーゼ タンドゥフィーユ",
      one_piece: "ワンピース",
      rakuma: "ラクマ",
      jumper_skirt: "ジャンパースカート",
      top: "トップス",
      vest: "ベスト",
      sax: "サックス",
      light_blue: "水色",
      pink: "ピンク",
      wunderwelt: "ワンダーウェルト"
    },
    en: {}
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
  },
  keywords_add_category: (keywords, category) => {
    let category_keyword = i18n.translations.jp[category]
    if (category_keyword) {
      keywords = keywords ? keywords + " " : ""
      keywords += category_keyword
    }
    return keywords
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

class keyword_selection_class {
  dom = {}
  get_selections() {
    return this.dom.input.value
  }
  build() {
    this.dom.input = crel("input", {
      "class": "input"
    })
    this.dom.input.addEventListener("keyup", event => {
      this.app.link_list.update()
    })
    this.dom.input.addEventListener("blur", event => {
      this.app.link_list.update()
    })
    let keyword_buttons = config.example_keywords.map(keyword => {
      let value = i18n.translations.jp[keyword]
      let button = crel("span", {"class": "link-button"}, i18n.translate(keyword))
      button.addEventListener("click", event => {
        this.dom.input.value = value
        this.app.link_list.update()
      })
      return button
    })
    this.dom.examples = crel("div", {"class": "example_keywords hlist"}, keyword_buttons)
    let section = helper.make_section("keyword_selection", i18n.translate("keywords"))
    section.appendChild(this.dom.input)
    section.appendChild(this.dom.examples)
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

class link_list_class {
  dom = {}
  make_label(shop, shop_key, brand, category, keywords) {
    if (shop.make_label) {
      [brand, category, keywords] = shop.make_label(brand, category, keywords)
    }
    let label = [i18n.translate(shop_key)]
    if (brand && 1 < Object.keys(shop.brands).length && "any" != brand) label.push(i18n.translate(brand))
    if (category && shop.categories && 1 < Object.keys(shop.categories).length) label.push(i18n.translate(category))
    if (keywords) label.push(keywords)
    return label.filter(a => a).join(" - ")
  }
  make_links(brands, category, keywords) {
    let links = Object.keys(config.shops).map(shop_key => {
      let shop = config.shops[shop_key]
      let links = brands.map(brand => {
        let shop_keywords = keywords
        if (!shop.brands || undefined == shop.brands[brand]) {
          if (shop.other_brand_as_keyword) {
            let brand_keyword = brand.replace(/_/g, " ")
            shop_keywords = shop_keywords ? shop_keywords + " " + brand_keyword : brand_keyword
          } else {
            return
          }
        }
        let shop_category = category
        if (category && !(shop.categories && (shop.categories[category] || (shop.category_alias && shop.categories[shop.category_alias[category]])))) {
          shop_keywords = helper.keywords_add_category(keywords, category)
          shop_category = ""
        }
        const url = shop.make_url(brand, shop_category, shop_keywords)
        if (!url) return
        let label = this.make_label(shop, shop_key, brand, shop_category, shop_keywords)
        return crel("a", {
          href: url,
          target: "_blank"
        }, label)
      })
      return links.filter(a => a)
    }).flat()
    let urls = {}
    return links.filter(a => {
      if (urls[a.href]) {
        return false
      } else {
        urls[a.href] = true
        return true
      }
    })
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
    let category = this.app.category_selection.get_selections()
    let keywords = this.app.keyword_selection.get_selections()
    let brands = this.app.brand_selection.get_selections()
    const links = this.make_links(brands, category, keywords)
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
  set_language() {
    const query = window.location.search
    if (!query) return
    const params = query.replace("?", "").split("&")
    if (params.includes("language=en")) i18n.language = "en"
  }
  constructor() {
    this.set_language()
    config.categories = config.categories.sort((a, b) => i18n.translate(a).localeCompare(i18n.translate(b)))
    this.category_selection = new category_selection_class(this)
    this.dom.container.appendChild(this.category_selection.dom.container)
    this.keyword_selection = new keyword_selection_class(this)
    this.dom.container.appendChild(this.keyword_selection.dom.container)
    this.brand_selection = new brand_selection_class(this)
    this.dom.container.appendChild(this.brand_selection.dom.container)
    this.link_list = new link_list_class(this)
    this.dom.container.appendChild(this.link_list.dom.container)
  }
}

new app_class
