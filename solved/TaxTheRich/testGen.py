from random import randrange
import subprocess
import sys

def generate(n):
    file = open('test0.in', 'w')
    file.write('%d\n' % (n))
    for i in range(n):
        file.write('%d ' % (100000))


def testRun(n):
    for i in range(n):
        generate(n)
        res1 = subprocess.run(['./a.out'], stdin=open('test0.in', 'r'), capture_output=True, text=True)
        ans1 = res1.stdout
        sys.stdout.flush()

        res2 = subprocess.run(['./test'], stdin=open('test0.in', 'r'), capture_output=True, text=True)
        ans2 = res2.stdout
        sys.stdout.flush()

        if (ans1 != ans2):
            print('error')
            break


generate(500000)