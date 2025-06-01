import random
import string
import subprocess
import sys


def generate(len: int):
    with open('t3.in', 'w') as file:
        file.write('%d\n' % random.randint(1, 3))
        
        str = ''.join(random.choices(string.ascii_lowercase, k=len))
        file.write('%s\n' % str)

        file.write('%d\n' % 0)


def runTests(tries: int, strlen: int):
    generate(strlen)
    
    for i in range(tries):
        res1 = subprocess.run(['./a.out'], stdin=open('t3.in', 'r'), capture_output=True, text=True)
        out1 = res1.stdout
        sys.stdout.flush()

        res2 = subprocess.run(['./test'], stdin=open('t3.in', 'r'), capture_output=True, text=True)
        out2 = res2.stdout
        sys.stdout.flush()

        if out1 != out2:
            print("error")
            break



runTests(5, 10000)