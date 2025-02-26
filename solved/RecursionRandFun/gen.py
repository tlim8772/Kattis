import random
import subprocess
import sys

def generate():
    with open('t3.in', 'w') as file:
        for _ in range(1):
            a, b, c, i, k = [random.randint(0, 10), random.randint(1, 10), random.randint(1, 10), random.randint(0, 100),
                                random.randint(1, 10000)]
            file.write('%d %d %d %d %d\n' % (a, b, c, i, k))

def generate2(a, b, c, i, k):
    with open('t3.in', 'w') as file:
        file.write('%d %d %d %d %d' % (a, b, c, i, k))

def findError(trials):
    for i in range(trials):
        generate()
        file = open('t3.in', 'r')
        out = subprocess.run(['./a.out'], stdin=file, capture_output=True, text=True).stdout
        
        sys.stdout.flush()

        file.seek(0)
        ans = subprocess.run(['./test'], stdin=file, capture_output=True, text=True).stdout
        
        if (out != ans):
            print(i)
            break


def bruteForce():
    for a in range(11):
        for b in range(1, 11):
            for c in range(1, 11):
                for i in range(0, 101):
                    for k in range(1, 101):
                        generate2(a, b, c, i, k)
                        file = open('t3.in', 'r')
                        
                        out = subprocess.run(['./a.out'], stdin=file, capture_output=True, text=True).stdout
                        
                        sys.stdout.flush()

                        file.seek(0)
                        ans = subprocess.run(['./test'], stdin=file, capture_output=True, text=True).stdout
                        
                        if out != ans:
                            print("error")
                            break


findError(100)