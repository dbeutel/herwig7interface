# herwig7interface
Repo for upcoming Herwig7 interface in CMSW

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
  
  
