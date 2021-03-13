import argparse
import os

# A function to emulate the outputs of foo.
def control(val):
    ret = 0
     
    if val < 2:
        ret = 2  

    for x in range(2, val):
        if val % x == 0:
            ret = 3

    return ret

# The main function, testing foo and comparing it with control().
def main():

    # The argument parser.
    parser = argparse.ArgumentParser()
    parser.add_argument("-s", help="The start of the test range. At a minimum, -s OR -e must be given, if not both. If -s is larger than -e, -s will be used.", type=int)
    parser.add_argument("-e", help="The end of the test range. At a minimum, -s OR -e must be given, if not both. If -s is larger than -e, -s will be used.", type=int)
    parser.add_argument("-v", help="Print additional information when a value fails.", action="store_true")

    # Parse the arguments and store them in args.
    args = parser.parse_args()

    # If the end value is not given...
    if args.e == None:
        # ...and the start value is not given...
        if args.s == None:
            # ...print the help menu and exit.
            parser.print_help()
            exit()
        # ...and the start value is given...
        elif args.s != None:
            # ...use start for end too.
            args.e = args.s
    # If the end value is given...
    else:
        # And the start value is not...
        if args.s == None:
            # ...use end for start too.
            args.s = args.e

    # If start is greater than end, use start for both values.
    if args.s > args.e:
        args.e = args.s

    # For each value in the range of start through end...
    for ii in range(args.s, args.e + 1):
        # ...get a control value from our python emulation and get the real value from running foo (Decode by dividing by 256 to shift bits right by 8).
        exp = control(ii)
        act = os.system(f"./foo {ii}") / 256

        # If the expected and actual values don't match, tell the user and exit.
        if exp != act:
            print("Something went wrong, exiting.")
            exit()
        # Otherwise, print the given value and returned status code, as well as additional information for values that failed if the verbose option is given.
        else:
            print(f"{ii} exited with status {int(act)}.")
            if args.v == True:
                if act == 1:
                    print(" A command line argument was not supplied to foo.")
                elif act == 2:
                    print(" An invalid command line argument was supplied to foo. Try an input of 2 or greater.")
                elif act == 3:
                    print(" For an unkown reason, foo failed.")
            print("\n")

# Run main().
if __name__ == "__main__":
    main()

     
