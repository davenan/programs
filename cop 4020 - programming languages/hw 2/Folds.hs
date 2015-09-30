-- Dave Nandlall
-- COP 4020 Programming Languages 
-- September 29, 2015
-- Homework 2

-- Tried foldr on count and foldl on elem2, caused the hang...
-- So this is the order that worked on the test cases

module Folds where

-- foldl by using the function f, which adds to counter
-- if the element we are searching for is found in the list, else count stays same
count :: Eq a => a -> [a] -> Integer
count x = foldl f 0 
          where f = (\count element -> if x == element then (count + 1) else count)

-- foldr by using the function f, which checks the second element y
-- by comparing it with the element we are looking for, if a match return True
-- if there is no match then we just return False
elem2 :: Eq a => a -> [a] -> Bool
elem2 x = foldr f False
          where f = (\y list -> if x == y then True else list)