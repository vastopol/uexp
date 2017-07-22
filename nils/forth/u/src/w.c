/* useless windowing interface ; NMH 2011; in the PD */

#include "ucwlib"

void wop(op)
int op;
{
	switch (op) {
	case '?': refresh(); U(xgetch()); break;
	case '!': addch(T()); P--; break;
	case '-': raw(); break;
	case '+': noraw(); break;
	case 'a': attr(T()); P--; break;
	case 'b': beep(); break;
	case 'd': nodelay(stdscr, T()); P--; break;
	case 'e': refresh(); endwin(); break;
	case 'g': U(COLS); U(LINES); break;
	case 'i': initcurses(); break;
	case 'l': clrtoeol(); break;
	case 'm': move(T(), T2()); P-=2; break;
	case 'c': color(T(), T2()); P-=2; break;
	case 'o': U(has_colors()); break;
	case 'q': U(ttyqueue()); break;
	case 'r': refresh(); break;
	case 's': clear(); break;
#ifdef USLEEP
	case 'u': usleep(T()*1000); P--; break;
#else
	case 'u': T(); P--; break;
#endif
	case 'w': writes(&B[T2()], T()); P-=2; break;
	default: e("w: invalid op", 0); break;
	}
}
