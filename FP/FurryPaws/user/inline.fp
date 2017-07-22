%
% inline.fp - inlining
%

% This module performs inlining of definitions that have a size
% less or equal to the current inline limit, or which are called
% only once (so called "contractables"). Setting the inline limit
% to 0 will disable inlining of non-contractables. 

%
% usage:
%
%   inline:inline : <defs*, limit, debug> -> defs*'
%   inline:mark_contractables : defs* -> defs*'
%   inline:report_contractables : defs*


inline = {

  % info structure accessors
  info_defs = s1
  info_done = s2
  info_limit = s3
  info_debug = s4

  % inlining entry point
  inline = [~1, x, [], y, z] +
  	   s3.while le.[x, len.y]
	     ([id, walk.[select.[x, y], tl]] +
	      [add1.s1.x, s2.x, y, s4.x, s5.x] )

  % walk : <def*, info> -> done*'
  walk = [def_name.x, info_done.y, walkrec.[def_node.x, x, y]] +
         ar.[y, cat.[[x, s1.z], tl.tl.s2.z]]

  % walkrec : <node, def*, info> -> <node', def*'>
  walkrec = eq.[~REF, node_type.x] -> walkref;
	    constnode.x -> tlr;
	    ([x, walklist.[node_args.x, y, z]] +
	     [cat.[take.[~2, x], s1.y], s2.y] )

  % walklist : <nodes, def*, info> -> <nodes', def*'>
  walklist = walklist'.[x, ~1, y, z, []] + [s5, z]
  walklist' = while le.[y, len.x] 
  	        ([id, walkrec.[select.[y, x], z, s4]] +
		 [s1.x, add1.s2.x, s2.y, s4.x, ar.[s5.x, s1.y]] )

  % walkref : <node, def*, info> -> <node', def*'>
  walkref = eq.[node_arg1.x, def_name.y] -> tlr; % ref to current definition?
  	    ([x, y, z, find_candidate.[node_arg1.x, z]] +
	     (s4 -> integrate.[x, s4, y, z]; tlr) )

  % find_candidate : <name, info> -> def* | F
  find_candidate = [find_in_defs.[x, info_done.y] | 
  		    find_in_defs.[x, info_defs.y], y] +
  		   x & (le.[def_size.x, info_limit.y]) -> x; ~F

  % find_in_defs : <name, defs*> -> def* | F
  find_in_defs = assoc + id & not.self_ref -> id; ~F
  self_ref = index.[def_name, @node_arg1.def_refs]

  % integrate : <node, def*(inlined), def*(target), info> -> <node', def*'>
  %   (returned def node is invalid)
  integrate = (info_debug.s4 -> report_inline);
  	      [def_node.y, def_update.[z, def_size.y]]
  
  % def_update : <def*, size> -> def*'
  def_update = [def_name.x, [~INVALID, node_loc.def_node.x], 
  	        add.[def_size.x, y], % mark node-slot
  	        def_refs.x, ~T]				   % mark with extra slot

  % report_inline : <node, def*(inlined), def*(target), info>
  report_inline = dribble.cat.[~"  ", tos.node_loc.def_node.z, ~": (",
  		  	       tos.def_name.z, ~"): ", tos.def_name.y, 
			       ~" (", tos.node_loc.x, ~", ",
			       def_size.y + zero -> 
			         ~"contractable"; 
			         cat.[~"size ", tos.id],
			       ~")"]

  % any integrations done? (also remove flags)
  touched = [any.s1, s2].trans.@is_flagged
  is_flagged = [id, ge.[len, ~5]] +
  	       y -> [def_flag.x, tlr.x]; [~F, x]

  % mark definitions called only once
  mark_contractables = @countrefs.spread
  countrefs = eq.[~1, refd.[def_name.x, @def_refs.y]] ->
  	        [def_name.x, def_node.x, ~0, def_refs.x];
		x
  refd = /add.@(count.[x, @node_arg1.y]).dl

  report_contractables = @(zero.def_size -> 
  		       	     dribble.cat.[~"  ", tos.node_loc.def_node,
					  ~": ", tos.def_name, 
					  ~" is contractable"] )

}
