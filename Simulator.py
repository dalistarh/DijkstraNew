import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt


NREPEATS = 10  #number of times to call Dijkstra in order to measure average time
seed = 0
prob = 0 #probability of having an edge, if 0 use social networsk graph
beta = 0 #probability of using two random queues

#build kpqueue

cmd = "make -C kpqueue"
args = shlex.split(cmd)

subprocess.call(args)

def RunDijkstra(version, p):
	
	time = []
	
	nthreads = 1
	while nthreads <= 32:
		#run concurrent Dijkstra with nthreads #threads
	
		cmd = "./kpqueue/build/src/bench/shortest_paths -m 4039 -n " +str(nthreads)+" -p "+str(p)+" -s "+str(seed)+" -b "+str(beta)+" "+version;
		args = shlex.split(cmd)

		t = 0.0
		
		for i in range(0, NREPEATS):
		
			ans = subprocess.check_output(args)

			st = subprocess.check_output(args);
			st = st[:-2]
			
			t  += float(st)
			
			
		time.append(t / NREPEATS)
		nthreads *= 2
		if version == "sequential": break
	return time
		

QueueVersions = ["dlsm", "globallock", "klsm", "multiq"]

f = open('RunTimes.txt', 'w')

for i in range(0, 4):
	time = RunDijkstra(QueueVersions[i], prob) 
	j = 1 
	k = 0
	while j <= 32:
		f.write(QueueVersions[i] + ' ' + str(j) + ' ' + str(time[k]) + '\n')
		j *= 2
		k += 1
		
time = RunDijkstra("sequential", prob)
f.write('Sequential 1 '+str(time[0])+'\n')














