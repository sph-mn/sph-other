#!/usr/bin/guile
!#
(import (only (rnrs hashtables) hashtable-set!) (sph hashtable) (sph base) (rnrs io ports) (sph random-data) (only (srfi srfi-1) delete-duplicates))

;trollface (do not mind me)
;variable name creator
;enter one or multiple words
;you should use
;starseal "newbie friendly!" "business performance optimised!" "might increase program efficiency!" "highly usable!" "readability!" "it has always worked that way!" "edit your code in word with formatting!"
;typos are no problem because there is autocomplete

;decreasing chances per run
;updating chances
;updating chances in relation
;chance deviation
;chance initialisation

(define vowel "aeiou")
(define consonant "bcdfghjklmnpqrstvwxyz")

(define-as chances symbol-hashtable
  uppercase 30
  vowel-removal 70
  shorten-to-consonant 60
  spaces-removal 20
  underscore 50
  camelcase 75)

(define-as settings symbol-hashtable
  chance-deviation 1.4
  chance-max-init-iterations 2000)

#;(define (update-chances)
  (hashtable-map!
    (l (k v)
      (- 100 (modulo (round ((if (random-boolean) + -) (hashtable-ref chances k) (/ (hashtable-ref chances k) 5))) 100)))
    chances))
;(n-times-map (random (hashtable-ref settings (q chance-max-init-iterations))) (l (n) (update-chances)))

(define (random-boolean) (= (random 2) 0))
(define (random-weighted-boolean percentage) (< (random 100) percentage))

(define-syntax-rule (apply-modification? name)
  (random-weighted-boolean (hashtable-ref chances (q name))))

(define (vowel-indices a)
  (map-with-index
    (l (i e) (string-index vowel e))
    a))

(define (remove-one-random-vowel a-list)
  (let*
    (
      (indices (vowel-indices a-list))
      (index-delete (+ 1 (random (- (length indices) 1)))))
    (delete #f (map-with-index (l (i e) (if (= i index-delete) #f e))a-list))))

(define (random-vowel-removal a)
  "string -> string"
  (list->string
    (delete #f
      (map-with-index
        (l (i e) (if (and (> i 0) (string-index vowel e) (apply-modification? vowel-removal)) #f e))
        (remove-one-random-vowel (string->list a))))))

(define (shorten-to-consonant a)
  (let loop ((index (- (string-length a) 1)))
    (if (or (< index (hashtable-ref settings (q minimum-word-length))) (and
          (string-index consonant (string-ref a index))
          (not (apply-modification? shorten-to-consonant))))
      (substring a 0 (+ index 1))
      (loop (- index 1)))))

(define (random-uppercase a)
  (if (apply-modification? uppercase) (string-upcase a) a))

(define (svn-word a) (fold (l (e r) (if (> (string-length r) (hashtable-ref settings (q minimum-word-length))) (e r) r))
    a
    (list (if (apply-modification? shorten-to-consonant) shorten-to-consonant identity) random-vowel-removal)))

(define (words-camelcase a)
  (pair (first a) (map string-capitalize (tail a))))

(define (join-words a)
  (string-join
    (if (apply-modification? camelcase) (words-camelcase a) a)
    (if (apply-modification? underscore)
      "_" "")))

(define (randomly-delete-spaces a)
  (string-delete
    (l (e) (and (eqv? #\space e) (apply-modification? spaces-removal)))
    a))

(define (svn a)
  (let*
    ( (words-wo-spaces (randomly-delete-spaces a))
      (words (delete "" (string-split (random-uppercase words-wo-spaces) #\space))))
    (if (> (string-count words-wo-spaces #\space) 0) (hashtable-set! settings (q minimum-word-length) 1) (hashtable-set! settings (q minimum-word-length) 3))
    (join-words (map svn-word words))) )

(let (line (string-join (tail (tail (program-arguments))) " "))
  (each
    (l (e) (display e) (newline))
    (delete-duplicates (n-times-map (string->number (first (tail (program-arguments)))) (l (n) (svn line))))))
