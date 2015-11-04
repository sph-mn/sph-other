(import (sph common) (sph other shitty-variable-name-generator))

(define (svn-cli)
  (let (arguments ((cli-create (ql (result-count word-part ...) #f #t))))
    (and-let*
      ( (words (false-if-exception (string-join (symbol-alist-ref arguments word-part) " ")))
        (result-count
          (false-if-exception (string->number (symbol-alist-ref arguments result-count)))))
      (each display-line (svn-multiple result-count words)))))

(svn-cli)