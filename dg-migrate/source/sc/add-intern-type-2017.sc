(define add-intern-types-2017-description char*
  "code for an executable that takes a dg 2017 root directory, opens the database and prepends an intern type identifier to all interns.
  intern type identifiers where introduced 2017-09-15")

(pre-include-once string-h "string.h"
  stdlib-h "stdlib.h"
  libgen-h "libgen.h"
  inttypes-h "inttypes.h"
  stdio-h "stdio.h"
  errno-h "errno.h" sys-stat-h "sys/stat.h" unistd-h "unistd.h" sys-types-h "sys/types.h")

(pre-include "../source/c/sph-dg-2017-09/main.c")

(define (get-dg-root-or-exit a-len a) (b8* b32 b8_s**)
  (define dg-root-input b8_s*) (if (>= a-len 2) (set dg-root-input (array-get a 1)) (exit 1))
  (if (not (file-exists? dg-root-input)) (exit 2)) (define dg-root b8*)
  (ensure-trailing-slash dg-root-input (address-of dg-root)) (return dg-root))

(define dbi-id->data MDB-dbi dbi-data-intern->id MDB-dbi)

(define (debug-display-data a size) (b0 b8* size-t)
  (if (not size) (return)) (printf "%x" (deref a 0))
  (define index size-t 1)
  (while (< index size) (printf " %x" (deref a index)) (set index (+ 1 index))) (printf "\n"))

(pre-define dg-guile-intern-bytevector 0
  dg-guile-intern-integer 1
  dg-guile-intern-string 2
  dg-guile-intern-rational 3 dg-guile-intern-scheme 4 dg-guile-intern-type-size 1)

(define (dg-intern-add-type txn run) (status-t dg-txn-t* boolean)
  status-init
  (dg-mdb-introduce-cursor txn id->data)
  (if run (mdb-drop txn dbi-data-intern->id  0))
  dg-mdb-introduce-val-data dg-mdb-introduce-val-id

  (dg-mdb-status-require!
    (mdb-cursor-get id->data (address-of val-id) (address-of val-data) MDB-FIRST))
  (define count b32 0)
  (while status-success?
    (dg-mdb-status-require!
      (mdb-cursor-get id->data (address-of val-id) (address-of val-data) MDB-GET-CURRENT))
    (if (and (dg-extern? (dg-mdb-val->id val-id)) (struct-get val-data mv-size))
      (debug-log "%s id:%lu size:%lu" "extern with size!"
        (dg-mdb-val->id val-id) (struct-get val-data mv-size)))
    (if (dg-intern? (dg-mdb-val->id val-id))
      (begin (define old-size size-t (struct-get val-data mv-size))
        (define new-size size-t (+ dg-guile-intern-type-size old-size)) (printf "--\n")
        (debug-log "size %lu -> %lu" old-size new-size)
        (debug-display-data (struct-get val-data mv-data) old-size)
        (define data b8* (calloc new-size 1))
        (if (not data) (dg-status-set-id-goto dg-status-id-memory))
        (set (deref data) dg-guile-intern-string)
        (memcpy (+ dg-guile-intern-type-size data) (struct-get val-data mv-data) old-size)
        (define old-data b8* (calloc old-size 1))
        (if (not old-data) (dg-status-set-id-goto dg-status-id-memory))
        (memcpy old-data (struct-get val-data mv-data) old-size)
        (struct-set val-data mv-size new-size mv-data data)
        (debug-display-data (struct-get val-data mv-data) (struct-get val-data mv-size))
        (if run
          (begin (define id dg-id-t (dg-mdb-val->id val-id))
            (dg-mdb-status-require! (mdb-cursor-del id->data 0))
            (struct-set val-id mv-data (address-of id) mv-size dg-size-octets-id)
            (dg-mdb-status-require!
              (mdb-cursor-put id->data (address-of val-id) (address-of val-data) 0))



            ))
        (set count (+ 1 count)) (free data) (free old-data)))
    (set status.id
      (mdb-cursor-get id->data (address-of val-id) (address-of val-data) MDB-NEXT-NODUP)))

  (label exit dg-status-success-if-mdb-notfound
    (printf "processed count %lu. status %d\n" count status.id) (return status)))

(define (main a-len a) (b32 b32 b8_s**)
  status-init (define dg-root b8* (get-dg-root-or-exit a-len a))
  (define run boolean (= a-len 3)) (status-require! (dg-init dg-root 0))
  dg-txn-introduce dg-txn-write-begin
  (status-require! (dg-intern-add-type dg-txn run)) (printf "%s converted\n" dg-root)
  dg-txn-commit (dg-exit)
  (label exit
    (if (struct-get status id) (begin (printf "error\n") (printf (dg-status-description status))))
    (return (struct-get status id))))
