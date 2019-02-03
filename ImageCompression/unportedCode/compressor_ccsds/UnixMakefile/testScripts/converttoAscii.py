import os
import struct
import sys

if __name__ == "__main__":
    in_file = open(str(sys.argv[1]),"rb")
    out_file = open(sys.argv[2],"w+")
    byte = in_file.read(1)
    width = int(sys.argv[3])
    count = 1
    while byte != b"":
        # Do stuff with byte
        out_file.write(str(int.from_bytes(byte,sys.byteorder))+" ")
        if count%width == 0:
            out_file.write("\n")
            count = 0
        byte = in_file.read(1)
        count += 1
    in_file.close()
    out_file.close()
