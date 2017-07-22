% treeshake.fp - removal of unused definitions
%
% usage:
%
%   treeshake:treeshake : <defs*, flags> -> defs*'
%
% requires:
%
%   util.fp


treeshake = {

  treeshake = loop.[x, fetch.[~DEBUG, y], 
  	            fetch.[~ENTRY_POINT, y] | configuration:entrypoint,
		    ~1]
  loop = [partition.[@used_def.dr.[spread.x, z], x], y, z, s4] + 
  	 null.s2.x -> s1.x;
           ((y -> report.[s4, s2.x]); 
	    loop.[s1.x, y, z, add1.s4])

  % used_def : <<def*, defs*>, entrypoint> -> bool
  used_def = null.s2.x -> special.[def_name.s1.x, y];
  	     eq.[def_name.s1.x, def_name.s1.s2.x] -> 
	       used_def.[[s1.x, tl.s2.x], y]; % skip current definition
	     refd.[def_name.s1.x, def_refs.s1.s2.x, y] -> ~T;
	     used_def.[[s1.x, tl.s2.x], y]
  refd = index.[x, @node_arg1.y] -> ~T; special.[x, z]
  special = eq

  report = dribble.cat.[~"  dropping unused definitions (pass ", tos.x, ~"):"];
   	   @list.y
  list = dribble.cat.[~"    ", tos.s2.s2, ~": ", tos.s1]

}
