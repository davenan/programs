-- Dave Nandlall
-- COP 4020 Programming Languages 
-- September 29, 2015
-- Homework 2

-- For this problem I did look at 2 different online sources
-- Source: http://learnyouahaskell.com/zippers
-- Source: http://aryweb.nl/2013/10/28/haskell-tree-traversal/

module Tree where

data Tree a = Nil | Node a (Tree a) (Tree a) 
                deriving (Eq, Show)

depth :: Tree a -> Integer
depth Nil            = 0
depth (Node n t1 t2) = 1 + max (depth t1) (depth t2)

-- collapses a tree into a list by visiting 
-- the elements of the tree 'inorder'
collapse :: Tree a -> [a]
collapse Nil            = []
collapse (Node x t1 t2) = collapse t1 ++ [x] ++ collapse t2

-- Stratify calls the function BFS
-- The binary tree is passed to BFS
stratify :: Tree a -> [a]
stratify tree = bfs[tree]

-- BFS runs a breadth first traversal or level order traversal of the tree
bfs :: [Tree a] -> [a]
bfs []   = []
bfs tree = concatMap root tree ++ bfs(concatMap nodes tree)

-- Root defines the root node of the binary tree
-- Empty root, return an empty list, if not return the root
root :: Tree a -> [a]
root  Nil           = []
root (Node x _ _)   = [x] 

-- Nodes defines the children of each parent in a binary tree
-- Traverse the left subtree t1, then traverse the right subtree t2
nodes :: Tree a -> [Tree a]
nodes  Nil           = []
nodes (Node x t1 t2) = [t1] ++ [t2]