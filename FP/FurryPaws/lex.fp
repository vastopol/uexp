%
% lex.fp - FP lexer
%


% Converts input-characters into tokens.

% exports:
%
%   lex:lex : filename -> <token, ...>
%   lex:lex_string : <file, string> -> <token, ...>
%
%   token = <class, loc, val>
%   class = ID | ATOM | SYM  (val = atom)
%   	    NUM              (val = num)
%           STRING    	     (val = string)
%   loc = atom  (source location)

% requires:
%
%   dribble, quit


lex = {

  lex = loop.skip_shebang.[_in, [~1, toa], []]
  lex_string = loop.skip_shebang.[y, [~1, toa.x], []]
  
  skip_shebang = [|?$#, ?$!, ..|].input -> skip_comment

  % loop : state -> <token, ...>
  %
  %   state = <input, loc, <token1, ...>>

  loop = (eof -> tokens;			% end of input reached?
	  eq.[next, ~$-] ->   			% minus sign?
	    (null.tl.input ->			% single $- at end of file?
  	      tokens.push.[[~SYM, sourceinfo, ~`-`],
			   [~<>, location, tokens]];
	     digit.s2.input ->   	        % negative number?
 	       loop.push.[[~NUM, sourceinfo.s1, neg.ton.s2], 
		          s1].scan.[numchars, advance];
	     eq.[s2.input, ~$>] ->	        % arrow?
  	       loop.push.[[~SYM, sourceinfo, ~`->`], 
 	                  [tl.tl.input, location, tokens]];
  	     loop.push.[[~SYM, sourceinfo, ~`-`], % other symbol
		        [tl.input, location, tokens]]);
	  eq.[next, ~$.] ->	   	        % one or two dots?
	    (null.tl.input | not.eq.[~$., s2.input] ->
    	       loop.push.[[~SYM, sourceinfo, ~`.`], [tl.input, location, tokens]];
    	       loop.push.[[~SYM, sourceinfo, ~`..`], [tl.tl.input, location, tokens]]);
	  eq.[next, ~$[] ->		        % open bracket?
	    (null.tl.input -> 			% single bracket at end of file?
  	      tokens.push.[[~SYM, sourceinfo, ~`[`],
			   [~<>, location, tokens]];
	     eq.[s2.input, ~$|] ->		% `[|' sequence?
	       loop.push.[[~SYM, sourceinfo, ~`[|`],
	       		  [tl.tl.input, location, tokens]];
	     loop.push.[[~SYM, sourceinfo, ~`[`],
	      		[tl.input, location, tokens]]);
	  eq.[next, ~$(] ->		        % open paren?
	    (null.tl.input -> 			% single paren at end of file?
  	      tokens.push.[[~SYM, sourceinfo, ~`(`],
			   [~<>, location, tokens]];
	     eq.[s2.input, ~$|] ->		% `(|' sequence?
	       loop.push.[[~SYM, sourceinfo, ~`(|`],
	       		  [tl.tl.input, location, tokens]];
	     loop.push.[[~SYM, sourceinfo, ~`(`],
	      		[tl.input, location, tokens]]);
	  eq.[next, ~$|] ->		        % vbar
	    (null.tl.input -> 			% vbar at end of file?
  	      tokens.push.[[~SYM, sourceinfo, ~`|`],
			   [~<>, location, tokens]];
	     eq.[s2.input, ~$]] ->		% `|]' sequence?
	       loop.push.[[~SYM, sourceinfo, ~`|]`],
	       		  [tl.tl.input, location, tokens]];
	     eq.[s2.input, ~$)] ->		% `|)' sequence?
	       loop.push.[[~SYM, sourceinfo, ~`|)`],
	       		  [tl.tl.input, location, tokens]];
	     loop.push.[[~SYM, sourceinfo, ~`|`],
	      		[tl.input, location, tokens]]);
	  %XXX lex `{|' and `|}' as tokens (not used yet)
          eq.[next, ~$$] ->	 	   	% character constant
	    loop.push.[[~NUM, sourceinfo, next], advance].advance;
	  eq.[next, ~$`] ->   		        % quoted atom
	    loop.push.[[~ATOM, sourceinfo.s1, s2], s1].scanquoted.[advance, ~$`];
	  eq.[next, ~$"] ->   		        % string (stored as atom)
	    loop.push.[[~STRING, sourceinfo.s1, s2], s1].scanquoted.[advance, ~$"];
	  idchar.next ->      		        % identifier
	    loop.push.[[~ID, sourceinfo.s1, toa.s2], s1].scan.[idchars, id];
	  (digit.next -> ~T; eq.[~$-, next] -> ~T; ~F) -> % number
	    loop.push.[[~NUM, sourceinfo.s1, ton.s2], s1].scan.[numchars, id];
	  atomchar.next ->      		% atom?
	    loop.push.[[~ATOM, sourceinfo.s1, toa.s2], s1].scan.[atomchars, id];
	  symbol.next ->      		     	% other symbol?
    	    loop.push.[[~SYM, sourceinfo, toa.[next]],
		       [tl.input, location, tokens]];
	  fail.[cat.[~"illegal syntax: `", [next], ~"'"], id]).skip

  % scan chars from given set:
  % 
  % scan : <set, state> -> <state, string>

  scan = [s1, s3].scan'.[s2, s1, []]		
  scan' = eof.s1 -> id;  	    		% scan' : <state, set, string>
  	  index.[next.s1, s2] ->
  	    scan'.[advance.s1, s2, ar.[s3, next.s1]];
	  id

  % scanquoted : <state, delimiter> -> <state, atom>

  scanquoted = [s1, toa.s2].scanquoted'.[s1, s2, []]
  scanquoted' = eof.x -> 	        % scanquoted' : <state, delimiter, string>
  	          fail.[~"unexpected end of literal", x];
  	        eq.[next.x, ~$\] ->
		  ([advance.x, y, z] +
		   (eof.x -> 
		     scanquoted'.[x, y, z];
 		     scanquoted'.[advance.x, y, ar.[z, escaped_char.next.x]]));
  	        eq.[next.x, y] -> [advance.x, z];
		scanquoted'.[advance.x, y, ar.[z, next.x]]
  escaped_char = ?$n -> ~10;
  	         ?$r -> ~13;
		 ?$t -> ~9;
		 id

  % state operations

  input = s1					% state accessors
  location = s2
  tokens = s3

  eof = null.input
  next = eof ->					% gives next available char
           fail.[~"unexpected end of input", id];
           s1.input
  skip = eof -> id;				% skip whitespace characters
         whitespace.next ->
           skip.advance;
	 eq.[next, ~$%] -> 
	   skip.skip_comment;
	 id
  advance = eq.[next, ~10] -> 			% advance input position
              [tl.input, [add1.s1.location, s2.location], tokens];
	    [tl.input, location, tokens]
  skip_comment = eof -> id;	 		% skip until eof or newline
  	       	 eq.[next, ~10] -> advance;
  	         skip_comment.advance

  % push : <X, state> -> state'			% append token

  push = [input.s2, location.s2, ar.[tokens.s2, s1]]

  % sourceinfo : state -> atom

  sourceinfo = toa.cat.[tos.s2, ~<$:>, tos.s1].location

  % character classes

  symbolchars = ~"@/\\~=:;(){}[]<>.,-&|+?!#^*"
  numchars = ~"0123456789"
  idchars = ~"abcdefghijklmnopqrstuvwxyz0123456789_%$'?!"
  atomchars = ~"ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_%$'?!"

  % character predicates

  whitespace = le.[id, ~$ ]
  upper = ge.[id, ~$A] -> le.[id, ~$Z]; ~F
  lower = ge.[id, ~$a] -> le.[id, ~$z]; ~F
  digit = ge.[id, ~$0] -> le.[id, ~$9]; ~F
  symbol = index.[id, symbolchars]
  idchar = ?$_ | lower
  atomchar = upper

  % lexer error:
  %
  % fail : <message, state>

  fail = quit.toa.cat.[tos.s2.location.y, ~":", 
       	               tos.s1.location.y, ~": error: ",
	   	       x]

}
