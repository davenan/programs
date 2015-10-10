import Prelude hiding (maximum)

maximum :: Ord a => [a] -> a
maximum [] = error “Empty List”
maximum (x:[]) = x 
maximum (x:xs) = max x (maximum xs)