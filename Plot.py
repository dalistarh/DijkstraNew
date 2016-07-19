
import matplotlib.pyplot as plt


QueueVersions = ["dlsm", "globallock", "klsm", "multiq"]
color = ['b', 'g', 'r', 'y']


f = open('RunTimes.txt', 'r')


plt.xlabel('number of threads')
plt.ylabel('runtime in seconds')

mx = 0.0

for i in range(0, 4):
	x = []
	y = []
	j = 1 
	k = 0
	while j <= 32:
		line = f.readline()
		a = line.split(' ')

		x.append(int(a[0]))
		y.append(float(a[1]))
		
		if float(a[1]) > mx: mx = float(a[1])
		
		j *= 2
		k += 1
		
	plt.plot(x, y, '.'+color[i]+'-') #plot dijkstra with locks in red
		
line = f.readline()
a = line.split(' ')
#plt.plot(int(a[0]), float(a[1]), 'k') 
#if float(a[1]) > mx: mx = float(a[1])

plt.ylim(0.0, mx)


print float(a[1])

plt.show()
	
	
	


		
















