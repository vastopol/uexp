%
% prelude.fp - library functions available by default
%


% Base library
% ------------


% some convenience definitions (quasi-Arguments)

x = s1
y = s2
z = s3

% zero : X -> B
% add1 : N1 -> N2
% sub1 : N1 -> N2
% not : B1 -> B2
% neg : N1 -> N2

zero = eq.[~0, id]
add1 = add.[id, ~1]
sub1 = sub.[id, ~1]
not = id -> ~F; ~T
neg = sub.[~0, id]

% gt : <X1, X2> -> B
% lt : <X1, X2> -> B
% ge : <X1, X2> -> B
% le : <X1, X2> -> B

gt = eq.[~1, cmp]
lt = eq.[~-1, cmp]
ge = not.lt
le = not.gt

% negative : N -> B
% positive : N -> B
% even : N -> B
% odd : N -> B

negative = lt.[id, ~0]
positive = gt.[id, ~0]
even = zero.band.[~1, id]
odd = not.even

% null : X -> B
% atomic : X -> B

null = ?<>
atomic = null | atom

% trans : <<X1, ...>, <Y1, ...>, ...> -> <<X1, Y1, ...>, ...>

trans = null -> id; trans'
trans' = null.s1 -> []; al.[@s1, trans'.@tl]

% max : <N1, ...> -> Nmax
% min : <N1, ...> -> Nmin
% abs : N1 -> N2
% sign : N -> 1 | 0 | -1

max = /(gt -> s1; s2)
min = /(lt -> s1; s2)
abs = negative -> neg; id
sign = negative -> ~-1; positive -> ~1; ~0

% take : <N, <X1, ...>> -> <X1, ..., Xn>
% drop : <N, <X1, ...>> -> <Xn+1, ...>

take = subseq.[~1, x, y]
drop = subseq.[add1.x, len.y, y]

% tlr : <X1, ..., Xn> -> <X1, ...>

tlr = take.[sub1.len, id]

% ravel : X -> S

ravel = atom -> [id]; cat.@ravel

