% c-backend.cp - C-generation from node tree


% Tree format:
%
% <REF, loc, atom>
% <COND, loc, x, y, z>
% <COMP, loc, x, y>            evaluates y before x
% <CONST, loc, lfindex>
% <ICONST, loc, constant>
% <ALPHA, loc, x>
% <INSERTR, loc, x>
% <INSERTL, loc, x>
% <KATA, loc, x, y>            
% <CONS, loc, x1, ...>
% <WHILE, loc, x1, x2>
% <CATCH, loc, x1, x2>
% <PCONS, loc, x1, ...>
% <PCONSL, loc, x1, ...>
% <SEQ, loc, x1, x2>
% <EXTERN, loc, constant>
% <OR, loc, x1, x2>
% <UNIT, loc, x>
% <EQ, loc, x1, x2>
% <SELECT, loc, x1, x2>
% <POINTER2, loc, atom>


% usage:
%
%   backend:translate : <<def1, ...>, flags, <lit1, ...>> -> codestring
%
%     def = <name, tree>
%     flags = <<flag1, val1>, ...>
%
% requires:
%
%  (toplevel): dribble, quit
%
%  c-literals: generate_frame


backend = {

  % translate definitions to C, returning string with generated code

  translate = (fetch.[~DEBUG, y] -> dribble.~"translating");
  	      cat.[header, literals:generate_frame.z, 
  	           cat.@prototype.x, cat.@t_def.x, footer]

  % generate prototypes for all defined functions
  %
  % prototype : def1 -> string

  prototype = cat.[nl, ~"DEFINE(", mangle.s1, ~");"]

  % translate a single definition
  %
  % t_def : def -> string

  t_def = cat.[def_header, t_def'.t_expr.[s2, newstate.s1]]
  t_def' = cat.[cat.code, nl, ~"RETURN(", arg, ~");}"]

  % expression translators
  %
  %   translator : <tree, state> -> state'
  %
  %   state = <arg, counter, current, code>
  %   current = F | current-function-name

  t_expr = eq.[~COMP, nodetype.x] -> t_comp;
  	   eq.[~COND, nodetype.x] -> t_cond;
	   eq.[~CONS, nodetype.x] -> t_cons;
	   eq.[~PCONS, nodetype.x] -> t_pcons;
	   eq.[~PCONSL, nodetype.x] -> t_pcons;
	   eq.[~CONST, nodetype.x] -> t_const;
	   eq.[~ICONST, nodetype.x] -> t_iconst;
	   eq.[~ALPHA, nodetype.x] -> t_alpha; 
	   eq.[~INSERTR, nodetype.x] -> t_insertr; 
	   eq.[~INSERTL, nodetype.x] -> t_insertl; 
	   eq.[~KATA, nodetype.x] -> t_kata; 
	   eq.[~REF, nodetype.x] -> t_ref;
	   eq.[~POINTER2, nodetype.x] -> t_pointer;
	   eq.[~SEQ, nodetype.x] -> t_seq;
	   eq.[~OR, nodetype.x] -> t_or;
	   eq.[~WHILE, nodetype.x] -> t_while;
	   eq.[~CATCH, nodetype.x] -> t_catch;
	   eq.[~EXTERN, nodetype.x] -> t_extern;
	   eq.[~UNIT, nodetype.x] -> t_unit;
	   eq.[~EQ, nodetype.x] -> t_eq;
	   eq.[~SELECT, nodetype.x] -> t_select;
	   _

  t_comp = [|?COMP, #, opref, [|?CONS, #, #, #|]|].x ->
  	     t_op;
  	     ([x, current.y, t_expr.[subnode2.x, nontail.y]] +
  	      t_expr.[subnode1.x, tail.[y, z]])

  t_cond = [x, gensym.y] +	% generate temporary
  	   [x, s1.y, s2.y,	% add save code and do test
  	    t_expr.[subnode1.x, 
		    nontail.conc.[[nl, ~"save(", arg.s2.y, ~"); // COND"], s2.y]]] +
	   [x, y, z,	% add restore + test code and do consequent
  	    t_expr.[subnode2.x,
		    concx.[[nl, ~"X ", y, ~";", nl, arg.z, ~"=restore();", 
			    nl, ~"if(", arg.s4, ~"!=F){"], arg.z, 
			   tail.[current.z, s4]]]] +
	   [x, y, 		% add else code and do alternative
            t_expr.[subnode3.x, 
  	       	    concx.[[nl, y, ~"=", arg.s4, ~";", nl, ~"}else{"], arg.z, 
		    tail.[current.z, s4]]]] +
           concx.[[nl, y, ~"=", arg.z, ~";}"], y, z]

  t_seq = [x, y, 
           t_expr.[subnode1.x, nontail.conc.[[nl, ~"save(", arg.y, ~"); // SEQ"], y]],
  	   current.y] +
	  [x, y, conc.[[nl, arg.y, ~"=restore();"], z], s4] +
   	  t_expr.[subnode2.x, al.[arg.y, tl.tail.[s4, z]]]

  t_or = [x, arg.y, current.y, gensym.nontail.y] +
         [x, y, z, s1.s4,	% node, orig. arg, current, gensym, state
	  t_expr.[subnode1.x, 
	          conc.[[nl, ~"save(", y, ~"); // OR"], s2.s4] ] ] +
         [z, s4,
	  t_expr.[subnode2.x, 
	          tail.[z, concx.[[nl, ~"X ", s4, ~"=", arg.s5, ~";", 
		  	   	   nl, y, ~"=restore();",
		     	           nl, ~"if(", s4, ~"==F){"], y, s5]]]] +
	 concx.[[nl, y, ~"=", arg.z, ~";}"], y, z]

  t_const = (al.[x, gensym.y]) +
  	    concx.[[nl, ~"X ", y, ~"=", literal.subnode1.x, ~"; // CONST"], y, z]

  t_iconst = (al.[x, gensym.y]) +
  	     concx.[[nl, ~"X ", y, ~"=", iliteral.subnode1.x, ~"; // ICONST"], y, z]

  t_cons = (al.[tl.tl.x, gensym.nontail.y]) +
  	   [x, y, 
	    conc.[[nl, ~"save(", arg.z, ~");", nl, ~"X ", y, ~"=allocate(",
	           tos.len.x, ~"); // CONS", nl, ~"save(", y, ~");"],
		  z]] +
	   [y, t_cons'.[x, y, iota.len.x, z]] +
	   concx.[[nl, x, ~"=restore();", nl, ~"restore();"], x, y]
  %XXX special case "id" here
  t_cons' = null.x -> s4;	% end reached? return state
  	      ([x, y, z, t_expr.[s1.x, conc.[[nl, arg.s4, ~"=pick(2);"], s4]]] +
	      t_cons'.[tl.x, y, tl.z, 
	    	     conc.[[nl, y, ~"=top();", nl, ~"S_DATA(", y, ~")[", 
		            tos.sub1.s1.z, ~"]=", arg.s4, ~";"],
			   s4]])

  t_pcons = al.[x, gensyms.[~2, nontail.y]] + % <node, <tmp1, tmp2>, state>
  	    [node_args.x, y, 
	     conc.[[nl, ~"X ", s1.y, ~"=F;", nl, ~"save(", arg.z, ~"); // ",
	     	    tos.node_type.x, nl,
	            ~"if(!IS_S(", arg.z, ~")||S_LENGTH(", arg.z, ~")",
		    eq.[~PCONS, node_type.x] -> ~"!="; ~"<",
		    tos.len.node_args.x, ~") goto ", s2.y, ~";"], z]] +
            [y, t_pcons'.[x, y, iota.len.x, z]] +
	    concx.[[nl, s1.x, ~"=T;", nl, s2.x, ~":", nl, ~"restore();"], 
	           s1.x, y]
  t_pcons' = null.x -> s4;		% end reached? return true
  	     [|?CONST | ?ICONST, #, id|].s1.x ->
	       t_pcons'.[tl.x, y, tl.z, conc.[[nl, ~"// skipped"], s4]];
	     ([x, y, tl.z, 
	       t_expr.[s1.x, 
		       conc.[[nl, arg.s4,~"=S_DATA(top())[", tos.sub1.s1.z, ~"];"], 
			     s4]]] +
	      t_pcons'.[tl.x, y, z, 
	 	        conc.[[nl, ~"if(", arg.s4, ~"==F) goto ", s2.y, ~";"], s4]])

  t_alpha = (al.[x, gensyms.[~3, y]]) +	% generate gensyms t, i, len
	    [x, y,  	 	% emit prelude
	     concx.[[nl, ~"int ", s2.y, ~"; // ALPHA", nl, ~"check_S(", arg.z, ~",", ~<$">,
	             ~"@", ~<$">, ~");", nl, ~"int ", s3.y, ~"=S_LENGTH(",
		     arg.z, ~");", nl, ~"save(", arg.z, ~");", nl,
		     ~"X ", s1.y, ~"=allocate(", s3.y, ~");", nl, ~"save(",
		     s1.y, ~");", nl, ~"for(", s2.y, ~"=0;", s2.y, ~"<", s3.y, 
		     ~";++", s2.y, ~"){", nl, ~"X ", s1.y, ~"=S_DATA(pick(2))[", 
		     s2.y, ~"];"],
		    s1.y, z]] +
	    [y, t_expr.[subnode1.x, nontail.z]] +
	    concx.[[nl, ~"S_DATA(top())[", s2.x, ~"]=", arg.y, ~";}", nl, s1.x,
	            ~"=restore();", nl, ~"restore();"],
		   s1.x, y]

  t_insertr = (al.[x, gensyms.[~4, y]]) + % generate gensyms t1, t2, i, len
  	      [x, y,
	       concx.[[nl, ~"int ", s3.y, ~"; // INSERTR", nl, ~"check_S(", arg.z, ~",", 
	               ~<$">, ~"/", ~<$">, ~");", nl, ~"int ", s4.y, 
		       ~"=S_LENGTH(", arg.z, ~");", nl, ~"X ", s1.y, ~"=",
		       s4.y, ~"==0?", unit_value.subnode1.x, ~":S_DATA(",
		       arg.z, ~")[", s4.y, ~"-1];", nl, ~"save(", arg.z, ~");",
		       nl, ~"for(", s3.y, ~"=", s4.y, ~"-2;", s3.y, ~">=0;--",
		       s3.y, ~"){", nl, ~"X ", s2.y, 
		       ~"=sequence(2,S_DATA(pick(1))[", s3.y, ~"],", s1.y, ~");"],
		      s2.y, z]] +
	      [y, t_expr.[subnode1.x, nontail.z]] +
	      concx.[[nl, s1.x, ~"=", arg.y, ~";}", nl, ~"restore();"],
	      	     s1.x, y]

  t_insertl = (al.[x, gensyms.[~4, y]]) + % generate gensyms t1, t2, i, len
  	      [x, y,
	       concx.[[nl, ~"int ", s3.y, ~"; // INSERTL", nl, ~"check_S(", arg.z, ~",", 
	               ~<$">, ~"/", ~<$">, ~");", nl, ~"int ", s4.y, 
		       ~"=S_LENGTH(", arg.z, ~");", nl, ~"X ", s1.y, ~"=",
		       s4.y, ~"==0?", unit_value.subnode1.x, ~":S_DATA(",
		       arg.z, ~")[0];", nl, ~"save(", arg.z, ~");",
		       nl, ~"for(", s3.y, ~"=1;", s3.y, ~"<", s4.y, ~";++",
		       s3.y, ~"){", nl, ~"X ", s2.y, 
		       ~"=sequence(2,", s1.y, ~",S_DATA(pick(1))[", s3.y, ~"]);"], 
		      s2.y, z]] +
	      [y, t_expr.[subnode1.x, nontail.z]] +
	      concx.[[nl, s1.x, ~"=", arg.y, ~";}", nl, ~"restore();"],
	      	     s1.x, y]

  t_kata = (al.[x, gensyms.[~4, y]]) + % generate gensyms t1, i, len, t2
  	   [x, y, conc.[[nl, ~"save(", arg.z, ~"); // KATA"], z]] +
	   [x, y, t_expr.[subnode1.x, nontail.z]] +
	   [x, y, arg.z,
	    concx.[[nl, ~"int ", s2.y, ~";", nl, ~"check_S(top(),", 
	            ~<$">, ~"(|...|)", ~<$">, ~");", nl, ~"int ", s3.y, 
	   	    ~"=S_LENGTH(top());", 
		    nl, ~"for(", s2.y, ~"=", s3.y, ~"-1;", s2.y, ~">=0;--",
	   	    s2.y, ~"){", nl, ~"X ", s1.y, 
	   	    ~"=sequence(2,S_DATA(top())[", s2.y, ~"],", arg.z, ~");"],
	   	   s1.y, z]] +
	   [y, z, t_expr.[subnode2.x, nontail.s4]] +
	   concx.[[nl, y, ~"=", arg.z, ~";}", nl, ~"restore();",
	   	   nl, ~"X ", s4.x, ~"=", y, ~";"],
	   	  s4.x, z]

  t_unit = al.[x, gensym.y] +
  	   concx.[[nl, ~"X ", y, ~"=", unit_value.subnode1.x, ~";"], 
	  	  y, z]

  t_ref = al.[x, gensym.y] +
  	  (eq.[s3.x, current.z] ->		% self-call?
  	     concx.[[nl, ~"x=", arg.z, ~"; // REF: ", tos.s3.x, nl, 
	     	     ~"goto loop; // tail call: ", tos.s3.x, 
		     nl, ~"X ", y, ~";"],
		    y, z];
	      concx.[[nl, ~"X ", y, ~"=", mangle.s3.x, ~"(", arg.z, ~"); // REF: ",
	              tos.s3.x], y, z])

  t_pointer = cat.[[x, mangle.node_arg1.x], gensym.y] +
              concx.[[nl, ~"X ", y, ~"(X);", nl,
	              ~"X ", z, ~"=fpointer((void*)", y, ~"); // POINTER2: ",
	              tos.node_arg1.x], z, s4]

  t_while = al.[x, gensym.y] +	% <x, tmp, state>
  	    [x, y, 		
	     concx.[[nl, ~"X ", y, ~"=", arg.z, ~"; // WHILE", nl, ~"for(;;){", nl,
	     	     ~"save(", y, ~");"], y, z]] +
	    [x, y, t_expr.[subnode1.x, nontail.z]] +
	    [x, y,				    
	     concx.[[nl, y, ~"=restore();", nl, ~"if(", arg.z, ~"==F) break;"], 
	            y,
	            z]] +
	    [y, t_expr.[subnode2.x, nontail.z]] +
	    concx.[[nl, x, ~"=", arg.y, ~";}"], x, y]

  t_catch = al.[x, gensyms.[~4, y]] +	% <x, <tmp, tmpjb, tmpx, tmpcp>, state>
  	    [x, y, arg.z,
	     concx.[[nl, ~"X *", s1.y, ~"=savedp,", s3.y, ~"; // CATCH", nl, ~"jmp_buf *",
	     	     s4.y, ~"=catcher,", s2.y, ~";", nl, ~"catcher=&", s2.y, ~";",
		     nl, ~"if(setjmp(", s2.y, ~")){", nl, s3.y, ~"=restore();",
		     nl, ~"savedp=", s1.y, ~";", nl, ~"catcher=", s4.y, ~";"], 
		    s3.y, z]] +
	    [x, y, z, t_expr.[subnode1.x, s4]] +
	    [x, y, concx.[[nl, s3.y, ~"=", arg.s4, ~";}", nl, ~"else{"], z, s4]] +
	    [y, t_expr.[subnode2.x, nontail.z]] +
	    concx.[[nl, ~"catcher=", s4.x, ~";", nl, s3.x, ~"=", arg.y, ~";}"], 
	           s3.x, y]

  t_extern = al.[x, gensym.y] + % <x, tmp, state>
  	     concx.[[nl, ~"extern X ", tos.s3.x, ~"(X x); // EXTERN", nl, ~"X ", y, ~"=", 
	     	     tos.s3.x, ~"(", arg.z, ~");"],
		    y, z]

  % specialized code for often-used intrinsics

  t_eq = two_arg_fun +
         concx.[[nl, ~"X ", y, ~"=(", arg.z, ~"==", arg.s4, ~")||eq1(", arg.z, 
	         ~",", arg.s4, ~")?T:F; // EQ"],
		y, s4]

  t_select = [|#, #, [|?ICONST, #, num|], [|?REF, #, ?`id`|]|].x -> % often used case
  	       t_fast_select;
               (two_arg_fun +
                concx.[[nl, ~"X ", y, ~"=select1(", arg.z, ~",", arg.s4, ~"); // SELECT"],
		       y, s4])
  t_fast_select = al.[x, gensym.y] +
  		  concx.[[nl, ~"X ", y, ~"=select1(TO_N(", tos.node_arg1.node_arg1.x, 
		          ~"),", arg.z, ~"); // SELECT (simple)"],
			 y, z]

  t_op = [node_arg1.node_arg1.x, two_arg_fun.[node_arg2.x, y]] +
         concx.[[nl, ~"X ", y.s2, ~"=___", tos.s1, ~"_5fop2(", 
	 	 arg.z.s2, ~",", arg.s4.s2, ~"); // OP2: ", tos.s1],
		y.s2, s4.s2]

  two_arg_fun = al.[x, gensym.y] +
                [x, y,
	         save_arg_if.[subnode2.x,
	       	              restore_arg_if.[subnode1.x, arg.z, 
	      			              t_expr.[subnode1.x, 
				                      save_arg_if.[subnode1.x, nontail.z]] ] ],
	         arg.z] +
	        [x, y, z, 
	         restore_arg_if.[subnode2.x, arg.z, 
	      		         t_expr.[subnode2.x, set_arg.[s4, nontail.z]]] ]

  % compute unit for function name, if possible

  unit_value = eq.[~REF, s1] ->
  	          (fetch.[s3, units] | no_unit);
		  no_unit
  no_unit = cat.[~"fail(", ~<$">, ~"no unit value", ~<$">, ~")"]		 
  units = ~<<`add`, "TO_N(0)">,
  	    <`mul`, "TO_N(1)">,
	    <`sub`, "TO_N(0)">,
	    <`div`, "TO_N(1)">,
	    <`band`, "TO_N(0)">,	    
	    <`bor`, "TO_N(0)">,	    
	    <`bxor`, "TO_N(0)"> >

  % emit literal construction
  %
  % literal : index -> string

  literal = cat.[~"lf[", tos, ~"]"]

  iliteral = num -> cat.[~"TO_N(", tos, ~")"];
             null -> ~"EMPTY";
             eq.[~T, id] -> ~"T";
             eq.[~F, id] -> ~"F";
	     _

  % translation state accessors

  newstate = [~"x", ~1, id, []]
  tail = [s1.y, s2.y, x, s4.y]
  nontail = [x, y, ~F, s4]

  arg = s1
  counter = s2
  current = s3
  code = s4

  nodetype = s1
  nodeloc = s2
  subnode1 = s3
  subnode2 = s4
  subnode3 = s1.drop.[~4, id]

  set_arg = cat.[[x], tl.y]

  % add code to output
  %
  % conc : <stringlist, state> -> state'
  % concx : <stringlist, newarg, state> -> state'
  % save_arg : state -> state'
  % restore_arg : <var, state> -> state'
  % save_arg_if : <node, state> -> state'
  % restore_arg_if : <node, arg, state> -> state'

  conc = [arg.y, counter.y, current.y, cat.[code.y, x]]
  concx = [y, counter.z, current.z, cat.[code.z, x]]
  save_arg = conc.[[nl, ~"save(", arg, ~");"], id]
  restore_arg = conc.[[nl, x, ~"=restore();"], y]
  save_arg_if = const_arg.x -> y; save_arg.y
  restore_arg_if = const_arg.x -> z; restore_arg.tl
  const_arg = [|?CONST | ?ICONST, ..|]

  % gensym : state -> <temp, state'>
  % gensyms : <count, state> -> <<temp1, ...>, state'>

  gensym = [gname.counter, [arg, add1.counter, current, code]]
  gensyms = [@(gname.add).dl.[counter.y, @sub1.iota.x], 
  	     [arg.y, add.[x, counter.y], current.y, code.y]]
  gname = al.[~$t, tos]

  % utilities
  %
  % mangle : atom/string -> string

  mangle = cat.al.[~"___", @mangle1.tos]
  mangle1 = c_id_char -> [id];
  	    lt.[id, ~10] -> ar.[~"_0", hexdigit.id];
	    [~$_, hexdigit.div.[id, ~16], hexdigit.mod.[id, ~16]]

  % hexdigit : num -> char
  
  hexdigit = add.[lt.[id, ~10] -> ~$0; sub.[~$a, ~10], id]

  % misc

  c_id_char = index.[id, ~"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"]

  % op table

  opref = [|?REF, #, index.[id, op_table]|]
  op_table = ~<`add`, `sub`, `mul`, `div`, `mod`, `band`, `bor`, `bxor`, `bshl`, `bshr`,
  	       `cmp`>

  % code pieces

  header = cat.[~"/* GENERATED BY fpc */", nl,
  	        ~"#define GENERATED", nl,
  	        ~"#ifndef STANDALONE", nl,
   	        ~"#include <fp/fp.h>", nl,
		~"#endif"]
  footer = cat.[includes.fetchall.[~INCLUDE, y],
		nl, ~"int main(int argc,char *argv[]){",
  	   	nl, ~"return init(argc,argv,",
		mangle.(fetch.[~ENTRY_POINT, y] | configuration:entrypoint),
		~",gen_lf);}",
		nl, ~"/* END OF FILE */", nl]
  includes = cat.@(cat.[nl, ~"#include \"", tos, ~"\""])

  def_header = cat.[nl, ~"//---------------------------------------- ", 
  	            tos.s1, ~" (", tos.def_loc, ~")",
  	            nl, ~"DEFINE(", mangle.s1, ~"){",
		    nl, ~"ENTRY;", 
  	            nl, ~"loop:;", nl,
  	            ~"tracecall(", ~<$">, tos.nodeloc.s2, ~":  ", tos.s1, 
		    ~<$">, ~");"]

}
