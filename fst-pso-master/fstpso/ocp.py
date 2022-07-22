import sys
import math
from fstpso import FuzzyPSO
import numpy as np
import struct

a=None
b=None
c=None
dims=None
controlP=None
e_cent=None
world_up = [[0.0,0.0,1.0]]
numVars = 5

def example_fitness( particle ):
    numControl = np.size(controlP,1)
    cover = np.zeros((numControl,),dtype=bool)
    # loop over each dimension
    for camNum in range(0,dims):
        currInd = camNum*numVars
        currVars = particle[currInd:currInd+numVars]
        # get xyz in cartesian coordinates form spherical corrdinates
        x=a*math.cos(currVars[0])*math.sin(currVars[1])
        y=b*math.sin(currVars[0])*math.sin(currVars[1])
        z=c*math.cos(currVars[1])
        # translate the point w.r.t. to world
        C_local = np.array([x,y,z]).transpose()
        C = C_local + e_cent
        # normalize the cam direction vector
        camDirTemp = np.array([[currVars[2],currVars[3],currVars[4]]])
        camDir = camDirTemp / np.sqrt(np.sum(camDirTemp**2))
        # get camup and camright vectors (x,y axes of camera)
        camRightTemp = np.cross(camDir, world_up)
        camRight = camRightTemp / np.sqrt(np.sum(camRightTemp**2))
        camUpTemp = np.cross(camRight,camDir)
        camUp = camUpTemp / np.sqrt(np.sum(camUpTemp**2))
        # rotate t=-rc
        R = np.concatenate((camRight,camUp,camDir), axis=0)
        t = -(R @ C)
        # create the camera matrix
        M_temp = np.concatenate((R,t[:,None]), axis=1)
        M = np.concatenate((M_temp,[[0,0,0,1]]), axis=0)
        for i in range(0,numControl):
            imPos = M @ controlP[:,i]
            # considers only the camera pointing towards the camera
            if(imPos[2] > 0):
                imX = imPos[0]/imPos[2]
                imY = imPos[1]/imPos[2]
                if(abs(imX)<=1):
                    if(abs(imY)<=1):
                        cover[i] = 1
    return -np.sum(cover)

if __name__ == '__main__':
    # ellipsoid parameters
    a = float(sys.argv[1])
    b = float(sys.argv[2])
    c = float(sys.argv[3])
    # number of cameras
    dims = int(sys.argv[4])
    # ellipsoid center
    x_e = float(sys.argv[5])
    y_e = float(sys.argv[6])
    z_e = float(sys.argv[7])
    e_cent = np.array([x_e,y_e,z_e]).transpose()
    # read control points from file
    filePath = "/media/anirudh/Data/Documents/PhD/optimal camera placement/python/fst-pso-master/fstpso/"
    with open(filePath+"cont_points.dat", mode='rb') as f:
        controlP=np.fromfile(f,dtype='>f8')
    numCont = int(len(controlP)/4)
    controlP = controlP.reshape(numCont,4).transpose()
    # optimization
    FP = FuzzyPSO()
    FP.set_search_space( [[0,2*math.pi-0.001],[0,math.pi/2],[-1,1],[-1,1],[-1,1]]*dims )
    FP.set_fitness(example_fitness)
    result =  FP.solve_with_fstpso(max_iter=100)
    print ("Best solution:", result[0])
    print ("Whose fitness is:", result[1])
    # write the best solution to a file
    finResult = np.array(result[0].X)
    finResult.astype('>f8').tofile(filePath+"out_result.dat")
