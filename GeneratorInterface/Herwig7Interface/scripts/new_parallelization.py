#! /usr/bin/python

# This script sets up parallel jobs for the integrate and run step when
# using Herwig within CMSSW.
# It takes a cmsRun file, that has to provide the options
#   runMode
#   maxJobs
#   integrationList
#   seed
# via VarParsing.

# Possible options:
# -b/--build : Choose to run the build step.
# -i/--integrate : sets the maximal number of integration jobs
#     This option already has to be set when the build step is invoked.
#     The integration step will be performed if this option is set,
#     unless --nointegration is chosen.
#     The actual number of integration jobs may be smaller. It is
#     determined by the number of files in Herwig-scratch/Build.
# -r/--run : sets the number of run jobs and starts the run step.
# -s/--seed : Give one or more seeds for the run mode. They will be
#     used in the given order. If the number of run jobs exceeds the
#     number of seeds, the list will be filled with the subsequent
#     numbers to the last element. If more seeds are given than needed,
#     the last ones will be ignored.
# --nointegration : use this option to set up several integration jobs
#     without actually performing them
# --l/--log: write the output of each shell command called in a
#     seperate log file
# --build_args/--integrate_args/--run_args : A string including
#     additional arguments to use when calling cmsRun for the
#     respective mode.
#     $(INT_ID) can be used in the string of --integrate_args and will
#     be replaced by the job id of the integration.
#     $(RUN_ID) can be used in the string of --run_args and will be
#     replaced by the seed of the run.
# --args : A string including additional arguments to use when calling
#     cmsRun

# If one uses this script with only 1 job for integration or run, it
# creates a slight overhead.

# A parallelized run step is achieved by calling cmsRun an according
# number of times with different seeds for Herwig. The built in feature
# of Herwig wil not be used.

# Author: Dominik Beutel


import argparse
import sys
import os
import subprocess
import re
import random



def uint(string):
    """Unsigned int type"""
    value = int(string)
    if value <= 0:
        msg = '{0} is negative or zero'.format(string)
        raise argparse.ArgumentTypeError(msg)
    return value

def callcmsRun(calls, logs):
    processes = []
    # An empty string means no logging to files
    if logs != []:
        if len(calls) != len(logs):
            print 'Length of calls does not match length of logs.'
            sys.exit(1)

        for call, log in zip(calls, logs):
            print 'Calling:\t' + ' '.join(call)
            print 'Writing ouput to log file: ' + str(log)
#            with open(str(log), 'w') as out:
#                processes.append(subprocess.Popen(call, stdout=out, stderr=subprocess.STDOUT))

    else:
        for call in calls:
            print 'Calling:\t' + ' '.join(call)
#            processes.append(subprocess.Popen(call))

    # Wait for all processes to finish
#    for process in processes:
#        process.wait()
 

       


##################################################
# Get command line arguments
##################################################

parser = argparse.ArgumentParser()

parser.add_argument('cmsRunfile', help='filename of the cmsRun configuration', nargs=1)
parser.add_argument('-b', '--build', help='choose build mode', action='store_true')
parser.add_argument('-i', '--integrate', help='set the maximal number of integration jobs', type=uint, nargs='?', const=1, default=0)
parser.add_argument('-r', '--run', help='set the number of run jobs', type=int, choices=range(1,11), nargs='?', const=1, default=0)
parser.add_argument('-s', '--seed', help='set seed(s)', type=int, nargs='+', default=[])
parser.add_argument('--nointegration', help='build -i integration jobs without actually integrating', action='store_true')
parser.add_argument('-l', '--log', help='write the output of each process in a separate log file', action='store_true')
parser.add_argument('--build_args', help='Additional arguments for the build mode', type=str, default='')
parser.add_argument('--integrate_args', help='Additional arguments for the integration mode', type=str, default='')
parser.add_argument('--run_args', help='Additional arguments for the run mode', type=str, default='')
parser.add_argument('--args', help='Additional arguments for all modes', type=str, default='')

args = parser.parse_args()

# Create a template name for all created files
template_name = args.cmsRunfile[0].replace('.', '_')

# Fill seed list to match to run length
if args.run:
    if args.seed == []:
        args.seed.append(random.randint(1000,32000))
    for i in range(len(args.seed),args.run):
        args.seed.append(args.seed[-1:][0]+1)
args.seed = args.seed[:args.run]

print args.seed

##################################################
# Execute the different run modes
##################################################

## Build ##
# If -b is used without -i, act as if -i 1 --nointegration was chosen
if args.build and not args.integrate:
    args.integrate = 1
    args.nointegration = True

if args.build:
    print '------------------'
    print 'Build mode started'
    print '------------------'
    if args.log:
        build_logs = [template_name + '_build.log']
    else:
        build_logs = []
    print 'Setting up a maximum of {0} integration job(s).'.format(str(args.integrate))

    build_calls = [['cmsRun'] + args.cmsRunfile + args.build_args.split() + args.args.split() + ['runMode=\'build\'', 'maxJobs='+str(args.integrate)]]
    callcmsRun(build_calls, build_logs)

    print '--------------------'
    print 'Build mode finished.'
    print '--------------------'

## Integrate ##

# Stop in case no integration is desired
if args.nointegration:
    sys.exit(0)

if args.integrate:
    # Determine number of integration jobs
    actual_int_jobs = len([string for string in os.listdir('Herwig-scratch/Build') if re.match(r'integrationJob[0-9]+', string)])
    
    # Stop if this number exceeds the given parameter
    if actual_int_jobs > args.integrate:
        print 'Actual number of integration jobs {0} exceeds maximum number of specified jobs {1}.'.format(actual_int_jobs, args.integrate)
        print 'Integration will not be performed.'
        sys.exit(0)

    # Start the integration jobs
    print '-------------------------'
    print 'Integration mode started.'
    print '-------------------------'
    print 'Found {0} integration jobs, a maxiumum of {1} was given.'.format(actual_int_jobs, args.integrate)
    if args.log:
        integration_logs = [template_name + '_integrate_' + str(i) + '.log' for i in range(actual_int_jobs)]
    else:
        integration_logs = []
        print '--- Output may be cluttered. (Try the option -l/--log) ---'

    integration_calls = [['cmsRun'] + args.cmsRunfile + args.integrate_args.split() + args.args.split() + ['runMode=\'integrate\'', 'integrationList=' + str(i)] for i in range(actual_int_jobs)]
    callcmsRun(integration_calls, integration_logs)

    print '--------------------------'
    print 'Integration mode finished.'
    print '--------------------------'

## Run mode ##
if args.run:
    print '-----------------'
    print 'Run mode started.'
    print '-----------------'

    # Start the run jobs
    if args.log:
        run_logs = [template_name + '_run_' + str(seed) + '.log' for seed in args.seed]
    else:
        run_logs = []
        print '--- Output may be cluttered. (Try the option -l/--log) ---'

    run_calls = [['cmsRun'] + args.cmsRunfile + args.run_args.split() + args.args.split() + ['runMode=\'run\'', 'seed='+str(seed)] for seed in args.seed]
    callcmsRun(run_calls, run_logs)

    print '------------------'
    print 'Run mode finished.'
    print '------------------'
