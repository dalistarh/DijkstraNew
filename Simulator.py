import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt


NREPEATS = 10  #number of times to call Dijkstra in order to measure average time
seed = 0
prob = 0.1 #probability of having an edge
betaValues = [1, 5, 15, 20]  #probability of using two random queues
nvertices = 10000
nthreads = 64
nqueues = 64
version = "multiq"

#build kpqueue
cmd = "make -C kpqueue"
args = shlex.split(cmd)
subprocess.call(args)

def RunDijkstra(beta):
	
		#run concurrent Dijkstra with betanthreads #threads
	
		cmd = "./kpqueue/build/src/bench/shortest_paths"+" -m " + str(nvertices)+ \
                                                         " -n " +str(nthreads)+  \
                                                         " -p " + str(prob)+        \
                                                         " -s "+str(seed)+      \
                                                         " -b "+str(beta)+          \
                                                         " -q "+str(nqueues)+       \
                                                         " " + version  
		args = shlex.split(cmd)

		t = 0.0
		
		for i in range(0, NREPEATS):
		
			st = subprocess.check_output(args)
			t  += float(st)
			
			
		return t / NREPEATS
		

f = open('RunTimes.txt', 'w')

f.write(str(nvertices)+" nodes;"+str(nthreads)+" threads;"+str(nqueues)+" queues;"+str(prob)+" edge probability;"+"\n");

for beta in betaValues:		
        time = RunDijkstra(beta)
        f.write("time="+str(time)+";"+"beta="+str(beta)+"\n")


















