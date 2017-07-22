%
% unparse.fp
%
% Convert internal representation back to FP
%

% exports:
%
%   unparse:unparse : defs
%
% requires: util.fp


unparse = {

  unparse = [~1, id] +
  	    while le.[x, len.y]
	      (unparse_def.select.[x, y];
	       [add1.x, y] )
  unparse_def = dribble.cat.[~"\n", tos.def_name, ~" =\t\t\t% ", tos.def_loc];
	        dribble.unparse1.[~2, y]

  % state = <prec, indent, string>

  up_prec = s1
  up_indent = s2
  up_str = s3
  initial_state = indent.[id, ~<1, "", "  ">]

  % indent : <n, state> -> state'
  
  indent = [up_prec.y, make.[add.[len.up_indent.y, x], ~32], up_str.y]

  % prec : <n, state> -> state'

  prec = [x, up_indent.y, up_str.y]

  % add output
  % pr : <str, state> -> state'
  % prnl : <str, state> -> state'
  % prnl0 : state -> state'

  pr = [up_prec.y, up_indent.y, cat.[up_str.y, x]]
  prnl = pr.[cat.[x, ~"\n", up_indent.y], y]
  prnl0 = pr.[cat.[~"\n", up_indent], id]

  % unparse1 : <indent, node> -> str

  unparse1 = up_str.up.[y, initial_state.x]

  % up : <node, state> -> state'
  % up' : <node, state> -> state'
  % up2 : <node, op, state> -> state'

  up = [|?EQ, #, [|?CONST | ?ICONST, ..|], [|?REF, #, ?`id`|]|].x ->
         up_const.[node_arg1.node_arg1.x, pr.[~"?", y]];
       [|?SELECT, #, [|?ICONST, ..|], [|?REF, #, ?`id`|]|].x ->
         up_const.[node_arg1.node_arg1.x, pr.[~"!", y]];
       parens ->
         pr.[~")", up1'.[x, pr.[~"(", y]]];
	 up1'
  up1' = (with_prec up').[x, node_prec.x, y]
  up' = [|?REF, ..|].x -> pr.[tos.node_arg1.x, y];
        [|?COND, #, #, #, [|?CONST, #, not|]|].x ->
	  up.[node_arg2.x, prec.[~3, pr.[~" & ", up.[node_arg1.x, y]]]];
        [|?COND, ..|].x -> 
	  indent.[~-2, up.[node_arg3.x, 
	                   prnl.[~";", up.[node_arg2.x, 
			                   prnl.[~" ->", indent.[~2, up.[node_arg1.x, y]]]]]]];
        [|?CONST | ?ICONST, ..|].x -> up_const.[node_arg1.x, pr.[~"~", y]];
	[|?COMP, #, [|?REF, #, ?`app`|], [|?CONS, #, #, #|]|].x ->
	  up.[node_arg2.node_arg2.x, 
	      prec.[~5, pr.[~"^", up.[node_arg1.node_arg2.x, prec.[~5, y]]]]];
	[|?COMP, ..|].x -> up2.[x, ~".", y];
	[|?ALPHA, ..|].x -> up.[node_arg1.x, pr.[~"@", y]];
	[|?INSERTR, ..|].x -> up.[node_arg1.x, pr.[~"/", y]];
	[|?INSERTL, ..|].x -> up.[node_arg1.x, pr.[~"\\", y]];
	[|?KATA, ..|].x ->
	  pr.[~"|)", up.[node_arg2.x, pr.[~", ", up.[node_arg1.x, pr.[~"(|", y]]]]];
	[|?WHILE, ..|].x ->
	  indent.[~-2, up.[node_arg2.x, 
	  	           prec.[~3, prnl0.indent.[~2, up.[node_arg1.x, pr.[~"while ", y]]]]]];
	[|?CATCH, ..|].x ->
	  indent.[~-2, up.[node_arg2.x, 
	  	           prec.[~3, indent.[~2, prnl0.up.[node_arg1.x, pr.[~"catch ", y]]]]]];
	[|?SEQ, ..|].x -> up.[node_arg2.x, pr.[~"; ", up.[node_arg1.x, y]]];
	[|?EXTERN, ..|].x -> up_const.[node_arg1.x, pr.[~"extern ", y]];
	[|?UNIT, ..|].x -> up.[node_arg1.x, pr.[~"unit ", y]];
	[|?OR, ..|].x -> up2.[x, ~" | ", y];
	[|?EQ, ..|].x -> up.[[~COMP, #, ~<REF, F, `eq`>, cat.[[~CONS, #], node_args.x]], y];
	[|?SELECT, ..|].x -> up.[[~COMP, #, ~<REF, F, `select`>, cat.[[~CONS, #], node_args.x]], y];
	[|?POINTER1, ..|].x -> up.[node_arg1.x, pr.[~"*", y]];
	[|?POINTER2, ..|].x -> pr.[cat.[~"*", tos.node_arg1.x], y];
	[|?CONS, ..|].x -> pr.[~"]", up_seq.[node_args.x, pr.[~"[", y]]];
	[|?PCONS, ..|].x -> pr.[~"|]", up_seq.[node_args.x, pr.[~"[|", y]]];
	[|?PCONSL, ..|].x -> pr.[~", ..|]", up_seq.[node_args.x, pr.[~"[|", y]]];
        _

  up2 =	up.[node_arg2.x, pr.[y, up.[node_arg1.x, z]]]

  % up_const : <x, state> -> state'

  up_const = atom.x -> pr.[num.x -> tos.x; cat.[~"`", tos.x, ~"`"], y];
  	     zero.len.x -> pr.[~"<>", y];
  	     pr.[~">", s3.up_const'.[~2, x, up_const.[s1.x, pr.[~"<", y]]]]
  up_const' = while le.[x, len.y]
  	        [add1.x, y, up_const.[select.[x, y], pr.[~", ", z]]]

  % up_seq : <nodes, state> -> state'

  up_seq = null.x -> y;
  	   up_seq'.[tl.x, (with_prec up).[s1.x, ~1, y]]
  up_seq' = null.x -> y;
  	    up_seq'.[tl.x, (with_prec up).[s1.x, ~1, pr.[~", ", y]]]
  	      
  % precedence checking
  % parens = <node, state> -> bool

  parens = [node_prec.x, y] +
  	   (x & lt.[x, up_prec.y])

  node_prec = fetch.[node_type, prec_table] | max_prec

  prec_table = ~<<SEQ, 1>,
  	         <COND, 2>,
		 <OR, 3>,
		 <WHILE, 4>,
		 <CATCH, 4>,
		 <COMP, 6> >
		 % "^" (7)
  max_prec = ~7

  % execute with temporarily modified precedence
  %
  % (with_prec FUNC) : <node, prec, state> -> FUNC : <node, state> -> state'

  with_prec = [up_prec.s3.x, s2.x, s1.x, s3.x, y] +  % <old-prec, new-prec, node, state, func>
  	      [x, s5^[z, prec.[y, s4]]] +	     % <old-prec, state'>
	      prec.[x, y]

}
