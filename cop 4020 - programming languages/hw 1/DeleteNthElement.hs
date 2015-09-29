-- Dave Nandlall
-- COP 4020 HW #1
-- September 15th, 2015

module DeleteNthElement where

deleteNthElement :: Int -> [a] -> [a]
deleteNthElement n list =
                  if n == 0 then drop (n+1) list
                  else take(n) list ++ drop (n+1) list
