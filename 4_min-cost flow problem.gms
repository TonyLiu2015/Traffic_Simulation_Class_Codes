$title min-cost flow Problem_2

set i nodes /1*5/;
alias (i, j);

parameter u(i,j) link capacity /
1. 2   15
1. 3   8
2. 3   999
2. 4   4
2. 5   10
3. 4   15
3. 5   5
5. 3   4
/;

parameter c(i,j) link cost /
1. 2   4
1. 3   4
2. 3   2
2. 4   2
2. 5   6
3. 4   1
3. 5   3
5. 3   1
/;

parameter b(i) demand /
1 20
2 0
3 0
4 -5
5 -15
/;

variable z;
positive variable f(i,j);

equations
obj                              define objective function
flow_blance_on_node(i)
link_cap(i,j)
;

obj.. z =e= sum((i,j)$(u(i,j)),c(i,j)*f(i,j));
flow_blance_on_node(i).. sum(j$(u(i,j)), f(i,j))-sum(j$(u(j,i)), f(j,i))=e= b(i);
link_cap(i,j)$(u(i,j)>0.1).. f(i,j)=l=u(i,j);

Model maximal_flow_problem /all/ ;

solve maximal_flow_problem using LP minimizing z;

display f.l;
display z.l;

