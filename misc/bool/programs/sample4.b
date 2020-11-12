[* this is a bool sample source file.  it was an actual assignment that
 * a couple of friends of mine had and it pointed out a series of bugs
 * that i had missed.]

f0= !y | (w & !x & !z);        [ y' + wx'z' ]
f1= x & !y;                    [ xy' ]
f2= !x & !z;                   [ x'z' ]
f3= (x & z) | (w & !x & !y);   [ xz + wx'y' ]
f4= !w;                        [ w' ]
f5= w ^ x ^ y ^ z;             [ w xor x xor y xor z ]
f6= !x & y & !z;               [ x'yz' ]
f7= 1;                         [ 1 ]
