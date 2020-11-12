;; Mini Scheme init file
;; By Atsushi Moriwaki <moriwaki@kurims.kurims.kyoto-u.ac.jp>
;;    Akira KIDA <SDI00379@niftyserve.or.jp>
;;    Nils M Holm <nmh@t3x.org>
;;
;; (There is not much left of the original init file. --nmh)
;;
;; THIS SOFTWARE IS IN THE PUBLIC DOMAIN
;; (But please leave the list of authors intact.)

(define call/cc call-with-current-continuation)

(define (list . x) x)

(define (map proc . lists)
  (letrec
    ((mapcar
       (lambda (f x)
         (if (null? x)
             '()
             (cons (f (car x)) (mapcar f (cdr x))))))
     (car-of   (lambda (x) (mapcar car x)))
     (cdr-of   (lambda (x) (mapcar cdr x)))
     (null-of? (lambda (x) (memq #t (mapcar null? x))))
     (map2
       (lambda (lists r)
         (if (null-of? lists)
             (reverse r)
             (map2 (cdr-of lists)
                   (cons (apply proc (car-of lists)) r))))))
    (map2 lists '())))

(define (for-each proc . lists)
  (apply map proc lists)
  #t)

(define (equal? x y)
  (cond ((eq? x y) #t)
        ((string? x)
          (and (string? y)
               (string=? x y)))
        ((pair? x)
          (and (pair? y)
               (equal? (car x) (car y))
               (equal? (cdr x) (cdr y))))
        (else
          (eqv? x y))))

(define (member obj lst)
  (let loop ((lst lst))
    (cond ((null? lst)
            #f)
          ((equal? obj (car lst))
            lst)
          (else
            (loop (cdr lst))))))

(define (assoc obj lst)
  (let loop ((lst lst))
    (cond ((null? lst)
            #f)
          ((equal? obj (caar lst))
            (car lst))
          (else
            (loop (cdr lst))))))

(define (string . x) (list->string x))

(define (string>? a b)  (string<? b a))
(define (string<=? a b) (not (string>? a b)))
(define (string>=? a b) (not (string<? a b)))

(define (string-ci>? a b)  (string-ci<? b a))
(define (string-ci<=? a b) (not (string-ci>? a b)))
(define (string-ci>=? a b) (not (string-ci<? a b)))

(define (number->string n . radix)
  (letrec
    ((digits
       (string->list "0123456789abcdef"))
     (conv
       (lambda (n rdx res)
         (if (zero? n)
             (if (null? res) '(#\0) res)
             (conv (quotient n rdx)
                   rdx
                   (cons (list-ref digits (remainder n rdx))
                         res)))))
     (conv-int
       (lambda (n rdx)
         (if (negative? n)
             (list->string (cons #\- (conv (abs n) rdx '())))
             (list->string (conv n rdx '())))))
     (get-radix
       (lambda ()
         (cond ((null? radix) 10)
               ((<= 2 (car radix) 16) (car radix))
               (else (error "number->string: invalid radix"
                            (car radix)))))))
    (conv-int n (get-radix))))

(define (string->number str . radix)
  (letrec
    ((digits
       (string->list "0123456789abcdef"))
     (value-of-digit
       (lambda (x)
         (letrec
           ((v (lambda (x d n)
                 (cond ((null? d) 17)
                       ((char=? x (car d)) n)
                       (else (v x (cdr d) (+ n 1)))))))
           (v (char-downcase x) digits 0))))
     (conv3
       (lambda (lst res rdx)
         (if (null? lst)
             res
             (let ((dval (value-of-digit (car lst))))
               (and (< dval rdx)
                    (conv3 (cdr lst)
                           (+ (value-of-digit (car lst))
                              (* res rdx))
                           rdx))))))
     (conv
       (lambda (lst rdx)
         (if (null? lst)
             #f
             (conv3 lst 0 rdx))))
     (sconv
       (lambda (lst rdx)
         (cond ((null? lst)
                 #f)
               ((char=? (car lst) #\+)
                 (conv (cdr lst) rdx))
               ((char=? (car lst) #\-)
                 (let ((r (conv (cdr lst) rdx)))
                   (if r (- r) #f)))
               (else
                 (conv lst rdx)))))
     (get-radix
       (lambda ()
         (cond ((null? radix) 10)
               ((<= 2 (car radix) 17) (car radix))
               (else (wrong "string->number: invalid radix" radix))))))
    (sconv (string->list str) (get-radix))))

(define-macro quasiquote
  (lambda (form)
    (letrec
      ((qq-cons
         (lambda (a b)
           (cond ((and (pair? a)
                       (eq? 'unquote-splicing (car a)))
                   (if (and (pair? b)
                            (eq? 'quote (car b))
                            (null? (cadr b)))
                       (cadr a)
                       (list 'append (cadr a) b)))
                 (else
                   (list 'cons a b)))))
       (qq-expand
         (lambda (x)
           (cond ((not (pair? x))
                   (list 'quote x))
                 ((and (eq? 'unquote (car x))
                       (pair? (cdr x)))
                   (cadr x))
                 ((and (eq? 'unquote-splicing (car x))
                       (pair? (cdr x)))
                   (list 'unquote-splicing (cadr x)))
                 ((and (eq? 'quasiquote (car x))
                       (pair? (cdr x)))
                   (error "quasiquote: may not be nested"))
                 (else
                   (qq-cons (qq-expand (car x))
                            (qq-expand (cdr x))))))))
      (qq-expand form))))

(define-macro do
  (lambda (vars test . body)
    (let ((v (map car vars))
          (i (map cadr vars))
          (s (map (lambda (x)
                    (if (null? (cddr x))
                        (car x)
                        (caddr x)))
                  vars))
          (o (gensym)))
      `(letrec
         ((,o (lambda ,v
                (if ,(car test)
                    (begin ,@(cdr test))
                    (begin ,@body (,o ,@s))))))
         (,o ,@i)))))

(define-macro delay
  (lambda (expr)
    `(let ((value #f))
       (lambda ()
         (if value
             (car value)
             (let ((x ,expr))
               (if value
                   (car value)
                   (begin (set! value (cons x '()))
                          (car value)))))))))

(define (force x) (x))
