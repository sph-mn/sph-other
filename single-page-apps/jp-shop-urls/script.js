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
      "any",
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
    "bolero",
    "cape",
    "coat",
    "gloves2",
    "gloves",
    "hair_accessories",
    "hair_ties",
    "half_pants",
    "jacket",
    "jewelry",
    "legwear",
    "one_piece",
    "ouji_style",
    "overknee_socks",
    "pants",
    "salopette",
    "skirt",
    "socks",
    "tights",
    "tops",
    "wristbands"
  ],
  shops: {
    mercari: {
      make_url: function(brand, category, keywords) {
        let category_child = ""
        if (category) {
          let category_ids = config.shops.mercari.categories[category]
          if (category_ids) {
            if (Array.isArray(category_ids)) {
              category_child = category_ids[0] + "&" + category_ids.slice(1).map(x => `category_grand_child[${x}]=1`).join("&")
            } else {
              category_child = category_ids
            }
          } else {
            keywords = helper.keywords_add_category(keywords, category)
            category = ""
          }
        }
        brand_config = config.shops.mercari.brands[brand]
        brand_name = brand_config.name
        brand_id = brand_config.id
        return `https://www.mercari.com/jp/search/?page=1&sort_order=created_desc&keyword=${keywords}&category_root=1&category_child=${category_child}&brand_name=${brand_name}&brand_id=${brand_id}&size_group=&price_min=&price_max=&status_on_sale=1`
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
        tights: 18,
        overknee_socks: 18,
        one_piece: 15,
        pants: 13,
        shoes: 16,
        skirt: 14,
        accessories: 23,
        tops: 11,
        blouse: [11, 121, 122]
      }
    },
    rakuma: {
      make_url: (brand, category, keywords) => {
        brand_id = config.shops.rakuma.brands[brand]
        let category_id
        if (category) {
          category_id = config.shops.rakuma.categories[category]
          if (!category_id) {
            keywords = helper.keywords_add_category(keywords, category)
            category_id = config.shops.rakuma.root_categories.ladies
          }
        } else {
          category_id = config.shops.rakuma.root_categories.ladies
        }
        return `https://fril.jp/s?query=${keywords}&category_id=${category_id}&brand_id=${brand_id}&transaction=selling&sort=created_at&order=desc`
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
        jewelry: 8,
        bags: 5,
        hair_accessories: 9,
        legwear: 15,
        one_piece: 2,
        pants: 3,
        shoes: 7,
        skirt: 4,
        accessories: 6,
        tops: 1
      }
    },
    closetchild: {
      make_label: (brand, category, keywords) => {
        if (keywords) return ["", "", keywords]
        let categories = config.shops.closetchild.categories
        let brands = config.shops.closetchild.brands
        if (category && brand && categories[brand] && categories[brand][category]) {
          return [brand, category, ""]
        } else if (category && !brand && categories[category]) {
          return ["", category, ""]
        } else if (!category && brand && brands[brand]) {
          return [brand, "", ""]
        } else if (category) {
          return ["", "", i18n.translations.jp[category]]
        } else {
          return [brand, "", ""]
        }
      },
      make_url: (brand, category, keywords) => {
        [brand, category, keywords] = config.shops.closetchild.make_label(brand, category, keywords)
        if (keywords) {
          return `https://www.closetchildonlineshop.com/product-list?available=1&keyword=${keywords}`
        }
        let category_id = ""
        let categories = config.shops.closetchild.categories
        if (category && brand) {
          category_id = categories[brand][category]
        } else if (category) {
          category_id = categories[category]
        } else if (brand) {
          let brands = config.shops.closetchild.brands
          category_id = brands[brand]
        }
        return `https://www.closetchildonlineshop.com/product-list/${category_id}?available=1`
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
        any: {
          one_piece: 1,
          skirt: 3,
          blouse: 2,
          tops: 5,
          pants: 10,
          jacket: 6,
          shoes: 51,
          bags: 51,
          jewelry: 52,
          hair_accessories: 52,
          legwear: 52,
          accessories: 223
        },
        alice_and_the_pirates: {
          one_piece: 214,
          skirt: 215,
          blouse: 216,
          tops: 217,
          pants: 219,
          jacket: 220,
          shoes: 220,
          bags: 222,
          jewelry: 223,
          hair_accessories: 223,
          legwear: 223,
          overknee_socks: 223,
          socks: 223,
          accessories: 223
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
          jewelry: 201,
          hair_accessories: 201,
          legwear: 201,
          overknee_socks: 201,
          socks: 201,
          accessories: 201
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
          jewelry: 212,
          hair_accessories: 212,
          legwear: 212,
          overknee_socks: 212,
          socks: 212,
          accessories: 212
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
          jewelry: 245,
          hair_accessories: 245,
          legwear: 245,
          overknee_socks: 245,
          socks: 245,
          accessories: 245
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
          jewelry: 289,
          hair_accessories: 289,
          legwear: 289,
          overknee_socks: 289,
          socks: 289,
          accessories: 289
        }
      }
    },
    baby_the_stars_shine_bright: {
      make_url: (brand, category, keywords) => {
        if (!keywords) return
        return `https://babyssb.shop/language/ja?q=${keywords}&search_button.x=11&search_button.y=8`
      },
      brands: {
        baby_the_stars_shine_bright: true,
        alice_and_the_pirates: true
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
        skirt: "jumperskirt",
        blouse: "top",
        tops: "top",
        pants: "bottom",
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
    },
    wunderwelt: {
      make_url: (brand, category, keywords) => {
        return
        var brand_id = config.shops.wunderwelt.brands[brand]
        var category_id = config.shops.wunderwelt.categories[category] || ""
        return `https://www.wunderwelt.jp/brands/${brand_id}/${category_id}?keywords=${keywords}`
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
        jewelry: 8,
        bags: 5,
        hair_accessories: 9,
        legwear: 15,
        one_piece: "one-piece",
        pants: 3,
        shoes: 7,
        skirt: 4,
        accessories: 6,
        tops: 1
      }
    },
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
      tights: "タイツ",
      socks: "ソックス",
      skirt: "スカート",
      salopette: "サロペット",
      pants: "パンツ",
      overknee_socks: "オーバーニー",
      ouji_style: "皇子系",
      bolero: "ボレロ",
      cape: "ケープ",
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
      gloves2: "手袋",
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
      skirts: "スカート",
      tops: "トップス",
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
    let section = helper.make_section("keyword_selection", i18n.translate("keywords"))
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

class link_list_class {
  dom = {}
  make_label(shop, shop_key, brand, category, keywords) {
    if (shop.make_label) {
      [brand, category, keywords] = shop.make_label(brand, category, keywords)
    }
    let label = [i18n.translate(shop_key)]
    if (brand && 1 < Object.keys(shop.brands).length) label.push(i18n.translate(brand))
    if (category && shop.categories && 1 < Object.keys(shop.categories).length) label.push(i18n.translate(category))
    if (keywords) label.push(keywords)
    return label.filter(a => a).join(" - ")
  }
  make_links(brands, category, keywords) {
    let links = Object.keys(config.shops).map(shop_key => {
      let shop = config.shops[shop_key]
      let links = brands.map(brand => {
        if (!shop.brands || undefined == shop.brands[brand]) return
        let shop_category = category
        let shop_keywords = keywords
        if (category && (!shop.categories || !shop.categories[category])) {
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
  constructor() {
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
