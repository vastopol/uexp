%
% test prelude functions
%


"lib/assert.fp"


% null

t_null = [t.[~NULL_T, ~T, null.~<>],
          t.[~NULL_F, ~F, null.~<1>]
         ]

% trans

t_trans = [t.[~TRANS_NULL, [], trans.[]],
	   t.[~TRANS, ~<<1, 4>, <2, 5>, <3, 6>>, trans.~<<1, 2, 3>, <4, 5, 6>>]
          ]

% ravel

t_ravel = t.[~RAVEL, ~<1,2,3,4>, ravel.~<<1,2>,<>,<3>,4>]

% replicate, compress, expand

t_replicate = t.[~REPLICATE, ~<A,A,A,B,D,D>, replicate.~<<3,1,0,2>, <A,B,C,D>>]
t_compress = t.[~COMPRESS, 
	        ~<X, Y, Z>, 
	        compress.~<<T, F, F, T, T>, <X, 1, 2, Y, Z>>]
t_expand = t.[~EXPAND, ~<1, 2, X, X, X, 6, X>,
	      expand.[~<T, T, F, F, F, T, F>, ~X, iota.~10]]

% index

t_index = [t.[~INDEX, ~3, index.~<C, <A, B, C>>],
	   t.[~INDEX_NOTFOUND, ~F, index.~<X, <>>]]

% fetch, store, fetchall, purge

t_fetch = [t.[~FETCH, ~X, fetch.~<A, <<U, V>, <A, X>>>],
	   t.[~FETCH_NOTFOUND, ~F, fetch.~<A, <>>]]

t_store = [t.[~STORE_NEW, ~<<X, 1>, <Y, 2>>, store.~<<Y, 2>, <<X, 1>>>],
	   t.[~STORE_OLD, ~<<Y, 2>, <X, 1>>, store.~<<Y, 2>, <<Y, 3>, <X, 1>>>]]

t_fetchall = [t.[~FETCHALL_0, ~<>, fetchall.~<X, <>>],
	      t.[~FETCHALL, ~<A, B>, fetchall.~<X, <<X, A>, <Y, C>, <X, B>>>]]

t_purge = [t.[~PURGE_ALL, ~<>, purge.~<X, <<X, 1>, <X, 2>>>],
	   t.[~PURGE, ~<<Y, 2>>, purge.~<X, <<X, 1>, <Y, 2>, <X, 2>>>]]

% chop

t_chop = t.[~CHOP, ~<<1,2,3>,<4,5,6>,<7, 8>>, chop.[~3, iota.~8]]

% rotl, rotr

t_rotate = t.[~ROTATE_LEFT, ~<2, 3, 1>, rotl.~<1, <1, 2, 3>>];
	   t.[~ROTATE_RIGHT, ~<3, 1, 2>, rotr.~<1, <1, 2, 3>>];
	   t.[~ROTATE_LEFT_ZERO, ~<1, 2, 3>, rotl.~<0, <1, 2, 3>>];
	   t.[~ROTATE_RIGHT_ZERO, ~<1, 2, 3>, rotr.~<0, <1, 2, 3>>]

% delete

t_delete = [t.[~DELETE_1, [], delete.~<X, <X>>],
	    t.[~DELETE_0, [], delete.~<X, <>>],
	    t.[~DELETE_2, ~<Y, Z>, delete.~<X, <X, Y, X, Z>>]]

% count

t_count = [t.[~COUNT_1, ~2, count.~<X, <X, Y, X, Y>>],
	   t.[~COUNT_0, ~0, count.~<X, <>>]]

% union, intersection, diff

t_setops = [t.[~UNION, ~<A, B, C>, union.~<<A, B>, <B, C>>],
	    t.[~INTERSECTION, ~<A, B>, intersection.~<<A, B, C>, <D, A, B, X>>],
	    t.[~DIFF, ~<A, B>, diff.~<<A, B, C, D>, <C, D>>],
	    t.[~DIFF2, ~<A, B>, diff.~<<A, B, C, D>, <C, D, X>>]]

% popcount

t_popcount = t.[~POPCOUNT, ~<<X, 1>, <A, 2>>, popcount.~<A, X, A>]

% and, or

t_andor = [t.[~AND_T_T, ~T, and.~<T, T>],
	   t.[~AND_T_F, ~F, and.~<T, F>],
	   t.[~AND_F_T, ~F, and.~<F, T>],
	   t.[~AND_F_F, ~F, and.~<F, F>],
	   t.[~OR_T_T, ~T, or.~<T, T>],
	   t.[~OR_T_F, ~T, or.~<T, F>],
	   t.[~OR_F_T, ~T, or.~<F, T>],
	   t.[~OR_F_F, ~F, or.~<F, F>]]

% take, drop

t_take = t.[~TAKE1, ~<1, 2>, take.~<2, <1, 2, 3, 4>>];
         t.[~TAKE0, ~<>, take.~<0, <1, 2>>]
t_drop = t.[~DROP1, ~<3, 4>, drop.~<2, <1, 2, 3, 4>>];
         t.[~DROP0, ~<1, 2>, drop.~<0, <1, 2>>]

% splice, copy

t_splice = t.[~SPLICE, ~<A, X, Y, C, D>, splice.~<2, <X, Y>, <A, B, C, D>>]
t_copy = t.[~COPY, ~<A, X, Y, D>, copy.~<2, <X, Y>, <A, B, C, D>>]

% match

t_match = t.[~MATCH1, ~T, match.~<2, <A, B, C>, <X, A, B, C, D>>];
	  t.[~MATCH2, ~F, match.~<1, <X>, <Y, Z>>]

% run all tests

main = [t_null; t_take; t_drop; 
        t_trans; t_ravel; 
        t_replicate; t_compress; t_expand; t_index; t_fetch; t_purge;
	t_store; t_fetchall; t_chop; t_rotate; t_delete; t_count; t_setops;
	t_popcount; t_andor; t_splice; t_copy; t_match, y]
