; DO NOT EDIT THIS FILE! EDIT "edoc/matcher.scm.edoc" INSTEAD.

; Scheme 9 from Empty Space, Function Library
; By Nils M Holm, 2009,2010
; Placed in the Public Domain
;
; (make-matcher symbol list ...)                  ==>  form
; (define-matcher symbol <clause> ...)            ==>  unspecific
; (let-matcher symbol (<clause> ...) <expr> ...)  ==>  object
;
; (load-from-library "matcher.scm")
;
; These constructs allow to write programs using pattern matching
; style. The MAKE-MATCHER procedure creates a matcher from the
; given clauses which are represented by LISTs. Each <clause> must
; have the form:
;
;       (<pattern> ... => <expression> ...)
;
; where each <pattern> is a Scheme datum and <expression> is an
; arbitrary expression. MAKE-MATCHER returns a list resembling
; a procedure which matches its argument list against the list of
; <pattern>s and evaluates the expressions associated with the
; first match.
;
; Each pattern is matched against the corresponding argument as
; follows:
;
; - Atomic objects, except for symbols, match themselves;
; - The symbol NIL matches the empty argument list (so
;   (nil => foo) is equal to (=> foo);
; - The symbol @ binds the remaining arguments to the subsequent
;   symbol and ends the match successfully -- it is the matcher's
;   equivalent to Scheme's improper argument lists;
; - Symbols match any value and bind the symbol to that value;
;   the <expression>s will be evaluated with these bindings in
;   effect;
; - The symbol _ matches any value, but does not bind to it;
; - A quoted symbol matches the symbol being quoted;
; - A list is matched by matching its members recursively.
;
; The SYMBOL passed to MAKE-MATCHER is the name of the resulting
; matcher. It is merely used for error reporting. In order for
; a matcher to recurse, it must be bound using DEFINE-MATCHER
; or LET-MATCHER.
;
; The DEFINE-MATCHER syntax binds a matcher to a symbol at the
; top-level. LET-MATCHER binds a matcher locally. These constructs
; correspond to DEFINE and LETREC.
;
; Example:   (begin
;              (define-matcher len
;                (()      => 0)
;                ((_ . x) => (+ 1 (len x))))
;              (len '(a b c d e f)))                  ==>  6
;
;            (let-matcher how-many
;              ((nil
;                 => 0)
;               (_ @ more
;                 => (+ 1 (apply how-many more))))
;              (how-many 1 2 3 4 5))                  ==> 5
;
;            (let-matcher appnd
;              ((() x      => x)
;               ((h . t) x => (cons h (appnd t x))))
;              (appnd '(a b c) '(d e f)))             ==>  (a b c d e f)

(load-from-library "position.scm")
(load-from-library "sublist.scm")
(load-from-library "group.scm")

(define (make-matcher name . clauses)

  (define *accessors* '())

  (define *args* (gensym))

  (define (cad*r-name a)
    (string->symbol
      (apply string-append
             (map symbol->string
                  (append '(c) a '(r))))))

  (define (make-accessor-call path x)
    (if (null? path)
        x
        (let ((a (cad*r-name path)))
          (if (not (memq a *accessors*))
              (set! *accessors* (cons a *accessors*)))
          `(,a ,x))))

  (define (make-accessor path)
    (let ((acc (make-accessor-call path '())))
      (if (null? acc)
          acc
          (car acc))))

  (define (match-object type-pred eqv-pred accessor pattern)
    (let ((tmp (gensym)))
      `((let ((,tmp ,(make-accessor-call accessor *args*)))
          (and (,type-pred ,tmp)
               (,eqv-pred ,tmp ,pattern))))))

  (define (single-matcher pattern accessor)
    (cond ((and (pair? pattern)
                (eq? 'quote (car pattern)))
             `((eq? ,pattern ,(make-accessor-call accessor *args*))))
          ((and (pair? pattern)
                (eq? 'nil (car pattern))
                (null? (cdr pattern)))
            `((null? ,(make-accessor-call accessor *args*))))
          ((pair? pattern)
            (let loop ((p pattern)
                       (a accessor)
                       (r '()))
              (cond ((and (pair? p)
                          (eq? '@ (car p))
                          (pair? (cdr p))
                          (symbol? (cadr p))
                          (null? (cddr p)))
                      (apply append (reverse r)))
                    ((pair? p)
                      (loop (cdr p)
                            (cons 'd a)
                            (cons (append `((pair? ,(make-accessor-call
                                                      a
                                                      *args*)))
                                          (single-matcher (car p)
                                                          (cons 'a a)))
                                  r)))
                    (else
                      (apply append (reverse (cons (single-matcher p a)
                                                   r)))))))
          ((symbol? pattern)
            `())
          ((null? pattern)
            `((null? ,(make-accessor-call accessor *args*))))
          ((number? pattern)
            (match-object 'number? '= accessor pattern))
          ((eq? #f pattern)
            `((not ,(make-accessor-call accessor *args*))))
          ((eq? #t pattern)
            `((eq? #t ,(make-accessor-call accessor *args*))))
          ((char? pattern)
            (match-object 'char? 'char=? accessor pattern))
          ((string? pattern)
            (match-object 'string? 'string=? accessor pattern))
          ((vector? pattern)
            (match-object 'vector? 'equal? accessor pattern))
          (else
            (error "unknown object in pattern" pattern))))

  (define (fetch-variables pattern accessor)
    (cond ((pair? pattern)
            (let loop ((p pattern)
                       (a accessor)
                       (r '()))
              (cond ((or (null? p)
                         (and (pair? p)
                              (eq? 'quote (car p))))
                      (apply append (reverse! r)))
                    ((and (not (memq 'a accessor))
                          (pair? p)
                          (eq? '@ (car p))
                          (pair? (cdr p))
                          (symbol? (cadr p))
                          (null? (cddr p)))
                      (append (apply append (reverse! r))
                              `((,(cadr p) . ,(make-accessor a)))))
                    ((and (not (memq 'a accessor))
                          (pair? p)
                          (eq? 'nil (car p))
                          (null? (cdr p)))
                      '())
                    ((pair? p)
                      (loop (cdr p)
                            (cons 'd a)
                            (cons (fetch-variables (car p) (cons 'a a))
                                  r)))
                    (else
                      (loop '() a (cons (fetch-variables p a)
                                        r))))))
          (else
            (if (and (symbol? pattern)
                     (not (eq? '_ pattern)))
                `(,(cons pattern (make-accessor accessor)))
                `()))))

  (define (split-clause clause)
    (let ((k (posq '=> clause)))
      (if k
          (cons (sublist clause 0 k)
                (list-tail clause (+ 1 k)))
          (error "make-matcher: syntax error (missing =>)" clause))))

  (define (make-case clause)
    (let* ((pat/body (split-clause clause))
           (pattern  (car pat/body))
           (body     (cdr pat/body))
           (matcher  (single-matcher pattern '()))
           (env      (fetch-variables pattern '()))
           (vars     (map car env))
           (args     (map (lambda (x)
                            (if (null? (cdr x))
                                *args*
                                `(,(cdr x) ,*args*)))
                          env)))
      `((and ,@matcher)
         ((lambda ,vars ,@body) ,@args))))

  (define (gen-cad*r path)
    (let* ((name (cad*r-name path))
           (body (let loop ((p (group path 4)))
                   (if (null? (cdr p))
                       `(,(cad*r-name (car p)) x)
                       (let ((op (cad*r-name (car p))))
                         `(,op ,(loop (cdr p))))))))
      `(,name (lambda (x) ,body))))

  (define (def-accessor accessor)
    (let* ((acc-chars (symbol->string accessor))
           (k         (string-length acc-chars))
           (acc-path  (map (lambda (x)
                             (string->symbol (string x)))
                           (string->list (substring acc-chars 1 (- k 1))))))
      (if (> (length acc-path) 4)
          (list (gen-cad*r acc-path))
          '())))

  (let* ((cases    (map make-case clauses))
         (acc-defs (apply append (map def-accessor *accessors*))))
    `(lambda ,*args*
       (let (,@acc-defs)
         (cond ,@cases
               (else (error "syntax error" (cons ',name ,*args*))))))))

(define-syntax (let-matcher name clauses . body)
  `(letrec ((,name ,(apply make-matcher name clauses))) ,@body))

(define-syntax (define-matcher name . clauses)
  `(define ,name ,(apply make-matcher name clauses)))

#|
(define-syntax (case x . cases)
  (let-matcher do-cases
    ((k
       => '(if #f #f))
     (k ((x . xs) . exprs) @ clauses
       => `(if (memv ,k '(,x ,@xs))
               (begin ,@exprs)
               ,(apply do-cases k clauses)))
     (k (else . exprs)
       => `(begin ,@exprs)))
    (let ((k (gensym)))
      `(let ((,k ,x))
         ,(apply do-cases k cases)))))
|#

