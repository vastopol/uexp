% check.fp - check definitions
%
% usage:
%
%   check:check : <def*, ...> -> bool


check = {

  check = any.[multiple_defs, intrinsic_redef, undefined_refs]

  intrinsics = ~<`add`, `sub`, `mul`, `div`, `mod`, `band`, `bor`,
  	         `bxor`, `bnot`, `bshl`, `bshr`, `id`, `eq`, `select`,
  	         `_in`, `_out`, `_get`, `_emit`, `_show`, `_system`, `al`,
  	         `ar`, `tl`, `cat`, `len`, `num`, `rev`, `subseq`,
  	         `atom`, `tos`, `toa`, `ton`, `_throw`, `%trace`, `_`,
  	         `_rnd`, `_env`, `cmp`, `string`, `_gc`, `app`,
  	         `_iostep`, `make`, `iota`, `dl`, `dr`>

  multiple_defs = any.@nonunique.spread
  nonunique = [x, fetchall.[s1.x, y]] +
  	      gt.[len.y, ~1] ->
	        (dribble.cat.[tos.s2.s2.x, ~": error: multiple definitions of `",
			      tos.s1.x, ~"'"];
		 ~T); ~F

  intrinsic_redef = any.@intrinsic_redef'
  intrinsic_redef' = index.[s1, intrinsics] ->
  		       (dribble.cat.[tos.s2.s2, 
		       		     ~": error: redefinition of intrinsic `",
				     tos.s1, ~"'"];
			~T); ~F

  undefined_refs = any.@(any.@undefined_ref.dr).dr.[@s4, @s1]

  % undefined_ref : <refnode, <id, ...>> -> bool
  undefined_ref = index.[s3.x, y] -> ~F;
  		  index.[s3.x, intrinsics] -> ~F;
  		  (dribble.cat.[tos.s2.x, ~": error: reference to undefined identifier `",
		  	        tos.s3.x, ~"'"];
		   ~T)

}
