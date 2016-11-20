% assert.fp

% t : <ATOM, EXPECTED, RESULT>

t = (eq.[s2, s3] -> ~OK; _.s1).trace `test`
