import random
import subprocess
import sys


cities = []
for i in range(100):
    cities.append("c" + str(i))


def generate(n):
    file = open('test0.in', 'w')
    #file.write('%d\n' % (n * (n - 1) / 2))
    file.write('%d\n' % (n * (n - 1) / 2))
    for i in range(0, n):
        for j in range(i + 1, n):
            #if j > i + 1: break
            option = random.randint(0, 9)
            if option <= 8:
                file.write('%s %s\n' % (cities[i], cities[j]))
            else:
                file.write('%s %s\n' % (cities[j], cities[i]))
    
    
    for i in range(0, n):
        file.write('%s\n' % (cities[i]))


def runTest():
    generate(20)
    result = subprocess.run(["./a.out"], stdin=open("test0.in", "r"), capture_output=True, text=True)
    output = result.stdout
    sys.stdout.flush()
    #print(output)

    result2 = subprocess.run(["./test"], stdin=open("test0.in", "r"), capture_output=True, text=True)
    output2 = result2.stdout
    sys.stdout.flush()
    #print(output2)
    
    if (output != output2):
        print("error")
    
runTest()