% replicate : <<N1, ...>, <X1, ...>> -> <X1, ..., Xn1, ...>
% compress : <<B1, ...>, <X1, ...>> -> S
%   this is like `repl', treating true as 1 and false as 0
% expand : <<B1, ...>, X, <X1, ...>>
%   replaces Xi with X where Bi is false. If the first argument is 
%   shorter than the third, the latter is cut off early
% partition : <<B1, ...>, <X1, ...>> -> <<Xa1, ...>, <Xb1, ...>>
%   returns elements for which Bi is T and F, respectively

replicate = cat.@make.trans
compress = replicate.[@(id -> ~1; ~0).s1, s2]
expand = null.s1 -> [];
         al.[s1.s1 -> s1.s3; s2, expand.[tl.s1, s2, tl.s3]]
partition = [compress, compress.[@not.x, y]]

% index : <X, S> -> F % N
% indexr: <X, S> -> F % N

index = index'.[s1, s2, ~1]
index' = gt.[z, len.y] -> ~F;
         eq.[x, select.[z, y]] -> z;
	 index'.[x, y, add1.z]
indexr = indexr'.[s1, s2, len.s2]
indexr' = zero.z -> ~F;
          eq.[x, select.[z, y]] -> z;
	  indexr'.[x, y, sub1.z]

% assoc : <X, <<X1, Y1>, ...>> -> F | <Xi, Yi>
% fetch : <X, <<X1, Y1>, ...>> -> F | Yx
% store : <<X, Y>, <<X1, Y1>, ...>> (add and remove old)
% fetchall : <X, <X1, Y1>, ...> -> <Ya, ...>
% purge : <X, <<X1, Y1>, ...>> -> (remove all items with matching head)

assoc = [x, y, ~1] +
         while not.(gt.[z, len.y] | eq.[x, s1.select.[z, y]])
	   [x, y, add1.z] +
         le.[z, len.y] & select.[z, y]
% alternative implementation:
%
%   assoc = null.s2 -> ~F;
%           eq.[s1, s1.s1.s2] -> s1.s2;
%	    assoc.[s1, tl.s2]

fetch = assoc + id -> s2
store = null.s2 -> [s1];
        eq.[s1.s1, s1.s1.s2] -> al.[s1, tl.s2];
	al.[s1.s2, store.[s1, tl.s2]]
fetchall = trans.@[eq.[x, s1.y], s2.y].dl + null -> id; compress
purge = compress.[@(not.eq.[x, s1.y]).dl, y]

% chop : <N, <X1, ...>> -> <<X1, ..., XN>, ...>
% pair : <X1, ...> -> <<X1, X2>, <X3, X4>, ...>

chop = null.s2 -> [];
       al.[take, chop.[s1, drop]]
pair = odd.len -> _; chop.[~2, id]

% rotl : <N, S>
% rotr : <N, S>
% flip: <X, Y> -> <Y, X>

rotl = cat.[drop, take]
rotr = cat.[drop, take].[sub.[len.y, x], y]
flip = [y, x]

% delete : <X, S> -> S'

delete = compress.[@(not.eq).dl, y]

% put : <N, X, S> -> S'

put = cat.[take.[sub1.x, z], [y], drop.[x, z]]

% I/O combinators
%
% (io desc f) : <x, io> -> <f:x, io'>  (desc : x -> atom)
% (return f) : <x, io> -> <f:x, io>
% (do f1 ...) : <x, io> -> <rN, io'>
%
%XXX we need something to cover the "f : <x, state> -> state'" idiom

io = [app.[z, s1.x], _iostep.[s2.x, app.[y, s1.x]]]
return = [app.[y, s1.x], s2.x]
do = \(app.[s1.y, [s2.y, s2.x]]).al.[x, dr.[tl, s1.x]]

% I/O Operations
%
% exit : <n, io>
% fail : <message, io>
% emit : <x, io> -> <x, io'>
% get : <x, io> -> <y, io'>
% in : <str, io> -> <str', io'>
% out : <str, s, io> -> <s, io'>
% system : <str, io> -> <n, io'>
% throw : <x, io>
% env : <a, io> -> <x, io'>
% show : <x, io> -> <x, io'>
% rnd : <n, io> -> <n', io'>
% gc : <x, io> -> <x, io'>

exit = _iostep.[y, ~`exit`]; _throw.[~EXIT, x]
fail = _iostep.[y, ~`fail`]; _throw.[~ERROR, x]
emit = (io ~`emit` _emit)
get = (io ~`get` _get)
out = (io ~`out` _out)
in = (io ~`in` _in)
system = (io ~`system` _system)
throw = _throw.[x, _iostep.[y, ~`throw`]]
env = (io ~`env` _env)
show = (io ~`show` _show)
rnd = (io ~`rnd` _rnd)
gc = (io ~`gc` _gc)

% selectors
%
%  s1 : S -> X
%  s2 : S -> X
%  s3 : S -> X
%  s4 : S -> X
%  s1r : S -> X
%  s2r : S -> X
%  s3r : S -> X
%  s4r : S -> X
%  selectr : <N, S> -> X

s1 = select.[~1, id]
s2 = select.[~2, id]
s3 = select.[~3, id]
s4 = select.[~4, id]
s1r = select.[len, id]
s2r = select.[sub1.len, id]
s3r = select.[sub.[len, ~2], id]
s4r = select.[sub.[len, ~3], id]
selectr = select.[sub.[len.y, x], y]

% join : <X, S> -> S'   (elements of S interspersed with X)
% split : <X, S> -> S'  (returns list of subsequences in S separated by X)

join = tl.cat.dl
split = [id, index] +
        (y -> 
	   al.[take.[sub1.y, s2.x], split.[s1.x, drop.[y, s2.x]]];
	   [s2.x])

% count : <X, S> -> N

count = null.y -> ~0; /add.@(eq -> ~1; ~0).dl

% set operations
%
%  nodups : S -> S'
%  union : <S1, ...> -> S
%  intersection : <S1, S2> -> S
%  diff: <S1, S2> -> S

nodups = null -> [];
         index.[s1, tl] -> nodups.tl;
	 al.[s1, nodups.tl]
union = nodups.cat
intersection = compress.[@index.dr, x]
diff = compress.[@(not.index).dr, x]

% popcount : S -> <<X1, N1>, ...>        (count population)

popcount = @[x, count].dr.[nodups, id]

%  and : <B1, B2> -> B
%  or : <B1, B2> -> B

and = x -> (y -> ~T; ~F); ~F
or = x -> ~T; y -> ~T; ~F 

% spread : S -> <<X1, S>, ...>

spread = dr.[id, id]

% any : <bool, ...> -> bool'
% all : <bool, ...> -> bool

any = null -> ~F; /or
all = null -> ~T; /and

% splice : <N, S1, S2> -> <S2/1, .., S1/1, ..., S2/N, ...>
% copy : <N, S1, S2> -> <S2/1, ..., S1/1, ..., S2/N-len(S1), ...>

splice = cat.[take.[sub1.x, z], y, drop.[x, z]]
copy = cat.[take.[sub1.x, z], y, drop.[add1.sub.[len.z, x], z]]

% match : <N, S1, S2> -> bool   (returns T if S1 is part of S2, starting at N)

match = ge.[sub.[len.z, x], len.y] & eq.[y, subseq.[x, sub1.add.[x, len.y], z]]

% infinity : _ -> n
% minfinity : _ -> n

infinity = ~1073741823
minfinity = ~-1073741824

% _start : <arg1, ...>

_start = main.[id, ~<IO, 0>] + exit.[~0, y]
