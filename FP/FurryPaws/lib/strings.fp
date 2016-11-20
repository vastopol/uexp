%
% strings.fp
%
% String utilities
%


str = {

  % trim : str -> str'
  % trimright : str -> str'
  % trimleft : str -> str'
  trim = trimright.trimleft
  trimright = while (not.null & whitespace.s1r) tlr
  trimleft = while (not.null & whitespace.s1) tl

  whitespace = index.[id, ~<9, 10, 13, 32>]
  nl = ~<10>
  crlf = ~<13, 10>

  % lines : str -> <line1, ...>
  lines = @(delete.[~13, id]).split.[~10, id]

  % ljustify : <len, str> -> str'
  % rjustify : <len, str> -> str'
  ljustify = cat.[make.[max.[~0, sub.[len.y, x]], ~$ ], y]
  rjustify = cat.[y, make.[max.[~0, sub.[len.y, x]], ~$ ]]

}
