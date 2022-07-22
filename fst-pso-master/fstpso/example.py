import math
from fstpso import FuzzyPSO	

def example_fitness( particle ):
    part1=particle[::2]
    part2=particle[1::2]
    return sum(map(lambda x1,x2: (math.sin(math.sqrt(x1**2+x2**2))**2 - 0.5) / (1+0.001*((x1**2+x2**2)**2))**2, part1,part2)) - 0.5

if __name__ == '__main__':
    dims = 10
    FP = FuzzyPSO()
    FP.set_search_space( [[-100, 100],[-100,100]]*dims )
    temp=[[5,15],[2,3]]*5
    tempo1=temp[::2]
    tempo2=temp[1::2]
    print(tempo1)
    print(tempo2)
    FP.set_fitness(example_fitness)
    result =  FP.solve_with_fstpso(max_iter=100)
    print ("Best solution:", result[0])
    print ("Whose fitness is:", result[1])
