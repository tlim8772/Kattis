import random
import string
import subprocess
import sys

def generate_random_string(k):
    characters = string.ascii_letters + string.digits + string.punctuation + ' '  # Include letters, digits, and symbols
    return ''.join(random.choices(characters, k=k))

def generate_all_same(c, k):
    return ''.join([c for i in range(k)])

def gen(dict_len, text_len):
    file = open('test1.in', 'w')
    file.write('%d\n' % (dict_len))

    for i in range(dict_len):
        file.write('%s\n' % (generate_all_same('a', 10)))

    file.write('%s\n' % (generate_all_same('a', text_len)))



def run_test():
    gen(10, 200)
    result = subprocess.run(["./a.out"], stdin=open("test1.in", "r"), capture_output=True, text=True)
    output1 = result.stdout
    sys.stdout.flush()

    ans = subprocess.run(["./test"], stdin=open("test1.in", "r"), capture_output=True, text=True)
    output2 = ans.stdout
    sys.stdout.flush()


    if (output1 != output2): 
        print("error")




run_test()


