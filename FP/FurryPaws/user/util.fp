%
% util.fp - various support routines
%


dribble = _emit; _emit.~10

terminate = _throw.~<EXIT, 0>
quit = _emit; _emit.~10; _throw.~<EXIT, 70>

s5 = select.[~5, id]

nl = [~10]

% node accesors
node_type = s1
node_loc = s2
node_args = tl.tl
node_arg1 = s3
node_arg2 = s4
node_arg3 = s5
  
% definition accessors
def_name = s1
def_node = s2
def_size = s3
def_refs = s4
def_flag = s5
def_loc = node_loc.def_node

constnode = index.[node_type, constnodes]
constnodes = ~<ICONST, CONST, EXTERN, REF, POINTER2>

constresult = index.[node_type, ~<POINTER2, ICONST, CONST>]
