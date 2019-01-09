from multiprocessing import Process, Queue
import time
def f(q):
	i=0
	while(True):
		q.put([42, i, 'hello'])
		q.put([42, i, 'hello'])
		print("a")
		i=i+1

if __name__ == '__main__':
	q = Queue()
	p = Process(target=f, args=(q,))
	p.start()
	time.sleep(1)
	while(True):
		print q.get()    # prints "[42, None, 'hello']"
	p.join()
