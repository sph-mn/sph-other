; sph-scheme-mode.el - simple standard scheme editing major mode.
; should be byte-compiled before usage (using the emacs command "byte-compile-file")
; version 2018-05-13
; copyright (C) 2011-2018 sph <sph@posteo.eu> (current maintainer)
; This program is free software; you can redistribute it and/or modify it
; under the terms of the GNU General Public License as published by
; the Free Software Foundation; either version 3 of the License, or
; (at your option) any later version.
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
; You should have received a copy of the GNU General Public License
; along with this program; if not, see <http://www.gnu.org/licenses/>.
;----
; features
; * syntax highlighting and indentation
; * regular indentation. every indentation step is represented by the same width
; * syntax highlighting for only a small subset of scheme keywords per default
; versus scheme-mode (scheme.el)
; * the original emacs scheme-mode additionally supports the dsssl-language and multiple extras for specific scheme implementations
; * lisp\scheme-mode indentation handling takes more than 500 lines, vs ~20 in this implementation using regular indentation
; * ~600 lines vs ~200
; * sph-scheme-mode is //much// faster

; changes to lines between "eval-when-compile" only take effect when (byte-) recompiled

(defalias (quote pointto-toplevel-sexp-begin) (quote beginning-of-defun))
(defalias (quote sequence-ref) (quote elt))

