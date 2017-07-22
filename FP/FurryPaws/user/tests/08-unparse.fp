% test unparser

"util.fp"
"unparse.fp"

test = ~<<`foo`, <COMP, T, 
       		  <CONS, T, 
		   <WHILE, T, <SEQ, T, 
		   	       <EQ, T, <CONST, T, 1>, <REF, T, `id`>>,
                               <REF, T, `id`> >, 
	            <COMP, T, <REF, T, `app`>, <CONS, T, <POINTER2, T, `foo`>, <ICONST, T, 42>>>>>,
		  <OR, T, <ICONST, T, 1>, <COMP, T, <REF, T, `add1`>, <ICONST, T, 0>>>>>>

main = unparse:unparse.test; id