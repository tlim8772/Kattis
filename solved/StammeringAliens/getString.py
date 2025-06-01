import sys

def helper(len, idx):
    input()
    str = input()
    print(str[idx:idx + len])



helper(int(sys.argv[1]), int(sys.argv[2]))