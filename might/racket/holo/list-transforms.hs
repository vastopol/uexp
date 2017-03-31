module Main where

add1 :: [Int] -> [Int]
add1 lst = 
  if null lst
  then []
  else (head lst + 1) : (add1 (tail lst))

sub1 :: [Int] -> [Int]
sub1 lst = 
  if null lst
  then []
  else (head lst - 1) : (sub1 (tail lst))



mapTest :: (a -> b) -> [a] -> [b]
mapTest f lst = 
  if null lst
  then []
  else (f (head lst)) : (mapTest f (tail lst))


mapHoist :: (a -> b) -> [a] -> [b]
mapHoist f lst = m lst
 where m lst = if null lst
               then []
               else (f (head lst)) : (m (tail lst))

mapMatch :: (a -> b) -> [a] -> [b]
mapMatch f [] = []
mapMatch f (hd:tl) = (f hd):(mapMatch f tl)


filterTest :: (a -> Bool) -> [a] -> [a]
filterTest p lst =
 if null lst
   then []
 else if (p (head lst))
   then (head lst) : (filterTest p (tail lst))
 else
   (filterTest p (tail lst))

filterMatch :: (a -> Bool) -> [a] -> [a]
filterMatch p [] = []
filterMatch p (hd:tl) | p hd = hd:(filterMatch p tl)
                      | otherwise = filterMatch p tl


abstractMap :: (c -> b -> b) -> b -> (a -> c) -> [a] -> b
abstractMap kons nil f lst = 
  if null lst
  then nil
  else kons (f (head lst)) (abstractMap kons nil f (tail lst))


foldrTest :: (a -> b -> b) -> b -> [a] -> b
foldrTest kons nil lst = 
  if null lst
  then nil
  else kons (head lst) (foldrTest kons nil (tail lst))

foldlTest :: (a -> b -> b) -> b -> [a] -> b
foldlTest kons nil lst = 
  if null lst
  then nil
  else foldlTest kons (kons (head lst) nil) (tail lst)
 

reduce :: (a -> a -> a) -> [a] -> a
reduce op []  = error "no elements in list"
reduce op [x] = x
reduce op (x:tl) = op x (reduce op tl)

myZip :: [a] -> [b] -> [(a,b)]
myZip [] [] = []
myZip (hd1:tl1) (hd2:tl2) = (hd1,hd2):(myZip tl1 tl2)


myUnzip :: [(a,b)] -> ([a],[b])
myUnzip [] = ([],[])
myUnzip ((x,y):tl) = 
 let (xs,ys) = myUnzip tl
  in (x:xs,y:ys)


unzipk :: [(a,b)] -> ([a] -> [b] -> d) -> d
unzipk [] k = k [] []
unzipk ((x,y):tl) k = 
 unzipk tl (\ xs ys -> k (x:xs) (y:ys))


partition :: (a -> Bool) -> [a] -> ([a],[a])
partition p [] = ([],[])
partition p (hd:tl) = 
 let (ins,outs) = partition p tl
  in if p hd
     then (hd:ins,outs)
     else (ins,hd:outs)


partitionk :: (a -> Bool) -> [a] -> ([a] -> [a] -> d) -> d
partitionk p [] k = (k [] [])
partitionk p (hd:tl) k =
 partitionk p tl (\ ins outs ->
  if p hd
  then k (hd:ins) outs
  else k ins (hd:outs))

main :: IO ()
main = 
 do putStrLn("hey!")
    return [ x + 1 | x <- [1,2,3], odd x ]
    return ()
