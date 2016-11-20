%
% John Backus' Matrix Multiply example
%
% expected result: <<46, 37, 28, 19>, <115, 94, 73, 52>>
%
% Note: prints as:
%
% <<46, 37, 28, 19>,
%  "s^I4">
%
% (use "-:s" run-time option to force output as numeric sequence)

mm = @@ip.@dl.dr.[s1, trans.s2]
ip = /add.@mul.trans

check = not.eq.[id, result] -> _

result = ~<<46, 37, 28, 19>, <115, 94, 73, 52>>

a = ~<<1, 2, 3>, <4, 5, 6>>
b = ~<<9, 8, 7, 6>,
      <5, 4, 3, 2>,
      <9, 7, 5, 3>>

make_m = chop.[y, @_rnd.make.[mul, ~1000]]

main = show.[run.x, y]

run = check.mm.[a, b];
      not.zero.len -> 
        mm.@make_m.chop.[~2, @ton];
	~OK
