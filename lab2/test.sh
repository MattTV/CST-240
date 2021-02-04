#! /bin/bash

# Matt Volpe
# January 21, 2021
# CST240 Lab 2

START=1
END=0
SGIVEN=1
EGIVEN=1
VERBOSE=1
RETCODE=0

# This function displays when -h is specified, or if -e AND -s are not given.
function show_help
{

    echo -e "\nWelcome to test.sh!\n"
    echo -e "This bash script tests the C program 'foo' with a range of different integer inputs.\n"
    echo "This script has four arguments:"
    echo "'-h' will bring you to this help menu."
    echo "'-v' will offer extra information when a tested input fails."
    echo "'-s #' takes an integer argument that specifies where to start testing. If not given, start will defaulit to 1."
    echo -e "'-e #' takes an integer argument that specifies where to end testing. If not given, end will default to 0.\n"
    echo "At least one of -s or -e needs to be specified, or you will see this help menu again."
    echo -e "\nLastly, if the start value is greater than the end value, both will be set to the given -s. If the end value is less than the start value, both will be set to the given -e.\n"

}

# Take in arguments. Errors are handled manually.
while getopts ':s:e:hv' OPT
do
    case $OPT in
        # If -s is specified, set its argument to START and reflect that a value was GIVEN.
        s) 
            START=$OPTARG 
            SGIVEN=0
        ;;
        # If -e is specified, set its argument to END and reflect that a value was GIVEN.
        e)
            END=$OPTARG
            EGIVEN=0
        ;;
        # If -h is specified, show the help menu and exit.
        h)
            show_help
            exit
        ;;
        # If -v is specified, set a flag.
        v)
            VERBOSE=0
        ;;
        # If the user tries anything funny, let them know we don't mess around in these parts.
        ?)
            echo "An unknown option was given. Please try again or specify -h for help."
        ;;
    esac
done

# If -s AND -e are not given, show help and exit the program.
if [ $SGIVEN -eq 1 ] && [ $EGIVEN -eq 1 ] ; then
    show_help
    exit
fi

# If START is greater than END, and START was the specified value, set END to START.
if [ $START -gt $END ] && [ $SGIVEN -eq 0 ] ; then
    END=$START
# If END is less than START, and END was the specified value, set START to END.
elif [ $END -lt $START ] && [ $EGIVEN -eq 0 ] ; then
    START=$END
fi

# Iterate through the range of test values.
ii=$START
while [ $ii -le $END ]
do

    # Run the program with the current test value.
    ./foo $ii
    
    # Capture the return code to use it multiple times.
    RETCODE=$?

    # Read foo's exit code. If it's 0, print that the program was successful.
    if [ $RETCODE -eq 0 ] ; then
        echo "$ii was successful"
    # If foo returned any other exit code, it failed, which we don't say unless asked with the VERBOSE option.
    else
        # If VERBOSE is true, print the error code and a reason.
        if [ $VERBOSE -eq 0 ] ; then
            # Display a message with information based on the return code of foo.
            case $RETCODE in
                1)
                    echo "When running $ii, the command line argument was missing."
                ;;
                2)
                    echo "$ii was an invalid command line argument."
                ;;
                3)
                    echo "$ii caused the program to fail for an unknown reason."
                ;;
            esac
        fi
    fi 

    # Increment to the next test value.
    ii=$(( $ii + 1 ))
done
