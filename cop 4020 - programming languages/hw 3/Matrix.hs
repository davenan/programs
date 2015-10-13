-- Dave Nandlall
-- COP 4020 - Programming Languages
-- October 8th, 2015
-- Homework Assignment #3

-- Moved each type signature above its corresponding function

module Matrix (Matrix, fillWith, fromRule, numRows, numColumns, 
               at, mtranspose, mmap, add, mult) 
where

-- newtype is like "data", but has some efficiency advantages
newtype Matrix a = Mat ((Int,Int),(Int,Int) -> a)

-- Without changing what is above, implement the above functions.
fillWith   :: (Int,Int) -> a -> (Matrix a)
fillWith (m,n) element = Mat ((m,n), \(i,j) -> element)

-- Creates a matrix of size (m x n), along with a rule
-- The rule is applied to the matrix when it is created
fromRule   :: (Int,Int) -> ((Int,Int) -> a) -> (Matrix a)
fromRule (m,n) f = Mat ((m,n), \(i,j) -> (f(i,j)))

-- m defines the # of rows within the matrix
numRows    :: (Matrix a) -> Int
numRows (Mat ((m,n), f)) = m

-- n defines the # of columns within the matrix 
numColumns :: (Matrix a) -> Int
numColumns (Mat ((m,n), f)) = n

-- First param is a matrix, second is an index in the matrix
-- This returns the tuple at a specified (row, col) in the matrix
at         :: (Matrix a) -> (Int, Int) -> a
at (Mat ((m,n), f)) (i,j) = f(i,j)

-- This was for the 'at' function, it checks wether the tuple
-- that is being searched for is in bounds. It works, however it
-- fails when it is used to implement the test cases given. I figured
-- I would leave it so it can be checked.
-- if ( i > m || j > n)
-- then error "Matrix out of bounds"
-- else f(i,j)

-- Each tuple within the (n x m) matrix is flipped
-- each index will then contain the flipped ordered pair
mtranspose :: (Matrix a) -> (Matrix a)
mtranspose (Mat((m,n), f)) = (Mat((m,n), \(i,j) -> f(j,i)))

-- This function takes in an (m x n) matrix and applies a function
-- to each tuple in the matrix
mmap       :: (a -> b) -> (Matrix a) -> (Matrix b)
mmap f (Mat((m,n), g)) = Mat ((m,n), (\(m,n) -> f(g(m,n))))


-- @params Matrix A and Matrix B
-- check the size of each matrix first, if they are not the same size then its an error
-- else we perform the operation which is add
-- this function makes calls to checkSize and performOp...
add       :: Num a => (Matrix a) -> (Matrix a) -> (Matrix a)
add (Mat((a,b), f)) (Mat((c,d), g)) = if (checkSize (Mat((a,b), f)) (Mat((c,d), g)))
                                      then performOp (Mat((a,b), f)) (+) (Mat((c,d), g))
                                      else error "Not the same size, can't be added"

-- @params Matrix A and Matrix B
-- check the size of each matrix first, if they are not the same size then its an error
-- else we perform the operation which is multiplication
-- this function makes calls to checkSize and performOp...
-- (Need to debug)
mult       :: Num a => (Matrix a) -> (Matrix a) -> (Matrix a)
mult (Mat((a,b), f)) (Mat((c,d), g)) = if (checkSize (Mat((a,b), f)) (Mat((c,d), g)))
                                       then performOp (Mat((a,b), f)) (*) (Mat((c,d), g))
                                       else error "Not the same size, can't be multiplied"


-- checkSize compares two matricies and checks the size 
-- Uses numRows and numColumns functions from above, to see if the matricies are equal
checkSize :: Matrix a -> Matrix b -> Bool
checkSize m1 m2 = if(numRows m1 == numRows m2 && numColumns m1 == numColumns m2)
                  then True
                  else error "The matricies are different sizes"


-- This function takes two matricies and an operation and creates a new matrix
performOp :: Matrix a -> (a -> a -> b) -> Matrix a -> Matrix b
performOp (Mat((a,b), f)) op (Mat((c,d), g)) = (Mat((a,b), \(a,b) -> ((f(a,b)) `op` (g(a,b)))))
