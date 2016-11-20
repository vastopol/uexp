%
% parse.fp - FP parser
%

% Parses tokens create by the "lex" module and returns a 
% tree of syntax-nodes.
%
% exports:
%
%   parse:parse : <token1, ...> -> <def, ...>
%
%   token = <class, loc, val>
%   class = ID | ATOM | SYM  (val = atom)
%   	    NUM              (val = num)
%           STRING    	     (val = string)
%   loc = atom  (source-location)
%   def = <name, expr>
%
% requires: util, lex


parse = {

  % parser state
  %
  %   <tokens, defs>
  %
  %   defs = <def, ...>  (accumulated parsed definitions)

  parse = parse'.[id, []]

  tokens = s1
  definitions = s2
  token_location = s2

  finished = null.tokens
  nextval = s3.s1.tokens

  parse' = finished -> definitions; p_top

  p_top = match.[id, ~STRING] ->			% include file?
  	    (native_include.tos.nextval ->                  % .c/.h include?
	      parse'.push.[toa.nextval, advance];
  	      parse'.al.[cat.[lex:lex.nextval, tl.tokens], tl]);  % insert tokens from file
	    parse'.push.p_definition

  % parse a single definition
  % 
  % p_definition : state -> <def, state'>

  p_definition = expect.[id, ~ID] +
  	         [nextval, advance] +
  	         matchval.[y, ~SYM, ~`[|`] ->
  	           p_guarded_def;
		   p_def'.[x, expectval.[y, ~SYM, ~`=`]]
  p_guarded_def = [x, p_value.y] + 
  		  [x, s1.y, p_seq.expectval.[s2.y, ~SYM, ~`=`]] +
  		  [[x, [~COND, node_loc.y, y, s1.z, [~REF, node_loc.y, ~`_`]]],
		   s2.z]
  p_def' = matchval.[y, ~SYM, ~`{`] ->			% module?
   	     p_module.[x, p_deflist.[[], advance.y]];
	     p_def''.[x, p_seq.y]
  p_def'' = [[x, s1.y], s2.y]

  p_deflist = matchval.[y, ~SYM, ~`}`] ->		% end reached?
  	        [x, advance.y];
		p_deflist'.[x, p_definition.y]
  p_deflist' = p_deflist.[ar.[x, s1.y], s2.y]

  % merge module definitions into global def-list
  %
  % p_module : <modulename, <<def1, ...>, state>> -> <F, state'>

  p_module = p_mod'.[rename.aliases.dl.[x, s1.y], s2.y]
  p_mod' = [~F, [tokens.y, cat.[definitions.y, x]]] % F -> ignored by "push"
  
  % aliases : <<modulename, <def1, ...>>, ...> -> <<defname1, alias1, def1>, ...>

  aliases = @([s1.y, prefix.[x, s1.y], y]) % parentheses due to broken fpc0 parser
  
  % rename : <<defname1, alias1, def1>, ...> -> <def', ...>

  rename = @rename1.dr.[id, id]
  rename1 = [s2.x, rename_rec.[s2.s3.x, y]]
  rename_rec = ?REF.s1.x -> rename_id.[x, fetch.[s3.x, y]];
  	       constnode.x -> x;
	       cat.[take.[~2, x], @rename_rec.dr.[drop.[~2, x], y]]
  rename_id = y -> [s1.x, s2.x, y]; x

  % expression parsers
  %
  %   parser : state -> <expr, state'>

  p_seq = p_exp0 +
  	  (matchval.[y, ~SYM, ~`;`] ->			% sequence?
	    ([x, token_location.s1.tokens.y, p_seq.advance.y] +
	     [[~SEQ, y, x, s1.z], s2.z]);
	    id)

  p_exp0 = p_conditional +
  	   (matchval.[y, ~SYM, ~`+`] ->			% reverse composition?
	     ([x, token_location.s1.tokens.y, p_exp0.advance.y] +
	      [[~COMP, y, s1.z, x], s2.z]);
	     id)

  p_conditional = p_bexpression + 
  		  (matchval.[y, ~SYM, ~`->`] ->		% conditional?
  		     p_cond''.[[token_location.s1.tokens.y, x],
		     	       p_bexpression.advance.y];
		     id)
  p_cond'' = (null.tokens.s2.y -> ~F; matchval.[s2.y, ~SYM, ~`;`]) ->	  % ";"?
  	       p_cond'''.[x, s1.y, advance.s2.y];
	       [[~COND, s1.x, s2.x, s1.y, [~REF, s1.x, ~`id`]], s2.y]
  p_cond''' = [x, y, p_conditional.z] +	% alternative part
    	      [[~COND, s1.x, s2.x, y, s1.z], s2.z]

  p_bexpression = p_bexp'.p_fexpression
  p_bexp' = matchval.[y, ~SYM, ~`&`] ->  % boolean and? (translated to "f -> g; ~F")
  	      ([x, token_location.s1.tokens.y, p_bexpression.advance.y] +
	       [[~COND, y, x, s1.z, [~ICONST, y, ~F]], s2.z]);
	    matchval.[y, ~SYM, ~`|`] ->  % boolean or? (uses own op, rewrite is too painful ...)
	      ([x, token_location.s1.tokens.y, p_bexpression.advance.y] +
	       [[~OR, y, x, s1.z], s2.z]);
            id

  p_fexpression = p_fexp'.p_expression
  p_fexp' = matchval.[y, ~SYM, ~`^`] ->  % application? (translated to "app.[f, g]")
  	      ([x, token_location.s1.tokens.y, p_fexpression.advance.y] +
	       [[~COMP, y, [~REF, y, ~`app`], [~CONS, y, x, s1.z]], s2.z] );
            id

  p_expression = p_exp'.p_value
  p_exp' = matchval.[y, ~SYM, ~`.`] ->	% composition?
  	     ([x, token_location.s1.tokens.y, p_expression.advance.y] +
	      [[~COMP, y, x, s1.z], s2.z]);
	   id

  p_value = matchval.[id, ~SYM, ~`(`] ->	% parenthesis?
  	      p_val_paren.p_seq.advance;
	    matchval.[id, ~SYM, ~`[`] ->	% construction?
	      p_val_cons.[token_location.s1.tokens, advance];
	    matchval.[id, ~SYM, ~`[|`] ->	% predicate construction?
	      p_val_pcons.[token_location.s1.tokens, advance];
	    matchval.[id, ~SYM, ~`(|`] ->	% catamorphism?
	      p_val_cata.[token_location.s1.tokens, advance];
	    matchval.[id, ~SYM, ~`~`] ->	% constant?
	      (literals:immediate.s1.y ->
	        [[~ICONST, x, s1.y], s2.y];
	        [[~CONST, x, s1.y], s2.y]).
	       [token_location.s1.tokens, p_constant.advance];
	    matchval.[id, ~SYM, ~`@`] ->	% alpha?
	      [[~ALPHA, x, s1.y], s2.y].
	        [token_location.s1.tokens, p_value.advance];
	    matchval.[id, ~SYM, ~`/`] ->	% insertr?
	      [[~INSERTR, x, s1.y], s2.y].
	        [token_location.s1.tokens, p_value.advance];
	    matchval.[id, ~SYM, ~`\\`] ->	% insertl?
	      [[~INSERTL, x, s1.y], s2.y].
	        [token_location.s1.tokens, p_value.advance];
%	    matchval.[id, ~SYM, ~`^`] ->	% tinsert?
%	      [[~TINSERT, x, s1.y], s2.y].
%	        [token_location.s1.tokens, p_value.advance];
	    matchval.[id, ~ID, ~`trace`] ->	% trace?
	      [[~COMP, x, [~REF, x, ~`%trace`], 
	        [~CONS, x, [~CONST, x, s1.y], [~REF, x, ~`id`]]],
	       s2.y].
	       [token_location.s1.tokens, p_constant.advance];
	    matchval.[id, ~SYM, ~`#`] ->	% "default"?
	      [[~ICONST, token_location.s1.tokens, ~T], advance];
	    matchval.[id, ~SYM, ~`?`] ->	% match?
	      [[~COMP, x, [~REF, x, ~`eq`], 
	        [~CONS, x, [~CONST, x, s1.y], [~REF, x, ~`id`]]],
	       s2.y].
	       [token_location.s1.tokens, p_constant.advance];	      
	    matchval.[id, ~SYM, ~`!`] ->	% select?
	      [[~COMP, x, [~REF, x, ~`select`], 
	        [~CONS, x, [~ICONST, x, s1.y], [~REF, x, ~`id`]]],
	       s2.y].
	       check_iconst.[token_location.s1.tokens, p_constant.advance];	      
	    matchval.[id, ~SYM, ~`*`] ->	% pointer?
	      ([token_location.s1.tokens, p_value.advance] +
	       [[~POINTER1, x, s1.y], s2.y] );
	    matchval.[id, ~ID, ~`while`] ->	% while?
	      ([token_location.s1.tokens, p_expression.advance] +
	       [[~WHILE, x, y, s1.z], s2.z].[x, s1.y, p_expression.s2.y]);
	    matchval.[id, ~ID, ~`catch`] ->	% catch?
	      ([token_location.s1.tokens, p_expression.advance] +
	       [[~CATCH, x, y, s1.z], s2.z].[x, s1.y, p_expression.s2.y]);
	    matchval.[id, ~ID, ~`unit`] ->	% unit?
	      ([token_location.s1.tokens, p_value.advance] +
	       [[~UNIT, x, s1.y], s2.y] );
	    matchval.[id, ~ID, ~`extern`] ->    % extern?
	      ([token_location.s1.tokens, p_constant.advance] +
	       [[~EXTERN, x, s1.y], s2.y]);
	    p_val_id.[s1.tokens, advance.expect.[id, ~ID]] % (possibly qualified) identifier
  p_val_paren = matchval.[y, ~SYM, ~`)`] -> 
  	          [x, advance.y];		   % expression in parens, otherwise...
		null.tokens.y ->
		  fail.[~"unexpected end of input", y];
		  p_val_fop.[token_location.s1.tokens.y, [x], y]	   % fancy syntax
  p_val_fop = matchval.[z, ~SYM, ~`)`] ->	   % end reached?
  	        [[~COMP, x, s1.y, 
		  cat.[[~CONS, x], al.[[~REF, x, ~`id`], @[~POINTER1, x, y].dl.[x, tl.y]]] ],
		 advance.z];
		([x, y, p_seq.z] + p_val_fop.[x, ar.[y, s1.z], s2.z])
  p_val_cons = matchval.[y, ~SYM, ~`]`] ->	% empty?
  	         [[~ICONST, x, []], advance.y];
		 p_val_cons'.[x, [], p_seq.y]
  p_val_cons' = matchval.[s2.z, ~SYM, ~`]`] ->      % end reached?
  	          [cat.[[~CONS, x], ar.[y, s1.z]], advance.s2.z];
		matchval.[s2.z, ~SYM, ~`,`] ->      % comma?
		  p_val_cons'.[x, ar.[y, s1.z], p_seq.advance.s2.z];
		  fail.[~"expected `,' or `]'", s2.z]
  p_val_pcons = matchval.[y, ~SYM, ~`|]`] ->	% empty?
  	          [[~PCONS, x], advance.y];
		  p_val_pcons'.[x, [], p_seq.y]
  p_val_pcons' = matchval.[s2.z, ~SYM, ~`|]`] -> % end reached?
  	           [cat.[[~PCONS, x], ar.[y, s1.z]], advance.s2.z];
		 matchval.[s2.z, ~SYM, ~`,`] ->	 % comma?
		   ([x, y, s1.z, advance.s2.z] +
		    matchval.[s4, ~SYM, ~`..`] -> % dots at end?
		      [cat.[[~PCONSL, x], ar.[y, z]], 
		       expectval.[advance.s4, ~SYM, ~`|]`] ];
		      p_val_pcons'.[x, ar.[y, z], p_seq.s4]);
		   fail.[~"expected `,' or `|]'", s2.z]
  p_val_cata = [x, p_seq.y] +
  	       [x, s1.y, p_seq.expectval.[s2.y, ~SYM, ~`,`]] +
	       [[~KATA, x, y, s1.z], expectval.[s2.z, ~SYM, ~`|)`]]
  p_val_id = matchval.[y, ~SYM, ~`:`] -> 	    % qualified?
  	       p_val_id'.[x, token_location.s1.tokens.y, expect.[advance.y, ~ID]];
	       [[~REF, token_location.x, s3.x], y]
  p_val_id' = [[~REF, y, prefix.[s3.x, nextval.z]], advance.z]

  p_constant = match.[id, ~NUM] ->		    % number?
  	         [nextval, advance];
	       match.[id, ~ATOM] ->	            % atom?
	         [nextval, advance];
	       matchval.[id, ~SYM, ~`<`] ->	    % sequence?
	         p_sequence.advance;
	       match.[id, ~STRING] ->		    % string?
	         [tos.nextval, advance];
		 fail.[~"invalid constant", id]
  p_sequence = matchval.[id, ~SYM, ~`>`] ->         % empty sequence?
  	         [~<>, advance];
	         p_seq'.[[x], y].p_constant
  p_seq' = matchval.[y, ~SYM, ~`>`] ->              % end reached?
   	     [x, advance.y];
	     p_seq''.[x, p_constant.expectval.[y, ~SYM, ~`,`]]
  p_seq'' = p_seq'.[ar.[x, s1.y], s2.y]

  % matching functions
  %
  % match : <state, class> -> bool
  % matchval : <state, class, val> -> bool
  % matchn : <state, <class1, ...>> -> bool
  % expect : <state, class> -> state
  % expectval : <state, class, val> -> state'

  match = null.tokens.x -> ~F; eq.[y, s1.s1.tokens.x]
  matchval = match -> eq.[z, s3.s1.tokens.x]; ~F
  matchn = /and.@(eq.[s1.x, y]).trans.[take.[len.y, tokens.x], y]
  expect = null.tokens.x ->
  	     fail.[~"unexpected end of input", x];
           match -> x; 
           fail.[cat.[~"expected ", classname.y, ~" but found ",
     	              classname.s1.s1.tokens.x],
		 x]
  expectval = null.tokens.x ->
  	        fail.[cat.[~"expected ", 
		           classname.y, 
			   ~" but found end of input"],
		      x];
                matchval -> advance.x;
		fail.[cat.[~"expected `", tos.z, ~"' but found `", 
		           tos.s3.s1.tokens.x, ~"'"],
		      x]

  % push : <def|F, state> -> state'  (push new def on def-list)
  
  push = x -> [tokens.y, ar.[definitions.y, x]]; y

  % utilities

  classname = fetch.[id, ~<<ID, "identifier">,
  	      		   <ATOM, "atom">,
			   <SYM, "symbol">,
			   <STRING, "string">,
			   <NUM, "number">>]
  prefix = toa.cat.[tos.x, ~":", tos.y]
  native_include = gt.[len, ~2] & 
  		   (drop.[index.[~$., id] | ~0, id] +
		    index.[id, ~<"c", "h">] )
  check_iconst = num.s1.y -> id;
  	         fail.[~"invalid index for SELECT", s2.y]

  % advance : state -> state'  (advance token list)

  advance = null.tokens ->
   	      fail.[~"unexpected end of input", id];
	      al.[tl.tokens, tl]

  % fail : <message, state>

  fail = quit.toa.cat.[null.tokens.y -> ~"Error"; 
                       tos.token_location.s1.tokens.y,
       	   	       ~": ", x]

}
