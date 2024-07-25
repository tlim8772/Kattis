from random import randrange

def generate(power_of_2):
    file = open('test0.in', 'w')
    file.write("%d %d\n" % (2 ** power_of_2 - 1, 10))
    s0 = ' '.join([str(randrange(1, 10)) for i in range(2 ** power_of_2 - 1)])
    file.write("%s\n" % (s0))
    
    lst = []
    for i in range(0, 2 ** power_of_2 - 2):
        vertex = i + 2
        parent = vertex // 2
        lst.append(parent)
    s = ' '.join([str(lst[i]) for i in range(len(lst))])
    file.write("%s\n" % (s))

    for i in range(10):
        if i % 2 == 0:
            file.write("%d %d\n" % (0, randrange(1, 2 ** power_of_2 - 1)))
        else:
            file.write("%d %d\n" % (randrange(1, 10), randrange(1, 2 ** power_of_2 - 1)))

generate(3)