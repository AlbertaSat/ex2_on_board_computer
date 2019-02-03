import os
import struct
import sys


if __name__ == "__main__":
    in_file = open(str(sys.argv[1]),"r")
    out_file = open(sys.argv[2],"wb+")
    arr = in_file.readline().split()

    while len(arr)>0:
        arr2 = list()
        for element in arr:
            arr2.append(int(element.rstrip()))
        out_file.write(bytes(arr2))
        arr = in_file.readline().split()

    in_file.close()
    out_file.close()
