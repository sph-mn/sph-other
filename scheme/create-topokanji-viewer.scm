(import (sph) (csv csv)
  (sxml html) (sph lang plcss) (sph vector) (sph lang sescript) (sph io) (sph string))

(define read-csv (make-csv-reader #\,))

(define data
  (call-with-input-file "/home/nonroot/ps/topokanji-deck/download/topokanji-deck.csv" read-csv))

(define-syntax-rule (js a ...) (sescript->ecmascript-string (quasiquote (begin a ...))))

(define (include-stroke-order id)
  (let*
    ( (content
        (file->string
          (string-append "/home/nonroot/temp/japanese/1/reading-writing/kanjivg/kanji/0"
            (number->string (char->integer (string-ref id 0)) 16) ".svg")))
      (index (string-contains content "<svg")))
    (regexp-replace (regexp-replace (regexp-replace (substring content index) " +" " ") "\n" "")
      "\t" "")))

(define sxml
  (qq
    (html
      (head (meta (@ (charset utf-8))) (title "topokanji viewer")
        (style
          (raw
            (unquote
              (css ("html" font-size "14px")
                ("body" background-color black
                  color "#ddd" ("a" color "#ddd") ("> :first-child ~ *" margin-top "2rem"))
                (".description" font-size "0.75rem")
                (".filter input" border 0 background-color "#eee" width "6rem")
                (".list" display table
                  width "100%"
                  (".i" display table-row
                    ("> *" display table-cell padding "2rem 0rem")
                    (".m" text-align center visibility hidden vertical-align middle)
                    (".k" width "10rem"
                      (".k1" display inline) (".k2" display none)
                      ("svg" height "10rem"
                        width "10rem" ("path" stroke "#fff !important") ("text" visibility hidden))
                      ("&:hover" ("svg text" visibility visible)
                        ("+ .m" display table-cell visibility visible)))
                    ("&.hidden" ("> *" padding 0) (".k1" display none) (".k2" display inline))
                    ("&.excluded" display none) ("&:first-child" padding-top 0))))))))
      (body
        (div (@ (class description)) "features"
          (ul (li "hover over kanji shows stroke order and meaning")
            (li
              "middle mouse button click on character minimises row. minimised rows are saved as long as the page address does not change")
            (li "on page open, the browser jumps to the most recently minimised or restored row")
            (li "when minimised, the characters can be selected and copied")
            (li "ctrl+middle-click minimises or restores all previous rows")
            (li
              "the page can be used offline after right click save page as. everything is contained in one html file")
            (li
              "jump to a kanji by adding # to the page address, for example file:///home/user/topokanji-viewer.html#大")
            (li "kanji list from "
              (a (@ (href "https://github.com/sph-mn/topokanji-deck")) "topokanji-deck"))
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
                    (div (@ (class m)) (unquote (vector-second a))))))
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
                        (row.classList.remove "excluded") (row.classList.add "excluded")))
                    (console.log input.value))
                  (input.addEventListener "keyup" on-filter)
                  (input.addEventListener "mouseup" on-filter))
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
                        (localStorage.setItem "most-recent" row.id)))))
                (init-filter) (scroll-to (localStorage.getItem "most-recent"))))))))))

(display "<!doctype html>")
(sxml->html sxml)
