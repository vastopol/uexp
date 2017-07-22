%
% 06-mss.fp - MSS example from Gibbons
%

main = show.[mss.randomseq.slen.x, y]

slen = null -> ~20; ton.s1

maximum = (|minfinity, max|)
inits = (|~<<>>, al.[~<>, @al.dl]|)
tails = (|~<<>>, al.[al.[x, s1.y], y]|)
segs = cat.@inits.tails
sum = (|~0, add|)
mss = maximum.@sum.segs

randomseq = @(sub.[_rnd, ~100]).make.[id, ~200]
