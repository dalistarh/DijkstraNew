import subprocess
import timeit
import shlex
import matplotlib.pyplot as plt


NREPEATS = 5  #number of times to call Dijkstra in order to measure average time
seed = 0
prob = 0

#complie Sequential Dijkstra
cmd = "g++ DijkstraS1.cpp -o DijkstraS1.out -std=c++11"
args = shlex.split(cmd)

subprocess.call(args)

#run DeSequential Dijkstra using popen
cmd = "./DijkstraS1.out"
args = shlex.split(cmd)

p = subprocess.Popen(args, stdin=subprocess.PIPE, stdout=subprocess.PIPE)

#timeDS = p.communicate(str(seed) + ' ' + str(prob))[0];
ansDS = p.communicate(str(seed) + ' ' + str(prob))[0]

#timeDS = timeDS[:-2]

def RunDijkstra(version, p):
	
	time = []
	
	nthreads = 1
	while nthreads <= 32:
		#run concurrent Dijkstra with nthreads #threads
	
		cmd = "./shortest_paths -n " +str(nthreads)+" -s "+str(seed)+" -p "+str(p)+" "+version;
		args = shlex.split(cmd)

	
		t = 0.0
		
		for i in range(0, NREPEATS):
		
			ans = subprocess.check_output(args)
			if (ans != ansDS): print 'faaaaalse'
			#st = subprocess.check_output(args);
			
			#print st
			
			#st = st[:-2]
			#t  += float(st)
			
			
		#print t / NREPEATS
		#time.append(t / NREPEATS)
		nthreads *= 2
		
	#return time
		

QueueVersions = ["dlsm", "globallock", "klsm", "multiq"]

#f = open('RunTimes.txt', 'w')

for i in range(0, 4):
	#time = 
	RunDijkstra(QueueVersions[i], prob) 
	#j = 1 
	#k = 0
	#while j <= 32:
		#f.write(str(j) + ' ' + str(time[k]) + '\n')
		#j *= 2
		#k += 1
		
#f.write('1 '+timeDS+'\n')
print ansDS
















