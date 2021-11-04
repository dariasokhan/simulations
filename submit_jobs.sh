#!/bin/sh

# A shell script to submit Auger jobs

# 1st version Ken Livingston, Nov 2007

# function defs first,  main script below that.

print_usage()
{
    echo
    echo "Description: A shell script to submit Auger jobs"
    echo
    echo "Usage: $0 [-r] [-m <maxjobs>] <jobdir>|<jobfile>"
    echo
    echo "Options:"
    echo "  -m <maxjobs>   Set the maximum no of user owned jobs on the farm (default = 500)"
    echo "  -r             Submit jobs in reverse order to their directory listing in <jobdir> "
    echo "Attempts to run jsub to submit <jobfile>, or all the *.sub files in <jobdir>"
    echo "For each jobfile, a test is made to see if jobfile.auger exists. If so, the assumption is"
    echo "that the file has already been submitted. To allow resubmission, the .auger file must be removed manually" 
    echo "The log of the each jsub command is put in <jobfile>.auger"
    echo "Furthermore, no jsub commands will be executed until there are less than <maxjobs> jobs pending" 
    echo "or running on the farm (counted using the jobstat comand)"
    echo "The maximum no of jobs can be adjusted using the -m option. (default = 500)"
    echo "If there are more than <maxjobs>, the script will loop, testing every hour until it is able to"
    echo "submit another job."
    echo
    echo 
    echo "Examples:"
    echo
    echo "$0 jobs/g13/pass0/v7/batch1/runs53764-53783.sub"
    echo "Submits  jobs/g13/pass0/v7/batch1/runs53764-53783.sub with result in jobs/g13/pass0/v7/batch1/runs53764-53783.sub.auger"
    echo
    echo "$0 -m 1000 jobs/g13/pass0/v7/batch1"
    echo "Submits all .sub files in jobs/g13/pass0/v7/batch1, with logs in corresponding .auger files,"
    echo "also increasing the maximum no of jobs to 1000." 
    echo 
    exit;
}

init_values()
{
#set the defaults
    default_maxq="500";
    rev="";
    maxq="${default_maxq}";
}

#  Function to set the command line options
function set_option {
 	case $1 in
	    m)
	    maxq="${OPTARG}" ;;
	    r)
	    rev="-r" ;;
            *)
	    print_usage ;;
	esac
    shift $[OPTIND-1]
} 

################################################################################################
#MAIN SCRIPT STARTS HERE

if [ $# -lt 1 ]; then
     print_usage;
fi

init_values;

#parse the command options and set variables accordingly
while getopts "rm:" opt ; do 
    set_option $opt  
done
shift $[$OPTIND-1]

#work out if we have a file or directory ( or neither)
if [ -f "$1" ]; then
    #make a filelist with a single file, for consistency
    dir=`dirname $1`
    rm -f ${dir}/allsubs.dat
    echo $1 > ${dir}/allsubs.dat
else
    dir=$1
    if [ -d "$dir" ]; then
	#make a filelist of all files
	rm -f ${dir}/allsubs.dat
	ls $rev $dir/*.sub > ${dir}/allsubs.dat
    else
	echo "Fatel Error: $1 doesn't seem to exist"
	print_usage
    fi
fi

#loop over all files in ${dir}/allsubs.dat

while read subfile
do
    #if not already submitted (ie no .auger file)
    if [ ! -f "${subfile}.auger" ]; then
	#count no of jobs already queued on farm
	jobcount=`jobstat -u ${USER} | gawk '{if(($3=="Q")||($3=="H")||($3=="A")) print $0}' | wc -l`
	#go if less than the (self imposed) maximum
	jobgo=`echo $jobcount | gawk -v maxq=$maxq '{if ($1>maxq){print"wait"}else{print"go"}}'`
	if [ "$jobgo" = "wait" ]; then
	    while [ "$jobgo" = "wait" ]; do
	        jobcount=`jobstat -u ${USER} | gawk '{if(($3=="Q")||($3=="H")||($3=="A")) print $0}' | wc -l`
		echo "There are aleady $jobcount jobs queued. Waiting until it drops below $maxq. Trying again in 1hr"
		jobgo=`echo $jobcount | gawk -v maxq=$maxq '{if ($1>maxq){print"wait"}else{print"go"}}'`
		sleep 1h
	    done
	fi
	#OK to go
	echo "$jobcount jobs on already on farm, submitting $subfile "
	echo "jsub $subfile >&  ${subfile}.auger"
	jsub $subfile >&  ${subfile}.auger
    fi
done < ${dir}/allsubs.dat


exit;

