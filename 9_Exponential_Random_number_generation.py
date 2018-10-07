'''
Exponential_Random number generation
expriment 1
'''
import math

x=[0]*101
x[0]=1

a=pow(7, 5)
c=0
m=pow(2, 31) - 1

lamda=10


# Arrival rates per hour: 10; 

for i in range(0,100):
        x[i+1]=(a*x[i]+c)%m 
        ran_num=1.0*x[i+1]/m

        exp_rand_num = -60 * math.log(ran_num) / lamda
        
        print('the exponential random number is {}\n'.format(exp_rand_num))
