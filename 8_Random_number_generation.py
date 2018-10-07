'''
Random number generation
expriment 1
'''
x=[0]*101

x[0]=1

a=pow(7, 5)
c=0
m=pow(2, 31) - 1

for i in range(0,100):
        x[i+1]=(a*x[i]+c)%m 
        ran_num=1.0*x[i+1]/m
        print(ran_num)
