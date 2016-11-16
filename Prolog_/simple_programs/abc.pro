
% index containing all of the cats
cat(fluffy).
cat(scruffy).

% index containing all of the dogs
dog(buddy).
dog(spike).

% index containing all of the birds
bird(peeps).
bird(chirp).

% lists of rules that apply to the above indices
can_fly(X) :- bird(X).
