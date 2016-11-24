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
	
		cmd = "./build/src/bench/random          -i 10000000 "+  \
                                                         " -b "+str(beta)+          \
                                                         " " + version  
		args = shlex.split(cmd)

		tmean = 0
		tmax = 0
		tdev = 0

		for i in range(0, NREPEATS):
		
			st = subprocess.check_output(args)
			ar = st.split(" ")
			tmean += float(ar[0])
			tmax += float(ar[1])
			tdev += float(ar[2])
			
		return tmean / NREPEATS, tmax / NREPEATS, tdev / NREPEATS
		

f = open('Quality.txt', 'w')


for beta in range(1, 16):		
        mn, mx, dv  = Run(beta)
        f.write("beta="+str(beta) + " mean= "+str(mn)+" max= "+str(mx)+" dev= "+str(dv)+"\n")





