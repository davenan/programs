-- Dave Nandlall
-- COP 4020 HW #1
-- September 15th, 2015

module AddList where

add_list_comprehension :: Integer -> [Integer] -> [Integer]
add_list_comprehension x list = [list+x | list <- list] 

add_list_recursion :: Integer -> [Integer] -> [Integer]
add_list_recursion 0 list = list
add_list_recursion x [] = []
add_list_recursion x (list:listxs) = (list + x) : add_list_recursion x listxs

add_list_map :: Integer -> [Integer] -> [Integer]
add_list_map x list = map(+x)list