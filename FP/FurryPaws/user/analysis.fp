% analysis.fp - node-tree analyzer
%
% usage:
%
%  analysis:analyze <def[*], ...> -> <def*, ...>
%
%    def = <id, node>
%    def* = <id, node, size, reflist>
%    reflist = <refnode, ...>
%
%  analysis:report_stats : defs*


analysis = {

  analyze = @[def_name, def_node, treesize.def_node, collectrefs.def_node]

  treesize = index.[node_type, constnodes] -> ~1;
  	     add1./add.@treesize.node_args

  collectrefs = (?REF | ?POINTER2).node_type -> [id];
   	        constnode -> [];
  	        cat.@collectrefs.node_args

  % report program statistics
  report_stats = dribble.cat.[~"  program size: ", tos.len, 
  	       	 	      ~" definitions, total size: ",
  	       	              tos./add.@def_size]

}
