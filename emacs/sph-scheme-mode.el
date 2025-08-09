;;; sph-scheme-mode.el --- simple Scheme editing -*- lexical-binding: t; -*-
; version 2025-08-09

(defgroup sph-scheme nil "Scheme editing" :group 'languages)

(defcustom sph-scheme-compress-on-indent nil
  "If non-nil, compress spaces on indent/newline outside strings and comments."
  :type 'boolean :group 'sph-scheme)

(defcustom sph-scheme-highlight-brackets t
  "If non-nil, highlight parentheses with `sph-scheme-bracket-face'."
  :type 'boolean :group 'sph-scheme)

(defface sph-scheme-bracket-face '((t :inherit default)) "Face for ()" :group 'sph-scheme)

(defalias 'sequence-ref 'elt)

(defvar sph-scheme-mode-syntax-table
  (let ((r (make-syntax-table)) (i 0))
    (while (< i ?0) (modify-syntax-entry i "_ " r) (setq i (1+ i)))
    (setq i (1+ ?9))
    (while (< i ?A) (modify-syntax-entry i "_ " r) (setq i (1+ i)))
    (setq i (1+ ?Z))
    (while (< i ?a) (modify-syntax-entry i "_ " r) (setq i (1+ i)))
    (setq i (1+ ?z))
    (while (< i 128) (modify-syntax-entry i "_ " r) (setq i (1+ i)))
    (modify-syntax-entry ?\t " " r)
    (modify-syntax-entry ?\n "> " r)
    (modify-syntax-entry ?\f " " r)
    (modify-syntax-entry ?\r " " r)
    (modify-syntax-entry ?\s " " r)
    (modify-syntax-entry ?\( "() " r)
    (modify-syntax-entry ?\) ")( " r)
    (modify-syntax-entry ?\; "< 2 " r)
    (modify-syntax-entry ?\" "\" " r)
    (modify-syntax-entry ?' "' " r)
    (modify-syntax-entry ?` "' " r)
    (modify-syntax-entry ?, "' " r)
    (modify-syntax-entry ?@ "' " r)
    (modify-syntax-entry ?# "' 14" r)
    (modify-syntax-entry ?\\ "\\ " r)
    r))

(defconst sph-scheme-sexp-comment-syntax-table
  (let ((r (make-syntax-table sph-scheme-mode-syntax-table)))
    (modify-syntax-entry ?\; "." r)
    (modify-syntax-entry ?\n " " r)
    (modify-syntax-entry ?# "'" r)
    r))

(defun sph-scheme--lfl-sff (state)
  (if (nth 3 state)
    (let ((startpos (nth 8 state)))
      (if (eq (char-after startpos) ?|)
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
  (when (and (null (nth 3 state))
      (eq (char-after (nth 8 state)) ?#)
      (eq (char-after (1+ (nth 8 state))) ?\;))
    (save-excursion
      (let ((pos (point))
          (end (condition-case err
              (let ((parse-sexp-lookup-properties nil))
                (goto-char (+ 2 (nth 8 state)))
                (forward-sexp 1)
                (point))
              (scan-error (nth 2 err)))))
        (when (< pos (- end 2))
          (put-text-property pos (- end 2) 'syntax-table sph-scheme-sexp-comment-syntax-table))
        (put-text-property (- end 1) end 'syntax-table '(12)))))
  (sph-scheme--lfl-sff state))

(defconst sph-scheme-font-lock-keywords-1
  '(("(\\(define[^ ]*?\\) +(\\(\\sw+\\)"
      (1 font-lock-keyword-face) (2 font-lock-function-name-face))
    ("[()]" . sph-scheme-bracket-face)))

(defconst sph-scheme-font-lock-keywords-2
  '(("(\\(define-syntax[^ ]*?\\) +(\\(\\sw+\\)"
      (1 font-lock-keyword-face) (2 font-lock-preprocessor-face))
    ("(\\(define[^ ]*?\\) +(\\(\\sw+\\)"
      (1 font-lock-keyword-face) (2 font-lock-function-name-face))
    ("(\\(define[^? ]*?\\) +\\(\\sw+\\)"
      (1 font-lock-keyword-face) (2 font-lock-variable-name-face))
    ((concat "(\\(" (regexp-opt '("if" "q" "l" "debug-log") t) "\\)\\> ")
      1 font-lock-builtin-face)
    ("\\_<#\\(t\\|f\\)\\_>" . font-lock-constant-face)))

(defvar sph-scheme-font-lock-keywords sph-scheme-font-lock-keywords-1)

(defvar-local sph-scheme--indent-width nil)

(defun sph-scheme--ppss-depth-at-bol ()
  (save-excursion (goto-char (line-beginning-position)) (car (syntax-ppss))))

(defun sph-scheme--line-compress (start)
  (save-excursion
    (move-to-column start)
    (while (re-search-forward "[ \t]+" (line-end-position) t) (replace-match " "))
    (move-to-column start)
    (when (re-search-forward "[ \t]+$" (line-end-position) t) (replace-match ""))))

(defun sph-scheme-indent-line ()
  (interactive "P")
  (let* ((restore (> (current-column) (current-indentation)))
      (depth (sph-scheme--ppss-depth-at-bol))
      (w (or sph-scheme--indent-width lisp-body-indent))
      (want (* (max 0 depth) w)))
    (if restore (save-excursion (indent-line-to want)) (indent-line-to want))
    (when sph-scheme-compress-on-indent (sph-scheme--line-compress want))))

(defvar sph-scheme-mode-map (let ((m (make-sparse-keymap))) m))

;;;###autoload
(define-derived-mode sph-scheme-mode prog-mode "sph-scheme"
  :syntax-table sph-scheme-mode-syntax-table
  (setq-local parse-sexp-ignore-comments t)
  (setq-local indent-line-function #'sph-scheme-indent-line)
  (setq-local comment-start ";")
  (setq-local comment-start-skip ";+[ \t]*")
  (setq-local comment-add 1)
  (setq-local comment-column 40)
  (setq-local sph-scheme--indent-width lisp-body-indent)
  (setq-local font-lock-defaults
    '((sph-scheme-font-lock-keywords sph-scheme-font-lock-keywords-1 sph-scheme-font-lock-keywords-2)
      nil t
      (("+-*/.<>=!?$%_&~^:" . "w") (?# . "w 14"))
      beginning-of-defun
      (font-lock-mark-block-function . mark-defun)
      (font-lock-syntactic-face-function . sph-scheme-font-lock-syntactic-face-function)
      (parse-sexp-lookup-properties . t)
      (font-lock-extra-managed-props syntax-table)))
  (when (not sph-scheme-highlight-brackets)
    (setq-local sph-scheme-font-lock-keywords-1 (remq '("[()]" . sph-scheme-bracket-face) sph-scheme-font-lock-keywords-1))
    (setq-local sph-scheme-font-lock-keywords-2 (remq '("[()]" . sph-scheme-bracket-face) sph-scheme-font-lock-keywords-2))
    (setq-local sph-scheme-font-lock-keywords sph-scheme-font-lock-keywords-1))
  (font-lock-refresh-defaults)
  (when (fboundp 'font-lock-flush) (font-lock-flush))
  (when (fboundp 'font-lock-ensure) (font-lock-ensure))
  (use-local-map sph-scheme-mode-map))

(provide 'sph-scheme-mode)