;-- syntax --;
; the syntax-table describes how various standard functions will treat text,
; for example movement within the buffer with "forward-word"
(defvar sph-scheme-mode-syntax-table
  (let ((r (make-syntax-table)) (i 0))
    (while (< i ?0)
      (modify-syntax-entry i "_ " r)
      (setq i (1+ i)))
    (setq i (1+ ?9))
    (while (< i ?A)
      (modify-syntax-entry i "_ " r)
      (setq i (1+ i)))
    (setq i (1+ ?Z))
    (while (< i ?a)
      (modify-syntax-entry i "_ " r)
      (setq i (1+ i)))
    (setq i (1+ ?z))
    (while (< i 128)
      (modify-syntax-entry i "_ " r)
      (setq i (1+ i)))
    (modify-syntax-entry ?\t "    " r)
    (modify-syntax-entry ?\n ">   " r)
    (modify-syntax-entry ?\f "    " r)
    (modify-syntax-entry ?\r "    " r)
    (modify-syntax-entry ?\s "    " r)
    (modify-syntax-entry ?\( "()  " r)
    (modify-syntax-entry ?\) ")(  " r)
    (modify-syntax-entry ?\; "< 2 " r)
    (modify-syntax-entry ?\" "\"  " r)
    (modify-syntax-entry ?' "'   " r)
    (modify-syntax-entry ?` "'   " r)
    (modify-syntax-entry ?, "'   " r)
    (modify-syntax-entry ?@ "'   " r)
    (modify-syntax-entry ?# "' 14" r)
    (modify-syntax-entry ?\\ "\\  " r)
    r))

;-- syntax highlighting --;
(defconst sph-scheme-sexp-comment-syntax-table
  (let ((r (make-syntax-table sph-scheme-mode-syntax-table)))
    (modify-syntax-entry ?\; "." r)
    (modify-syntax-entry ?\n " " r)
    (modify-syntax-entry ?# "'" r)
    r))

(defun lisp-font-lock-syntactic-face-function (state)
  (if (nth 3 state)
    ;; This might be a (doc)string or a |...| symbol.
    (let ((startpos (nth 8 state)))
      (if (eq (char-after startpos) ?|)
        ;; This is not a string, but a |...| symbol.
        nil
        (let* ((listbeg (nth 1 state))
            (firstsym (and listbeg
                (save-excursion
                  (goto-char listbeg)
                  (and (looking-at "([ \t\n]*\\(\\(\\sw\\|\\s_\\)+\\)")
                    (match-string 1)))))
            (docelt (and firstsym
                (function-get (intern-soft firstsym)
                  lisp-doc-string-elt-property))))
          (if (and docelt
              ;; It's a string in a form that can have a docstring.
              ;; Check whether it's in docstring position.
              (save-excursion
                (when (functionp docelt)
                  (goto-char (match-end 1))
                  (setq docelt (funcall docelt)))
                (goto-char listbeg)
                (forward-char 1)
                (condition-case nil
                  (while (and (> docelt 0) (< (point) startpos)
                      (progn (forward-sexp 1) t))
                    (setq docelt (1- docelt)))
                  (error nil))
                (and (zerop docelt) (<= (point) startpos)
                  (progn (forward-comment (point-max)) t)
                  (= (point) (nth 8 state)))))
            font-lock-doc-face
            font-lock-string-face))))
    font-lock-comment-face))

(defun sph-scheme-font-lock-syntactic-face-function (state)
  ; for highlighting srfi-62-comments #;()
  (when (and (null (nth 3 state))
      (eq (char-after (nth 8 state)) ?#)
      (eq (char-after (1+ (nth 8 state))) ?\;))
    (save-excursion
      (let ((pos (point))
          (end
            (condition-case err
              (let ((parse-sexp-lookup-properties nil))
                (goto-char (+ 2 (nth 8 state)))
                ; this does not handle the case where the s-exp itself contains a #; comment.
                (forward-sexp 1)
                (point))
              (scan-error (nth 2 err)))))
        (when (< pos (- end 2))
          (put-text-property pos (- end 2) (quote syntax-table) sph-scheme-sexp-comment-syntax-table))
        (put-text-property (- end 1) end (quote syntax-table) (quote (12))))))
  (lisp-font-lock-syntactic-face-function state))

(defface sph-scheme-parentheses-face
  (quote ((((background light)) (:foreground "#868686"))
      (((background dark)) (:foreground "#444444"))))
  "face to highlight parentheses in")

; the levels () are a font-lock feature to loosely set what should be highlighted
(defconst sph-scheme-font-lock-keywords-1
  (eval-when-compile
    (quote ("(\\(define[^ ]*?\\) +(\\(\\sw+\\)" (1 (quote font-lock-keyword-face)) (2 (quote font-lock-function-name-face))))))

(defconst sph-scheme-font-lock-keywords-2
  (eval-when-compile
    (list
      (quote ("(\\|)" . (quote sph-scheme-parentheses-face)))
      (quote ("(\\(define-syntax[^ ]*?\\) +(\\(\\sw+\\)" (1 (quote font-lock-keyword-face)) (2 (quote font-lock-preprocessor-face))))
      (quote ("(\\(define[^ ]*?\\) +(\\(\\sw+\\)" (1 (quote font-lock-keyword-face)) (2 (quote font-lock-function-name-face))))
      (quote ("(\\(define[^? ]*?\\) +\\(\\sw+\\)" (1 (quote font-lock-keyword-face)) (2 (quote font-lock-variable-name-face))))
      (list
        (concat
          "(\\("
          (regexp-opt
            (quote ("if" "q" "l" "debug-log"))
            t)
          "\\)\\> ")
        1 (quote font-lock-builtin-face))
      (quote ("\\<#t\\|#f\\>" . font-lock-constant-face)))))

(defvar sph-scheme-font-lock-keywords sph-scheme-font-lock-keywords-1 "default expressions to highlight in scheme modes")

;-- indentation --;
;todo: value of this variable not visible in sph-scheme-indent-line
(defvar compress-whitespace-on-indent)

(defun sph-scheme-indent-line ()
  ; restore-cursor-position makes for the effect of either jumping to the indented beginning of the line,
  ; or letting the cursor move with the line
  (interactive "P")
  (let
    ( (restore-cursor-position (> (current-column) (current-indentation)))
      (indent (save-excursion (sph-scheme-calculate-indentation))))
    (if restore-cursor-position
      (save-excursion (indent-line-to indent))
      (indent-line-to indent))
    ;(sph-scheme-compress-whitespace-on-line indent)
    ))

(defun sph-scheme-calculate-indentation ()
  "return the column to which the current line should be indented.
  it finds the current sexp-depth and indents by (* sexp-depth lisp-body-indent)"
  (let ((indent-line-beginning (line-beginning-position)))
    (pointto-toplevel-sexp-begin)
    (if (>= (point) indent-line-beginning) ;depth 1 begins on the current line
      0
      ; indent-point-depth - parse until indent-line-beginning is reached while counting expression nesting-depth
      (* (max 0 (sequence-ref (parse-partial-sexp (point) indent-line-beginning) 0)) lisp-body-indent))))

(defun sph-scheme-compress-whitespace-on-line (start)
  ; this compresses whitespace in strings
  (save-excursion
    (move-to-column start)
    (while (re-search-forward "[ \t]+" (line-end-position) t)
      (replace-match " "))
    (move-to-column start)
    (while (re-search-forward "[ \t]$" (line-end-position) t)
      (replace-match ""))))

;-- key binding --;
(defvar sph-scheme-mode-map
  (let ((keymap (make-sparse-keymap "sph-scheme")))
    keymap)
  "keymap for sph-scheme-mode")

;-- mode initialisation --;
(defcustom sph-scheme-mode-hook nil "normal hook run when entering \"sph-scheme-mode\". see \"run-hooks\""
  :type (quote hook) :group (quote scheme))

(defgroup scheme nil
  "editing scheme code."
  :link (quote (custom-group-link :tag "Font Lock Faces group" font-lock-faces))
  :group (quote lisp))

(defun sph-scheme-mode-variables ()
  "major mode for editing scheme code"
  (set-syntax-table sph-scheme-mode-syntax-table)
  (setq-local parse-sexp-ignore-comments t)
  (setq-local indent-line-function nil)
  (setq indent-line-function (quote sph-scheme-indent-line))
  (setq-local comment-start ";")
  (setq-local comment-add 1)
  (make-local-variable (quote comment-start-skip))
  (setq-local comment-start-skip ";+[ \t]*")
  (setq-local font-lock-comment-start-skip ";+ *")
  (setq-local comment-column 40)
  (setq font-lock-defaults
    (quote ((sph-scheme-font-lock-keywords sph-scheme-font-lock-keywords-1 sph-scheme-font-lock-keywords-2)
        nil t
        (("+-*/.<>=!?$%_&~^:" . "w") (?#. "w 14"))
        pointto-toplevel-sexp-begin
        (font-lock-mark-block-function . mark-defun)
        (font-lock-syntactic-face-function . sph-scheme-font-lock-syntactic-face-function)
        ;this is required for s-exp-comment parsing
        (parse-sexp-lookup-properties . t)))))

(define-derived-mode sph-scheme-mode prog-mode "sph-scheme"
  (sph-scheme-mode-variables))

(provide (quote sph-scheme-mode))
