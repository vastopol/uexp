* MINP.inc - MINP(S) will return the minimum number of print
*	     positions needed to print the string S.
*
-INCLUDE "spacing.inc"
-INCLUDE "count.inc"
	DEFINE('MINP(S)T')
						:(MINP_END)
MINP	MINP  =  SPACING(S)
	EQ(JUSTIFY, 0)				:S(RETURN)
	MINP  =  MINP - COUNT(S,'  ')		:(RETURN)
MINP_END
