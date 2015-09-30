-- Dave Nandlall
-- COP 4020 Programming Languages 
-- September 29, 2015
-- Homework 2

module AverageTailRecursive where

average :: Fractional a => [a] -> a
average []     = error "Empty list"
average (x:xs) = average_iter xs (x, 1)  


average_iter :: Fractional a => [a] -> (a, a) -> a
average_iter []     (sum, len) = sum / len
average_iter (x:xs) (sum, len) = average_iter xs (sum + x, len + 1)

