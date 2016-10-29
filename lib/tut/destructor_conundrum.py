
import gc

class my_class:
	def __init__(this):
		this.prev = None
		this.next = None
	def __del__(this):
		# Python doesn't care about its beauty *sniff*
		# Python doesn't care about all the work I put into this code *sob*
		# Python won't run this, because it's scared of what it could do.
		print "Destroying:",this
		print "	>> prev:",this.prev
		print "	>> next:",this.next
		print "	>> prev.prev:",this.prev.prev
		print "	>> prev.next:",this.prev.next
		print "	>> next.prev:",this.next.prev
		print "	>> next.next:",this.next.next

print "Constructing"
a = my_class()
b = my_class()
c = my_class()

a.next = b
b.prev = a
b.next = c
c.prev = b

del a,b,c
gc.collect()

