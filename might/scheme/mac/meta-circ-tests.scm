(load "meta-circ.scm")


;; Testing and debugging.

(define-syntax check-expect 
  (syntax-rules ()
    ((_ check expect)
       (let ((checked check)
             (expected expect))
         (if (not (equal? checked expect))
             (begin 
               (display "expression: ")
               (write (quote check))
               (newline)
               (display "received:   ")
               (write checked)
               (newline)
               (display "expected:   ")
               (write expected)
               (newline))
             (if #f #t))))))


(define-syntax check-expect-eval
  (syntax-rules ()
    ((_ check-exp expected)
     ; =>
     (begin
       (set! num-test-cases (+ 1 num-test-cases))
       (check-expect (eval (quote check-exp) (initial-environment))
                     expected)))))





                                   
;; Supplied test cases.

(check-expect-eval
 (cond (#t 10))
 10)


(check-expect-eval
 (begin
   (define x 3)
   x)
 3)


(check-expect-eval
 (begin
   (define (f x) x)
   (f 100))
 100)


(check-expect-eval
 (apply (lambda (x) x) 
        (cons 5 '()))
 5)


(check-expect-eval
 (let ((identity-macro (macro (lambda (x) x))))
   (identity-macro 120))
 120)


(check-expect-eval
 (let ((f (macro (lambda (x)
                   (cond 
                     ((eq? x 'foo) 100)
                     ((eq? x 'bar) 200))))))
   (+ (f foo) (f bar)))
 300)



(check-expect-eval
 (let ((f (macro (lambda (x)
                   (cond 
                     ((eq? x 'foo) 100)
                     ((eq? x 'bar) 200)))))
       (g (lambda (h)
            (+ (h foo) (h bar)))))
   (g f)) 
 300)



(check-expect-eval
 (let ((thunk (macro (lambda (e)
                       (list 'lambda '() e)))))
   (let ((force (lambda (thunk) (thunk))))
     (let ((x     50))
       (let ((delayed-value (thunk (begin
                                (set! x 20)
                                x))))
         (let ((y     x))
           (let ((z     (force delayed-value)))
             (+ y z)))))))
 70)


(check-expect-eval
 (let ((f (macro (lambda ()
                   (list '+ 3 4)))))
   (f))
 7)


(check-expect-eval 
 (let ((f (macro (lambda ()
                   (list '+ 3 4)))))
   (let ((+ (lambda (a b) (+ a a b b))))
     (f)))
 14)

(check-expect-eval
 (let ((f (macro (lambda ()
                   (+ 3 4)))))
   (f))
 7)

(check-expect-eval
 (let ((f (macro (lambda ()
                   (+ 3 4)))))
   (let ((+ (lambda (a b) (+ a a b b))))
     (f)))
 7)
              
(check-expect-eval
 (let ((f (macro (lambda (arg)
                   arg))))
   (f (list 3 4 5)))
 '(3 4 5))


(check-expect-eval 
 (let ((f (macro (lambda (x y z)
                   (list 
                    'list
                    (list 'quote x)
                    (list 'quote y)
                    (list 'quote (car z)))))))
   (f not an (expression)))
'(not an expression))
                    

;; Hidden hygiene tests

(check-expect-eval
 (let ((lambda 3))
   (let ((x lambda))
     (+ x x)))
 6)


(check-expect-eval
 (let ((let 10))
   (letrec ((f (lambda (n)
                 (+ n let))))
     (f 15)))
 25)


(check-expect-eval
 (let ((set! 20))
   (letrec ((f (lambda (n)
                 (+ n set!))))
     (f 25)))
 45)


(check-expect-eval
 (let ((if 300))
   (cond 
     (#f   (error))
     (else (+ if if))))
 600)


(check-expect-eval
 (let ((if #f))
   (and if if))
 #f)


(check-expect-eval
 (let ((t #f)
       (tmp #f)
       ($t #f)
       ($tmp #f))
   (and 1 2 3))
 3)


;; Scoping tests.


(check-expect-eval
 (begin
   (define f (lambda () x))
   (define x 10)
   (f))
 10)


(check-expect-eval
 (letrec ((f (lambda (n) (g 10)))
          (j (lambda (n) n))
          (h (lambda (n) (j 16) n))
          (g (lambda (n) (h 20))))
   (f 30))
 20)
   

(check-expect-eval
 (let ((x 3))
   (let ((f (lambda () x)))
     (let ((x 10))
       (f))))
 3)



(check-expect-eval
 (let ((x 3))
   (let ((f (lambda () x)))
     (let ((x 10))
       (set! x 20)
       (f))))
 3)



(check-expect-eval
 (let ((x 3))
   (let ((f (lambda () (set! x 200))))
     (let ((x 10))
       (begin
         (f)
         x))))
 10)



;; Prim-op tests.

(check-expect-eval
 (apply (lambda x x) (list 'a 'b 'c))
 '(a b c))


(check-expect-eval
 (apply (lambda (x y z) (list x y z)) (list 1 2 3))
 '(1 2 3))


(check-expect-eval 
 (+ 10 20 30 40 50)
 150)


(check-expect-eval 
 (not (not 10))
 #t)


(check-expect-eval
 (begin (set! + not)
        (+ #f))
 #t)




;; Small programs.

(check-expect-eval
 (begin
   (define (flactorial n)
     (if (= n 0)
         1
         (+ n (flactorial (+ n -1)))))
   (flactorial 5))
 16)


(check-expect-eval
 ((((lambda (h) (lambda (F) (F (lambda (x) (((h h) F) x)))))
    (lambda (h) (lambda (F) (F (lambda (x) (((h h) F) x))))))
   (lambda (f)
     (lambda (n)
       (if (= n 0)
           1
           (+ n (f (+ n -1)))))))
  5)
 16)



;; Macros

(check-expect-eval
 (let ((my-or (macro (lambda exps 
                       (cond
                         ((null? exps) #f)
                         (else         (list 'or (car exps)
                                             (cons 'my-or (cdr exps)))))))))
   (my-or #f #f #f 10))
 10)





 