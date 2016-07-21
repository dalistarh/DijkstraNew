import subprocess
import timeit
import shlex
#import matplotlib.pyplot as plt


NREPEATS = 10  #number of times to call Dijkstra in order to measure average time
seed = 0
prob = 0

#build kpqueue

#cmd = "make -C kpqueue"
#args = shlex.split(cmd)

#subprocess.call(args)

#complie Sequential Dijkstra
cmd = "g++ DijkstraOpt.cpp -o DijkstraOpt.out -std=c++11"
args = shlex.split(cmd)

subprocess.call(args)

#run Sequential Dijkstra using popen
cmd = "./DijkstraOpt.out"
args = shlex.split(cmd)

timeDS = -1.0

for i in range (0, NREPEATS):

	p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)

	stimeDS = p.communicate(str(seed) + ' ' + str(prob))[0];
	stimeDS = stimeDS[:-2]
	if (timeDS < 0.0 or timeDS > float(stimeDS)): timeDS = float(stimeDS)
	


def RunDijkstra(version, p):
	
	time = []
	
	nthreads = 1
	while nthreads <= 32:
		#run concurrent Dijkstra with nthreads #threads
	
		cmd = "./kpqueue/build/src/bench/shortest_paths -n " +str(nthreads)+" -s "+str(seed)+" -p "+str(p)+" "+version;
		args = shlex.split(cmd)

		t = 0.0
		
		for i in range(0, NREPEATS):
		
			ans = subprocess.check_output(args)

			st = subprocess.check_output(args);
			st = st[:-2]
			
			t  += float(st)
			
			
		time.append(t / NREPEATS)
		nthreads *= 2
		
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
		
f.write('Sequential 1 '+str(timeDS)+'\n')














