%
% sqrt.fp
%
% Integer squareroot, simple binary search method
% from "Hacker's Delight"
%


sqrt = negative -> _;
       sub1.s1.sqrt'.[~1, min.[~65535, add.[~8, bshr.[id, ~5]]], id] % <a, b, x>
sqrt' = [x, y, z, bshr.[add.[x, y], ~1]] + % <a, b, x, m>
        (gt.[mul.[s4, s4], z] ->
	  [x, sub1.s4, z];
	  [add1.s4, y, z]) +
	ge.[y, x] -> sqrt'
