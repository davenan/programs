-- Dave Nandlall
-- COP 4020 Programming Languages 
-- September 29, 2015
-- Homework 2

module Zip3unzip3 where
import Prelude hiding (zip3, unzip3)

-- Did not use the zip3 from Prelude, but used the zip function
-- First created a list of pairs by zipping [a] and [b], then zipped those with [c]
-- Last step was to pattern matched it to a triple
zip3 :: [a] -> [b] -> [c] -> [(a,b,c)]
zip3 xs ys zs = [(x,y,z) | ((x,y),z) <- zip (zip xs ys) zs]

-- unzip takes in a list of triples, and uses foldr to seperate the elements
-- elements are seperate by using pattern matching on the triples
-- g defines a triple with empty elements
unzip3 :: [(a,b,c)] -> ([a], [b], [c])
unzip3 xs = foldr f g xs
  where
       g                    = ([],   [],  [])
       f (a,b,c) (as,bs,cs) = (a:as, b:bs, c:cs)
    