import os
import struct
import sys

if __name__ == "__main__":
    original = open(str(sys.argv[1]),"rb")
    decompressed = open(str(sys.argv[2]),"rb")
    oByte = original.read(1)
    dByte = decompressed.read(1)
    eqaul = True
    pixel = 1
    while oByte != b"" or dByte != b"":
        if oByte == b"":
            equal = False
            print(equal)
            print(str(sys.argv[2])+" is longer than "+str(sys.argv[1]))
            exit()
        if dByte == b"":
            equal = False
            print(equal)
            print(str(sys.argv[1])+" is longer than "+str(sys.argv[2]))
        if oByte != dByte:
            equal = False
            print(equal)
            print("Pixel "+str(pixel)+" is not equal")
            exit()
        oByte = original.read(1)
        dByte = decompressed.read(1)
        pixel += 1
    print("The files contain the same contents")
    original.close()
    decompressed.close()
