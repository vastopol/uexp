%
% c-literals.fp - Literal-frame extraction and indexing for C
%

% Extracts non-immediate literals and converts CONST nodes
% to contain index into literal frame.

% usage:
%
%   literals:extract : defs* -> <lit, ...>
%   literals:index_literals : <defs*, lits> -> defs*'
%   literals:generate_frame : <lit, ...> -> string
%   literals:immediate : lit -> bool
%
% requires:
%
%   util.fp


literals = {

  extract = nodups.cat.@(extract1.def_node)
  extract1 = specialnode -> [];
  	     eq.[~CONST, node_type] -> [node_arg1];
	     cat.@extract1.node_args

  index_literals = null.y -> x;
  		   (trans.[x, walklist.[@def_node.x, y]] +
                    @def_replace_node)

  % walk : <node, lits> -> node'
  walk = specialnode.x -> x;
         eq.[node_type.x, ~CONST] ->
	   [~CONST, node_loc.x, sub1.index.[node_arg1.x, y]];
	 ([x, walklist.[node_args.x, y]] +
	  cat.[[node_type, node_loc].x, y] )

  % walklist : <<node1, ...>, lits> -> <node1', ...>
  walklist = @walk.dr

  construct = num -> cat.[~"TO_N(", tos, ~")"];
  	      atom -> cat.[~"intern(", ~<$">, escape, ~<$">, ~")"];
	      cat.[~"sequence(", tos.len, cat.@(al.[~$,, construct]), ~")"]

  generate_frame = cat.[nl, ~"static X lf[", tos.len, ~"];", nl, 
  		        ~"static void gen_lf(){",
			cat.@generate1.trans.[iota.len, id],
			~"}"]
  generate1 = cat.[nl, ~"lf[", tos.sub1.x, ~"]=", construct.y, ~";"]

  % helper functions

  specialnode = index.[s1, ~<REF, POINTER2, EXTERN, ICONST>]
  escape = cat.@(index.[id, ~<$", $\>] -> [~$\, id]; [id]).tos
  immediate = null | num | index.[id, ~<T, F>]
  def_replace_node = put.[~2, y, x]

}
