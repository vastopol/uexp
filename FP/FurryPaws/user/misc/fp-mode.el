;; *****************************************************************************
;;
;;  fp-mode.el
;;  Font-lock support for FP files
;;
;;  derived from:
;;
;;  Copyright (C) 2003, 2004, Rob Walker <rob@tenfoot.org.uk>
;;    http://www.tenfoot.org.uk/emacs/
;;  12 May 2004 - 0.3 - Fix keyword quoting, XEmacs support
;;  22 Mar 2003 - 0.2 - Autoload
;;  04 Mar 2003 - 0.1 - Added imenu support and basic indentation
;;
;;  Copyright (C) 2000, Eric Scouten
;;  Started Sat, 05 Aug 2000
;;
;; *****************************************************************************
;;
;;  This is free software; you can redistribute it and/or modify
;;  it under the terms of the GNU General Public License as published by
;;  the Free Software Foundation; either version 2, or (at your option)
;;  any later version.
;;
;;  jam-mode.el is distributed in the hope that it will be useful,
;;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
;;  General Public License for more details.
;;
;;  You should have received a copy of the GNU General Public License
;;  along with GNU Emacs; see the file COPYING.  If not, write to the
;;  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
;;  Boston, MA 02111-1307, USA.
;;
;; *****************************************************************************
;;
;;  To add font-lock support for Jam files, simply add the line
;;  (require 'jam-mode) to your .emacs file. Make sure generic-mode.el
;;  is visible in your load-path as well.
;;  
;; *****************************************************************************


;; Generic-mode is a meta-mode which can be used to define small modes
;; which provide basic comment and font-lock support. Jam-mode depends on
;; this mode.

;; generic.el for GNU emacs, generic-mode.el for XEmacs
(if (string-match "XEmacs\\|Lucid" emacs-version)
    (require 'generic-mode)
  (require 'generic))

;;;###autoload
(define-generic-mode 'fp-mode

  ; Fp comments always start with '%'
  (list ?% )

  ; Fp keywords
  (list "trace" "while" "catch" "unit" "extern")

  ; Extra stuff to colorize
  (list

	(generic-make-keywords-list
	 (list
	  "add" "sub" "mul" "div" "gt" "lt" "ge" "le" "band" "bor"
	  "bxor" "bnot" "not" "neg" "bshl" "bshr" "id" "eq" "s1" "s2"
	  "s3" "s4" "s1r" "s2r" "s3r" "s4r" "al" "ar" "dl" "dr" "mod"
	  "cat" "tl" "trans" "len" "atom" "atomic" "num" "tos" "toa" "ton" "cmp"
	  "zero" "add1" "sub1" "null" "rev" "_in" "in" "iota" "halt" "purge"
	  "_emit" "emit" "_get" "get" "_out" "out" "_show" "show" "max" "min" 
	  "abs" "tlr" "take" "_gc" "_debug"
	  "drop" "ravel" "replicate" "make" "compress" "index" "fetch"
	  "store" "expand" "chop" "rotl" "rotr" "negative" "positive" "assoc"
	  "x" "y" "z" "select" "selectr" "indexr" "join" "split" "rnd"
	  "_system" "system" "exit" "_throw" "throw" "_dumptrace" 
	  "spread" "any" "partition" "_rnd" "rnd"
	  "_env" "env" "pair" "fetchall" "union" "intersection" "delete" "even"
	  "count" "nodups" "diff" "_start" "popcount" "or" "and" "odd"
	  "fail" "put" "sign" "flip" "string" "all" "gc" "app" "splice" "copy"
	  "subseq" "main" "match")
	 'font-lock-builtin-face)

	'("\\([@/\\~*]\\)" 1 font-lock-keyword-face)

	'("\\([_a-z][a-z0-9_'%]*\\)\\s-*=" 1 font-lock-variable-name-face)

	; FP atoms
	'("\\([A-Z][A-Z0-9_'%]*\\)" 1 font-lock-constant-face)
	'("\\(\\`[^\\`]*\\`\\)" 1 font-lock-constant-face)

	)

  ; Apply this mode to all files called Fpfile, Fprules or Fpbase
  (list "\\.fp\\'")

  ; Attach setup function so we can modify syntax table.
  (list 'fp-mode-setup-function)

  ; Brief description
  "Generic mode for Fp rules files")

(defun fp-mode-setup-function ()	; likely to be all wrong
  (modify-syntax-entry ?_ "w")
  (modify-syntax-entry ?. ".")
  (modify-syntax-entry ?: ".")
  (modify-syntax-entry ?| ".")
  (modify-syntax-entry ?\; ".")
  (modify-syntax-entry ?, ".")
  (modify-syntax-entry ?/ "'")
  (modify-syntax-entry ?@ "'")
  (modify-syntax-entry ?\" "\"")
  (modify-syntax-entry ?\` "\"")
  (modify-syntax-entry ?$ "\\")
  (modify-syntax-entry ?% "<")
  (modify-syntax-entry ?\n ">")
  (run-hooks 'fp-mode-hook)
  )

(defvar fp-mode-hook nil)

(provide 'fp-mode)

;; fp-mode.el ends here
