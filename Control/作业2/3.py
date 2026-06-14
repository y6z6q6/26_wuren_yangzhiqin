import osqp
import numpy as np
from scipy import sparse

n = 2    # 变量数
m = 1    # 约束个数

P = np.array([1.0, 10.0])
#非零元素的值
Pi = np.array([0, 1])
#每一列非零元素出现在的行数 
Pj = np.array([0, 1, 2])
#Pj[0]=0
#Pj[i]=Pj[i-1]+第i-1列的非零元素个数 
P = sparse.csc_matrix((P, Pi, Pj), shape=(n, n))

q = np.array([-3.0, -30.0])

A = np.array([1.0, 1.0])
Ai = np.array([0, 0])
Aj= np.array([0, 1, 2])
A = sparse.csc_matrix((A, Ai, Aj), shape=(m, n))

l = np.array([-np.inf])
u = np.array([4.0])
#设置不等式约束的上下限
prob = osqp.OSQP()
prob.setup(P, q, A, l, u, verbose=False)
res = prob.solve()

print(f"x={res.x[0]:.6f} y={res.x[1]:.6f}")