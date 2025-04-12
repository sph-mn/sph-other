(defun sph-wisp-expand-prefixes (rules)
  (let ((prefixes '("^\\s-*\\(?:([ \t]*\\)?" " : "))
      result)
    (dolist (rule rules)
      (dolist (prefix prefixes)
        (let ((pattern (car rule))
            (rest (cdr rule)))
          (push (cons (concat prefix pattern) rest) result))))
    (nreverse result)))

(defconst sph-wisp-builtin-keywords '("set" "if" "debug-log" "declare"))

(defconst sph-wisp-builtin-regexp
  (concat "\\(" (regexp-opt sph-wisp-builtin-keywords t) "\\)\\_>"))

(defvar sph-wisp-font-lock-keywords
  (append
    '(("\\<#t\\|#f\\>" . font-lock-constant-face)
      ("#\\\\[^ ]" . font-lock-constant-face)
      ("\\<\\(\\+\\|-\\)?[0-9]+\\(\\.[0-9]+\\)?" . font-lock-constant-face)
      (";.*$" . font-lock-comment-face))
    (sph-wisp-expand-prefixes
      `((,sph-wisp-builtin-regexp 1 font-lock-builtin-face)
        ("\\(\\sw+\\)\\_>" 1 font-lock-function-name-face)))
    (sph-wisp-expand-prefixes
      '(("\\(define[^ ]*?\\) +(\\(\\sw+\\)" 1 font-lock-keyword-face 2 font-lock-function-name-face)
        ("\\(define[^ ]*?\\) +(\\(\\sw+\\) \\(.*?\\))" 1 font-lock-keyword-face 2 font-lock-function-name-face)
        ("\\(define[^? ]*?\\) +\\(\\sw+\\)" 1 font-lock-keyword-face 2 font-lock-variable-name-face)))))

(defun sph-wisp-compress-whitespace-on-line (start)
  (save-excursion
    (move-to-column start)
    (while (re-search-forward "[ \t]+" (line-end-position) t)
      (replace-match " "))
    (move-to-column start)
    (while (re-search-forward "[ \t]$" (line-end-position) t)
      (replace-match ""))))

(defcustom sph-wisp-tab-width 2
  "Indentation step size for `sph-wisp-mode'."
  :type 'integer)

(defun sph-wisp-insert-spaces (n)
  (insert (make-string n ?\s)))

(defun sph-wisp-previous-indent ()
  (save-excursion
    (forward-line -1)
    (while (and (not (bobp)) (looking-at-p "^[ \t]*$"))
      (forward-line -1))
    (current-indentation)))

(defun sph-wisp-line-wants-indent ()
  (save-excursion
    (beginning-of-line)
    (looking-at ".*[:({]\\s-*$")))

(defun sph-wisp-newline-and-indent ()
  "Insert a newline and indent to match or increase the previous line."
  (interactive)
  (let ((prev-indent (current-indentation)))
    (newline)
    (if (sph-wisp-line-wants-indent)
      (sph-wisp-insert-spaces (+ prev-indent sph-wisp-tab-width))
      (sph-wisp-insert-spaces prev-indent))))

(defun sph-wisp-indent-line ()
  (interactive)
  (let* ((curindent (current-indentation))
      (prev-indent (sph-wisp-previous-indent))
      (indent-p (sph-wisp-line-wants-indent))
      (repeated (eq last-command this-command))
      (next-indent (- curindent sph-wisp-tab-width))
      (new-indent
        (cond
          (repeated
            (if (< next-indent 0)
              (+ prev-indent (if indent-p sph-wisp-tab-width 0))
              next-indent))
          (t
            (+ prev-indent (if indent-p sph-wisp-tab-width 0))))))
    (unless (= curindent new-indent)
      (save-excursion
        (goto-char (line-beginning-position))
        (delete-horizontal-space)
        (sph-wisp-insert-spaces new-indent)))
    (when (< (current-column) new-indent)
      (back-to-indentation))))

(defcustom sph-wisp-mode-hook nil "hook run when entering sph-wisp-mode"
  :type 'hook :group 'scheme)

(defvar sph-wisp-mode-map
  (let ((map (make-sparse-keymap)))
    (define-key map [remap newline-and-indent] 'sph-wisp-newline-and-indent)
    (define-key map (kbd "RET") 'sph-wisp-newline-and-indent)
    map))

(defvar sph-wisp-mode-syntax-table
  (let
    ((st (make-syntax-table)))
    (modify-syntax-entry ?\( "() " st)
    (modify-syntax-entry ?\) ")( " st)
    (modify-syntax-entry ?\" "\" " st)
    (modify-syntax-entry ?\\ "\\ " st)
    (modify-syntax-entry ?\; "< 2 " st)
    (modify-syntax-entry ?\n "> " st)
    (modify-syntax-entry ?' "' " st)
    (modify-syntax-entry ? "' " st)
    (modify-syntax-entry ?, "' " st)
    (modify-syntax-entry ?@ "' " st)
    (modify-syntax-entry ?# "' 14" st)
    (modify-syntax-entry ?\t " " st)
    (modify-syntax-entry ?\r " " st)
    (modify-syntax-entry ?\f " " st)
    (modify-syntax-entry ?\s " " st)
    (dolist (c '(?- ?_ ?+ ?= ?< ?> ?/ ?*))
      (modify-syntax-entry c "w" st))

    st))

(define-derived-mode sph-wisp-mode prog-mode "sph-wisp"
  "Major mode for editing wisp"
  :syntax-table sph-wisp-mode-syntax-table
  :keymap sph-wisp-mode-map
  (setq font-lock-defaults '((sph-wisp-font-lock-keywords)))
  (setq-local indent-line-function 'sph-wisp-indent-line)
  (setq-local comment-start ";"))

(defgroup scheme nil "editing scheme code." :link '(custom-group-link :tag "Font Lock Faces group" font-lock-faces) :group 'lisp)
(provide 'sph-wisp-mode)
