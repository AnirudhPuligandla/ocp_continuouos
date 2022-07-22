import math
from fstpso import FuzzyPSO
import numpy as np

def example_fitness( particle ):
    a=2
    b=2
    c=1
    x=a*math.cos(particle[0])*math.sin(particle[1])
    y=b*math.sin(particle[0])*math.sin(particle[1])
    z=c*math.cos(particle[1])
    controlP = np.empty((4,34),dtype=float)
    contCount=0
    for x in np.arange(-4,4.5,0.5):
        for y in range(3,5):
            controlP[:,contCount]=[x,y,0,1]
            contCount=contCount+1
    camMat=[[particle[2],particle[3],particle[4],x],
            [particle[5],particle[6],particle[7],y],
            [particle[8],particle[9],particle[10],z],
            [0,0,0,1]]
    sum=0
    for i in range(0,np.size(controlP,1)):
        imPos = np.dot(camMat,controlP[:,i])
        imX = imPos[0]/-imPos[2]
        imY = imPos[1]/-imPos[2]
        if(imX>=-1 and imX<=1):
            if(imY>=-1 and imY<=1):
                sum=sum+1
    return -sum

if __name__ == '__main__':
    dims = 1
    FP = FuzzyPSO()
    FP.set_search_space( [[0,2*math.pi-0.001],[0,math.pi/2],[-1,1],[-1,1],[-1,1],[-1,1],[-1,1],[-1,1],[-1,1],[-1,1],[-1,1]]*dims )
    FP.set_fitness(example_fitness)
    result =  FP.solve_with_fstpso(max_iter=100)
    print ("Best solution:", result[0])
    print ("Whose fitness is:", result[1])
