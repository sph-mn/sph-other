(import (sph) (csv csv)
  (sxml html) (sph lang plcss)
  (sph vector) (sph lang sescript) (sph io) (sph string) (sxml simple) (sph tree) (sph list))

(define read-csv (make-csv-reader #\,))

(define data
  (call-with-input-file "/home/nonroot/ps/topokanji-deck/download/topokanji-deck.csv" read-csv))

(define-syntax-rule (js a ...) (sescript->ecmascript-string (quasiquote (begin a ...))))

(define (include-stroke-order id)
  (let*
    ( (source-path
        (string-append "/home/nonroot/temp/japanese/1/reading-writing/kanjivg/kanji/0"
          (number->string (char->integer (string-ref id 0)) 16) ".svg"))
      (content
        (call-with-input-file source-path
          (l (in) (xml->sxml in #:namespaces (list (pair (q kvg) "http://example.org/kvg"))))))
      (clean-sxml
        (l (a)
          (third
            (tree-map-lists
              (l (a)
                (let
                  (a (remove (l (a) (or (null? a) (equal? "\n" a) (equal? "\n\t" a))) a))
                  (case (first a)
                    ( (id kvg:type kvg:variant
                        kvg:position kvg:phon kvg:element kvg:radical width height)
                      null)
                    ((http://www.w3.org/2000/svg:path) (pair (q path) (tail a)))
                    ((http://www.w3.org/2000/svg:svg) (pair (q svg) (tail a)))
                    ((http://www.w3.org/2000/svg:g) (pair (q g) (tail a)))
                    ( (http://www.w3.org/2000/svg:text)
                      (pairs (q text) (append (first (tail a)) (list (list (q font-size) 8)))
                        (tail (tail a))))
                    ((style) (list))
                    ((@) (let (a (remove null? (tail a))) (if (null? a) null (pair (q @) a))))
                    (else a))))
              a))))
      (clean-sxml-string (l (a) (regexp-replace (regexp-replace a "\n|\t|\r" "") ">\\s+<" ""))))
    (clean-sxml-string (call-with-output-string (l (out) (sxml->html (clean-sxml content) out))))))

(define sxml
  (qq
    (html
      (head (meta (@ (charset utf-8))) (meta (@ (name viewport) (content "initial-scale=1")))
        (title "kanji viewer")
        (style
          (raw
            (unquote
              (css ("html" font-size "16px")
                ("body" background-color black
                  color "#ddd" ("a" color "#ddd") ("> :first-child ~ *" margin-top "2rem"))
                (".description" font-size "0.75rem")
                (".filter input" border 0 background-color "#eee" width "6rem")
                (".list" width "100%"
                  display flex
                  flex-wrap wrap
                  (".i" width "25rem"
                    ("&:not(.hidden) + .i" clear left) ("> *" float left)
                    (".k" (".k1" display inline) (".k2" font-size "20px" display none)
                      ("svg" width "15rem"
                        height "15rem" ("path" stroke-width 2 stroke "#fff !important") ("text" visibility hidden))
                      ("&:hover" ("svg text" visibility visible color "#808080")
                        ("+ .m" display block)))
                    (".m" text-align center display none ("> div" position relative top "4rem"))
                    ("&.hidden" clear none
                      ("> *" height "22px") (".k1" display none)
                      (".k2" display inline)
                      (".m" text-align left
                        margin-right "0.25rem" width initial ("> div" top "2px")))
                    ("&.excluded" display none) ("&:first-child" padding-top 0))))))))
      (body
        (div (@ (class description)) "features"
          (ul (li "hover over kanji shows stroke order and meaning")
            (li
              "middle mouse button click on character minimises row. minimised rows are saved as long as the path to the html file does not change")
            (li "when minimised, characters can be selected and copied")
            (li "ctrl+middle-click minimises or restores all previous rows")
            (li "multiple kanji and meanings can be used to filter")
            (li
              "the page can be used offline with right click save page as. everything is contained in the html file")
            (li
              "jump to a kanji by adding # to the page address, for example file:///home/user/kanji-viewer.html#大")
            (li "kanji list from "
              (a (@ (href "https://github.com/sph-mn/topokanji-deck")) "topokanji-deck")
              ", graphics and stroke order from "
              (a (@ (href "https://kanjivg.tagaini.net/")) "kanjivg"))
            (li "this page is under a cc-by-sa license and can be copied, modified and rehosted")))
        (div (@ (class filter)) (input (@ (id filter-input) (placeholder "filter..."))))
        (div (@ (class list))
          (unquote-splicing
            (map
              (l (a)
                (qq
                  (div (@ (class i) (id (unquote (vector-first a))))
                    (div (@ (class k))
                      (span (@ (class k1)) (raw (unquote (include-stroke-order (vector-first a)))))
                      (span (@ (class k2)) (unquote (vector-first a))))
                    (div (@ (class m)) (div (unquote (vector-second a)))))))
              data)))
        (script
          (raw
            (unquote
              (js (define rows (document.querySelectorAll ".list .i"))
                (define (set-row row hidden)
                  (if hidden (begin (row.classList.add "hidden") (localStorage.setItem row.id #t))
                    (begin (row.classList.remove "hidden") (localStorage.removeItem row.id))))
                (define (init-row row)
                  (and (localStorage.getItem row.id) (row.classList.add "hidden")))
                (define (set-previous-rows row hidden)
                  (for ((define i 0) (< i rows.length) (set i (+ 1 i)))
                    (if (= row rows[i]) (return) (set-row rows[i] hidden))))
                (define (scroll-to id)
                  (and id (window.scrollTo 0 (get (document.getElementById id) "offsetTop"))))
                (define (init-filter) (define input (document.getElementById "filter-input"))
                  (define (on-filter event)
                    (for ((define i 0) (< i rows.length) (set i (+ 1 i))) (define row rows[i])
                      (if
                        (or (= 0 input.value.length) (input.value.includes row.id)
                          (chain includes (get (row.querySelector ".m") "innerHTML") input.value))
                        (row.classList.remove "excluded") (row.classList.add "excluded"))))
                  (input.addEventListener "keyup" on-filter)
                  (input.addEventListener "change" on-filter))
                (chain forEach (document.querySelectorAll ".k")
                  (l (a) (init-row a.parentNode)
                    (a.addEventListener "mouseup"
                      (l (event) (if (not (this.classList.contains "k")) return)
                        (define row this.parentNode)
                        (if (= 1 event.button)
                          (if event.ctrlKey
                            (begin
                              (set-previous-rows row
                                (not (row.previousElementSibling.classList.contains "hidden")))
                              (scroll-to row.id))
                            (set-row row (not (row.classList.contains "hidden")))))
                        ;(localStorage.setItem "most-recent" row.id)
                        ))))
                (init-filter)
                ;(scroll-to (localStorage.getItem "most-recent"))
                ))))))))

(display "<!doctype html>")
(sxml->html sxml)
