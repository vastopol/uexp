% 
% rewrite.fp
%
% Algebraic rewrites
%

% usage:
%
%   rewrite:rewrite : <defs, pass#, flags> -> <defs', touched>
%
%   state = <defs, stats, pass#>
%   stats = <<atom1, loc1>, ...>

% requires:
%
%   util.fp


rewrite = {

  rewrite = (fetch.[~DEBUG, z] -> dribble.~"applying rewrite rules");
  	    null.x -> ~<<>, F>; % will never happen
	    finish.[\rewrite_def.al.[[[], [], y], x], z]
  finish = (fetch.[~DEBUGDUMP, y] -> report_full.state_stats.x);
  	   (fetch.[~DEBUG, y] -> report.state_stats.x);
  	   [state_defs.x, not.zero.len.state_stats.x]

  % rewrite_def : <state, def> -> state'
  %XXX transform tail-recursion to catamorphism:
  %    f = ?<> -> g; h.[?1, f.tl]  -->  f = (|g, h|)
  rewrite_def = looping.y -> rewrite_def2.loopify;
  	        rewrite_def2
  rewrite_def2 = [y, rewrite1.[def_node.y, x]] +
  	         add_def.[def_name.x, s1.y, s2.y]

  % test + transform simple tail-recursion to loop:
  %    f = g -> h; f.k  -->  f = h.(while (g -> ~F; ~T) k)
  looping = [|?COND, #, #, #, [|?COMP, #, [|?REF, ..|], #|]|].def_node &
  	    eq.[def_name, node_arg1.node_arg1.node_arg3.def_node]
  loopify = [log.[~`converting recursive definition into loop`, def_loc.y, x],
  	     [def_name, 
   	      [~COMP, def_loc, 
	       node_arg2.def_node,
	       [~WHILE, def_loc, 
	        [~COND, def_loc, node_arg1.def_node,
		 [~ICONST, def_loc, ~F], [~ICONST, def_loc, ~T]],
	        node_arg2.node_arg3.def_node] ] ].y ]

  % report : stats
  % report_full : stats
  report = not.null -> @report'.dr.[nodups.s1.trans, id]
  report' = dribble.cat.[~"  ", tos.x, ~" (", tos.len.fetchall.[x, y], ~")"]

  report_full = @(dribble.cat.[~"  ", tos.y, ~": ", tos.x])

  % state accessors
  state_defs = s1
  state_stats = s2
  state_pass = s3

  % add_def : <name, node, state> -> state'
  add_def = al.[ar.[state_defs.z, [x, y]], tl.z]

  % node_eq : <node1, node2> -> bool
  node_eq = atom.x -> eq;
  	    atom.y -> ~F;
	    not.eq.[len.x, len.y] -> ~F;
  	    eq.[node_type.x, node_type.y] ->
  	     (constnode.x -> constnode.y & eq.[node_args.x, node_args.y];
	       null.node_args.x -> ~T;
	       all.@node_eq.trans.[node_args.x, node_args.y] );
	    ~F

  % gen_name : state -> name
  gen_name = toa.cat.[~"tmp%", tos.state_pass, ~"%", tos.len.state_defs]

  % log : <atom, loc, state> -> state'
  log = [state_defs.z, ar.[state_stats.z, [x, y]], state_pass.z]

  % gen_def : <node, state> -> <name, state'>
  gen_def = [gen_name.y, x, y] +
  	    [x, add_def]

  % check if all nodes are compositions with the second arg being identical
  % eq_comp2 : <cnodes, node> -> bool
  eq_comp2 = /(y & [|?COMP, ..|].x & node_eq.[node_arg2.x, y] -> y; ~F).ar

  % find common composition component (not currently used, but intended for CONS-hoisting)
  % common_comp : nodes -> node | F
  common_comp = (|~T, common_comp'|) + not.?T & id
  common_comp' = constresult.x -> y;
  	         [|?COMP, ..|].x &
		  (eq.[~T, y] | node_eq.[y, node_arg2.x]) &
		  node_arg2.x

  % remove compositional part, if there is one
  % uncomp : node -> node'
  uncomp = [|?COMP, ..|] -> node_arg1

  % check constant SELECT index
  % check_select : <select-node, seq> -> element
  check_select = (ge.[x, ~0] & le).[node_arg1.node_arg1.x, len.y] ->
  	           select.[node_arg1.node_arg1.x, y];
		   quit.cat.[~"Error: ", tos.node_loc.x, 
		   	     ~": constant index ", tos.node_arg1.node_arg1.x,
			     ~" out of range"]

  % Rewrite rules
  recurse = constnode.x | eq.[~2, len.x] -> id;
  	    ([x, al.[[y, []], node_args.x] +
  	         \([s2.x, rewrite1.[y, s1.x]] + [s2.y, ar.[x, s1.y]]) ] +
             [cat.[[node_type.x, node_loc.x], s2.y], s1.y])

  % rewrite1 : <node, state> -> <node', state'>
  rewrite1 = % id.X --> X
  	     [|?COMP, #, [|?REF, #, ?`id`|], #|].x -> 
	       rewrite1.[node_arg2.x, log.[~`COMP/id`, node_loc.x, y]];
  	     % X.id --> X
	     [|?COMP, #, #, [|?REF, #, ?`id`|]|].x -> 
	       rewrite1.[node_arg1.x, log.[~`COMP/id`, node_loc.x, y]];
	     % (X.Y).Z --> X.(Y.Z)
	     [|?COMP, #, [|?COMP, ..|], #|].x ->
	       rewrite1.[[~COMP, node_loc.x, node_arg1.node_arg1.x, 
	       		  [~COMP, node_loc.node_arg1.x, node_arg2.node_arg1.x,
			   node_arg2.x] ],
			 log.[~`normalized composition`, node_loc.x, y]];
	     % eq.[X, Y] --> <EQ, _, X, Y>
	     [|?COMP, #, [|?REF, #, ?`eq`|], [|?CONS, ..|]|].x ->
	       rewrite1.[cat.[[~EQ, node_loc.x], node_args.node_arg2.x], 
	       		 log.[~"primitive EQ", node_loc.x, y]];
	     [|?COMP, #, [|?REF, #, ?`eq`|], [|?COMP, #, [|?CONS, ..|], ..|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, 
	       	          cat.[[~EQ, node_loc.x], node_args.node_arg1.node_arg2.x],
			  node_arg2.node_arg2.x],
	       		 log.[~"primitive EQ (regrouped)", node_loc.x, y]];
	     % select.[X, Y] --> <SELECT, _, X, Y>
	     [|?COMP, #, [|?REF, #, ?`select`|], [|?CONS, ..|]|].x ->
	       rewrite1.[cat.[[~SELECT, node_loc.x], node_args.node_arg2.x], 
	       		 log.[~"primitive SELECT", node_loc.x, y]];
	     [|?COMP, #, [|?REF, #, ?`select`|], [|?COMP, #, [|?CONS, ..|], ..|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, 
	       	          cat.[[~SELECT, node_loc.x], node_args.node_arg1.node_arg2.x],
			  node_arg2.node_arg2.x],
	       		 log.[~"primitive SELECT (regrouped)", node_loc.x, y]];
	     % @X.@Y --> @(X.Y)
	     [|?COMP, #, [|?ALPHA, ..|], [|?ALPHA, ..|]|].x ->
	       rewrite1.[[~ALPHA, node_loc.node_arg1.x,
	     		  [~COMP, node_loc.x, node_arg1.node_arg1.x, 
	     	           node_arg1.node_arg2.x] ],
	     	         log.[~`ALPHA/ALPHA fusion`, node_loc.x, y] ];
	     [|?COMP, #, [|?ALPHA, ..|], [|?COMP, #, [|?ALPHA, ..|], ..|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, 
	       		   [~ALPHA, node_loc.node_arg1.x,
	     		     [~COMP, node_loc.x, node_arg1.node_arg1.x, 
	     	              node_arg1.node_arg1.node_arg2.x] ],
			   node_arg2.node_arg2.x],
	     	         log.[~`ALPHA/ALPHA fusion (regrouped)`, node_loc.x, y] ];
	     % ~C -> X; Y --> X or Y
	     [|?COND, #, [|?CONST | ?ICONST, ..|], ..|].x ->
	       rewrite1.[node_arg1.node_arg1.x -> node_arg2.x; s5.x,
	       		 log.[~`constant conditional`, node_loc.x, y] ];
             % eq.[id, id] --> ~T
	     [|?EQ, #, [|?REF, #, ?`id`|], [|?REF, #, ?`id`|]|].x ->
	       rewrite1.[[~ICONST, node_loc.x, ~T], 
	                 log.[~`always equal`, node_loc.x, y] ];
             % eq.[~X, ~Y] --> result
	     [|?EQ, #, [|?CONST | ?ICONST, ..|], [|?CONST | ?ICONST, ..|]|].x ->
	       rewrite1.[[~ICONST, node_loc.x, 
	                  eq.[node_arg1.node_arg1.x, node_arg1.node_arg2.x]], 
	                 log.[~`constant equal`, node_loc.x, y] ];
	     % *exp --> *tmp
	     [|?POINTER1, ..|].x ->
	       ([x, gen_def.[node_arg1.x, y]] +
	        rewrite1.[[~POINTER2, node_loc.x, s1.y], 
			  log.[~`introducing f-pointer temporary`, node_loc.x, s2.y] ]);
	     % app.[*f, x] --> f.x
	     [|?COMP, #, [|?REF, #, ?`app`|], 
	     	         [|?CONS, #, [|?POINTER2, ..|], #|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, al.[~REF, tl.node_arg1.node_arg2.x],
	                  node_arg2.node_arg2.x],
			 log.[~`direct function application`, node_loc.x, y] ];
	     [|?COMP, #, [|?REF, #, ?`app`|], 
	     	         [|?COMP, #, [|?CONS, #, [|?POINTER2, ..|], #|], ..|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, al.[~REF, tl.node_arg1.node_arg1.node_arg2.x],
	                  [~COMP, node_loc, node_arg2.node_arg1, node_arg2.node_arg2].node_arg2.x],
			 log.[~`direct function application (regrouped)`, node_loc.x, y] ];
	     % cat.@@f --> @f.cat
	     [|?COMP, #, [|?REF, #, ?`cat`|], [|?ALPHA, #, [|?ALPHA, ..|]|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, node_arg1.node_arg2.x, node_arg1.x],
	       		 log.[~`cat/ALPHA promotion`, node_loc.x, y] ];
	     [|?COMP, #, [|?REF, #, ?`cat`|], 
              [|?COMP, #, [|?ALPHA, #, [|?ALPHA, ..|]|], ..|]|].x ->
	       rewrite1.[[~COMP, node_loc.x, node_arg1.node_arg1.node_arg2.x, 
	                  [~COMP, node_loc.node_arg2.x, node_arg1.x, node_arg2.node_arg2.x] ],
	       		 log.[~`cat/ALPHA promotion (regrouped)`, node_loc.x, y] ];
	     % @id --> id
	     [|?ALPHA, #, [|?REF, #, ?`id`|]|].x ->
	       rewrite1.[node_arg1.x, log.[~`ALPHA/id elimination`, node_loc.x, y]];
	     % (|f, g|).@h --> (|f, g.[h.x, y]|)
	     [|?COMP, #, [|?KATA, ..|], [|?ALPHA, ..|]|].x ->
	       rewrite1.[[~KATA, node_loc.node_arg1.x, node_arg1.node_arg1.x,
	       	          [~COMP, node_loc.node_arg1.x, 
			   node_arg2.node_arg1.x, 
			   [~CONS, node_loc.x,
			    [~COMP, node_loc.x, node_arg1.node_arg2.x, 
			     [~SELECT, node_loc.x, [~ICONST, node_loc.x, ~1], 
			      [~REF, node_loc.x, ~`id`] ] ],
			    [~SELECT, node_loc.x, [~ICONST, node_loc.x, ~2],
			     [~REF, node_loc.x, ~`id`] ] ] ] ],
			 log.[~`catamorphism/ALPHA fusion`, node_loc.x, y] ];
	     [|?COMP, #, [|?KATA, ..|], 
	      [|?COMP, #, [|?ALPHA, ..|], #|]|].x ->
	       rewrite1.[[~COMP, node_loc.node_arg2.x,
	                  [~KATA, node_loc.node_arg1.x, node_arg1.node_arg1.x,
	       	           [~COMP, node_loc.node_arg1.x, 
			    node_arg2.node_arg1.x, 
			    [~CONS, node_loc.x,
			     [~COMP, node_loc.x, node_arg1.node_arg1.node_arg2.x,
			      [~SELECT, node_loc.x, [~ICONST, node_loc.x, ~1],
			       [~REF, node_loc.x, ~`id`] ] ],
			     [~SELECT, node_loc.x, [~ICONST, node_loc.x, ~2],
			      [~REF, node_loc.x, ~`id`] ] ] ] ],
			  node_arg2.node_arg2.x],
			 log.[~`catamorphism/ALPHA fusion (regrouped)`, node_loc.x, y] ];
	     % @f.[g1, ...] --> [f.g1, ...]  (extract `f')
	     [|?COMP, #, [|?ALPHA, ..|], [|?CONS, ..|]|].x ->
	       ([x, gen_def.[node_arg1.node_arg1.x, y]] +
	        rewrite1.[cat.[[~CONS, node_loc.node_arg2.x], 
		               @[~COMP, s1.y, s2.y, x].dr.
			        [node_args.node_arg2.x, 
				 [node_loc.x, 
				  [~REF, node_loc.node_arg1.node_arg1.x, s1.y]] ] ],
			  log.[~`ALPHA/CONS fusion`, node_loc.x, s2.y] ]);
	     [|?COMP, #, [|?ALPHA, ..|], [|?COMP, #, [|?CONS, ..|], #|]|].x ->
	       ([x, gen_def.[node_arg1.node_arg1.x, y]] +
	        rewrite1.[cat.[[~CONS, node_loc.node_arg1.node_arg2.x], 
		               @[~COMP, s1.y, s2.y, x].dr.
			        [node_args.node_arg1.node_arg2.x, 
				 [node_loc.x, 
				  [~REF, node_loc.node_arg1.node_arg1.x, s1.y] ] ] ],
			  log.[~`ALPHA/CONS fusion (regrouped)`, node_loc.x, s2.y] ]);
	     % [f1.h, ...] --> [f1, ...].h
%
%            Disabled, as it would render 2-op optimization mostly useless.
%
%	     ([|?CONS, ..|].x & common_comp.node_args.x) ->
%	       % should call common_comp just once, but this makes everything even harder to read
%	       rewrite1.[[~COMP, node_loc.x, 
%	       	          cat.[[~CONS, node_loc.x], @uncomp.node_args.x],
%			  common_comp.node_args.x],
%			 log.[~`composition hoisting (construction)`, node_loc.x, y] ];
%
	     % [(|f1, g1|), ...] --> 
	     %      (|[f1, f2, ...], 
             %        [g1.[select.[~1, id], select.[~1, select.[~2, id]]], 
             %         g2.[select.[~1, id], select.[~2, select.[~2, id]]], ...]|)
	     [|?CONS, #, #, ..|].x & all.@[|?KATA, ..|].node_args.x ->
	       rewrite1.[[~KATA, node_loc.x, 
	       		  cat.[[~CONS, node_loc.x], @node_arg1.node_args.x],
			  cat.[[~CONS, node_loc.x], 
			       @[~COMP, x, s1.y,
			         [~CONS, x,
			          [~SELECT, x, [~ICONST, x, ~1], [~REF, x, ~`id`]],
				  [~SELECT, x, [~ICONST, x, s2.y], 
				   [~SELECT, x, [~ICONST, x, ~2], [~REF, x, ~`id`]] ] ] ].
			       dl.[node_loc.x, trans.[@node_arg2.node_args.x, 
			      		      	      iota.len.node_args.x] ] ] ],
			 log.[~`banana split`, node_loc.x, y] ];
	     % select.[f, id].g -> select.[f, g] (internal form)
	     [|?COMP, #, [|?SELECT, #, #, [|?REF, #, ?`id`|]|], ..|].x ->
	       rewrite1.[[~SELECT, node_loc.x, node_arg1.node_arg1.x, node_arg2.x],
	       	         log.[~`merged SELECT/composition`, node_loc.x, y] ];
	     % eq.[f, id].g -> eq.[f, g] (internal form)
	     [|?COMP, #, [|?EQ, #, #, [|?REF, #, ?`id`|]|], ..|].x ->
	       rewrite1.[[~EQ, node_loc.x, node_arg1.node_arg1.x, node_arg2.x],
	       	         log.[~`merged EQ/composition`, node_loc.x, y] ];
	     % [~C1, ...] --> ~<C1, ...>
	     ([|?CONS, #, [|? CONST | ?ICONST, ..|], ..|].x &
	      (|~T, y & [|?ICONST | ?CONST, ..|].x|).node_args.x) ->
	       rewrite1.[[~CONST, node_loc.x, @node_arg1.node_args.x],
	       	         log.[~`constant construction`, node_loc.x, y] ];
	     % select.[~N, ~<..., X, ...>] --> X
	     [|?SELECT, #, [|?ICONST, ..|], [|?CONST, ..|]|].x ->
	       (atom.node_arg1.node_arg2.x ->
	         quit.cat.[~"Error: ", tos.node_loc.node_arg1.node_arg2.x,
		           ~": constant SELECT of non-sequence"];
		 rewrite1.[[check_select.[x, node_arg1.node_arg2.x], x, y] +
		  	   [num.x -> ~ICONST; ~CONST, node_loc.y, x],
			   log.[~`constant SELECT`, node_loc.x, y] ] );
	     % select.[~N, [..., f, ...]] --> f
 	     [|?SELECT, #, [|?ICONST, ..|], [|?CONS, ..|]|].x ->
 	       rewrite1.[check_select.[x, tl.tl.node_arg2.x],
 			 log.[~`construction cancellation`, node_loc.x, y] ];

	     % ~X.f --> ~X
	     % *X.f --> *X
	     [|?COMP, #, constresult, #|].x ->
	       rewrite1.[node_arg2.x, log.[~`composition with constant`, node_loc.x, y] ];

	     % ~C | f --> C or f
	     [|?OR, #, [|?ICONST | ?CONST, ..|], #|].x ->
	       rewrite1.[node_arg1.node_arg1.x -> node_arg1.x; node_arg2.x,
	       		log.[~`constant OR`, node_loc.x, y] ];

	     % f.g | h.g --> (f | h).g
	     ([|?OR, #, [|?COMP, ..|], #|].x & common_comp.node_args.x) ->
	       rewrite1.[[~COMP, node_loc.x, 
		         [~OR, node_loc.x, uncomp.node_arg1.x, uncomp.node_arg2.x],
			 common_comp.node_args.x],
	       	        log.[~`composition hoisting (OR)`, node_loc.x, y] ];

	     % f.u -> g.u; h.u --> (f -> g; h).u
	     ([x, y, [|?COND, #, [|?COMP, ..|], ..|].x & common_comp.node_args.x] +
	       (z ->
	          rewrite1.[[~COMP, node_loc.x, 
		  	     cat.[[~COND, node_loc.x], @uncomp.node_args.x],
			     z],
			    log.[~`composition hoisting (conditional)`, node_loc.x, y] ];

	     % f -> g.h; g.u --> g.(f -> h; u)
	     [|?COND, #, #, [|?COMP, ..|], [|?COMP, ..|]|].x & 
	      node_eq.[node_arg1.node_arg2, node_arg1.node_arg3].x ->
	       rewrite1.[[~COMP, node_loc.x, node_arg1.node_arg2.x,
	       		  [~COND, node_loc.x, node_arg1.x, node_arg2.node_arg2.x,
			   node_arg2.node_arg3.x] ],
			 log.[~`reverse composition hoisting (conditional)`, node_loc.x, y] ];

	     recurse) )

}
