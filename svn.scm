(import (sph common) (sph other shitty-variable-name-generator))

(define (svn-cli)
  (let (arguments ((cli-create (ql (result-count word-part ...) #f #t))))
    (and-let*
      ( (words (false-if-exception (string-join (alist-quoted-ref arguments word-part) " ")))
        (result-count
          (false-if-exception (string->number (alist-quoted-ref arguments result-count)))))
      (each display-line (svn-multiple result-count words)))))

(svn-cli)