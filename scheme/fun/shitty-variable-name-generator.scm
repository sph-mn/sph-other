(library (sph fun shitty-variable-name-generator)
  (export
    svn
    svn-multiple)
  (import
    (rnrs io ports)
    (sph base)
    (sph random-data))

  (define vowel "aeiou")
  (define consonant "bcdfghjklmnpqrstvwxyz")
  (define prefix-special "$_")
  (define alphabet (string-append vowel consonant))

  (define (random-string-from-set len character-string)
    (list->string (map-integers len (l (n) (string-ref-random character-string)))))

  (define (create-prefix-random) (random-string-from-set (+ 1 (random 3)) alphabet))

  (define (create-prefix-special)
    (list->string (make-list (+ 1 (random 2)) (string-ref-random prefix-special))))

  (define (words-prefix a)
    (pair (if (apply-modification? prefix-special) (create-prefix-special) (create-prefix-random))
      a))

  (define (string-ref-random a) (string-ref a (random (string-length a))))

  (define-as chances ht-create-symbol
    vowel-removal 75
    consonant-removal 10
    uppercase 30 prefix 2 prefix-special 2 shorten-word 35 spaces-removal 35 camelcase 75)

  ; decreasing chances per run
  ; updating chances
  ; updating chances in relation
  ; chance deviation
  ; chance initialisation
  ; random settings and chances mutation disabled
  (define-as settings ht-create-symbol chance-deviation 1.4 chance-max-init-iterations 20)

  (define (update-chances-one)
    (ht-map!
      (l (k v)
        (- 100
          (modulo (round ((if (random-boolean) + -) (ht-ref chances k) (/ (ht-ref chances k) 5)))
            100)))
      chances))

  (define (update-chances)
    (map-integers (random (ht-ref settings (q chance-max-init-iterations)))
      (l (n) (update-chances-one))))

  (define-syntax-rule (apply-modification? name)
    (random-weighted-boolean (ht-ref chances (q name))))

  (define (vowel-indices a) (map-with-index (l (i e) (string-index vowel e)) a))
  (define (consonant-indices a) (map-with-index (l (i e) (string-index consonant e)) a))

  (define (remove-one-random-vowel a-list)
    (let* ((indices (vowel-indices a-list)) (index-delete (+ 1 (random (- (length indices) 1)))))
      (delete #f (map-with-index (l (i e) (if (= i index-delete) #f e)) a-list))))

  (define (remove-one-random-consonant a-list)
    (let*
      ((indices (consonant-indices a-list)) (index-delete (+ 1 (random (- (length indices) 1)))))
      (delete #f (map-with-index (l (i e) (if (= i index-delete) #f e)) a-list))))

  (define (random-vowel-removal a) "string -> string"
    (list->string
      (delete #f
        (map-with-index
          (l (i e)
            (if (and (> i 0) (string-index vowel e) (apply-modification? vowel-removal)) #f e))
          (remove-one-random-vowel (string->list a))))))

  (define (random-consonant-removal a) "string -> string"
    (list->string
      (delete #f
        (map-with-index
          (l (i e) (if (and (> i 0) (string-index consonant e) (random-boolean)) #f e))
          (remove-one-random-consonant (string->list a))))))

  (define (shorten-word a)
    (let loop ((index (- (string-length a) 1)))
      (if (or (random-boolean) (< index (ht-ref settings (q minimum-word-length))))
        (substring a 0 (+ index 1)) (loop (- index 1)))))

  (define (random-uppercase a) (if (apply-modification? uppercase) (map string-upcase a) a))

  (define (svn-word a)
    (fold (l (e r) (if (> (string-length r) (ht-ref settings (q minimum-word-length))) (e r) r)) a
      (list (if (apply-modification? shorten-word) shorten-word identity)
        (if (apply-modification? consonant-removal) random-consonant-removal identity)
        (if (apply-modification? vowel-removal) random-vowel-removal identity))))

  (define (string-join-camelcase a) "list -> string"
    (if (null? a) "" (string-join (pair (first a) (map string-capitalize (tail a))) "")))

  (define (string-join-underscore a) "list -> string" (string-join a "_"))

  (define (join-words a)
    ( (if (apply-modification? camelcase) string-join-camelcase string-join-underscore)
      (if (apply-modification? prefix) (words-prefix (random-uppercase a)) (random-uppercase a))))

  (define (randomly-delete-spaces a)
    (string-delete (l (e) (and (eqv? #\space e) (apply-modification? spaces-removal))) a))

  (define (svn words-string)
    (let*
      ( (words-wo-spaces (randomly-delete-spaces words-string))
        (words (delete "" (string-split words-wo-spaces #\space))))
      (if (> (string-count words-wo-spaces #\space) 0) (ht-set! settings (q minimum-word-length) 1)
        (ht-set! settings (q minimum-word-length) 3))
      (join-words (map svn-word words))))

  (define (svn-multiple count words) "integer string -> (string ...)"
    (update-chances)
    (let (words (string-trim words))
      (if (string-null? words) #f
        (let*
          ( (acceptable
              (compose
                (l (e)
                  (let (words (string-split (string-downcase words) #\space))
                    (or (string-equal? e (string-join words "_"))
                      (string-equal? e (string-join words "")))))
                string-downcase))
            (create-names
              (nullary
                (delete-duplicates (remove acceptable (map-integers count (l (n) (svn words))))))))
          (let loop ((names (create-names)) (limit 10))
            (if (and (< (length names) count) (> limit 0))
              (loop (delete-duplicates (append (create-names) names)) (- limit 1)) names)))))))
