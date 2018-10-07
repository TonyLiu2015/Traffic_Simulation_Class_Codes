$title warehouse location Problem

set i /1*2/;
set j /1*3/;

parameter c(i,j)/
1.1 4
1.2 5
1.3 6
2.1 6
2.2 4
2.3 3
/;

parameter f(i)/
1 50
2 1000
/;

parameter d(j)/
1 80
2 270
3 250
/;

variable z;
positive variables x(i,j);
binary variables y(i)

equations
obj                              define objective function
demand_satifiscation(j)
suppy_faciliaty(i)
;

obj.. z =e= sum((i,j),c(i,j)*x(i,j))+sum(i,f(i)*y(i));
demand_satifiscation(j).. sum(i,x(i,j)) =e= d(j);
suppy_faciliaty(i).. sum(j,x(i,j))-y(i)*sum(j,d(j)) =l= 0;

Model warehouse_facility_problem/all/ ;

solve warehouse_facility_problem using MIP minimizing z;

display x.l;
display y.l;
display z.l;

