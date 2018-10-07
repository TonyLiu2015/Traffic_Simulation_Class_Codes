$title maximal flow Problem

set i nodes /1*6/;
alias (i, j);

parameter u(i,j) link capacity /
1. 2   2
1. 3   8
2. 4   3
2. 5   4
3. 4   4
3. 5   2
4. 6   1
5. 6   5
/;

parameter origin(i);
origin('1') = 1;

parameter destination(i);
destination('6') = 1;

parameter intermediate_node(i);
intermediate_node(i) = (1- origin(i))*(1- destination(i));

variable z;
positive variable v;
positive variable x(i,j);

equations
obj                              define objective function
flow_on_node_origin
flow_on_node_intermediate(i)
flow_on_node_destination
link_cap(i,j)
;

obj.. z =e= v;
flow_on_node_origin.. sum(j$(u('1',j)), x('1',j)) =e= v;
flow_on_node_intermediate(i)$(intermediate_node(i)=1).. sum(j$(u(i,j)), x(i,j))-sum(j$(u(j,i)), x(j,i))=e= 0;
flow_on_node_destination..  sum(j$(u(j,'6')), x(j,'6'))=e= v;
link_cap(i,j)$(u(i,j)>0.1).. x(i,j)=l=u(i,j);

Model maximal_flow_problem /all/ ;

solve maximal_flow_problem using LP maximizing z;

display x.l;
display v.l;
display z.l;

