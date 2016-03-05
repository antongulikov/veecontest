#!/usr/bin/env python3

import sys
import turtle
from random import randint

screen = turtle.Screen()

aS, ddS, qS, drS = 0,0,0,0
R = 50000.
xxx, yyy = 900., 500.
dist = []

drSpeed = 5
beginSpeed = 1000

colors = ['red', 'orange', 'yellow', 'green', 'blue', 'violet']

colors += colors


class Obj:

	def __init__(self, x, y, id, gara = 0):
		self.tr = turtle.Turtle()
		self.tr.speed(beginSpeed)
		self.x = x
		self.y = y
		self.gara = gara
		self.tr.penup()
		self.id = id		
		xx = float(x)
		yy = float(y)
		xx = ((xx / R) * xxx)	
		yy = ((yy / R) * yyy)
		self.xxx = int(xx)
		self.yyy = int(yy)
		self.tr.setposition(int(xx), int(yy))

	def hide(self):
		self.tr.hideturtle()


class Event:

	def __init__(self, time, type, who, id):
		self.time = time
		self.type = type
		self.who = who
		self.id = id

	def __lt__(self, other):
		return self.time < other.time



aiports = []
garages = []
persons = []
drivers = []

allw = []

def readSome(n, to):
	for i in range(n):
		x, y = list(map(int, input().split()))		
		to.append(Obj(x, y, i))		
		allw.append(to[-1])
ddd = []

ev = []
def read():
	seed = input()
	aS, dS, qS, drS = list(map(int, input().split()))	
	ddd.append(drS)	
	readSome(aS, aiports)
	readSome(dS, garages)
	readSome(qS, persons)	
	N = aS + dS + qS
	for q in range(qS):
		ev.append(list(map(int, input().split())))
	for q in range(drS):
		w = list(map(int, input().split()))
		x = w[0]		
		drivers.append(Obj(allw[x].x, allw[x].y, q, x - aS))
	for i in range(N):
		tmp = input()
	for i in range(N):
		dist.append(list(map(int, input().split())))

events = []

def readAction():
	seed = input()	
	for _ in range(ddd[0]):
		print(_)
		a, b, c = input().split()
		if c == "no":
			continue
		c = input()
		while c != "end":
			a = c.split()			
			typeq = a[0]
			timeq = int(a[1])
			who = a[2:]
			who = list(map(int, who))
			events.append(Event(timeq, typeq, who, _))
			c = input()


def doEvent(event):		
	drivers[event.id].tr.pendown()
	drivers[event.id].tr.pensize(3)
	drivers[event.id].tr.pencolor(colors[1 + drivers[event.id].gara])
	drivers[event.id].tr.speed(drSpeed)
	screen.title(str(event.time))
	if event.type == 'move':
		pos = event.who[0]		
		drivers[event.id].tr.setposition(allw[pos].tr.xcor(), allw[pos].tr.ycor())
		drivers[event.id].tr.clear()
	elif event.type[0] == 'd':
		for x in event.who:
			evs = ev[x]
			pos = -1
			if evs[0] < evs[1]:
				pos = evs[1]
			if pos > -1:
				allw[pos].tr.hideturtle()
	elif event.type[0] == 'p':
		for x in event.who:
			pos = -1
			evs = ev[x]
			if evs[0] > evs[1]:
				pos = evs[0]
			if pos > -1:			
				if allw[pos].tr.xcor() == drivers[event.id].tr.xcor() and allw[pos].tr.ycor() == drivers[event.id].tr.ycor():
					allw[pos].tr.hideturtle()


def main():
    # Create screen and turtle.        
    screen.setup (width=1000, height=600, startx=0, starty=0)
    screen.title('SCORPION')
    screen_x, screen_y = screen.screensize()
    print(screen_x, screen_y)
    read()    
    cnt = 0    
    for x in aiports:
    	x.tr.speed(drSpeed)
    	x.tr.pencolor(colors[cnt])
    	x.tr.pendown()
    	x.tr.pensize(10)
    	x.tr.circle(20)    	
    	cnt += 1
    cnt = 0    
    for x in garages:
    	cnt += 1
    	x.tr.speed(drSpeed)
    	x.tr.pencolor(colors[cnt])
    	x.tr.pendown()
    	x.tr.pensize(4)
    	x.tr.circle(10)
    readAction()
    #events.sort()
    for x in events:
    	doEvent(x)
        
if __name__ == '__main__':
    main()