-- Dave Nandlall
-- COP 4020 HW #1
-- September 15th, 2015

module Unique where 

-- Unique takes in a list and removes duplicated elements
-- Unique keeps the first element and calls delete on rest of list
unique :: Eq a => [a] -> [a]
unique []     = []
unique [x]    = [x]
unique (x:xs) = x : unique(delete x xs)

-- Delete is a helper function, which checks for repeats recursively
-- Deletes the repeats in the list
delete :: Eq a => a -> [a] -> [a]
delete _ []                 =   []
delete x (y:ys)
              | x == y = delete x ys
              | otherwise = y : delete x ys