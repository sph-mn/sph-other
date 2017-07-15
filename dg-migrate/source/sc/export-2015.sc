(define export-2015-description char*
  "code for an executable that takes the path to a sph-dg version 1 root directory and displays the contained metadata in an s-expression format. format description is displayed at the top of the output.
   does not depend on the old sph-dg version")

(pre-define debug-log? #t)
(sc-include "source/sc/foreign/sph" "source/sc/foreign/sph/one" "source/sc/foreign/sph/status")
(pre-include-once stdio-h "stdio.h" inttypes-h "inttypes.h" string-h "string.h" lmdb-h "lmdb.h")

(pre-define bits-type-ide 0
  bits-type-pair 1
  bits-type-intern 2
  bits-type-extern 3
  bits-type 3
  types-max-count 4
  type-ide b64
  size-data 32
  size-ide 8
  size-pair 16
  size-pair-data 24
  mdb-maxreaders 100000 mdb-env-open-flags (bit-or MDB-NOSUBDIR MDB-NOTLS MDB-WRITEMAP MDB-MAPASYNC))

(pre-define (val-data-get a) (convert-type (struct-get a mv-data) type-ide*))
(pre-define (val-ide-ref a) (deref (convert-type (struct-get a mv-data) type-ide*)))
(pre-define (val-data-ref a index) (deref (val-data-get a) index))
(pre-define (dg-intern? ide) (= bits-type-intern (bit-and ide bits-type)))
(pre-define (dg-extern? ide) (= bits-type-extern (bit-and ide bits-type)))
(pre-define (dg-ide? ide) (= bits-type-ide (bit-and ide bits-type)))
(pre-define (dg-pair? ide) (= bits-type-pair (bit-and ide bits-type)))

(define (cmp-ide a b) ((static int) MDB-val* MDB-val*)
  (return
    (if* (< (val-ide-ref (deref a)) (val-ide-ref (deref b))) -1
      (> (val-ide-ref (deref a)) (val-ide-ref (deref b))))))

(pre-define (return-cmp-pair a b)
  (return
    (if* (> (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) 1
      (if* (< (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) -1
        (if* (< (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)) -1
          (> (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)))))))

(define (cmp-pair a b) ((static int) MDB-val* MDB-val*) (return-cmp-pair a b))

(define (cmp-pair-data b a) ((static int) (const MDB-val*) (const MDB-val*))
  (return
    (if* (> (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) 1
      (if* (< (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) -1
        (if* (> (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)) 1
          (if* (< (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)) -1
            (if* (< (deref (val-data-get (deref a)) 2) (deref (val-data-get (deref b)) 2)) -1
              (> (deref (val-data-get (deref a)) 2) (deref (val-data-get (deref b)) 2)))))))))

(define (cmp-data a b) ((static int) (const MDB-val*) (const MDB-val*))
  (return
    (if* (> (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) 1
      (if* (< (deref (val-data-get (deref a))) (deref (val-data-get (deref b)))) -1
        (if* (> (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)) 1
          (if* (< (deref (val-data-get (deref a)) 1) (deref (val-data-get (deref b)) 1)) -1
            (if* (> (deref (val-data-get (deref a)) 2) (deref (val-data-get (deref b)) 2)) 1
              (if* (< (deref (val-data-get (deref a)) 2) (deref (val-data-get (deref b)) 2)) -1
                (if* (< (deref (val-data-get (deref a)) 3) (deref (val-data-get (deref b)) 3)) -1
                  (> (deref (val-data-get (deref a)) 3) (deref (val-data-get (deref b)) 3)))))))))))

(define mdb-env MDB-env*
  dbi-left->pair-data MDB-dbi
  dbi-right->pair-data MDB-dbi
  dbi-pair->ide MDB-dbi dbi-ide->data MDB-dbi dbi-intern->ide MDB-dbi dbi-ide->alias MDB-dbi)

(pre-define (status-id-require! a) (status-set-id a) status-require)

(define (open-environment dg-root) (status-t b8*)
  status-init (define dg-path-metadata b8* (string-append dg-root "metadata"))
  (status-id-require! (mdb-env-create (address-of mdb-env)))
  (status-id-require! (mdb-env-set-maxreaders mdb-env mdb-maxreaders))
  (status-id-require! (mdb-env-set-maxdbs mdb-env 7))
  (status-id-require! (mdb-env-open mdb-env dg-path-metadata mdb-env-open-flags 384))
  (label exit (return status)))

(define (open-databases txn) (status-t MDB-txn*)
  status-init (status-id-require! (mdb-dbi-open txn "ide->data" 0 (address-of dbi-ide->data)))
  (status-id-require! (mdb-dbi-open txn "intern->ide" 0 (address-of dbi-intern->ide)))
  (status-id-require! (mdb-dbi-open txn "pair->ide" 0 (address-of dbi-pair->ide)))
  (status-id-require! (mdb-dbi-open txn "ide->alias" 0 (address-of dbi-ide->alias)))
  (status-id-require! (mdb-set-compare txn dbi-ide->data (convert-type cmp-ide MDB-cmp-func*)))
  (status-id-require! (mdb-set-compare txn dbi-intern->ide (convert-type cmp-data MDB-cmp-func*)))
  (status-id-require! (mdb-set-compare txn dbi-pair->ide (convert-type cmp-pair MDB-cmp-func*)))
  (status-id-require! (mdb-set-compare txn dbi-ide->alias (convert-type cmp-ide MDB-cmp-func*)))
  (define db-options b8 (bit-or MDB-DUPSORT MDB-DUPFIXED))
  (status-id-require!
    (mdb-dbi-open txn "left->pair-data" db-options (address-of dbi-left->pair-data)))
  (status-id-require!
    (mdb-dbi-open txn "right->pair-data" db-options (address-of dbi-right->pair-data)))
  (status-id-require!
    (mdb-set-compare txn dbi-left->pair-data (convert-type cmp-ide MDB-cmp-func*)))
  (status-id-require!
    (mdb-set-compare txn dbi-right->pair-data (convert-type cmp-ide MDB-cmp-func*)))
  (status-id-require!
    (mdb-set-dupsort txn dbi-left->pair-data (convert-type cmp-pair-data MDB-cmp-func*)))
  (status-id-require!
    (mdb-set-dupsort txn dbi-right->pair-data (convert-type cmp-pair-data MDB-cmp-func*)))
  (label exit (return status)))

(define (get-dg-root-or-exit a-len a) (b8* b32 b8_s**)
  (define dg-root-input b8_s*) (if (= 2 a-len) (set dg-root-input (array-get a 1)) (exit 1))
  (if (not (file-exists? dg-root-input)) (exit 2)) (define dg-root b8*)
  (ensure-trailing-slash dg-root-input (address-of dg-root)) (return dg-root))

(define (display-export-data txn) (status-t MDB-txn*)
  status-init (define ide->data MDB-cursor*)
  (status-id-require! (mdb-cursor-open txn dbi-ide->data (address-of ide->data)))
  (define val-key MDB-val) (define val-data MDB-val)
  (status-id-require!
    (mdb-cursor-get ide->data (address-of val-key) (address-of val-data) MDB-FIRST))
  (define id type-ide) (printf "; (intern {hex-id} {hex-byte} ...)\n")
  (printf "; (id {hex-id})\n") (printf "; (alias {hex-id} {hex-target-id})\n")
  (printf "; (extern {hex-id})\n")
  (printf "; (pair {hex-id} {hex-id-left} {hex-id-right} {hex-ordinal-left} {hex-ordinal-right})\n")
  (while (= MDB-SUCCESS (struct-get status id)) (set id (val-ide-ref val-key))
    (cond
      ( (dg-intern? id) (printf "(intern #x%x" id) (define index b32 0)
        (while (< index (struct-get val-data mv-size))
          (printf " #x%x" (deref (+ index (convert-type (struct-get val-data mv-data) b8*))))
          (set index (+ index 1)))
        (printf ")\n"))
      ( (dg-ide? id)
        (if (struct-get val-data mv-size) (printf "(alias #x%x #x%x)\n" id (val-ide-ref val-data))
          (printf "(id #x%x)\n" id)))
      ((dg-extern? id) (printf "(extern #x%x)\n" id))
      ( (dg-pair? id)
        (printf "(pair #x%x #x%x #x%x #x%x #x%x)\n" id
          (val-data-ref val-data 0) (val-data-ref val-data 1)
          (val-data-ref val-data 2) (val-data-ref val-data 3))))
    (status-set-id
      (mdb-cursor-get ide->data (address-of val-key) (address-of val-data) MDB-NEXT-NODUP)))
  (if (= MDB-NOTFOUND (struct-get status id)) (struct-set status id 0))
  (label exit (mdb-cursor-close ide->data) (return status)))

(define (main a-len a) (b32 b32 b8_s**)
  status-init (define dg-root b8* (get-dg-root-or-exit a-len a))
  (open-environment dg-root) (define txn MDB-txn*)
  (status-id-require! (mdb-txn-begin mdb-env 0 MDB-RDONLY (address-of txn))) (open-databases txn)
  (display-export-data txn) (mdb-txn-abort txn)
  (mdb-env-close mdb-env)
  (label exit (if (struct-get status id) (printf "%s\n" (mdb-strerror (struct-get status id))))
    (return (struct-get status id))))
