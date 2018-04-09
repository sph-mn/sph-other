(library (sph fun phrase-generator)
  (export
    base-constructors
    load-config
    sph-fun-phrase-generator-description
    two-words-getter)
  (import
    (rnrs exceptions)
    (sph)
    (sph alist)
    (sph hashtable)
    (sph lang config)
    (sph list)
    (sph list one)
    (only (guile) make-list))

  (define sph-fun-german-names-description
    "create word combinations from a base word list.
     config format is
       config: base-config ...
       base-config: string:base/base-expression list:suffix-definition
       suffix-definition: (symbol:allowed-position-key/(symbol:allowed-position-key string:possible-suffix-at-position ...) ...)
       allowed-position-key: p1/s1/p2/s2
       base-expression: (base-constructor-name base-constructor-argument ...)
     config example
       (define config (quote (
         \"berg\" ((p1 \"en\") p2 (s1 \"er\"))
         \"binkel\" (p1)
         \"bratz\" ((p1 \"en\") (p2 \"en\") (s2 \"e\" \"en\"))))
         (w1 (\"kk\" \"g\" \"gg\") ((\"h\") (\"a\" \"e\"))) ((p1 \"el\") (s1 \"ler\" \"er\") (s2 \"ler\" \"e\" \"er\"))
     allowed position keys
       when a key is given, the base may be used at that position
       p1: first word prefix
       s1: first word suffix
       p2: second word prefix
       s2: second word suffix
     base-constructors
       can generate multiple bases using procedures.
       currently only w1 is defined, which is defined in the module
     usage example
       (define make-german-names (two-words-getter (load-config config)))
       (define (display-german-names count) (each display-line (make-german-names count)))")

  (define* (w1 base #:optional p s)
    "string [prefixes:(string ...) suffixes:(suffix-prefixes:(string ...) [suffix-suffixes:(string ...)])] -> (string ...)
     constructs bases by appending and prepending strings in all possible combinations"
    (let
      ( (b (any->list base))
        (p (or (and p (apply produce string-append (map any->list (any->list p)))) (list "")))
        (s (or (and s (apply produce string-append (map any->list (any->list s)))) (list ""))))
      (produce string-append p b s)))

  (define-as base-constructors ht-create-binding w1)

  (define (make-bases base)
    (cond ((string? base) (list base))
      ((list? base) (apply (ht-ref base-constructors (first base)) (tail base)))
      (else (raise (q invalid-base-specified)))))

  (define (load-config config)
    "list -> list
     evaluate base constructors and normalise config object"
    (apply append
      (map-slice 2
        (l (base suffixes)
          (let ((bases (make-bases base)) (suffixes (map any->list (any->list suffixes))))
            (map (l (b) (pair b suffixes)) bases)))
        config)))

  (define (prepend-retry-duplicates gen tries lis)
    (let (a (gen))
      (if (and (containsq? lis a) (< 0 tries)) (prepend-retry-duplicates gen (- tries 1) lis)
        (pair a lis))))

  (define (config-filter config key)
    (filter (l (a) (let (suffixes (tail a)) (alist-ref suffixes key))) config))

  (define (make-one-word base suffixes key)
    (let (a (alist-ref suffixes key))
      (if a (if (null? a) base (string-append base (list-ref-random a))) null)))

  (define (two-words-getter config) (config-filter config (q s1))
    (let
      ( (p1 (config-filter config (q p1))) (s1 (config-filter config (q s1)))
        (p2 (config-filter config (q p2))) (s2 (config-filter config (q s2))))
      (l (count)
        (map-slice 4
          (l (p1 s1 p2 s2)
            (string-append (make-one-word (first p1) (tail p1) (q p1))
              (make-one-word (first s1) (tail s1) (q s1)) " "
              (make-one-word (first p2) (tail p2) (q p2)) (make-one-word (first s2) (tail s2) (q s2))))
          (fold-right
            (l (parts result)
              (prepend-retry-duplicates (nullary (list-ref-random parts)) 20 result))
            (list) (apply append (make-list count (list p1 s1 p2 s2)))))))))
