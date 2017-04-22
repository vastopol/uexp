;-------------------------------------------------------;
; Lambda Calculator                                     ;
; By Nils M Holm, 2016, 2017                            ;
;                                                       ;
; Provided under the Creative Common Zero (CC0) license ;
; (https://creativecommons.org/publicdomain/zero/1.0/)  ;
;                                                       ;
; Example: (nr '((lam (x y z) y) M N))  ->>  (lam z N)  ;
;                                                       ;
; (repl) will start a REPL; ? will print help           ;
;-------------------------------------------------------;

; If your Scheme has "char-ready?", ENTER can be used to
; interrupt reductions

(define (interrupted?) (char-ready?))
; (define (interrupted?) #f)

; Write output to the terminal and, optionally, a log file

(define log #f)

(define (log-disp x)
  (if log (display x log))
  (display x))

(define (log-nl)
  (if log (newline log))
  (newline))

; Rename the bound variables of a lambda term, e.g.
; ((lam x (lam y (x y))) y)  -->  ((lam x_1 (lam y_2 (x_1 y_2))) y)
;
; Used to avoid name collisions like the above:
; ((lam x (lam y (x y))) y)          -->  (lam y (y y))
; ((lam x_1 (lam y_2 (x_1 y_2))) y)  -->  (lam y_2 (x_1 y))

(define (rename x . t)

  (define id (char->integer #\a))
  (define sub 0)

  (define (next-var)
    (let ((v (string-append
               (string (integer->char id))
               (if (> sub 0)
                   (string-append "_" (number->string sub))
                   ""))))
      (set! id (+ 1 id))
      (if (> id (char->integer #\z))
          (begin (set! sub (+ 1 sub))
                 (set! id (char->integer #\a))))
      (string->symbol v)))

  (define (var x)
    (set! sub (+ 1 sub))
    (string->symbol
      (string-append
        (symbol->string x)
        "_"
        (number->string sub))))

  (define (bind x v e)
    (cons (cons x v) e))

  (define (newvar x n)
    (cond ((memq 'pnf t)
            (next-var))
          ((memq 'evec t)
            (string->symbol (number->string n)))
          (else
            (var x))))

  (define (aconv x e n)
    (cond ((assq x e)
            => cdr)
          ((not (pair? x))
            x)
          ((eq? 'lam (car x))
            (let ((v (newvar (cadr x) n)))
              `(lam ,v ,(aconv
                          (caddr x)
                          (bind (cadr x) v e)
                          (+ 1 n)))))
          (else
            (map (lambda (x)
                   (aconv x e n))
                 x))))

  (aconv x '() 0))

; Convert normal forms to Church principal NFs (PNFs), i.e.:
; (lam x (lam y z))  -->  (lam a (lam b c))

(define (pnf x)  (contract (rename (expand x) 'pnf)))

; Convert terms to evec notation; see "Compiling Lambda Calculus"
; (http://t3x.org/clc/) for an explanation
; ((lam x (lam y x)) (lam x x))  --> ((lam 0 (lam 1 0)) (lam 0 0))

(define (evec x) (contract (rename (expand x) 'evec)))

; Revert renaming of bound variables, i.e. remove subscripts from
; variable names:
; (lam x_1 (lam y_2 x_1))  -->  (lam x (lam y x))

(define (unrename x)

  (define (normvar x)
    (let loop ((y (string->list (symbol->string x)))
               (s '()))
      (if (or (null? y)
              (char=? #\_ (car y)))
          (string->symbol (list->string (reverse s)))
          (loop (cdr y)
                (cons (car y) s)))))

  (cond ((symbol? x)
          (normvar x))
        ((not (pair? x))
          x)
        (else
          (map unrename x))))

; Resolve symbolic names (%names) and Church Numerals in terms, e.g.:
; (%S c2)  -->  ((lam (n f x) (f (n f x))) (lam (f x) (f (f x))))

(define (resolve x)

  (define defchar #\%)
  (define numchar #\c)

  (define (ref? x)
    (and (symbol? x)
         (char=? defchar (string-ref (symbol->string x) 0))))

  (define (cval x)
    (and (symbol? x)
         (let* ((s (symbol->string x))
                (k (string-length s)))
           (and (> k 1)
                (char=? numchar (string-ref s 0))
                (string->number (substring s 1 k))))))

  (cond ((ref? x)
          (let ((v (assq x defs)))
            (if v
                (resolve (cadr v))
                x)))
        ((cval x)
          => cn)
        ((not (pair? x))
          x)
        (else
          (map resolve x))))

; Expand abbreviated expressions, e.g.:
; (lam (x y) z)  -->  (lam x (lam y z))
; (m n p)        -->  ((m n) p)
; For input that is not well-formed, return the symbol
; "malformed-expression"; see "helpdb" for a definition
; of well-formed expressions

(define (expand x)

  (define err #f)

  (define (x-lambda x)
    (if (or (null? (cdr x))
            (null? (cddr x))
            (null? (cadr x)))
        (begin (set! err #t)
               x)
        (let ((t (exp (caddr x))))
          (if (pair? (cadr x))
              (if (null? (cdadr x))
                  `(lam ,(caadr x)
                        ,t)
                  `(lam ,(caadr x)
                        ,(x-lambda
                           `(lam ,(cdadr x)
                            ,t))))
              `(lam ,(cadr x) ,t)))))

  (define (x-app x)
    (cond ((null? (cdr x))
            (set! err #t)
            x)
          ((null? (cddr x))
            x)
          (else
            (x-app `((,(car x) ,(cadr x))
                     ,@(cddr x))))))

  (define (proper-symbol? x)
    (not (memq #f (map (lambda (x)
                         (or (char-alphabetic? x)
                             (char-numeric? x)))
                       (string->list (symbol->string x))))))

  (define (exp x)
    (cond ((eq? 'lam x)
            (set! err #t)
            x)
          ((not (pair? x))
            (if (not (proper-symbol? x))
                (set! err #t))
            x)
          ((eq? 'lam (car x))
            (x-lambda x))
          (else
            (x-app (map exp x)))))

  (let ((x (exp x)))
    (if err 'malformed-expression x)))

; Contract unabbreviated expressions; this is basically
; the inverse operation of "expand", above.

(define (contract x)

  (define (lambda? x)
    (and (pair? x)
         (eq? 'lam (car x))))

  (define (redex? x)
    (and (pair? x)
         (lambda? (car x))))

  (define (conc a b)
    (if (pair? a)
        (if (pair? b)
            (append a b)
            (append a (list b)))
        (if (pair? b)
            (append (list a) b)
            (append (list a) (list b)))))

  (cond ((not (pair? x))
          x)
        ((lambda? x)
          (if (lambda? (caddr x))
              (contract `(lam ,(conc (cadr x)
                                     (cadr (caddr x)))
                              ,(caddr (caddr x))))
              `(lam ,(cadr x) ,(contract (caddr x)))))
        ((redex? x)
          `(,(contract (car x)) ,@(map contract (cdr x))))
        ((and (pair? (car x))
              (pair? (cdr x)))
          (contract (conc (car x) (cdr x))))
        (else
          (map contract x))))

; Pretty-print a term using TROFF/EQN commands, e.g.:
; (lam (x y) x))  -->  { lambda {x}{y}.{x}}
; Used by the trace function of "nr";
; see also "tt" command in the REPL.

(define (pprint x bf)

  (define done #f)

  (define (lambda? x)
    (and (pair? x)
         (eq? 'lam (car x))))

  (define (redex? x)
    (and (pair? x)
         (lambda? (car x))))

  (define (print-sym x)
    (let* ((y (symbol->string x))
           (k (string-length y)))
      (cond ((char=? #\% (string-ref y 0))
              (if (= k 2)
                  (log-disp (char-upcase (string-ref y 1)))
                  (begin (log-disp "{")
                         (log-disp (substring y 1 k))
                         (log-disp "} under ~"))))
            ((and (char=? #\c (string-ref y 0))
                  (string->number (substring y 1 k)))
              => (lambda (n)
                   (log-disp "{")
                   (log-disp n)
                   (log-disp "} under ")))
            (else
              (let loop ((i 0))
                (cond ((= i k)
                        (log-disp x))
                      ((char=? #\_ (string-ref y i))
                        (log-disp (substring y 0 i))
                        (log-disp " sub{ size -1 ")
                        (log-disp (substring y (+ 1 i) k))
                        (log-disp "}"))
                      (else
                        (loop (+ 1 i)))))))))

  (define (print-app x lb rb top)
    (if (not top) (log-disp lb))
    (let loop ((x x))
      (if (null? x)
          (if (not top) (log-disp rb))
          (begin (if (and (not done)
                          (redex? x))
                     (begin (set! done #t)
                            (if bf (log-disp " bold "))))
                 (ppf (car x))
                 (if (and (not (null? (cdr x)))
                          (redex? x))
                     (log-disp "~"))
                 (loop (cdr x))))))

  (define (print-lambda x top)
    (if (not top) (log-disp "("))
    (log-disp " lambda ")
    (if (pair? (cadr x))
        (for-each ppt (cadr x))
        (ppt (cadr x)))
    (if (or (pair? (cadr x))
            (pair? (caddr x)))
        (log-disp "."))
    (if (pair? (caddr x))
        (print-app (cddr x) "" "" #t)
        (ppt (caddr x)))
    (if (not top) (log-disp ")")))

  (define (pp x top)
    (log-disp "{")
    (cond ((lambda? x)
            (print-lambda x top))
          ((pair? x)
            (print-app x "(" ")" top))
          (else
            (print-sym x)))
    (log-disp "}"))

  (define (ppt x) (pp x #t))
  (define (ppf x) (pp x #f))

  (ppt x)
  (log-nl))

; Reduce a lambda term using applicative order reduction, a.k.a.
; inside-out or call-by-value reduction. This is the reduction
; strategy used by restricted lambda-K-beta-calculus. It is not
; guaranteed to yield a normal form for a term M, even if M does
; have a NF. E.g.:
; ((lam x M) %omega)  -->  undefined
; where %omega == ((lam x (x x)) (lam x (x x)))

(define (ar x eta)

  (define (subst x v m)
    (cond ((eq? x v)
            m)
          ((not (pair? x))
            x)
          ((and (eq? (car x) 'lam)
                (eq? (cadr x) v))
            x)
          (else
            (map (lambda (x)
                   (subst x v m))
                 x))))

  (define (beta-conv x)
    (let ((f (car x)))
      (let ((v (cadr f))
            (t (caddr f))
            (a (red (cadr x))))
        (subst t v a))))

  (define (lambda? x)
    (and (pair? x)
         (eq? 'lam (car x))))

  (define (redex? x)
    (and (pair? x)
         (lambda? (car x))))

  (define eta-conv caaddr)

  (define (ext? x)
    (and (lambda? x)
         (let ((t (caddr x)))
           (and (pair? t)
                (eq? (cadr x) (cadr t))))))

  (define (normal? x)
    (cond ((not (pair? x)))
          ((redex? x) #f)
          (else
            (not (memq #f (map normal? x))))))

  (define (red x)
    (cond ((interrupted?)
            'interrupted)
          ((and eta (ext? x))
            (red (eta-conv x)))
          ((normal? x)
            x)
          ((redex? x)
            (red (beta-conv x)))
          (else
            (red (map red x)))))

  (unrename (contract (red (rename (expand (resolve x)))))))

; Reduce a lambda term using normal order reduction, a.k.a.
; outside-in or left-to-right reduction. Always uses call-by-name
; semantics. Guaranteed to yield a normal form for M, if M does
; have a normal form. E.g.:
; ((lam x M) %omega)  -->  M
; where %omega == ((lam x (x x)) (lam x (x x)))

; tr = #t   means print trace
; ttr = #t  means pretty-print trace (using TROFF/EQN)
; eta = #t  means do eta-conversion:
;           (lam x (M x))  -->  M

(define (nr3 x tr ttr eta)

  (define (subst x v m)
    (cond ((eq? x v)
            m)
          ((not (pair? x))
            x)
          ((and (eq? (car x) 'lam)
                (eq? (cadr x) v))
            x)
          (else
            (map (lambda (x)
                   (subst x v m))
                 x))))

  (define (beta-conv x)
    (let ((f (car x)))
      (let ((v (cadr f))
            (t (caddr f))
            (a (cadr x)))
        (subst t v a))))

  (define eta-conv caaddr)

  (define (lambda? x)
    (and (pair? x)
         (eq? 'lam (car x))))

  (define (redex? x)
    (and (pair? x)
         (lambda? (car x))))

  (define (ext? x)
    (and (lambda? x)
         (let ((t (caddr x)))
           (and (pair? t)
                (eq? (cadr x) (cadr t))))))

  (define (step x)

    (define done #f)

    (define (s x)
      (cond ((or done
                 (not (pair? x)))
              x)
            ((redex? x)
              (set! done #t)
              (beta-conv x))
            ((and eta (ext? x))
              (set! done #t)
              (eta-conv x))
            ((lambda? x)
              `(lam ,(cadr x) ,(s (caddr x))))
            (else
              (let ((head (s (car x))))
                (list head
                      (s (cadr x)))))))

    (let ((n (s x)))
      (if done n #f)))

  (define (print-trace pfx x)
    (if ttr
        (begin (log-disp ".EQ")
               (log-nl)
               (log-disp pfx)
               (pprint (contract x) #t)
               (log-disp ".EN")
               (log-nl)
               (log-disp ".br")
               (log-nl))
        (if tr
            (begin (log-disp pfx)
                   (log-disp (unrename (contract x)))
                   (log-nl)))))

  (define (red x)
    (let ((n (step x)))
      (print-trace "-> " x)
      (if (interrupted?)
          'interrupted
          (if n
              (red n)
              x))))

  (print-trace "" x)
  (unrename (contract (red (rename (expand (resolve x)))))))

; Shorthand

(define (nr x) (nr3 x #f #f #f))

; Create Church Numeral (CN) with value n, e.g.:
; 3  -->  (lam (f x) (f (f (f x))))

(define (cn n)
  (define (c n)
    (if (zero? n)
        'x
        `(f ,(c (- n 1)))))
  `(lam (f x) ,(c n)))

; Convert CN to integer, i.e. inverse of above.

(define (int x)
  (define (c x r)
    (if (or (not (pair? x))
            (not (pair? (cdr x))))
        r
        (c (cadr x) (+ 1 r))))
  (if (not (pair? x))
      0
      (c (caddr x) 0)))

; Definitions for use in Scheme programs, e.g.:
; (nr `(,plus ,c2 ,c3))

(define true  '(lam (x y) x))
(define false '(lam (x y) y))
(define succ  '(lam (n f x) (f (n f x))))
(define plus  '(lam (m n f x) (m f (n f x))))
(define times '(lam (m n f) (m (n f))))
(define exp   '(lam (m n) (n m)))
(define pred  '(lam (n f x) ((n (lam (g h) (h (g f)))) (lam u x) (lam u u))))
(define minus `(lam (m n) (n ,pred m)))
(define zerop `(lam n (n (lam x ,false) ,true)))
(define conj  `(lam (m n) (m n ,false)))
(define disj  `(lam (m n) (m ,true n)))
(define neg   `(lam (n) (n ,false ,true)))
(define lteq  `(lam (m n) (,zerop (,minus m n))))
(define gteq  `(lam (m n) (,lteq n m)))
(define grtr  `(lam (m n) (,neg (,lteq m n))))
(define less  `(lam (m n) (,neg (,gteq m n))))
(define eq    `(lam (m n) (,lteq m n) (,lteq n m) ,false))
(define yc    '(lam f ((lam x (f (x x))) (lam x (f (x x))))))
(define c0    '(lam (f x) x))
(define c1    (nr `(,succ ,c0)))
(define c2    (nr `(,succ ,c1)))
(define c3    (nr `(,succ ,c2)))
(define c4    (nr `(,succ ,c3)))
(define c5    (nr `(,succ ,c4)))
(define c6    (nr `(,succ ,c5)))
(define c7    (nr `(,succ ,c6)))
(define c8    (nr `(,succ ,c7)))
(define c9    (nr `(,succ ,c8)))
(define div   `(lam (m n)
                 ((,yc (lam (r d) (,zerop d ,c0 (,succ (r (,minus d n))))))
                  (,minus (,succ m) n))))
(define kp    `(lam p ((,yc (lam (f n) (p n n (f (,succ n))))) ,c0)))
(define div   `(lam (m n) (,kp (lam x (,eq m (,times n x))))))
(define omega '((lam x (x x)) (lam x (x x))))

; Definitions for use in the Lambda Calculator REPL, e.g.
; (%plus c2 c3)

(define defs
  '((%T     (lam (x y) x))
    (%F     (lam (x y) y))
    (%S     (lam (n f x) (f (n f x))))
    (%plus  (lam (m n f) (lam x (m f (n f x)))))
    (%times (lam (m n f) (m (n f))))
    (%exp   (lam (m n) (n m)))
    (%P     (lam (n f x) ((n (lam (g h) (h (g f)))) (lam u x) (lam u u))))
    (%minus (lam (m n) (n %P m)))
    (%zerop (lam n (n (lam x %F) %T)))
    (%conj  (lam (m n) (m n %F)))
    (%disj  (lam (m n) (m %T n)))
    (%neg   (lam (n) (n %F %T)))
    (%lteq  (lam (m n) (%zerop (%minus m n))))
    (%gteq  (lam (m n) (%lteq n m)))
    (%grtr  (lam (m n) (%neg (%lteq m n))))
    (%less  (lam (m n) (%neg (%gteq m n))))
    (%eq    (lam (m n) (%lteq m n) (%lteq n m) %F))
    (%Y     (lam f ((lam x (f (x x))) (lam x (f (x x))))))
    (%div   (lam (m n)
              ((%Y (lam (r d) (%zerop d c0 (%S (r (%minus d n))))))
               (%minus (%S m) n))))
    (%kp    (lam p ((%Y (lam (f n) (p n n (f (%S n))))) c0)))
    (%kdiv  (lam (m n) (%kp (lam x (%eq m (%times n x))))))
    (%omega ((lam x (x x)) (lam x (x x))))))

; REPL

(define (repl)

  (define eta #f)

  (define (print x)
    (log-disp x)
    (log-nl))

  ; Bind latest result to "%%" and print it

  (define (result x)
    (let ((a (assq '%% defs)))
      (set-car! (cdr a) x)
      (print x)))

  ; Open or close a log file

  (define (logfile s)
    (if (= 0 (string-length s))
        (if log
            (begin (newline log)
                   (close-output-port log)
                   (set! log #f)))
        (begin (if log (close-output-port log))
               (set! log (open-output-file s)))))

  ; Read space or newline delimited string from command line, i.e.:
  ; _____READ-THIS_____NL
  ; where _ is blank and NL is newline.

  (define (read-string)
    (let loop ((c (read-char)))
      (if (eqv? #\space c)
          (loop (read-char))
          (let loop ((c c)
                     (s '()))
            (if (or (eof-object? c)
                    (char=? #\newline c)
                    (char=? #\space c))
                (list->string (reverse s))
                (loop (read-char)
                      (cons c s)))))))

  (define (prompt)
    (log-disp ": ")
    (read))

  ; Look up and print definition ("/" command)

  (define (lookup x)
    (let ((v (assq x defs)))
      (if v
          (result (cadr v))
          (print "undefined"))))

  (define (pretty-print x)
    (cond ((assq x defs)
            => (lambda (x)
                 (pprint (cadr x) #f)))
          (else
            (pprint x #f))))

  ; Add new definition ("==" command)

  (define (newdef v t)
    (let loop ((d defs))
      (cond ((or (null? d)
                 (eq? (caar d) '%%))
              (set! defs (cons `(,v ,t) defs)))
            ((eq? (caar d) v)
              (set-cdr! (car d) (list t)))
            (else
              (loop (cdr d))))))

  (define helpdb
    '((""
       "Lambda Calculator 20170103 by Nils Holm"
       "Distributed under the CC0 License (public domain)"
       ""
       "Help topics:  ? cmd  = commands"
       "              ? expr = expressions")
      ("cmd"
       "/ %v     look up definition %v"
       "a x      reduce expression x using applicative order reduction"
       "c n      print value of church numeral n"
       "d        print all definitions"
       "e x      NO-reduce expression x, result in evec notation"
       "eta b    eta-conversion on/off (b = 0,off,nil means off, else on,"
       "         off by default)"
       "n x      reduce expression x using normal order (NO) reduction"
       "         (where the \"n\" is optional)"
       "p x      NO-reduce expression x, result in principal normal form"
       "pp x     pretty-print expression x using TROFF/EQN"
       "q        quit"
       "r x      resolve definitions, but do not evaluate"
       "t x      trace NO reduction of expression x"
       "tt x     trace NO reduction of x using TROFF output"
       "u x      unexpand (contract) abbreviations in expression x"
       "w f      copy output to logfile f"
       "w        stop copying output"
       "x x      expand abbreviations in expression x"
       "== %v x  create/change definition, %v = name, x = expression"
       "         also: (== %v x)")
      ("expr"
       "       <term> := <variable>"
       "               | <abstraction>"
       "               | <application>"
       ""
       "   <variable> := a | b | ... | z"
       "               | a0 | b0 | ... | z0"
       "               | ..."
       "               | %name"
       ""
       "<abstraction> := (lam <variable> <term>)"
       ""
       "<application> := (<term> <term>)"
       ""
       "Abbreviations:"
       "(lam (v1 v2 ...) t)  =  (lam v1 (lam v2 (... t ...)))"
       "     (t1 t2 t3 ...)  =  (... ((t1 t2) t3) ...)"
       "                 cN  =  (lam (f x) (f^N x))")
       ("")))

  ; Find and display help topic

  (define (help)
    (let ((t (assoc (read-string) helpdb)))
      (if (not t)
          (print "topic not found")
          (for-each
            (lambda (x)
              (display x)
              (newline))
            (append '("") (cdr t) '(""))))))

  (newdef '%% '())

  ; Read-eval-print-loop

  (let loop ((x (prompt)))
    (cond ((eof-object? x)
            (log-nl))
          ((eq? '? x)
            (help)
            (loop (prompt)))
          ((eq? '/ x)
            (lookup (read))
            (loop (prompt)))
          ((eq? 'a x)
            (result (ar (read) eta))
            (loop (prompt)))
          ((eq? 'c x)
            (print (int (resolve (read))))
            (loop (prompt)))
          ((eq? 'd x)
            (for-each print defs)
            (loop (prompt)))
          ((eq? 'e x)
            (result (evec (nr3 (read) #f #f eta)))
            (loop (prompt)))
          ((eq? 'eta x)
            (set! eta (not (memv (read) '(0 off nil))))
            (loop (prompt)))
          ((eq? 'n x)
            (result (nr3 (read) #f #f eta))
            (loop (prompt)))
          ((eq? 'p x)
            (result (pnf (nr3 (read) #f #f eta)))
            (loop (prompt)))
          ((eq? 'pp x)
            (pretty-print (read))
            (loop (prompt)))
          ((eq? 'q x)
            (if #f #f))
          ((eq? 'r x)
            (result (resolve (read)))
            (loop (prompt)))
          ((eq? 't x)
            (result (nr3 (read) #t #f eta))
            (loop (prompt)))
          ((eq? 'tt x)
            (result (nr3 (read) #f #t eta))
            (loop (prompt)))
          ((eq? 'u x)
            (result (contract (resolve (read))))
            (loop (prompt)))
          ((eq? 'w x)
            (logfile (read-string))
            (loop (prompt)))
          ((eq? 'x x)
            (result (expand (resolve (read))))
            (loop (prompt)))
          ((eq? '== x)
            (newdef (read) (read))
            (loop (prompt)))
          ((and (pair? x)
                (eq? '== (car x))
                (pair? (cdr x))
                (pair? (cddr x)))
            (newdef (cadr x) (caddr x))
            (loop (prompt)))
          (else
            (result (nr3 x #f #f eta))
            (loop (prompt))))))
