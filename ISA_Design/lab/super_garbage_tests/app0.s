
 WORD, 0, 0, 0
 WORD, 1, -1, 0
 WORD, 2, 1, 0 
 WORD, 3, 2, 0
 WORD, 4, 3, 0
 WORD, 5, -24, 0
 WORD, 6, -12, 0
 
 
 
 IN, 3, 0, 22	// $22 <= channel(2)
 

 
 // a = channel(3)
 // b = channel(3)
 // add two numbers
 IN, 4, 0, 11   // $11 <= channel(3)
 IN, 4, 0, 12   // $12 <= channel(3)
 NOR, 12, 12, 12  // $12 <= ~$12
 SUB, 12, 1, 12  // $12 <= $12 + 1
 SUB, 11, 12, 13  // $13 <= $11 - $12
 
 
 // addr = channel(3)
 // store sum at mem(addr)
 IN, 4, 0, 14          // $14 <= channel(3)
 SWAP, 14, 13, 13      // store, $0 is dummy
 SWAP, 14, 0, 13      
 SWAP, 14, 13, 0      
 
 // load $15 = mem(addr)
 SWAP, 14, 0, 15      // $15 <= mem($14), mem($14) <= $0
 SWAP, 14, 15, 0      // $0 <= mem($14), mem($14) <= $15
 
 // c = channel(3);
 // if c > (sum / 2)
 //   put c on channel(3)
 // else put (sum/2) on channel(3)
 IN, 4, 0, 16     // $16 <= channel(3)
 RSFT, 15, 0, 15   // $15 <= $15 >> 1
 SUB, 16, 15, 17    // $17 <= $16 - $15 
 BLZ, 0, 0, 19   // $19 <= PC
 SUB, 19, 5, 20 // $20 <= PC + 28
 BLZ, 17, 20, 21  // conditional branch 
 OUT, 16, 4, 0   // out sum/2
 BLZ, 0, 0, 19   // $19 <= PC
 SUB, 19, 6, 20 // $20 <= PC + 16
 BLZ, 1, 20, 21  // branch 
 OUT, 15, 4, 0   // out c  
 IN, 3, 0, 23	// $23 <= channel(2)
 SUB 23, 22, 22
 OUT, 22, 4, 0   // out execution time
 HALT, 0, 0, 0 
