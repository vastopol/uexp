%
% test intrinsics
%


"lib/assert.fp"


% cat

t_cat = [t.[~CAT_NULL, [], cat.[]],
         t.[~CAT, ~<1, 2, 3, 4>, cat.~<<1, 2>, <3, 4>, <>>]
        ]

% tos, ton, toa

t_tox = t.[~TOX, ~<"XY", "XYZ", "123", 
      		   123, 123, 123,
		   OK, `123`, YES>,
           [tos.~<$X, $Y>, tos.~XYZ, tos.~123,
	    ton.~123, ton.~`123`, ton.~"123",
	    toa.~OK, toa.~123, toa.~"YES"]
  	  ]

% ar, al

t_ar = t.[~AR1, ~<1, 2, 3, 4>, ar.[~<1, 2, 3>, ~4]]
t_al = t.[~AL1, ~<1, 2, 3, 4>, al.[~1, ~<2, 3, 4>]]

% dl, dr

t_dl = [t.[~DL_NULL, [], dl.[~X, []]],
        t.[~DL, ~<<X, 1>, <X, 2>, <X, 3>>, dl.[~X, ~<1, 2, 3>]]
       ]

t_dr = [t.[~DR_NULL, [], dr.[[], ~X]],
        t.[~DR, ~<<1, X>, <2, X>, <3, X>>, dr.[~<1, 2, 3>, ~X]]
       ]

% tl

t_tl = t.[~TL, ~<2, 3>, tl.~<1,2,3>]

% iota

t_iota = [t.[~IOTA_NULL, [], iota.~0],
          t.[~IOTA, ~<1, 2, 3>, iota.~3]
	 ]

% make

t_make = t.[~MAKE, ~<X,X,X,X>, make.~<4, X>];
         t.[~MAKE, ~<>, make.~<0, X>]

% rev

t_rev = [t.[~REV_NULL, [], rev.[]],
         t.[~REV, ~<3, 2, 1>, rev.~<1, 2, 3>]
        ]

% comparison

t_cmp = t.[~CMP, ~<F, T, T, F, T, T, T>,
           [gt.~<1, A>,
            lt.~<T, <>>,
	    gt.~<"abc", "ABC">,
	    lt.~<"abc", "ABC">,
	    ge.~<"abc", "abc">,
	    ge.~<"abc", "abcd">,
	    ge.~<"abcd", "abc">] ]

% subseq

t_subseq = t.[~SUBSEQ, ~<3, 4>, subseq.~<2, 3, <2,3,4,5>>];
	   t.[~SUBSEQ_MIN1, ~<>, subseq.~<-5, 0, <A, B>>];
	   t.[~SUBSEQ_MIN2, ~<A>, subseq.~<0, 1, <A, B>>];
	   t.[~SUBSEQ_MAX, ~<>, subseq.~<5, 5, <A, B>>];
	   t.[~SUBSEQ_HEAD, ~<1, 2>, subseq.~<0, 2, <1, 2, 3, 4>>];
	   t.[~SUBSEQ_TAIL, ~<3, 4>, subseq.~<3, 5, <1, 2, 3, 4>>];
	   t.[~SUBSEQ_1, ~<X>, subseq.~<1, 1, <X>>]

% app

t_app = t.[~APP, ~42, app.[*add1, ~41]]

% run tests

main = t_ar; t_al; t_dl; t_dr; t_tl; t_make; t_iota; 
       t_cat; t_tox; t_rev; t_cmp; t_subseq; t_app; id
