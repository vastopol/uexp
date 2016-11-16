% relationships for families
% mother, father, brother, sister, aunt, uncle, cousin, grandfather, grandmother, ...

mother(alice,lea).
mother(john,julia).
mother(lea,alberta).

father(james,alfred).
father(lea,john).

% rules defining family hierarchy
parent(X, Y) :- father(X, Y).
parent(X, Y) :- mother(X, Y).
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).

