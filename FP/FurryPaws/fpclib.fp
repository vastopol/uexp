%
% fpclib.fp - FP->C compiler
%

%XXX use functional I/O instead of directly calling side-effecting intrinsics


"version.fp"
"configuration.fp"
"util.fp"
"lex.fp"
"parse.fp"
"analysis.fp"
"check.fp"
"treeshake.fp"
"inline.fp"
"rewrite.fp"
"c-literals.fp"
"c-backend.fp"
"unparse.fp"


% toplevel compiler driver

toplevel = {

  % run : <sourcefile, outputfile, flags, ccoptions>

  run = [y, s4, parse.[x, z]] +
	[x, y, z, (debugdump.s2.z -> (dribble.~"=== nodes:"; _show.s1.z))] +
        compile.[s1.z, x, s2.z, y]

  % parse : <sourcefile, flags> -> <defs, flags'>

  parse = [y, cat.[lex:lex.prelude.y, lex:lex.x]] +
  	  [x, y, debugdump.x -> (dribble.~"=== tokens:"; _show.y)] +
	  [x, partition.[@atom, id].parse:parse.y] +
  	  [s2.y, cat.[x, @[~INCLUDE, id].s1.y]]

  % optimize : <defs, flags> -> defs'

  optimize = loop

  % compile : <sourcefile, outputfile, flags, ccoptions> -> message

  compile = [tl, loop.[x, z]] +
	    (fetch.[~NODES, s2.x] -> (_show.y; terminate);
	     fetch.[~UNPARSE, s2.x] -> (unparse:unparse.y; terminate);
	    ([x, y, literals:extract.y] +
 	     [x, y, z, 
	      debugdump.s2.x -> 
	        (dribble.~"=== literals:"; @_show.trans.[@sub1.iota.len.z, z]) ] +
	     [x, literals:index_literals.[y, z], z] +
	     [x, backend:translate.[y, s2.x, z]] +
	     [x, _out.[s1.x, y]] +
 	     fetch.[~COMPILE, s2.x] -> cc_and_link.[s1.x, s3.x, s2.x]);
             terminate)

  prelude = fetch.[~PRELUDE, id] | configuration:prelude

  % optimization loop:
  %
  % loop : <defs*, flags> -> defs*'

  loop = loop'.[~1, x, y]
  loop' = (debug.z -> dribble.cat.[~"optimization pass ", tos.x]);
  	  [x, analysis:analyze.y, z] +
  	  ((debug.z -> analysis:report_stats.y);
	   (eq.[~1, x] & not.fetch.[~NO_CHECK, z] ->
	     (check:check.y -> quit.~"unresolved");
	     id) ) +
 	  [x, treeshake:treeshake.[y, z], z] +
	  [x, inline:mark_contractables.y, z] +
	  [x, y,
	   (debug.z -> (dribble.~"inlining"; inline:report_contractables.y));
	   z] +
	  [x, 
	   inline:inline.[y, fetch.[~LIMIT, z] | configuration:limit,
	 	          debug.z ],
           z] +
  	  [x, z, inline:touched.y] +
	  ((debugdump.y -> (dribble.~"=== nodes:"; unparse:unparse.s2.z; s2.z));
	   s1.z ->
	     loop'.[add1.x, s1.rewrite:rewrite.[s2.z, x, y], y];
	     ((debug.y -> analysis:report_stats.s2.z); 
	      % do rewrites after report, as it removes size from defs
	      ([x, rewrite:rewrite.[s2.z, x, y], y] +
	       s2.y ->		% some rules fired?
	         loop'.[add1.x, s1.y, z];
		 s1.y)))

  % cc_and_link : <cfile, ccoptions, flags>

  cc_and_link = [cat.join.[~" ", cat.[[cc, configuration:cflags, x], y]], z] +
		(debug.y -> (dribble.cat.[~"  ", x]; x); x) +
  	        (zero._system -> ~T;
		 quit.toa.cat.[~"command failed with non-zero exit status: ", id])
  cc = tos.(_env.~CC | configuration:cc)

  usage = quit.~"usage: fpc [-help] [-version] [-debug] [-dump] [-c] [-heap HEAPSIZE] 
           [-<ccoption>] [-o FILENAME] [-include FILENAME] [-prelude FILENAME] 
           [-limit LIMIT] [-no-check] [-nodes] [-unparse] [-entry-point NAME]
	   FILENAME"

  debug = fetch.[~DEBUG, id]
  debugdump = fetch.[~DEBUGDUMP, id]

  % options : <arg1, ...> -> <sourcefile, outputfile, flags, ccoptions>

  options = options'.[@tos, [~F, ~F, [], []]]
  options' = null.x -> ((options''.y) +
  	     	        (fetch.[~COMPILE, z] ->
			  [x, replace_ext.[y, ~"c"], z, cat.[s4, [~"-o", remove_c_ext.y]]];
			  id));
             null.s1 -> usage;
	     eq.[~"-help", s1.x] -> usage;
	     eq.[~"-version", s1.x] -> (_emit.cat.[configuration:version, nl]; terminate);
	     eq.[~"-debug", s1.x] -> options'.[tl.x, flag.[~DEBUG, y]];
	     eq.[~"-dump", s1.x] -> 
	       options'.[tl.x, flag.[~DEBUG, flag.[~DEBUGDUMP, y]]];
	     eq.[~"-c", s1.x] -> options'.[tl.x, flag.[~COMPILE, y]];
	     eq.[~"-limit", s1.x] -> inline_limit.[tl.x, y];
	     eq.[~"-heap", s1.x] -> heap_size.[tl.x, y];
	     eq.[~"-o", s1.x] -> output_file.[tl.x, y];
	     eq.[~"-prelude", s1.x] -> alternative_prelude.[tl.x, y];
	     eq.[~"-include", s1.x] -> include_file.[tl.x, y];
	     eq.[~"-nodes", s1.x] -> options'.[tl.x, flag.[~NODES, y]];
	     eq.[~"-unparse", s1.x] -> options'.[tl.x, flag.[~UNPARSE, y]];
	     % disable checks, in case set of intrinsics change
	     eq.[~"-no-check", s1.x] -> options'.[tl.x, flag.[~NO_CHECK, y]];
	     eq.[~"-entry-point", s1.x] -> alternative_entry_point.[tl.x, y];
	     eq.[~$-, s1.s1.x] -> ccoption;
	     source_file
  options'' = not.x -> usage;
              not.y -> [x, replace_ext.[x, ~"c"], z, s4];
	      id

  % replace_ext : <filename, newextension> -> filename'
  % remove_c_ext : filename -> filename'
  replace_ext = [x, y, indexr.[~$., x]] +
  	        (z -> cat.[take.[z, x], y]; cat.[x, ~".", y])
  remove_c_ext = eq.[~"c.", take.[~2, rev]] -> % remove .c extension
  	           take.[sub.[len, ~2], id];
	           id

  flag = [s1.y, s2.y, ar.[s3.y, [x, ~T]], s4.y]
  flag2 = [s1.z, s2.z, ar.[s3.z, [x, y]], s4.z]
  output_file = null.x -> usage;
  	        options'.[tl.x, [s1.y, s1.x, s3.y, s4.y]]
  source_file = options'.[tl.x, [s1.x, s2.y, s3.y, s4.y]]
  ccoption = options'.[tl.x, [s1.y, s2.y, s3.y, ar.[s4.y, s1.x]]]
  heap_size = null.x -> usage;
  	      options'.[tl.x, [s1.y, s2.y, s3.y, 
	      		       ar.[s4.y, cat.[~"-DDEFAULT_HEAP_SIZE=", 
			       		      tos.option_value.s1.x]]]]
  alternative_prelude = null.x -> usage;
  		      	options'.[tl.x, flag2.[~PRELUDE, s1.x, y]]
  alternative_entry_point = null.x -> usage;
  			    options'.[tl.x, flag2.[~ENTRY_POINT, toa.s1.x, y]]
  include_file = null.x -> usage;
	         options'.[tl.x, flag2.[~INCLUDE, s1.x, y]]
  inline_limit = null.x -> usage;
  	         options'.[tl.x, flag2.[~LIMIT, option_value.s1.x, y]]

  % option_value : string -> num (handles 'k'/'m'/'g' suffix)
  option_value = [id, not.zero.len & s1r] +
  	       	 index.[y, ~<$k, $K>] -> mul.[~1024, ton.tlr.x];
  	       	 index.[y, ~<$m, $M>] -> mul.[~1048576, ton.tlr.x];
  	       	 index.[y, ~<$g, $G>] -> mul.[~1073741824, ton.tlr.x];
		 ton.x

}
