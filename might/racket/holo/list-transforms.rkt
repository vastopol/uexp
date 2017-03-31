#lang racket

;; add1
(define (add1 lst)
  (if (null? lst)
      '()
      (cons (+ 1 (car lst))
            (add1 (cdr lst)))))
       
(add1 '(1 2 3))

;; sub1
(define (sub1 lst)
  (if (null? lst)
      '()
      (cons (- (car lst) 1)
            (sub1 (cdr lst)))))
       
(sub1 '(1 2 3))


;; map/test
(define (map/test f lst)
  (if (null? lst)
      '()
      (cons (f (car lst))
            (map/test f (cdr lst)))))

(map/test (λ (x) (+ x 1)) '(1 2 3 4))

;; map/hoist
(define (map/hoist f lst)
  
  (define (m l)
    (if (null? l)
        '()
        (cons (f (car l))
              (m (cdr l)))))
  
  (m lst))


;; map/match
(define (map/match f lst)
  (match lst
    ['()            '()]
    [(cons hd tl)    (cons (f hd) (map/match f tl))]))


;; for/list
(for/list ([x '(1 2 3 4 5)])
  (+ x 1))


;; filter/test
(define (filter/test p? lst)
  (cond
    [(null? lst)      '()]
    [(p? (car lst))    (cons (car lst)
                             (filter/test p? (cdr lst)))]
    [else              (filter/test p? (cdr lst))]))


;; filter/match
(define (filter/match p? lst)
  (match lst
    ['() '()]
    [(cons (? p?) tl)   (cons (car lst) (filter/match p? tl))]
    [(cons hd tl)       (filter/match p? tl)]))

;; for/list
(for/list ([x '(1 2 3 4 5)]
           #:when (odd? x))
  (+ x 1))

;; abstract-map
(define (abstract-map kons nil f lst)
  (if (null? lst)
      nil
      (kons (f (car lst))
            (abstract-map kons nil f (cdr lst)))))


(abstract-map cons '() identity '(1 2 3 4))

(abstract-map + 0 identity '(1 2 3 4))

;; foldr
(define (foldr/test kons nil lst)
  (if (null? lst)
      nil
      (kons (car lst)
            (foldr/test kons nil (cdr lst)))))

(foldr/test cons '() '(1 2 3 4))

(foldr/test + 0 '(1 2 3 4))


;; foldl
(define (foldl/test kons nil lst)
  (if (null? lst)
      nil
      (foldl/test kons (kons (car lst) nil) (cdr lst))))


(foldl/test cons '() '(1 2 3 4))

;; for/fold
(for/fold ([sum 0])
  ([x '(1 2 3 4)])
  (+ x sum)) 
  
(for/fold ([sum 0] [product 1])
  ([x '(1 2 3 4)])
  (values (+ x sum) (* x product)))
  


;; reduce
(define (reduce op lst)
  (match lst
    ['()  (error "no elements in list")]
    [(list a) a]
    [(cons hd tl)
     ; =>
     (op hd (reduce op tl))]))


(reduce + '(1 2 3 4))



;; zip
(define (zip lst1 lst2)
  (match* [lst1 lst2]
    [{'() '()} 
     ; =>
     '()]
    
    [{(cons hd1 tl1) (cons hd2 tl2)}
     ; =>
     (cons (list hd1 hd2) 
           (zip tl1 tl2))]))
      

(zip '(1 2 3 4) '(4 5 6 7))


;; zip/map
(map list '(1 2 3 4) '(4 5 6 7))

;; for zip
(for/list ([x '(1 2 3 4)]
           [y '(4 5 6 7)])
  (list x y))


;; unzip/values
(define (unzip/values lst)
  (match lst
    ['() (values '() '())]
    [(cons (list a b) tl)
     (define-values (as bs) (unzip/values tl))
     (values (cons a as) (cons b bs))]))

(unzip/values '((1 2) (3 4) (5 6)))
 

;; unzip/callback
(define (unzip/callback lst k)
  (match lst
    ['() (k '() '())]
    [(cons (list a b) tl)
     ; =>
     (unzip/callback tl (λ (as bs)
                          (k (cons a as) (cons b bs))))]))


(unzip/callback '((1 2) (3 4) (5 6)) (λ (as bs)
  as)) ; yields '(1 3 5)
                                       
;; partition/values
(define (partition/values p? lst)
  (match lst
    ['() (values '() '())]
    [(cons hd tl)
     (let-values ([{ins outs} (partition/values p? tl)])
       (if (p? hd)
           (values (cons hd ins) outs)
           (values ins (cons hd outs))))]))


(partition/values number? '(1 2 a b c 3 4 d e 5 6))


;; partition/callback
(define (partition/callback p? lst k)
  (match lst
    ['() (k '() '())]
    [(cons hd tl)
     (partition/callback p? tl (λ (ins outs)
       (if (p? hd)
           (k (cons hd ins) outs)
           (k ins (cons hd outs)))))]))

(partition/callback number? '(1 2 a b c 3 4 d e 5 6) list)
