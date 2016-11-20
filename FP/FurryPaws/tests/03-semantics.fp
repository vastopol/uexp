%
% test semantics of some core constructs
%


"lib/assert.fp"


% @

t_at = t.[~AT, ~<1, 4, 9>, @(mul.[id, id]).~<1, 2, 3>]; id

% while

t_while = (return (while lt.[id, ~1000000] (once; add1)).~1)
once = zero.mod.[id, ~100000] -> _show

% catch/throw

t_catch = spin.(return ~1)

spin = catch (num.x -> spin;
              eq.[~FINI, x] -> id;
	      _)
       	 throw.(return lt.[id, ~1000000] -> (once; add1); ~FINI)

% and/or

t_and = t.[~AND_F_T, ~F, ~F & ~T];
        t.[~AND_T_T, ~3, ~T & ~3]; 
	id

t_or = t.[~OR_F_X, ~1, ~F | ~1];
       t.[~OR_X, ~3, ~3 | _];
       id

% unit

t_unit = t.[~UNIT, ~<0, 1, 0, 1, 0, 0, 0>,
            [unit add, unit mul, unit sub, unit div, 
	     unit band, unit bor, unit bxor]]; id

% insert

t_insert = t.[~INSERTL, ~<<<1, 2>, 3>, 4>, \id.iota.~4];
	   t.[~INSERTR, ~<1, <2, <3, 4>>>, /id.iota.~4]; id

% catamorphism

t_cata = t.[~CATA0, ~0, (|~0, _|).[]];
         t.[~CATA1, ~1, (|x, y|).~<1>];
	 t.[~CATA2, ~<1, 2, 3>, (|~<>, al|).iota.~3];
	 id

% apply

t_apply = t.[~APPLY, ~42, *add1^~41]; id

% select

t_select = t.[~SELECT, ~X, !2.~<A, X, C>]; id

% run all tests

main = (do t_at t_and t_or t_while t_catch t_unit t_insert t_cata t_apply t_select)
