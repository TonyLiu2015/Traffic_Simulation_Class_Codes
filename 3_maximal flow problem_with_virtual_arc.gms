$title maximal flow Problem_2

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
6. 1   100
/;

variable z;
positive variable x(i,j);

equations
obj                              define objective function
flow_blance_on_node(i)
link_cap(i,j)
;

obj.. z =e= x('6','1');
flow_blance_on_node(i).. sum(j$(u(i,j)), x(i,j))-sum(j$(u(j,i)), x(j,i))=e= 0;
link_cap(i,j)$(u(i,j)>0.1).. x(i,j)=l=u(i,j);

Model maximal_flow_problem /all/ ;

solve maximal_flow_problem using LP maximizing z;

display x.l;
display z.l;

