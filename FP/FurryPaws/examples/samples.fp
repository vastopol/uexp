%
% samples.fp
%
% This is code taken from the FP implementation from Any Valencia, available
% in the CMU AI repository:


% bubsort.fp:

% {swap concat@[ [2,1],tl@tl ]}
% {step (>@[1,2] -> swap ; id) }
% {pass
%     (<@[length,%2] -> id ;
%     apndl@[1,pass@tl]@step)
% }
% {bubsort
%     (<@[length,%2] -> id ;
%     apndr@[bubsort@tlr,last]@pass)
% }

bubsort = {

  swap = cat.[[s2, s1], tl.tl]
  step = gt.[s1, s2] -> swap
  pass = lt.[len, ~2] -> id;
         al.[s1, pass.tl].step
  bubsort = lt.[len, ~2] -> id;
  	    ar.[bubsort.tlr, s1r].pass

}

% bsort.fp:

% #
% # A divide-and-conquer sorting algorithm
% #
% {grpleft
%     concat @ &( > -> tl ; %<>) @ distl }
% {grpright
%     concat @ &( < -> tl ; %<>) @ distl }
% {arb 1}
% {bsort
%     (>@[length %1] ->
%     concat@[bsort@grpleft [1] bsort@grpright]@[arb id]
%     ; id)
% }

bsort = {

  grpleft = cat.@(gt -> tl; []).dl
  grpright = cat.@(lt -> tl; []).dl
  arb = s1
  bsort = gt.[len, ~1] ->
  	    cat.[bsort.grpleft, [s1], bsort.grpright].[arb, id]

}


main = (do show.(return bubsort:bubsort.~<1, 5, 26, 22, 4, 5, 10, 100, -5>)
           show.(return bsort:bsort.~<1, 5, 26, 22, 4, 5, 10, 100, -5>) )
