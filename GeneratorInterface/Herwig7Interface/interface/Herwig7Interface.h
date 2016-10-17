/**
Marco A. Harrendorf
**/


#ifndef GeneratorInterface_Herwig7Interface_Herwig7Interface_h
#define GeneratorInterface_Herwig7Interface_Herwig7Interface_h



#include <memory>
#include <string>

#include <boost/shared_ptr.hpp>

#include <HepMC/GenEvent.h>
#include <HepMC/PdfInfo.h>
#include <HepMC/IO_BaseClass.h>

#include <ThePEG/Repository/EventGenerator.h>
#include <ThePEG/EventRecord/Event.h>

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "GeneratorInterface/Herwig7Interface/interface/RandomEngineGlue.h"
#include "GeneratorInterface/Herwig7Interface/interface/HepMCTemplate.h"
#include "GeneratorInterface/Herwig7Interface/interface/HerwigUIProvider.h"

namespace CLHEP {
  class HepRandomEngine;
}

class Herwig7Interface {
    public:
	Herwig7Interface(const edm::ParameterSet &params);
	virtual ~Herwig7Interface();

        void setPEGRandomEngine(CLHEP::HepRandomEngine*);

	mutable ThePEG::EGPtr				eg_;

    protected:
	void initRepository(const edm::ParameterSet &params) const;
	void initGenerator();
	void flushRandomNumberGenerator();

	static std::auto_ptr<HepMC::GenEvent>
				convert(const ThePEG::EventPtr &event);

	static double pthat(const ThePEG::EventPtr &event);

	

	std::auto_ptr<HepMC::IO_BaseClass>	iobc_;

	// HerwigUi contains settings piped to Herwig7
	Herwig::HerwigUIProvider* HwUI_;


    private:
	boost::shared_ptr<ThePEG::RandomEngineGlue::Proxy>
						randomEngineGlueProxy_;

	const std::string			dataLocation_;
	const std::string			generator_;
	const std::string			run_;
	// File name containing Herwig input config 
	std::string			dumpConfig_;
	const unsigned int			skipEvents_;
};







#endif // GeneratorInterface_Herwig7Interface_Herwig7Interface_h
