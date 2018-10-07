$title Integer programming Problem

set i /1*2/;
variable z;
integer variables x(i);

equations
obj                              define objective function
constraint1
constraint2
;

obj.. z =e= 15*x('1')+20*x('2');
constraint1.. x('1')+2*x('2') =l= 20;
constraint2.. 2*x('1')+x('2') =l= 20;

Model linear_programming /all/ ;

solve linear_programming using MIP maximizing z;

display x.l;
display z.l;

