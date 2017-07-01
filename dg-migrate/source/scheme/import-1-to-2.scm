(import (except (rnrs hashtables) ht-ref) (rnrs bytevectors) (sph common) (sph storage dg))
;all ids will change because they new ones are generated by the new database.
;for each type id, dg-id-create, memoise old->new
;for each type extern, dg-extern-create, memoise old->new
;for each type intern, dg-intern-create, memoise old->new
;for each type pair, dg-relation-create, translate left/right to new ids
;for each type alias, dg-relation-create with context "dg-1-alias", translate identifiers
;what if an identifier is used that has not been created yet? we would need two passes. we just assume a relation or alias has never been created before it existed. we know identifiers have always been created lineraly. what about edits? we did allow pair editing. so we do two passes anyway...
(define id-old->new (make-eqv-hashtable 500))
(define bits-type-ide 0)
(define bits-type-pair 1)
(define bits-type-intern 2)
(define bits-type-extern 3)
(define bits-type 3)
(define (dg-1-intern? ide) (= bits-type-intern (logand ide bits-type)))
(define (dg-1-extern? ide) (= bits-type-extern (logand ide bits-type)))
(define (dg-1-ide? ide) (= bits-type-ide (logand ide bits-type)))
(define (dg-1-pair? ide) (= bits-type-pair (logand ide bits-type)))

(define (dg-1-id->type id)
  (cond ((dg-1-intern? id) "intern") ((dg-1-pair? id) "pair")
    ((dg-1-extern? id) "extern") ((dg-1-ide? id) "id")))

(define (dg-1-export-data-each path proc)
  (call-with-input-file path
    (l (source-data)
      (let loop ((a (read source-data)))
        (if (not (eof-object? a)) (begin (error-and (proc a) (loop (read source-data)))))))))

(define (exit-if-error a) (if (error? a) (begin (debug-log a) (exit 1)) a))
(define (list-trim-right a b) (drop-while (l (a) (eqv? b a)) (reverse a)))

(define (create-nodes source-path) "create nodes and initialise id-old->new"
  (exit-if-error
    (dg-txn-call-write
      (l (txn)
        (dg-1-export-data-each source-path
          (l (a)
            (let (id-old (first (tail a)))
              (case (first a)
                ( (id alias)
                  (error-let* ((id-new (dg-id-create txn)))
                    (ht-set! id-old->new id-old (first id-new))))
                ( (extern)
                  (error-let* ((id-new (dg-extern-create txn)))
                    (ht-set! id-old->new id-old (first id-new))))
                ( (intern)
                  (error-let*
                    ( (id-new
                        (dg-intern-ensure txn
                          (list (u8-list->bytevector (list-trim-right (tail (tail a)) 0))))))
                    (ht-set! id-old->new id-old (first id-new))))))))))))

(define (relation-part-inexistant old-left old-right new-left new-right)
  (display
    (string-append
      (string-join
        (pair "info: relation part inexistant:"
          (map (l (a) (if a (number->string a 16) "_"))
            (list old-left old-right new-left new-right)))
        " ")
      " " (dg-1-id->type old-left) " " (dg-1-id->type old-right) "\n")))

(define (create-relations source-path) "create nodes and initialise id-old->new"
  (exit-if-error
    (dg-txn-call-write
      (l (txn)
        (dg-1-export-data-each source-path
          (l (a)
            (let (id-old (first (tail a)))
              (case (first a)
                ( (pair)
                  (apply
                    (l (old-left old-right ordinal-left ordinal-right)
                      (let
                        ( (new-left (ht-ref id-old->new old-left))
                          (new-right (ht-ref id-old->new old-right)))
                        (if (and new-left new-right)
                          (dg-relation-ensure txn (list new-left) (list new-right))
                          (relation-part-inexistant old-left old-right new-left new-right))))
                    (tail (tail a))))
                ( (alias)
                  (apply
                    (l (old-left old-right new-left new-right)
                      (if (and new-left new-right)
                        (dg-relation-ensure txn (list new-left)
                          (list new-right) (dg-intern-ensure txn (list "dg-1-alias")))
                        (relation-part-inexistant old-left old-right new-left new-right)))
                    (append (tail a) (map (l (a) (ht-ref id-old->new a)) (tail a)))))))))))))

(define (dg-import-1-to-2 source-path target-dg-root recreate-database?)
  (let (target-dg-root (ensure-trailing-slash target-dg-root))
    (if recreate-database?
      (let (target-dg-data (string-append target-dg-root "data"))
        (if (false-if-exception (equal? (q regular) (stat:type (stat target-dg-data))))
          (delete-file target-dg-data))))
    (error-and (dg-init target-dg-root) (create-nodes source-path)
      (create-relations source-path) (dg-exit))))

;(dg-import-1-to-2 "/home/nonroot/temp/dg-1-export-data/sph-cms.dg-1" "/tmp/dg-migrate-test" #t)

(dg-import-1-to-2 "/home/nonroot/temp/dg-1-export-data/sph-info-main.dg-1" "/tmp/dg-migrate-test"
  #t)