(library (sph fun marketing-bs)
  (export
    make-marketing-bs
    make-marketing-bs-multiple)
  (import
    (ice-9 regex)
    (sph base)
    (sph list one)
    (sph random-data))

  (define-as noun vector
    "silos" "services"
    "findability" "activity monitoring"
    "framework" "mediation"
    "orchestration" "bean binding"
    "delivery" "aggregation"
    "transformation" "architecture"
    "association teardown" "technology"
    "marketing" "integration"
    "microservice" "information superhighway"
    "value-add" "mashup"
    "mindshare" "datafication"
    "data mining" "innovation"
    "tooling" "web services"
    "synergies" "strategy"
    "ecosystem" "analytics"
    "return of investment" "master-data management"
    "service bus" "data warehouse"
    "point-to-point channels" "pipeline" "big data" "backporting" "logic" "big data" "provisioning")

  (define-as synonym-with list "by" "with" "through" "via")

  (define-as goal-source list
    "increase sales" "beat the competition"
    "control strategic metrics" "become a key market player" "disrupt the market" "take the lead")

  (define goal (list->vector goal-source))
  (define goal-prefix (list->vector (produce (l e (string-join e " ")) goal-source synonym-with)))

  (define-as verb vector
    "extend" "revolutionise"
    "offer" "obtain"
    "access" "improve"
    "invest into" "build"
    "manage" "streamline"
    "successfully market" "kick-start"
    "provide" "increase" "leverage" "enable" "deliver" "create" "develop" "empower" "facilitate")

  (define-as combinator vector "and" "with" "by" "with" "through" "by using" "over" "under" "for")

  (define-as predicative-adjective vector
    "processing" "rules"
    "agility" "wizard" "competence" "management" "chaining" "integration" "alignment" "backlog")

  (define-as adjective vector
    "cloud" "enterprise-ready"
    "variable" "all"
    "handcrafted" "object-oriented"
    "high-quality" "ground-breaking"
    "mind-boggling" "simple, proven"
    "agile" "highly durable"
    "continuous" "best of breed"
    "hyperlocal" "beautiful"
    "high-volume" "integrated"
    "holistic" "game-changing"
    "flexible" "multi-domain"
    "project-driven" "data-driven"
    "deep dive" "disruptive"
    "modern" "connectivity"
    "better" "instant-value"
    "user-oriented" "core"
    "next-generation" "multi-dimensional"
    "rock-solid" "social"
    "seamless" "first-class"
    "full-stack" "powerful"
    "new" "best-in-class"
    "integrated" "unmatched"
    "leading" "real-time"
    "responsive" "holistic"
    "break-through" "best-of-breed"
    "buzzword compliant" "world-class"
    "business" "business"
    "business" "an enterprise"
    "enterprise" "an enterprise"
    "enterprise" "enterprise"
    "data" "cloud-based"
    "dynamic" "ultra-fast"
    "high-throughput" "cohesive"
    "multi-channel" "hybrid"
    "service oriented" "enterprise"
    "multiple" "very, very complex" "premium" "confident" "embedabble")

  (define (random-choice . a) (list-ref-random a))

  (define patterns
    (list->vector
      (let*
        ( (source
            (list (list verb "and" verb noun "to" goal "with" adjective noun)
              (list verb "," verb "and" verb noun)
              (list adjective noun predicative-adjective combinator adjective noun)
              (list (random-choice "" verb goal-prefix) adjective
                adjective noun (random-choice "" noun noun))
              (list verb "your" adjective noun "and" verb adjective noun)
              (list verb noun predicative-adjective "and" verb adjective noun predicative-adjective)
              (list (random-choice verb goal) noun
                predicative-adjective combinator noun combinator adjective noun predicative-adjective)
              (list verb "and" verb adjective adjective noun)
              (list noun noun combinator noun predicative-adjective)))
          (source+goal
            (map (l (e) (if (eq? verb (first e)) (append e (list "and" goal)) e)) source)))
        (append source source+goal))))

  (define (random-vector-ref v) (vector-ref v (max 0 (- (random (vector-length v)) 1))))

  (define (get-unused words use-list)
    (let loop ((e (random-vector-ref words)) (try-count (vector-length words)))
      (if (or (not (contains? use-list e)) (< try-count 0)) e
        (loop (random-vector-ref words) (- try-count 1)))))

  (define words->use-list-name
    (let
      (table
        (ht-create-symbol noun (q noun)
          verb (q verb) adjective (q adjective) predicative-adjective (q predicative-adjective)))
      (l (words) "vector -> symbol" (ht-ref table words (q combinator)))))

  (define (get-use-list words use-lists) "vector list -> list"
    (alistv-ref use-lists (words->use-list-name words)))

  (define (use-list-add words v use-lists)
    (alist-set! use-lists (words->use-list-name words)
      (pair v (alistv-ref use-lists (words->use-list-name words)))))

  (define (fix-a-an words)
    (append
      (map-segments 2
        (l (a b) (if (and (string-equal? "a" a) (string-match "^[aeiouh]" b)) "an" a)) words)
      (list (last words))))

  (define (compress-commas a) "string -> string" (string-replace-string a " ," ","))

  (define (make-marketing-bs)
    (compress-commas
      (string-join
        (fix-a-an
          (remove string-null?
            (let loop
              ( (pattern (random-vector-ref patterns)) (r (list))
                (use-lists
                  (alist-q noun (list)
                    verb (list) adjective (list) predicative-adjective (list) combinator (list))))
              (if (null? pattern) (reverse r)
                (let (words (first pattern))
                  (if (string? words) (loop (tail pattern) (pair words r) use-lists)
                    (let (word (get-unused words (get-use-list words use-lists)))
                      (loop (tail pattern) (pair word r) (use-list-add words word use-lists))))))))))))

  (define (make-marketing-bs-multiple count) (map-integers count (l (n) (make-marketing-bs)))))
