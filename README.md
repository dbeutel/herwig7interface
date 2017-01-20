# herwig7interface
Repo for upcoming Herwig7 interface in CMSW

Table of Contents
=================

  * [herwig7interface](#herwig7interface)
    * [Setup of Herwig7 interface (Status of 15.11.2016)](#setup-of-herwig7-interface-status-of-15112016)
    * [Changing code of Herwig7 interface](#changing-code-of-herwig7-interface)
    * [Code structure](#code-structure)
      * [Main folder](#main-folder)
      * [GeneratorInterface/Herwig7Interface](#generatorinterfaceherwig7interface)
    * [Matchbox interface: Available external matrix element providers](#matchbox-interface-available-external-matrix-element-providers)
    * [Current workflow](#current-workflow)
    * [Implemented options of the Herwig UI](#availible-options)


## Setup of Herwig7 interface (Status of 15.11.2016)
* For the setup and test of the Herwig7 interface the following steps are necessary.
* The `cmsenv` and the `source herwig7-env.sh` has to be done always, meaning, each time you open a new terminal. All the other steps have to be done only once.

1. Installation of CMSSW 7_1_25_patch1 
  * For the moment I provide a local Herwig7 installation since the global Herwig7 executable is not working due to some wrongly set paths
  ```
scram p CMSSW_7_1_25_patch1
  ```
2. Set CMSSW environment 
  * Has to be done in each new terminal
  ```
cd CMSSW_7_1_25_patch1/src
cmsenv
  ```
3. Clone newest version of Herwig7 interface
  * If you would like to contribute it is maybe better to fork my repository and to clone your own version since you can then push stuff to your repository and make use of pull requests.
  ```
  git clone git@github.com:mharrend/herwig7interface.git
  ```
4. Sourcing of local Herwig7 installation 
  * Has to be done in each new terminal
  * In this way, the path to my local Herwig7 installation is set. Otherwise, CMSSW would provide only  Herwig++ 2.7.1
  ```
source herwig7-env.sh
  ```
5. Reset scram build and rebuild of Herwig7 interface
  * During the second step you can ignore the following two warnings:
    * ****WARNING: Invalid tool madgraph5amcatnlo. Please fix herwigpp file.
    * fatal: ambiguous argument 'CMSSW_7_1_25_patch1': unknown revision or path not in the working tree.
  ```
scram b clean
scram b -j 10
  ```
6. Test of the Herwig7 interface
  * For testing the recently build interface you can run one of the following cmsRun files via ```cmsRun [Filename]```
    * Herwigpp_Herwig7interfaceTestProcess_cff_py_GEN_SIM.py
    * Herwigpp_MatchboxHerwig7interface_cff_py_GEN_SIM.py
    * Herwigpp_ConfigFile_cff_py_GEN_SIM.py: Note this cmsRun config works only if you provide a Herwig inputfile called TestConfig.in
  * Note the cmsRun config file Herwigpp_TestProcess_cff_py_GEN_SIM.py needs some update and does not work.
  
## Changing code of Herwig7 interface
* You can change the code of the Herwig7 interface and rebuild the interface.
* For this, in a new terminal you have always to setup the CMSSW environment and source the local Herwig7 installation via
  ```
  cd CMSSW_7_1_25_patch1/src
  
  cmsenv
  source herwig7-env.sh
  ```
* After a code change you can rebuild the interface via
  ```
  scram b -j 10
  ```
* If strange errors occur, try a ``` scram b clean``` before rebuild the interface.
  
## Code structure
### Main folder
* Herwigpp_ConfigFile_cff_py_GEN_SIM.py: Uses Herwig7 interface to read in an external Herwig7 input file called TestConfig.in. Difference to other cmsRun files is that the Herwig7 config is not provided in the python code but as an external file.
* Herwigpp_Herwig7interfaceTestProcess_cff_py_GEN_SIM.py: Similar to Herwigpp_TestProcess_cff_py_GEN_SIM.py. Use the old Herwig++ parametrization style. In more detail, an internal matrix element for ttH is used.
* Herwigpp_MatchboxHerwig7interface_cff_py_GEN_SIM.py: Uses the new Herwig7 Matchbox interface to use MadGraph5aMC@NLO as a matrix element provider.
* Herwigpp_TestProcess_cff_py_GEN_SIM.py: Needs some update. Uses Herwig's internal matrixelement as a basic example. 
* TestConfig.in: Herwig7 inputfile used as a test config together with the Herwigpp_ConfigFile_cff_py_GEN_SIM.py cmsRun file
* herwig7-env.sh: Bash script to set paths for local Herwig7 installation
* herwigpp.xml: Needed to overwrite scram config, so that Herwig7 is available instead of CMSSW's Herwig++
* madgraph5amcatnlo.xml: Needed to provide MadGraph5aMC@NLO as external matrix element provider
* thepeg.xml: Needed to overwrite scram config, so that ThePEG 2.0.X is available instead of CMSSW's 1.9.2

### GeneratorInterface/Herwig7Interface
* doc folder: Short information about TWiki page in which new interface should be documented in the long term.
* interface: C++ header files
  * HepMCTemplate.h: Definition of HepMC to CMSSW EDM root file converter
  * Herwig7Interface.h: Main interface which is called by plugins/HerwigHadronizer.cc
  * HerwigUIProvider.h: Provides basic settings to Herwig7 API. This class is adopted from Herwig's own UIProvider class.
  * Proxy.h: Should not be necessary any longer, kept for now, but should be removed in the future.
  * RandomEngineGlue.h: Should not be necessary any longer, kept for now, but should be removed in the future.
* plugins: Folder which defines a Generator interface
  * BuildFile.xml: Defining a Herwig7GeneratorFilter and Herwig7GeneratorHadronizer plugin. 
  * Herwig7Hadronizer.cc: File which is derived from CMSSW/GeneratorInterface/Core base classes.
* python: Deprecated, needs some update
* src: C++ source files compare with interface folder
* scripts
* test: Folder is outdated and needs some update. I am planning to copy the test files from the main folder to this folder as soon as our interface API is stable.
* BuildFile.xml: Necessary to build interface plugin

## Matchbox interface: Available external matrix element providers
* MadGraph5aMC@NLO
* GoSam
* OpenLoops

## Current workflow
* If you use a cmsRun config file together with cmsRun the following happens:

1. The configuration in the "Herwig7GeneratorFilter" part and the external Herwig7 inputfile which is set via the optional "configFiles" option is converted by the Herwig7 interface into a Herwig7 input file. The default file name is HerwigConfig.in. Furthermore, it defines a HerwigUI object with some basic settings.

2. The Herwig7 interface will call then the read mode of Herwig7 via the Herwig7 API handing over the HerwigUI object which was constructed before. Herwig7 will produce then a run file with the default name InterfaceTest.run.

3. After the successful read step, the Herwig7 interface will invoke the Herwig7 API again. This time handing over a slightly changed HerwigUI object which requests the Herwig7 run mode and points to the freshly created Herwig7 run file. Herwig7 will then be in the run mode producing events.

* The workflow described above is the default behaviour. It can be changed via the runModeList option, which is an untracked string. It has to be a comma seperated list (without whitespace), which can contain read, build, integrate and run. For example "build,integrate" will only perform the build and integrate step. If no run step is chosen in the end, some error warnings of subsequent tasks to the event generation may occur, since no events were generated.


## Implemented options of the Herwig UI
* The following options of the Herwig user interface are implemented as untracked parameters:

  * jobs (int): Set the number of parallel jobs (doesn't work in run mode)
  * maxJobs (unsigned int): Set the number of integrations to set up 
  * jobSize (unsigned int): Number of subprocesses to integrate per job
  * integrationList (string): Number of the integration job to run

  * setupFile (string): Use Herwig input file to modify run parameters
  * runTag (string): Append tag to run name of files created by Herwig

  * debutOutput (unsigned int): Set the verbosity of the ouput
  * depugFPE (bool): Enable floating point exceptions
  * exitOnError (bool): Don't try to recover from failures. Exit with non-zero return value.

  * resume (bool): ??
  * numberEvents (int): !!
  * hideTics (bool): !!
  * appendReadDirectories (vector of strings): Append path to search for Herwig filesystem
  * prependReadDirectories (vector of strings): Prepend path to search for Herwig filesystem
  * appendPath (vector of strings): Append path to search for library
  * prependPath(vector of strings): Prepend path to search for library


* Additionally the tracked parameter repository exists. It choses the repository for Herwig to use. If left empty it defaults to "HerwigDefaults.rpo".
