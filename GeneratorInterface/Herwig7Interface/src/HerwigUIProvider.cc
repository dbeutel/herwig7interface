/**
Marco A. Harrendorf
**/

#include "GeneratorInterface/Herwig7Interface/interface/HerwigUIProvider.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "GeneratorInterface/Core/interface/ParameterCollector.h"
#include "GeneratorInterface/Herwig7Interface/interface/RandomEngineGlue.h"

#include <ThePEG/Utilities/DynamicLoader.h>
#include <ThePEG/Utilities/Debug.h>
#include <ThePEG/Repository/Repository.h>
#include <ThePEG/Handlers/SamplerBase.h>


namespace Herwig {

void HerwigUIProvider::quitWithHelp() const {
edm::LogError("Herwig7Interface") << "An error occured, interface quits now.";
  quit();
}

void HerwigUIProvider::quit() const {
  ThePEG::Repository::cleanup();
  edm::LogInfo("Herwig7Interface") << "Quitting HerwigUIProvider after ThePEG repository cleanup.";
}

HerwigUIProvider::~HerwigUIProvider() {
  ThePEG::Repository::cleanup();
}

HerwigUIProvider::HerwigUIProvider(const edm::ParameterSet &pset, std::string inputFileName) 
  : runMode_(RunMode::ERROR), 
    resume_(false), tics_(true), tag_(),
    inputfile_(inputFileName), repository_(), setupfile_(),
    integrationList_(),
    N_(-1), seed_(0), jobs_(1),
    jobsize_(0), maxjobs_(0)
{

  // Define runMode of program
  std::string tmpRunMode = pset.getUntrackedParameter<std::string>("runMode", "read");
  if      ( tmpRunMode == "init" )       { runMode_ = RunMode::INIT; }
  else if ( tmpRunMode == "read" )       { runMode_ = RunMode::READ; }
  else if ( tmpRunMode == "build" )      { runMode_ = RunMode::BUILD; }
  else if ( tmpRunMode == "integrate" )  { runMode_ = RunMode::INTEGRATE; }
  else if ( tmpRunMode == "mergegrids" ) { runMode_ = RunMode::MERGEGRIDS; }
  else if ( tmpRunMode == "run" )        { runMode_ = RunMode::RUN; }
  else {
    runMode_ = RunMode::ERROR;
    quitWithHelp();
  }

	// File path to repository file
	//repository_  = gen::ParameterCollector::resolve(pset.getParameter<std::string>("dataLocation"));
	repository_ = std::string("/afs/cern.ch/work/m/mharrend/public/herwig7tutorial-71X704/herwig704-install/share/Herwig/HerwigDefaults.rpo");

  // Number of events
  if ( pset.getUntrackedParameter<int>("numberEvents", -1) != -1 )
    N_ = pset.getUntrackedParameter<int>("numberEvents", 1);


  // run name tag (default given in ggo file)
  if ( pset.getUntrackedParameter<std::string>("runTag", "") != "")
	tag_ = pset.getUntrackedParameter<std::string>("runTag", "Tag1");

  // Debugging level
  if ( pset.getUntrackedParameter<unsigned int>("debugOutput", 0) )
    ThePEG::Debug::setDebug( pset.getUntrackedParameter<unsigned int>("debugOutput", 0) );

  // Floating point exceptions
  if ( pset.getUntrackedParameter<bool>("debugFPE", false) ) 
    ThePEG::Debug::unmaskFpuErrors();

  // Exit-on-error flag
  if ( pset.getUntrackedParameter<bool>("exitOnError", false) )
    ThePEG::Repository::exitOnError() = 1;

  // Tics
  if ( pset.getUntrackedParameter<bool>("hideTics", false) )
    tics_ = false;

/*

 

  // RNG seed
  if ( args_info.seed_given ) {
    seed_ = args_info.seed_arg;
  }



  // run modification file
  if ( args_info.setupfile_given )
    setupfile_ = args_info.setupfile_arg;

  // parallel jobs
  if ( args_info.jobs_given )
    jobs_ = args_info.jobs_arg;
  
  // Directories from which Herwig reads filesystemfor ( size_t i = 0; i < args_info.append_read_given; ++i )
  for ( size_t i = 0; i < args_info.append_read_given; ++i )
    appendReadDirectories_.push_back( args_info.append_read_arg[i] );
  for ( size_t i = 0; i < args_info.prepend_read_given; ++i )
    prependReadDirectories_.push_back( args_info.prepend_read_arg[i] );

  // Library search path for dlopen()
  for ( size_t i = 0; i < args_info.append_given; ++i )
    ThePEG::DynamicLoader::appendPath( args_info.append_arg[i] );
  for ( size_t i = 0; i < args_info.prepend_given; ++i )
    ThePEG::DynamicLoader::prependPath( args_info.prepend_arg[i] );
  








  // integration list
  if ( args_info.jobid_given ) {
    integrationList_ = "integrationJob" + std::string(args_info.jobid_arg);
  }

  // job size
  if ( args_info.jobsize_given ) {
    if ( runMode_ != RunMode::BUILD ) {
      std::cerr << "--jobsize option is only available in 'build' mode.\n";
      quitWithHelp();
    }
    jobsize_ = args_info.jobsize_arg;
    ThePEG::SamplerBase::setIntegratePerJob(jobsize_);
  }

  // max integration jobs
  if ( args_info.maxjobs_given ) {
    if ( runMode_ != RunMode::BUILD ) {
      std::cerr << "--maxjobs option is only available in 'build' mode.\n";
      quitWithHelp();
    }
    maxjobs_ = args_info.maxjobs_arg;
    ThePEG::SamplerBase::setIntegrationJobs(maxjobs_);
  }

  // Resume
  if ( args_info.resume_flag )
    resume_ = true;
  
*/
}





void HerwigUIProvider::setRunMode(RunMode::Mode runMode, std::string inputFile)
{
	runMode_ = runMode;
	if( !inputFile.empty())
		inputfile_ = inputFile;
}


// End Herwig namespace
}

