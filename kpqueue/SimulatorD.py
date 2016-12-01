import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt


NREPEATS = 4  #number of times to call Dijkstra in order to measure average time
version = "multiq"

#build kpqueue
cmd = "make"
args = shlex.split(cmd)
subprocess.call(args)

def Run(beta):
	
		#run concurrent Dijkstra with betanthreads #threads
	
		cmd = "./build/src/bench/shortest_paths -b "+str(beta)+          \
                                                         " " + version  
		args = shlex.split(cmd)

		t = 0
		
		for i in range(0, NREPEATS):
		
			st = subprocess.check_output(args)
			t  += float(st)
			
			
		return t / NREPEATS
		

f = open('RunTimesDijkstra.txt', 'w')


for beta in range(0, 21):		
        parameters = Run(beta)
        f.write("beta="+str(beta) + " DijkstraRunTime= "+str(parameters)+"\n")





