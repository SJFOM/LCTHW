#!/usr/bin/env python

class Stack:
    def __init__(self, stack_size):
        self.nodes = []

    def push(self, value):
        self.nodes.append(value)

    def top(self):
        return self.nodes[-1]

    def _pop(self):
        return self.nodes.pop()

    def is_empty(self):
        return self.size() == 0

    def is_full(self):
        return self.size() == stack_size

    def size(self):
        return len(self.nodes)

stack_size = 10
stack = Stack(stack_size)

i = 1
while not stack.is_full():
    stack.push(i)
    i += 1

while not stack.is_empty():
    print stack.top()
    stack._pop()

